#ifndef SQLITE_DELIMITED_EXPORT_H
#define SQLITE_DELIMITED_EXPORT_H

#define MAX_ERR_LEN 1024

#include <cstring>
#include <string>
#include "sqlite3.h"

/**
 * Class responsible for producing delimited files
 * from SQLite tables
 */
class SQLiteDelimitedExport {

public:
    SQLiteDelimitedExport(char delim, const char *database, const char *output, const char *sqlQuery);
    ~SQLiteDelimitedExport();
    bool exportDb();
    const char* err() const;

private:
    bool openFile(const char* filename);
    void closeFile();
    void writeDelimiter() {
        fprintf(file, "%c", delimiter);
    }
    void writeNewLine() {
        fprintf(file, "\n");
    }
    void writeColumnName(const char *value) {
        writeString(value);
    }
    void writeInt(const int value) {
        fprintf(file, "%i", value);
    }
    void writeDouble(const double value) {
        fprintf(file, "%g", value);
    }
    void writeEmpty() { }
    void writeString(const char* value);

protected:
    char delimiter;
    const char *db;
    const char *out;
    const char *sql;
    FILE* file{};
    char error[MAX_ERR_LEN]{};
    static bool existsFile(const char* filename);
};

#endif
