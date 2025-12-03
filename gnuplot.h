#ifndef _GNUPLOT_H_
#define _GNUPLOT_H_

FILE* StartGnuplot();
void FinishGnuplot(FILE* gp_script);
void AddFuncGraphGnuplot(FILE* gp_script, char* data_filename, char* func_name);
void CreateGnuplotGraph(char* script_filename);
FILE* OpenDataGnuplotFile(const char* filename, double** data_array)
void CloseDataGnuplotFile(FILE* file);
void AddDotToDataGnuplotFile(FILE* file, double argument, double value);

#endif //_GNUPLOT_H_
