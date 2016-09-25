#!/bin/bash

# Compile on host:
echo "# compilation command on host:"
echo "#   icc mkl.cpp -fopenmp -O3 -I${MKLROOT}/include -mmic -Wl,--start-group ${MKLROOT}/lib/mic/libmkl_intel_lp64.a ${MKLROOT}/lib/mic/libmkl_core.a ${MKLROOT}/lib/mic/libmkl_intel_thread.a -Wl,--end-group -lpthread -lm -o mkl_knc"
echo "# After compilation, run this script on the Xeon Phi"

# Run:
progname=mkl_knc

  ./$progname matrices/cant.mtx              539
  ./$progname matrices/mac_econ_fwd500.mtx    15
  ./$progname matrices/mc2depi.mtx            17
  ./$progname matrices/rma10.mtx             313
  ./$progname matrices/pdb1HYS.mtx            86
  ./$progname matrices/conf5_4-8x8-05.mtx    150 #complex arithmetic
  ./$progname matrices/shipsec1.mtx          901
  ./$progname matrices/consph.mtx            927
  ./$progname matrices/pwtk.mtx             1252

echo " "

  ./$progname matrices/cop20k_A.mtx          160
  ./$progname matrices/amazon0312.mtx         57
  ./$progname matrices/ca-CondMat.mtx          8
  ./$progname matrices/cit-Patents.mtx       164
  ./$progname matrices/scircuit.mtx           17
  ./$progname matrices/email-Enron.mtx       103
  ./$progname matrices/p2p-Gnutella31.mtx      1
  ./$progname matrices/roadNet-CA.mtx         35
  ./$progname matrices/webbase-1M.mtx        139
  ./$progname matrices/web-Google.mtx        121
  ./$progname matrices/wiki-Vote.mtx           9


