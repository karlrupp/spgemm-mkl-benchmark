
# Sparse Matrix-Matrix Multiplication Benchmark Code for Intel Xeon and Xeon Phi

This repository contains the benchmark code supplementing my [blog post on a matrix-matrix multiplication benchmark on Intel Xeon and Xeon Phi](https://www.karlrupp.net/2016/09/sparse-matrix-matrix-multiplication-on-intel-xeon-and-xeon-phi-knc-knl/).


## Build and Run

The following instructions are for *nix-based systems.
The code should also work on Windows, but is not tested.

First run the script

    $> ./init.sh

to download and extract the sparse matrices.
This will pull about 350MB of data and thus may take a while.

Second, run the executables via

    $> ./run_mkl_xeon
    $> ./run_mkl_knl 0    # for DDR4
    $> ./run_mkl_knl 1    # for MCDRAM
    $> ./run_mkl_knc

This assumes that your Intel compiler variables are set accordingly.
Note that for the Xeon Phi KNC you have to cross-compile on the host and run the script on the device.

The folder results/ contains the results I obtained on different systems.
Feel free to paste your results in there in replot the graphs via

    $> Rscript plot.R

The free open source statistics package R is required for the plots.

## License

The code is provided under a permissive MIT/X11-style license.
See file LICENSE.txt for details.

