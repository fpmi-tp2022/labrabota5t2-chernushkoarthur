#ifndef OWNER_H
#define OWNER_H


#include <string.h>
#include "hippodrome.h"
#include <sqlite3.h>

void owner_loop(sqlite3* db, const char* surname);
void owner_read_loop(sqlite3* db, const char* surname);

#endif // OWNER_H
