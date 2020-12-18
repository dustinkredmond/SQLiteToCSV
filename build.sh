# Include SQLite amalgamation and header in build, not provided in this repo
# Builds standalone executable
gcc -c sqlite3.c -ldl -lpthread -o sqlite3.o && g++ -c main.cpp -o main.o && g++ main.o sqlite3.o -ldl -lpthread -o SQLiteToCsv

