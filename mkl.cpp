
#include <iostream>
#include <vector>
#include <map>

#include "mkl.h"
#include "mkl_types.h"
#include "mkl_spblas.h"

#include "matrix_market.hpp"
#include "benchmark-utils.hpp"
#include "common.hpp"

void set_1based_ind(int *rowptr, int *colidx, int n, int nnz)
{
    int i;
    for(i=0; i <= n; i++)
        rowptr[i]++;
    for(i=0; i < nnz; i++)
        colidx[i]++;
}

int main(int argc, char **argv)
{
  if (argc < 3)
  {
    std::cerr << "Argument missing: [Matrix Market file] [flops]" << std::endl;
    return EXIT_FAILURE;
  }

  std::size_t runs = 10;
  std::vector<double> timings(runs);

  double flops = atoi(argv[2]);

  std::vector<std::map<unsigned int, double> > stl_A;

  // Read matrix
  if (!read_matrix_market_file(stl_A, argv[1]))
  {
    std::cout << "Error reading Matrix file" << std::endl;
    return EXIT_FAILURE;
  }

  // convert to plain CSR arrays:
  MKL_INT nnz = 0;
  for (std::size_t row=0; row<stl_A.size(); ++row)
    nnz += stl_A[row].size();


  MKL_INT *row_handle_A = (MKL_INT *)mkl_malloc(sizeof(MKL_INT) * (stl_A.size()+1), 128);
  MKL_INT *row_handle_C = (MKL_INT *)mkl_malloc(sizeof(MKL_INT) * (stl_A.size()+1), 128);
  MKL_INT *col_handle_A = (MKL_INT *)mkl_malloc(sizeof(MKL_INT) * nnz, 128);
  MKL_INT *col_handle_C;
  double *values_A = (double *)mkl_malloc(sizeof(double) * nnz, 128);
  double *values_C;

  int current_index = 0;
  for (std::size_t row=0; row<stl_A.size(); ++row)
  {
    row_handle_A[row] = current_index;
    for (typename std::map<unsigned int, double>::const_iterator it = stl_A[row].begin(); it != stl_A[row].end(); ++it)
    {
      col_handle_A[current_index] = it->first;
      values_A[current_index] = it->second;
      ++current_index;
    }
  }
  row_handle_A[stl_A.size()] = current_index;

  set_1based_ind(row_handle_A, col_handle_A, stl_A.size(), nnz);

  Timer timer;

  MKL_INT sort = 0;
  MKL_INT N = stl_A.size();
  MKL_INT info = 0;
  MKL_INT request;
  //std::cout << "Starting benchmark... " << std::endl;
  for (std::size_t i=0; i<runs; ++i)
  {
    timer.start();
    request = 1;
    mkl_dcsrmultcsr("n", &request, &sort, &N, &N, &N,
                    values_A, col_handle_A, row_handle_A,
                    values_A, col_handle_A, row_handle_A,
                    values_C, col_handle_C, row_handle_C,
                    &nnz, &info);
    col_handle_C = (MKL_INT *)mkl_malloc(sizeof(MKL_INT) * (row_handle_C[stl_A.size()]-1), 128);
    values_C     = (double *)mkl_malloc(sizeof(double) * (row_handle_C[stl_A.size()]-1), 128);
    request = 2;
    mkl_dcsrmultcsr("n", &request, &sort, &N, &N, &N,
                    values_A, col_handle_A, row_handle_A,
                    values_A, col_handle_A, row_handle_A,
                    values_C, col_handle_C, row_handle_C,
                    &nnz, &info);
    timings[i] = timer.get();

    mkl_free(values_C);
    mkl_free(col_handle_C);
  }

  mkl_free(row_handle_A);
  mkl_free(col_handle_A);
  mkl_free(values_A);

  std::cout << stl_A.size() << " " << get_median(timings) << " " << double(flops) * 1e-3 / get_median(timings) << std::endl;

  return EXIT_SUCCESS;
}

