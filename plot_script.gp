set title 'Функция, ряд Тейлора и производная'
set terminal qt font "Arial,12"
set xlabel 'X'
set ylabel 'Y'
set yrange [-1.5:1.5]
set xrange [-5:5]
set grid
plot 'gp_data/function_data.txt' lc rgb "#FF0000" with lines title 'Функция', 'gp_data/derivative_data.txt' lc rgb "#00FF00" with lines title 'Производная', 'gp_data/taylor_srs_data.txt' lc rgb "#0000FF" with lines title 'ряд Тейлора'
