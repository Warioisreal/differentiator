#ifndef _GNUPLOT_H_
#define _GNUPLOT_H_

#include <stdio.h>

const size_t GP_DATA_FILENAME_LEN = 100;
const size_t GP_COMMAND_SIZE = 100;

FILE* StartGnuplot(double height, double width, double dotX, double dotY);
void FinishGnuplot(FILE* gp_script);
void AddFuncGraphGnuplot(FILE* gp_script, const char* data_filename, const char* func_name, size_t color);
void WriteGnuplotCMD(FILE* gp_script, const char* command);
void CreateGnuplotGraph(const char* script_filename);
FILE* OpenDataGnuplotFile(const char* filename);
void CloseDataGnuplotFile(FILE* file);
void AddDotToDataGnuplotFile(FILE* file, double argument, double value);
void UpdateGPDataFolder(const char* folder_name);
void BuildGPDataFilePath(char* buffer, const char* folder_name, const char* filename);

#endif //_GNUPLOT_H_
