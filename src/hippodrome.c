#include "../inc/hippodrome.h"

static int callback_user_type(void *data, int argc, char **argv, char **azColName) {
  int i;
  for (i = 0; i < argc; i++) {
	printf("%s ", argv[i] ? argv[i] : "(null)");
  }
  printf("\n");
  return 0;
}

static int callback(void *data, int argc, char **argv, char **azColName) {
  int i;

  for (i = 0; i < argc; i++) {
	printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "(null)");
  }

  printf("\n");
  return 0;
}

static void SQL_Error(int rc, char *zErrMsg) {
  if (rc != SQLITE_OK) {
	fprintf(stderr, "SQL error: %s\n", zErrMsg);
	sqlite3_free(zErrMsg);
  }
}

void Authentication(sqlite3 *db) {
  char *zErrMsg = 0;
  char *sql;
  printf("Choose user type:\n");
  sql = "SELECT * FROM user_type";
  int rc;
  rc = sqlite3_exec(db, sql, callback_user_type, 0, &zErrMsg);
  SQL_Error(rc, zErrMsg);
  int user_type;
  scanf("%d", &user_type);
  printf("Enter your surname:\n");
  char surname[30];
  scanf("%s", surname);
  printf("Enter your password:\n");
  char password[30];
  scanf("%s", password);
  sql = "INSERT INTO users (user_type, surname, password) VALUES (?, ?, ?);";

  sqlite3_stmt *res;
  rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

  if (rc == SQLITE_OK) {
	sqlite3_bind_int(res, 1, user_type);
	sqlite3_bind_text(res, 2, surname, -1, 0);
	sqlite3_bind_text(res, 3, password, -1, 0);
  } else {
	fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
  }

  sqlite3_step(res);
  sqlite3_finalize(res);
}

void LogIn(sqlite3 *db, int *user_type, char *surname) {
  char password[30];
  char *sql;
  int rc;
  while (1) {
	printf("Enter your surname:\n");
	scanf("%s", surname);
	printf("Enter your password:\n");
	scanf("%s", password);
	sql = "SELECT * FROM users WHERE surname = ? and password = ?;";
	sqlite3_stmt *res;
	rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

	if (rc == SQLITE_OK) {
	  sqlite3_bind_text(res, 1, surname, -1, 0);
	  sqlite3_bind_text(res, 2, password, -1, 0);
	} else {
	  fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
	}

	int step = sqlite3_step(res);
	if (step != SQLITE_ROW) {
	  printf("Wrong surname or password. Try again.\n");
	  sqlite3_finalize(res);
	  continue;
	}
	*user_type = sqlite3_column_int(res, 1);
	sqlite3_finalize(res);
	return;
  }
}

void LogInMenu(sqlite3 *db, int *user_type, char *surname) {
  while (1) {
	printf("Log In:\n");
	printf("1. Log In\n");
	printf("2. Authentication\n");
	int choice;
	scanf("%d", &choice);
	switch (choice) {
	  case 1: {
		LogIn(db, user_type, surname);
		return;
	  }
	  case 2: {
		Authentication(db);
		break;
	  }
	  default: {
		printf("Wrong input. Try again.\n");
		break;
	  }
	}
  }
}

void Select2(sqlite3 *db) {
  char *zErrMsg = 0;
  char *sql = "SELECT id, surname, experience, birthday, address, races_number FROM jockeys\n"
			  "INNER JOIN (SELECT jockey_id, max(races_count) as races_number "
			  "FROM (SELECT jockey_id, count(jockey_id) as races_count FROM races GROUP BY jockey_id))\n"
			  "WHERE jockey_id=jockeys.id;";
  int rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
  SQL_Error(rc, zErrMsg);
}