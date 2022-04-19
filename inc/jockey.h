#ifndef JOCKEY_H
#define JOCKEY_H

#include <string.h>
#include "hippodrome.h"
#include <sqlite3.h>


void jockey_loop(sqlite3* db, const char *surname);
void jockey_read_loop(sqlite3* db, const char* surname);


#endif // JOCKEY_H
