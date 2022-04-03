#include <stdio.h>
#include "../inc/hippodrome.h"

int main() {
  const char *db_path = "../bin/hippodrome.db";
  sqlite3 *db;
  int rc = sqlite3_open(db_path, &db);
  if (rc) {
	fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
	return 0;
  }
  int user_type;
  LogInMenu(db, &user_type);
  printf("%d", user_type);
  sqlite3_close(db);
  return 0;
}