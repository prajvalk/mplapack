set xlabel font "Helvetica,20"
set ylabel font "Helvetica,20"
set key font "Helvetica,16"
set title font "Helvetica,20"
set title "Rdot on Intel(R) Core(TM) i5-8500B CPU @ 3.00GHz "
set xlabel "Dimension"
set ylabel "MFLOPS"
#set terminal postscript eps color enhanced
set terminal pdf

plot \
"log.Rdot.mpfr"            using 1:2 title 'MPFR 512bit'            with lines linewidth 1, \
"log.Rdot.mpfr_opt"        using 1:2 title 'MPFR 512bit(OpenMP)'    with lines linewidth 1, \
"log.Rdot.gmp"             using 1:2 title 'GMP  512bit'            with lines linewidth 1, \
"log.Rdot.gmp_opt"         using 1:2 title 'GMP  512bit(OpenMP)'    with lines linewidth 1, \
"log.Rdot.qd"              using 1:2 title 'quad-double'            with lines linewidth 1, \
"log.Rdot.qd_opt"          using 1:2 title 'quad-double(OpenMP)'    with lines linewidth 1
