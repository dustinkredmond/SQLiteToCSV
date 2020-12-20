#include "SQLiteDelimitedExport.h"

/**
 * Constructor for building delimited SQLite exports
 * @param delim The delimiter to be used
 * @param database Path to the SQLite database
 * @param output  Path to the output file
 * @param sqlQuery A SQL query to retrieve data from SQLite
 */
SQLiteDelimitedExport::SQLiteDelimitedExport(char delim, const char *database,
                                             const char *output, const char *sqlQuery) {
    delimiter = delim;
    db = database;
    out = output;
    sql = sqlQuery;
    strcpy(error, "");
    file = nullptr;
}

/**
 * On destroy, close file
 */
SQLiteDelimitedExport::~SQLiteDelimitedExport() {
    closeFile();
}

/**
 * The exports the results returned by SQL into a CSV
 * file based on parameters passed in the constructor.
 * @return Returns true if the operation was successful, else false.
 */
bool SQLiteDelimitedExport::exportDb() {
    sqlite3 *sqlite;
    int rc;

    if (!existsFile(db)) {
        sprintf(error, "Database \"%s\" does not exist.\n", db);
        return false;
    }

    rc = sqlite3_open(db, &sqlite);
    if (rc) {
        strncpy(error, sqlite3_errmsg(sqlite), MAX_ERR_LEN);
        sqlite3_close(sqlite);
        return false;
    }

    int sqlLength = strlen(sql);
    sqlite3_stmt *stmt = nullptr;
    rc = sqlite3_prepare_v2(sqlite, sql, sqlLength, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        strncpy(error, sqlite3_errmsg(sqlite), MAX_ERR_LEN);
        sqlite3_close(sqlite);
        return false;
    }

    bool open = openFile(out);
    if (!open) {
        sprintf(error, "Error: Could not open file \"%s\"\n", out);
        sqlite3_finalize(stmt);
        sqlite3_close(sqlite);
        return false;
    }

    int row;
    int columnCount = sqlite3_column_count(stmt);
    for (int col = 0; col < columnCount; col++) {
        if (col > 0) {
            writeDelimiter();
        }
        const char* colName = sqlite3_column_name(stmt, col);
        writeColumnName(colName);
    }
    writeNewLine();

    // each field
    row = 1;
    do {
        rc = sqlite3_step(stmt);

        if (rc == SQLITE_ROW) {
            for (int col = 0; col < columnCount; col++) {
                if (col > 0) {
                    writeDelimiter();
                }

                int colType = sqlite3_column_type(stmt, col);
                switch (colType) {
                    case SQLITE_INTEGER:
                        writeInt(sqlite3_column_int(stmt, col));
                        break;
                    case SQLITE_FLOAT:
                        writeDouble(sqlite3_column_double(stmt, col));
                        break;
                    case SQLITE_TEXT:
                        writeString((const char*) sqlite3_column_text(stmt, col));
                        break;
                    case SQLITE_BLOB:
                        writeString("<BLOB format not supported>");
                        break;
                        // SQLITE_NULL or nothing
                    default:
                        writeEmpty();
                        break;
                }
            }
            writeNewLine();
        }
        row++;
    } while (rc != SQLITE_DONE);
    closeFile();

    sqlite3_finalize(stmt);
    sqlite3_close(sqlite);
    return true;
}


/**
 * Attempts to open a file handle, returns true if successful.
 * @param filename Path to the file
 * @return True if file handle does not equal nullptr.
 */
bool SQLiteDelimitedExport::openFile(const char* filename) {
    file = fopen (filename,"w");
    return (file != nullptr);
}


/**
 * Checks whether a file exists
 * @param filename  File to check
 * @return True if fopen() doesn't return nullptr
 */
bool SQLiteDelimitedExport::existsFile(const char* filename) {
    FILE* temp = fopen (filename, "r");
    if (temp != nullptr) {
        fclose(temp);
        return true;
    }
    return false;
}

/**
 * Attempts to close the currently opened file
 */
void SQLiteDelimitedExport::closeFile() {
    if (file != nullptr) {
        fclose(file);
        file = nullptr;
    }
}

/**
 * Returns the most recent SQLite error
 * @return SQLite error
 */
const char* SQLiteDelimitedExport::err() const {
    return error;
}

/**
 * Iterates through the characters in the value and
 * writes them to the currently opened file.
 * @param value A value to write to the output file
 */
void SQLiteDelimitedExport::writeString(const char* value) {
    const char* v = value;
    while (*v != 0) {
        if (*v == delimiter) {
            putc('\\', file);
            putc(delimiter, file);
        } else if (*v == '\n') {
            putc('\\', file);
            putc('n', file);
        } else {
            putc(*v, file);
        }
        v++;
    }
}

void SQLiteDelimitedExport::writeDouble(const double value) {
    fprintf(file, "%g", value);
}

void SQLiteDelimitedExport::writeInt(const int value) {
    fprintf(file, "%i", value);
}

void SQLiteDelimitedExport::writeColumnName(const char *value) {
    writeString(value);
}

void SQLiteDelimitedExport::writeNewLine() {
    fprintf(file, "\n");
}

void SQLiteDelimitedExport::writeDelimiter() {
    fprintf(file, "%c", delimiter);
}

