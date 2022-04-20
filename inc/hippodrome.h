#ifndef HIPPODROME_H
#define HIPPODROME_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

#include "admin.h"
#include "owner.h"
#include "jockey.h"


enum user_type {owner = 1, jockey = 2, admin = 3};

void Authentication(sqlite3* db);

void LogIn(sqlite3* db, int* user_type, char* surname);

void main_loop(sqlite3* db, int* user_type, char* surname);

void Select1	(sqlite3* db, const char* surname);
void Select2	(sqlite3* db);
void Select3	(sqlite3* db, const char* surname);
void Select4	(sqlite3* db, const char* surname);
void Select5	(sqlite3* db);
void Select6	(sqlite3* db, const char* jockey);

void Insert	(sqlite3* db);
void Update	(sqlite3* db);
void Delete	(sqlite3* db);

int protected_horse_id	(sqlite3* db);
int protected_jockey_id	(sqlite3* db);

void divide_prize (double prize);

#endif // HIPPODROME_H 
