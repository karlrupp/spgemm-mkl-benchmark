#!/bin/bash

# Compile:
icc mkl.cpp -fopenmp -O3 -I${MKLROOT}/include -L${MKLROOT}/lib/intel64 -lmkl_intel_lp64 -lmkl_core -lmkl_intel_thread -lpthread -lm -o mkl_knl

# Run (pass argument 0 for DDR4, argument 1 for MCDRAM):
progname=mkl_knl

  numactl --membind $1 ./$progname matrices/cant.mtx              539
  numactl --membind $1 ./$progname matrices/mac_econ_fwd500.mtx    15
  numactl --membind $1 ./$progname matrices/mc2depi.mtx            17
  numactl --membind $1 ./$progname matrices/rma10.mtx             313
  numactl --membind $1 ./$progname matrices/pdb1HYS.mtx            86
  numactl --membind $1 ./$progname matrices/conf5_4-8x8-05.mtx    150 #complex arithmetic
  numactl --membind $1 ./$progname matrices/shipsec1.mtx          901
  numactl --membind $1 ./$progname matrices/consph.mtx            927
  numactl --membind $1 ./$progname matrices/pwtk.mtx             1252

echo " "

  numactl --membind $1 ./$progname matrices/cop20k_A.mtx          160
  numactl --membind $1 ./$progname matrices/amazon0312.mtx         57
  numactl --membind $1 ./$progname matrices/ca-CondMat.mtx          8
  numactl --membind $1 ./$progname matrices/cit-Patents.mtx       164
  numactl --membind $1 ./$progname matrices/scircuit.mtx           17
  numactl --membind $1 ./$progname matrices/email-Enron.mtx       103
  numactl --membind $1 ./$progname matrices/p2p-Gnutella31.mtx      1
  numactl --membind $1 ./$progname matrices/roadNet-CA.mtx         35
  numactl --membind $1 ./$progname matrices/webbase-1M.mtx        139
  numactl --membind $1 ./$progname matrices/web-Google.mtx        121
  numactl --membind $1 ./$progname matrices/wiki-Vote.mtx           9


