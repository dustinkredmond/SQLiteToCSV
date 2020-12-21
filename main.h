#ifndef SQLITETOCSV_MAIN_H
#define SQLITETOCSV_MAIN_H

#include "sqlite3.h"

void help();
int confirm(const char* db, const char* out, char delimiter, const char* sql);
void writeString(const char *value, char delimiter, FILE *file);
int exportDb(char delim, const char *db, const char *out, const char *sql);
char *error = "";

#endif
