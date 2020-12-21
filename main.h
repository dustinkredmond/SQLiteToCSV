#ifndef SQLITETOCSV_MAIN_H
#define SQLITETOCSV_MAIN_H

#include <stdbool.h>
#include "sqlite3.h"

void help();
bool confirm(const char* db, const char* out, char delimiter, const char* sql);
void writeString(const char *value, char delimiter, FILE *file);
bool exportDb(char delim, const char *db, const char *out, const char *sql);

#endif
