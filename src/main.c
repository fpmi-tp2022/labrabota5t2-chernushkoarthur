#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

#include "../inc/hippodrome.h"

#define _CRT_SECURE_NO_WARNINGS

int main() {
	const char* db_path = "hippodrome.db";
	sqlite3* db;
	int rc = sqlite3_open(db_path, &db);
	if (rc) {
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		return 0;
	}
	int user_type;
	char surname[30];
	main_loop(db, &user_type, surname);
	sqlite3_close(db);
	return 0;
}
