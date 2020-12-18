#ifndef SQLITETOCSV_MAIN_H
#define SQLITETOCSV_MAIN_H

void help();
bool confirm(const char* db, const char* out, char delimiter, const char* sql);

#endif
