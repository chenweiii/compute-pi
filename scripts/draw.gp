set title "Execution time plot"
set xlabel "N"
set ylabel "Execution time(s)"
set terminal png font "Times_New_Roman,12"
set output "statistic.png"

plot \
"output.txt" using 1:2 with linespoints linewidth 2 title "BASELINE", \
"output.txt" using 1:3 with linespoints linewidth 2 title "PTHREAD_2"
