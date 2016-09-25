

#include <vector>
#include <map>
#include <algorithm>

template<typename NumericT>
NumericT get_median(std::vector<NumericT> & timings)
{
  std::sort(timings.begin(), timings.end());
  return timings[timings.size()/2];
}

template<typename IndexT, typename NumericT>
void reference_prod(std::vector<std::map<IndexT, NumericT> > const & stl_A,
                    std::vector<std::map<IndexT, NumericT> > const & stl_B,
                    std::vector<std::map<IndexT, NumericT> >       & stl_C)
{
  for (std::size_t i=0; i<stl_A.size(); ++i)
    for (typename std::map<IndexT, NumericT>::const_iterator it_A = stl_A[i].begin(); it_A != stl_A[i].end(); ++it_A)
    {
      IndexT row_B = it_A->first;
      for (typename std::map<IndexT, NumericT>::const_iterator it_B = stl_B[row_B].begin(); it_B != stl_B[row_B].end(); ++it_B)
        if (std::fabs(it_A->second * it_B->second) > 0) // write only nonzero contributions
          stl_C[i][it_B->first] += it_A->second * it_B->second;
    }
}

// verifies that A and B are identical (same nonzero pattern, same entries up to round-off)
template<typename IndexT, typename NumericT>
void check(std::vector<std::map<IndexT, NumericT> > const & C,
           std::vector<std::map<IndexT, NumericT> > const & C_ref,
           std::vector<std::map<IndexT, NumericT> > const & A,
           std::vector<std::map<IndexT, NumericT> > const & B)
{
  // check number of nonzeros per row:
  for (std::size_t i=0; i<C.size(); ++i)
  {
    if (C[i].size() != C_ref[i].size())
    {
      std::cerr << "Check for nonzeros per row failed at row " << i << std::endl;
      std::cerr << "NNZ in C:     " <<     C[i].size() << std::endl;
      std::cerr << "NNZ in C_ref: " << C_ref[i].size() << std::endl;

      // print relevant lines in C if not too large:
      if (C_ref[i].size() < 100)
      {
        std::cout << "Result in C:     ";
        for (typename std::map<IndexT, NumericT>::const_iterator it_C = C[i].begin(); it_C != C[i].end(); ++it_C)
          std::cout << it_C->first << "  ";
        std::cout << std::endl;

        std::cout << "Result in C_ref: ";
        for (typename std::map<IndexT, NumericT>::const_iterator it_C_ref = C_ref[i].begin(); it_C_ref != C_ref[i].end(); ++it_C_ref)
          std::cout << it_C_ref->first << "  ";
        std::cout << std::endl;

        for (typename std::map<IndexT, NumericT>::const_iterator it_A = A[i].begin(); it_A != A[i].end(); ++it_A)
        {
          std::cout << "B, line " << it_A->first << ": ";
          for (typename std::map<IndexT, NumericT>::const_iterator it_B = B[it_A->first].begin(); it_B != B[it_A->first].end(); ++it_B)
            std::cout << it_B->first << "  ";
          std::cout << std::endl;
        }
      }
      exit(EXIT_FAILURE);
    }
  }

  // check sparsity pattern and numerical values:
  NumericT ref_value = 0;
  for (std::size_t i=0; i<C.size(); ++i)
  {
    typename std::map<IndexT, NumericT>::const_iterator it_C     = C[i].begin();
    typename std::map<IndexT, NumericT>::const_iterator it_C_ref = C_ref[i].begin();

    for (; it_C != C[i].end(); ++it_C, ++it_C_ref)
    {
      if (it_C->first != it_C_ref->first)
      {
        std::cerr << "Nonzero pattern mismatch in row " << i << std::endl;
        std::cerr << "Index in C:     " << it_C->first << std::endl;
        std::cerr << "Index in C_ref: " << it_C_ref->first << std::endl;
      }

      ref_value = std::max(ref_value, std::fabs(it_C_ref->second));
      NumericT rel_diff = std::fabs(it_C->second - it_C_ref->second) / std::max(std::fabs(it_C->second), std::fabs(it_C_ref->second));
      if (rel_diff > NumericT(0.01) && std::max(std::fabs(it_C->second), std::fabs(it_C_ref->second)) > ref_value * NumericT(1e-6))
      {
        std::cerr << "Numerical value mismatch in row " << i << std::endl;
        std::cerr << "Index in C:     " << it_C->first     << " with value " << it_C->second << std::endl;
        std::cerr << "Index in C_ref: " << it_C_ref->first << " with value " << it_C_ref->second << std::endl;
      }
    }
  }

  std::cout << "Check passed!" << std::endl;

}


