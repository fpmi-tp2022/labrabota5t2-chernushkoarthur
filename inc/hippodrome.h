#ifndef HIPPODROME_H
#define HIPPODROME_H

#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

#include "admin.h"
#include "owner.h"
#include "jockey.h"


enum user_type {owner = 1, jockey = 2, admin = 3};

void Authentication(sqlite3* db);

void LogIn(sqlite3* db, int* user_type, char* surname);

void LogInLoop(sqlite3* db, int* user_type, char* surname);

void Select1(sqlite3* db, const char* surname);
void Select2(sqlite3* db);
void Select3(sqlite3* db, const char* surname);
void Select4(sqlite3* db, const char* surname);
void Select5(sqlite3* db);

void Insert(sqlite3* db);
void Update(sqlite3* db);
void Delete(sqlite3 *db);




#endif // HIPPODROME_H 