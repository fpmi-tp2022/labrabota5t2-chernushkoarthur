#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

void Authentication(sqlite3* db);

void LogIn(sqlite3* db, int* user_type, char* surname);

void LogInMenu(sqlite3* db, int* user_type, char* surname);