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
  char surname[30];
  LogInMenu(db, &user_type, surname);
  printf("%d\n", user_type);
  printf("%s\n", surname);
  sqlite3_close(db);
  return 0;
}