#ifndef ADMIN_H
#define ADMIN_H

#include "hippodrome.h"
#include <sqlite3.h>
#include <string.h>

void admin_loop(sqlite3* db);
void admin_read_loop(sqlite3* db);
void admin_misc_loop(sqlite3* db); // Best horse, best jockey etc. (Select 1, 2)

#endif // ADMIN_H
