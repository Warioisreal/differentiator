#include <stdlib.h>

#include "gnuplot.h"

FILE* StartGnuplot(double height, double width, double dotX, double dotY) {
    const char* filename = "plot_script.gp";
    FILE* gp_script = fopen(filename, "wb");

    if (gp_script == nullptr) { printf("Ошибка открытия gnuplot.\n"); }

    fprintf(gp_script, "set title 'Функция, ряд Тейлора и производная'\n"); // заголовок

    fprintf(gp_script, "set terminal qt font \"Arial,12\"\n"); // настройки шрифта

    // названия осей
    fprintf(gp_script, "set xlabel 'X'\n");
    fprintf(gp_script, "set ylabel 'Y'\n");
    // размер окна по осям
    fprintf(gp_script, "set yrange [%lg:%lg]\n", -height + dotY, height + dotY);
    fprintf(gp_script, "set xrange [%lg:%lg]\n", -width + dotX, width + dotX);
    // добавление сетки
    fprintf(gp_script, "set grid\n");
    fprintf(gp_script, "plot \'-\' with points pointtype 7 pointsize 1 linecolor rgb \'magenta\' title \'Точка аппроксимации\', ");

    return gp_script;
}

void FinishGnuplot(FILE* gp_script) {
    fclose(gp_script);
}

void AddFuncGraphGnuplot(FILE* gp_script, const char* data_filename, const char* func_name, size_t color) {
    fprintf(gp_script, "'%s' lc rgb \"#%06zX\" with lines title '%s'", data_filename, color, func_name);
}

void WriteGnuplotCMD(FILE* gp_script, const char* command) {
    fprintf(gp_script, "%s", command);
}

void CreateGnuplotGraph(const char* script_filename) {
    char command[256];
    snprintf(command, 256, "gnuplot -persist %s", script_filename);

    system(command);
}

FILE* OpenDataGnuplotFile(const char* filename) {
    FILE* file = fopen(filename, "wb");

    if (file == nullptr) { printf("Ошибка открытия: %s\n", filename); }

    return file;
}

void CloseDataGnuplotFile(FILE* file) {
    fclose(file);
}

void AddDotToDataGnuplotFile(FILE* file, double argument, double value) {
    fprintf(file, "%lg %lg\n", argument, value);
}

void UpdateGPDataFolder(const char* folder_name) {
    char command[GP_COMMAND_SIZE] = "";
    snprintf(command, GP_COMMAND_SIZE, "rm -r %s", folder_name);
    system(command);
    snprintf(command, GP_COMMAND_SIZE, "mkdir %s", folder_name);
    system(command);
}

void BuildGPDataFilePath(char* buffer, const char* folder_name, const char* filename) {
    snprintf(buffer, GP_DATA_FILENAME_LEN, "%s/%s", folder_name, filename);
}
