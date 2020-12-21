#include <string.h>
#include <stdio.h>
#include "main.h"
#include "sqlite3.h"
#define MAX_ERR_LEN 1024


/**
 * Processes arguments and calls exportDb(), which
 * does the actual processing.
 */
int main(int argc, char **argv) {
    const char* db = "";   // path to SQLite db
    const char* out = "";  // output file path
    char delimiter = ',';  // delimiter (default comma)
    const char* sql = "";  // SQL query to retrieve CSV data
    int noPrompt = 0;

    // process arguments
    for (int arg = 0; arg < argc; arg++) {
        if (strcmp(argv[arg], "-help") == 0) {
            help();
            return 0;
        }
        if (strcmp(argv[arg], "-db") == 0) {
            db = argv[arg + 1];
        }
        if (strcmp(argv[arg], "-out") == 0) {
            out = argv[arg + 1];
        }
        if (strcmp(argv[arg], "-delimiter") == 0) {
            delimiter = *argv[arg + 1];
        }
        if (strcmp(argv[arg], "-sql") == 0) {
            sql = argv[arg + 1];
        }
        if (strcmp(argv[arg], "-no-prompt") == 0) {
            noPrompt = 1;
        }
    }

    if (!strcmp(db,"") || !strcmp(out,"") || !strcmp(sql,"")) {
        printf("Error: parameters -db, -out, and -sql are required.\n");
        printf("Enter \"-help\" for usage.\n");
        return 0;
    }

    if (!noPrompt) {
        if (!confirm(db,out,delimiter,sql)) {
            printf("Exiting program.\n");
            return 0;
        }
    }

    printf("Processing SQLite database...\n");

    if (exportDb(delimiter, db, out, sql)) {
        printf("File written to: %s\n", out);
        return 0;
    } else {
        printf("Error writing file: %s\n", error);
        return -1;
    }
}

bool confirm(const char* db, const char* out, char delimiter, const char* sql) {
    printf("Found parameters:\n"
           "\tDB: %s\n"
           "\tOutput: %s\n"
           "\tDelimiter: %c\n"
           "\tSQL: %s\n"
           "\nOkay to continue? (Y/N)\n",
           db,out,delimiter,sql);
    int c = getchar();
    if (c == 'Y' || c == 'y') {
        return true;
    }
    return false;
}

void help() {
    printf("SQLiteToCsv - Usage\n"
           "Parameters:\n"
           "\t-db          - Path to SQLite database\n"
           "\t-out         - Path to export CSV file\n"
           "\t-delimiter   - Delimiter to use in CSV file (optional)\n"
           "\t-sql         - SQL query to retrieve from database\n"
           "\t-no-prompt   - Does not prompt the user to continue\n"
           "\t-help        - Print information seen here\n");
}

bool exportDb(char delim, const char *db, const char *out, const char *sql) {
    sqlite3 *sqlite;
    FILE *file = fopen(out, "w");
    if (file == NULL) {
        sprintf(error, "Could not open output file (%s) for writing.\n", out);
        return false;
    }
    if (fopen(db,"r") == NULL) {
        sprintf(error, "Could not open database (%s) for reading.\n", db);
        return false;
    }

    int rc = sqlite3_open(db, &sqlite);
    if (rc) {
        strncpy(error, sqlite3_errmsg(sqlite), MAX_ERR_LEN);
        sqlite3_close(sqlite);
        return false;
    }

    int sqlLength = strlen(sql);
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(sqlite, sql, sqlLength, &stmt, 0);
    if (rc != SQLITE_OK) {
        strncpy(error, sqlite3_errmsg(sqlite), MAX_ERR_LEN);
        sqlite3_close(sqlite);
        return false;
    }

    if (!file) {
        sprintf(error, "Error: Could not open file \"%s\"\n", out);
        sqlite3_finalize(stmt);
        sqlite3_close(sqlite);
        return false;
    }

    int row;
    int columnCount = sqlite3_column_count(stmt);
    for (int col = 0; col < columnCount; col++) {
        if (col > 0) {
            fprintf(file, "%c", delim);
        }
        const char* colName = sqlite3_column_name(stmt, col);
        writeString(colName, delim, file);
    }
    fprintf(file, "\n");

    // each field
    row = 1;
    do {
        rc = sqlite3_step(stmt);

        if (rc == SQLITE_ROW) {
            for (int col = 0; col < columnCount; col++) {
                if (col > 0) {
                    fprintf(file, "%c", delim);
                }

                int colType = sqlite3_column_type(stmt, col);
                switch (colType) {
                    case SQLITE_INTEGER:
                        fprintf(file, "%i", sqlite3_column_int(stmt, col));
                        break;
                    case SQLITE_FLOAT:
                        fprintf(file, "%g", sqlite3_column_double(stmt, col));
                        break;
                    case SQLITE_TEXT:
                        writeString((const char*) sqlite3_column_text(stmt, col), delim, file);
                        break;
                    case SQLITE_BLOB:
                        writeString("<BLOB format not supported>", delim, file);
                        break;
                    default:
                        // empty field
                        break;
                }
            }
            fprintf(file, "\n");
        }
        row++;
    } while (rc != SQLITE_DONE);

    if (file != NULL) {
        fclose(file);
        file = NULL;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(sqlite);
    return true;
}


void writeString(const char *value, char delimiter, FILE *file) {
    const char *v = value;
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
