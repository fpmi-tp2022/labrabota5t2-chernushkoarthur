#ifndef ADMIN_H
#define ADMIN_H

#include <string.h>
#include <sqlite3.h>

#include "hippodrome.h"


void admin_loop(sqlite3* db);
void admin_misc_loop(sqlite3* db); // Best horse, best jockey etc. (Select 1, 2)

#endif // ADMIN_H
