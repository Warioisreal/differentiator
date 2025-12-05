#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <stdio.h>

#include "dot.h"
#include "tree.h"


static const char* LOGS_DIR = "logs";
static const char* LOG_FOLDER_PREFIX = "log_folder";
static const size_t LOG_FOLDER_NAME_SIZE = 100;

static const char* DUMP_PREFIX = "dump";
static const size_t LOG_FILE_NAME_SIZE = 100;

static const size_t COMMAND_SIZE = 1000;

static const size_t LOG_MESSAGE_SIZE = 100;


struct LOG {
    FILE* file_log    = nullptr;
    size_t dump_count = 0;
    char name[LOG_FILE_NAME_SIZE] = "";
};


void GetFullFolderName(const char* list_name, char* buffer);
void UpdateLogsFolder();
void UpdateFolder(const char* name_folder);
void StartLog(FILE** file, const char* list_name);
void FinishLog(FILE** file);
void GoLog(Node_t* root, size_t size, Node_t* node, const char* message, LOG* log);

#endif //_LOGGER_H_
