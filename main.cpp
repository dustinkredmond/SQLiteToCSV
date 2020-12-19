#include <cstdio>
#include <cstring>
#include "SQLiteDelimitedExport.h"
#include "main.h"


/**
 * Processes arguments and uses to create an instance of
 * SQLiteDelimitedExport class, which does the actual
 * processing.
 */
int main(int argc, char **argv) {
    const char* db = "";   // path to SQLite db
    const char* out = "";  // output file path
    char delimiter = ',';  // delimiter (default comma)
    const char* sql = "";  // SQL query to retrieve CSV data
    bool noPrompt = false;

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
            noPrompt = true;
        }
    }

    if (!strcmp(db,"") || !strcmp(out,"") || !strcmp(sql,"")) {
        printf("Error: parameters -db, -out, and -sql are required..\n");
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
    SQLiteDelimitedExport* exporter;
    exporter = new SQLiteDelimitedExport(delimiter, db, out, sql);

    exporter->exportDb() ?
    printf("File written to: %s", out) : printf("Error: %s", exporter->err());
    delete exporter;
    return 0;
}

bool confirm(const char* db, const char* out, char delimiter, const char* sql) {
    printf("Found parameters:\n"
           "\tDB: %s\n"
           "\tOutput: %s\n"
           "\tDelimiter: %c\n"
           "\tSQL: %s\n"
           "\nOkay to continue? (Y/N)\n",
           db,out,delimiter,sql);
    int c;
    c = getchar();
    return c == 'Y' || c == 'y';
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
