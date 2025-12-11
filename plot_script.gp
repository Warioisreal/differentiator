set title 'Функция, ряд Тейлора и производная'
set terminal qt font "Arial,12"
set xlabel 'X'
set ylabel 'Y'
set yrange [-3.98769:2.01231]
set xrange [-4.5:5.5]
set grid
plot '-' with points pointtype 7 pointsize 1 linecolor rgb 'magenta' title 'Точка аппроксимации', 'gp_data/function_data.txt' lc rgb "#FF0000" with lines title 'Функция', 'gp_data/derivative_data.txt' lc rgb "#00FF00" with lines title 'Касательная', 'gp_data/taylor_srs_data.txt' lc rgb "#0000FF" with lines title 'Ряд Тейлора'
0.5 -0.987685
e
