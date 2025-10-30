# =============================
#   Script: lyapunov_plot.gp
#   Archivo de datos: lyapunov.dat
# =============================

set terminal pngcairo size 1200,800 enhanced font 'Arial,14'
set output "lyapunov.png"

set title "Exponentes de Lyapunov con β=0.2"
set xlabel "Tiempo"
set ylabel "λ(t)"
set grid

# Paleta de colores tipo arcoíris
set palette rgbformulae 33,13,10
unset colorbox

# Si deseas líneas gruesas y suaves
set style line 1 lc rgb "blue" lw 2
set style line 2 lc rgb "red" lw 2
set style line 3 lc rgb "green" lw 2

# En caso de múltiples columnas (ej: tiempo, λ1, λ2, λ3)
# Si tu archivo tiene una sola columna, deja solo la primera línea del plot
 
plot "lyapunov.dat" using 1:4 with lines ls 3 title "λ/t"

unset output

