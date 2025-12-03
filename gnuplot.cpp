#include "gnuplot.h"

FILE* StartGnuplot() {
    const char* filename = "plot_script.gp";
    FILE* gp_script = fopen(filename, "wb");

    if (gp_script == nullptr) { printf("Ошибка открытия gnuplot.\n"); }

    fprintf(gp_script, "set title 'Функция, ряд Тейлора и производная'\n"); // заголовок
    // названия осей
    fprintf(gp_script, "set xlabel 'X'\n");
    fprintf(gp_script, "set ylabel 'Y'\n");
    // размер окна по осям
    fprintf(gp_script, "set yrange [-1.5:1.5]\n");
    fprintf(gp_script, "set xrange [-5:5]\n");
    // добавление сетки
    fprintf(gp_script, "set grid\n");
}

void FinishGnuplot(FILE* gp_script) {
    fprintf(gp_script, "pause -1 'Нажмите любую клавишу для выхода...'\n");

    fclose(gp_script);
}

void AddFuncGraphGnuplot(FILE* gp_script, char* data_filename, char* func_name) {
    fprintf(gp_script, "'%s' with lines title '%s'\n", data_filename, func_name);
}

void CreateGnuplotGraph(char* script_filename) {
    char command[256];
    sprintf(command, "gnuplot %s", script_filename);

    system(command);
}

FILE* OpenDataGnuplotFile(const char* filename, double** data_array) {
    FILE* file = fopen(filename, "wb");

    if (file == nullptr) { printf("Ошибка открытия: %s\n", filename); }
}

void CloseDataGnuplotFile(FILE* file) {
    fclose(file);
}

void AddDotToDataGnuplotFile(FILE* file, double argument, double value) {
    fprintf(file, "%lg %lg\n", argument, value);
}
