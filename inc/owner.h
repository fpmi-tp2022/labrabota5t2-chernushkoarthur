#ifndef OWNER_H
#define OWNER_H


#include <string.h>
#include <sqlite3.h>

#include "hippodrome.h"

void owner_loop(sqlite3* db, const char* surname);
void owner_read_loop(sqlite3* db, const char* surname);

#endif // OWNER_H
