set xlabel font "Helvetica,20"
set ylabel font "Helvetica,20"
set key font "Helvetica,16"
set title font "Helvetica,20"
set title "Rsyrk on AMD Ryzen Threadripper 3970X 32-Core Processor "
set xlabel "Dimension"
set ylabel "MFLOPS"
set key above
set terminal pdf

plot \
"log.Rsyrk.double_opt"	    using 1:3 title 'double (OpenMP)'        with lines linewidth 1, \
"log.Rsyrk.double"          using 1:3 title 'double'                 with lines linewidth 1, \
"log.dsyrk.ref"	            using 1:3 title 'double (Ref.BLAS)'      with lines linewidth 1, \
"log.dsyrk.openblas"        using 1:3 title 'double (OpenBLAS)'      with lines linewidth 1
