#include <stdio.h>
#include <sqlite3.h>

void Authentication(sqlite3* db);

void LogIn(sqlite3* db, int* user_type);

void LogInMenu(sqlite3* db, int* user_type);