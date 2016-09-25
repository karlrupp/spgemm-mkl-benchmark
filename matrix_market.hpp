#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <cctype>

/** Trims whitespace from the beginning and the end of the string. */
inline void my_trim(char * buffer, long max_size)
{
  //trim at beginning of string
  long start = 0;
  for (long i=0; i<max_size; ++i)
  {
    if (buffer[i] == ' ')
      ++start;
    else
      break;
  }

  //trim at end of string
  long stop = start;
  for (long i=stop; i<max_size; ++i)
  {
    if (buffer[i] == 0)   //end of string
      break;

    if (buffer[i] != ' ')
      stop = i;
  }

  for (long i=0; i<=stop - start; ++i)
  {
    buffer[i] = buffer[start + i];
  }

  if (buffer[0] != ' ')
    buffer[stop - start + 1] = 0; //terminate string
  else
    buffer[0] = 0;
}

/** Converts a string to lowercase */
inline std::string my_tolower(std::string & s)
{
  std::transform(s.begin(), s.end(), s.begin(), static_cast < int(*)(int) > (std::tolower));
  return s;
}


/** Reads a matrix from file. Returns the number of lines read, or 0 in case of an error. */
template<typename NumericT, typename IndexT>
long read_matrix_market_file(std::vector<std::map<IndexT, NumericT> > & mat, const char * file)
{
  char buffer[2049];
  std::ifstream reader(file);
  std::string token;
  long linenum = 0;
  bool symmetric = false;
  bool dense_format = false;
  bool is_header = true;
  bool pattern_matrix = false;
  long cur_row = 0;
  long cur_col = 0;
  long valid_entries = 0;
  long nnz = 0;


  if (!reader)
  {
    std::cerr << "Matrix Market Reader: Cannot open file " << file << std::endl;
    return EXIT_FAILURE;
  }

  while (reader.good())
  {
    // get a non-empty line
    do
    {
      reader.getline(buffer, 2048);
      ++linenum;
      my_trim(buffer, 2048);
    }
    while (reader.good() && buffer[0] == 0);

    if (buffer[0] == '%')
    {
      if (buffer[1] == '%')
      {
        //parse header:
        std::stringstream line(std::string(buffer + 2));
        line >> token;
        if (my_tolower(token) != "matrixmarket")
        {
          std::cerr << "Error in file " << file << " at line " << linenum << " in file " << file << ": Expected 'MatrixMarket', got '" << token << "'" << std::endl;
          return 0;
        }

        line >> token;
        if (my_tolower(token) != "matrix")
        {
          std::cerr << "Error in file " << file << " at line " << linenum << " in file " << file << ": Expected 'matrix', got '" << token << "'" << std::endl;
          return 0;
        }

        line >> token;
        if (my_tolower(token) != "coordinate")
        {
          if (my_tolower(token) == "array")
          {
            dense_format = true;
            std::cerr << "Error in file " << file << " at line " << linenum << " in file " << file << ": 'array' type is not supported yet!" << std::endl;
            return 0;
          }
          else
          {
            std::cerr << "Error in file " << file << " at line " << linenum << " in file " << file << ": Expected 'array' or 'coordinate', got '" << token << "'" << std::endl;
            return 0;
          }
        }

        line >> token;
        if (my_tolower(token) == "pattern")
        {
          pattern_matrix = true;
        }
        else if (my_tolower(token) == "complex")
        {
          //is_complex = true;
        }
        else if (my_tolower(token) != "real" && my_tolower(token) != "integer")
        {
          std::cerr << "Error in file " << file << ": The MatrixMarket reader supports only real valued floating point arithmetic or pattern type matrices." << std::endl;
          return 0;
        }

        line >> token;
        if (my_tolower(token) == "general"){ }
        else if (my_tolower(token) == "symmetric"){ symmetric = true; }
        else
        {
          std::cerr << "Error in file " << file << ": The MatrixMarket reader supports only general or symmetric matrices." << std::endl;
          return 0;
        }

      }
    }
    else
    {
      std::stringstream line(std::stringstream::in | std::stringstream::out);
      line << std::string(buffer);

      if (is_header)
      {
        //read header line
        std::size_t rows;
        IndexT cols;

        if (line.good())
          line >> rows;
        else
        {
          std::cerr << "Error in file " << file << ": Could not get matrix dimensions (rows) in line " << linenum << std::endl;
          return 0;
        }

        if (line.good())
          line >> cols;
        else
        {
          std::cerr << "Error in file " << file << ": Could not get matrix dimensions (columns) in line " << linenum << std::endl;
          return 0;
        }
        if (!dense_format)
        {
          if (line.good())
            line >> nnz;
          else
          {
            std::cerr << "Error in file " << file << ": Could not get matrix dimensions (columns) in line " << linenum << std::endl;
            return 0;
          }
        }

        if (rows > 0 && cols > 0)
          mat.resize(rows);

        is_header = false;
      }
      else
      {
        //read data
        if (dense_format)
        {
          NumericT value;
          line >> value;
          mat[cur_row][cur_col] = value;

          if (++cur_row == mat.size())
          {
            //next column
            ++cur_col;
            cur_row = 0;
          }
        }
        else //sparse format
        {
          long row;
          long col;
          NumericT value = NumericT(1);

          //parse data:
          if (line.good())
            line >> row;
          else
          {
            std::cerr << "Error in file " << file << ": Parse error for matrix row entry in line " << linenum << std::endl;
            return 0;
          }

          if (line.good())
            line >> col;
          else
          {
            std::cerr << "Error in file " << file << ": Parse error for matrix col entry in line " << linenum << std::endl;
            return 0;
          }

          //take index base 1 into account:
          row -= 1;
          col -= 1;

          if (!pattern_matrix) // value for pattern matrix is implicitly 1, so we only need to read data for 'normal' matrices
          {
            if (line.good())
            {
                line >> value;
            }
            else
            {
              std::cerr << "Error in file " << file << ": Parse error for matrix entry in line " << linenum << std::endl;
              return 0;
            }
          }

          if (row >= mat.size() || row < 0)
          {
            std::cerr << "Error in file " << file << " at line " << linenum << ": Row index out of bounds: " << row << " (matrix dim: " << mat.size() << " x " << mat.size() << ")" << std::endl;
            return 0;
          }

          if (col >= mat.size() || col < 0)
          {
            std::cerr << "Error in file " << file << " at line " << linenum << ": Column index out of bounds: " << col << " (matrix dim: " << mat.size() << " x " << mat.size() << ")" << std::endl;
            return 0;
          }

          mat[row][col] = value;
          if (symmetric)
            mat[col][row] = value;

          if (++valid_entries == nnz)
            break;

        } //else dense_format
      }
    }
  }

  reader.close();
  return linenum;
}


