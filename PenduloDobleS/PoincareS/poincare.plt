set terminal pngcairo size 800,600 enhanced font 'Arial,12'
set output 'poincare.png'

set title 'Sección de Poincaré'
set xlabel 'θ1'
set ylabel 'P1'
set palette rgbformulae 33,13,10
unset colorbox

plot 'poincare.dat' using 1:2:1 with points pt 7 ps 0.3 palette notitle
