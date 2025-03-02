set xlabel font "Helvetica,20"
set ylabel font "Helvetica,20"
set key font "Helvetica,16"
set title font "Helvetica,20"
set title "Rgemm on AMD Ryzen Threadripper 3970X 32-Core Processor "
set xlabel "Dimension"
set ylabel "MFLOPS"
set key above
set terminal pdf

plot \
"log.Rgemm.double_opt"	    using 1:4 title 'double (OpenMP)'        with lines linewidth 1, \
"log.Rgemm.double"          using 1:4 title 'double'                 with lines linewidth 1, \
"log.dgemm.ref"	            using 1:4 title 'double (Ref.BLAS)'      with lines linewidth 1, \
"log.dgemm.openblas"        using 1:4 title 'double (OpenBLAS)'      with lines linewidth 1
