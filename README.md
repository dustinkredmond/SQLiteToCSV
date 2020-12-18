## SQLiteToCSV

Provides the means to copy SQLite query results to a CSV file with your 
choice of delimiter. I originally wrote this program in about 30 lines of Python code, but found
that for extremely large SQLite databases I wasn't getting the performance that I needed. Hence this...


Compiled and tested with GCC and G++. Include the `sqlite.c` and `sqlite.h` amalgamations
from the SQLite website to make this a standalone executable. Compiling with the SQLite amalgamations
offer a drastic performance benefit as well, since the compiler can do all sorts of crazy optimizations.

---

### Command line options

- -db - Path to SQLite database
- -out - Path to export CSV file
- -delimiter - Delimiter to use in CSV file (optional) defaults to comma if not provided
- -sql - SQL query to retrieve from database
- -no-prompt - Does not prompt the user to continue
- -help - Print information seen here

---

### Usage

Let's say we have a database called `Test.db` in the current working directory.
`Test.db` has a table `EMPLOYEE`. If we wanted a subset of the records in this
table we could make a call to SQLiteToCSV like below.

```shell
./SQLiteToCSV -db ./Test.db -out ./Test.csv -sql "SELECT * FROM EMPLOYEE WHERE DEPARTMENT_ID = 15"
```

This would in turn run the query against the SQLite database, and pop out a `Test.csv` file
in the directory specified.