## SQLiteToCSV

Provides the means to copy SQLite query results to a CSV file with your 
choice of delimiter. I originally wrote this program in about 30 lines of Python code, but found
that for extremely large SQLite databases I wasn't getting the performance that I needed. Hence this...


---

### Building

If building from source, ensure that you have GCC and G++ installed. Next,
install the SQLite library. If using the `apt` package manager, this is
accomplished by the following command.

```
sudo apt install libsqlite3-dev
```

Next, clone the project from GitHub and change directory.

```
git clone https://github.com/dustinkredmond/SQLiteToCSV && cd SQLiteToCSV
```

To compile from source, run the following (requires CMake and Make)

```
cmake . && make
```

An executable will be generated, run it as so.

```
./SQLiteToCsv
```

---

### Command line options

- -db - Path to SQLite database
- -out - Path to export CSV file
- -delimiter - Delimiter to use in CSV file (optional) defaults to comma if not provided
- -sql - SQL query to retrieve from database
- -prompt - Prompt the user to continue (doesn't by default)
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

---

### Not supported

BLOBs or other binary objects. Instead, the string '&lt;BLOB format not supported&gt;' will be written.
