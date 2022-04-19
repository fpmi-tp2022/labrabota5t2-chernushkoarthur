#include "../inc/hippodrome.h"

static int callback_user_type(void* data, int argc, char** argv, char** azColName) {
	int i;
	for (i = 0; i < argc; i++) {
		printf("%s ", argv[i] ? argv[i] : "(null)");
	}
	printf("\n");
	return 0;
}

static int callback(void* data, int argc, char** argv, char** azColName) {
	int i;

	for (i = 0; i < argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "(null)");
	}

	printf("\n");
	return 0;
}

static void SQL_Error(int rc, char* zErrMsg) {
	if (rc != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
}

void Authentication(sqlite3* db) {
	char* zErrMsg = 0;
	char* sql;
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

	sqlite3_stmt* res;
	rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

	if (rc == SQLITE_OK) {
		sqlite3_bind_int(res, 1, user_type);
		sqlite3_bind_text(res, 2, surname, -1, 0);
		sqlite3_bind_text(res, 3, password, -1, 0);
	}
	else {
		fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
	}

	sqlite3_step(res);
	sqlite3_finalize(res);
}

void LogIn(sqlite3* db, int* user_type, char* surname) {
	char password[30];
	char* sql;
	int rc;
	while (1) {
		printf("Enter your surname:\n");
		scanf("%s", surname);
		printf("Enter your password:\n");
		scanf("%s", password);
		sql = "SELECT * FROM users WHERE surname = ? and password = ?;";
		sqlite3_stmt* res;
		rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

		if (rc == SQLITE_OK) {
			sqlite3_bind_text(res, 1, surname, -1, 0);
			sqlite3_bind_text(res, 2, password, -1, 0);
		}
		else {
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

		switch (*user_type)
		{
		case owner:
			owner_loop(db, surname);
			break;

		case admin:
			admin_loop(db);
			break;

		case jockey:
			jockey_loop(db, surname);
			break;

		default:
			printf("poshlo po pizde (user type)\n");
			break;
		}


		return;
	}
}

void LogInLoop(sqlite3* db, int* user_type, char* surname) 
{
	while (1) {
		printf("Log In:\n");
		printf("1. Log In\n");
		printf("2. Authentication\n");
		printf("3. Exit\n");
		int choice = 10;
		scanf("%d", &choice);
		switch (choice) {
		case 1: 
			LogIn(db, user_type, surname);
			break;
		
		case 2: 
			Authentication(db);
			break;
		

		case 3: 
			return;

		default: 
			printf("Wrong input. Try again.\n");
			break;
		
		}
	}
}

void Select1(sqlite3* db, const char* surname) 
{

	char* sql = "SELECT horses.id, name, age, horses.experience, price, races.date as races_date,"
		"surname as jockey_surname, jockeys.experience, birthday, address\n"
		"FROM (SELECT *, max(victory_count) FROM (SELECT owner_horses.id, name, age, experience, price, count(*) as victory_count FROM races\n"
		"INNER JOIN (SELECT * FROM horses WHERE owner=?) as owner_horses \n"
		"WHERE taken_place=1 and horse_id=owner_horses.id GROUP BY owner_horses.id)) as horses\n"
		"INNER JOIN races\n"
		"INNER JOIN jockeys\n"
		"WHERE horses.id =races.horse_id and races.jockey_id = jockeys.id;";

	//pohuj
	if (strcmp(surname, "*") == 0)
	{
		sql = "SELECT horses.id, name, age, horses.experience, price, races.date as races_date,"
		"surname as jockey_surname, jockeys.experience, birthday, address\n"
		"FROM (SELECT *, max(victory_count) FROM (SELECT owner_horses.id, name, age, experience, price, count(*) as victory_count FROM races\n"
		"INNER JOIN (SELECT * FROM horses) as owner_horses \n"
		"WHERE taken_place=1 and horse_id=owner_horses.id GROUP BY owner_horses.id)) as horses\n"
		"INNER JOIN races\n"
		"INNER JOIN jockeys\n"
			"WHERE horses.id =races.horse_id and races.jockey_id = jockeys.id;";
	}

	sqlite3_stmt* res;
	int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

	if (rc == SQLITE_OK) {
		sqlite3_bind_text(res, 1, surname, -1, 0);
	}
	else {
		fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
	}

	int step = sqlite3_step(res);
	if (step != SQLITE_ROW) {
		printf("This owner has no winning horses\n");
	}
	while (step == SQLITE_ROW) {
		for (int i = 0; i < sqlite3_column_count(res); ++i) {
			printf("%s = %s\n", sqlite3_column_name(res, i), sqlite3_column_text(res, i));
		}
		printf("\n");
		step = sqlite3_step(res);
	}

	sqlite3_finalize(res);
}

void Select2(sqlite3* db) {
	char* zErrMsg = 0;
	char* sql = "SELECT id, surname, experience, birthday, address, races_number FROM jockeys\n"
		"INNER JOIN (SELECT jockey_id, max(races_count) as races_number "
		"FROM (SELECT jockey_id, count(jockey_id) as races_count FROM races GROUP BY jockey_id))\n"
		"WHERE jockey_id=jockeys.id;";
	int rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
	SQL_Error(rc, zErrMsg);
}

void Select3(sqlite3* db, const char* surname) {
	char* sql = "SELECT date, horse_id, name, age, experience, owner, price, taken_place FROM races\n"
		"INNER JOIN (SELECT id FROM jockeys WHERE surname=?) as jockey\n"
		"INNER JOIN horses\n"
		"WHERE jockey_id = jockey.id and horse_id = horses.id;";
	sqlite3_stmt* res;
	int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

	if (rc == SQLITE_OK) {
		sqlite3_bind_text(res, 1, surname, -1, 0);
	}
	else {
		fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
	}

	int step = sqlite3_step(res);
	if (step != SQLITE_ROW) {
		printf("This jockey didn't participate in the races\n");
	}
	while (step == SQLITE_ROW) {
		for (int i = 0; i < sqlite3_column_count(res); ++i) {
			printf("%s = %s\n", sqlite3_column_name(res, i), sqlite3_column_text(res, i));
		}
		printf("\n");
		step = sqlite3_step(res);
	}

	sqlite3_finalize(res);
}


void Select4(sqlite3* db, const char* surname) {
	char* sql = "SELECT horse_id, name, age, experience, price, date, taken_place FROM\n"
		"(SELECT * FROM horses WHERE owner = ?) as owner_horses\n"
		"INNER JOIN races\n"
		"WHERE horse_id = owner_horses.id;";
	sqlite3_stmt* res;
	int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

	if (rc == SQLITE_OK) {
		sqlite3_bind_text(res, 1, surname, -1, 0);
	}
	else {
		fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
	}

	int step = sqlite3_step(res);
	if (step != SQLITE_ROW) {
		printf("This owner has no horses\n");
	}
	while (step == SQLITE_ROW) {
		for (int i = 0; i < sqlite3_column_count(res); ++i) {
			printf("%s = %s\n", sqlite3_column_name(res, i), sqlite3_column_text(res, i));
		}
		printf("\n");
		step = sqlite3_step(res);
	}

	sqlite3_finalize(res);
}

void Select5(sqlite3* db) {
	char* sql = "SELECT races.id, date, race_number, horse_id, name, age, experience, owner, price, jockey_id, taken_place FROM \n"
		"(SELECT *, (substr(date,7,4)||'-'||substr(date,1,2)||'-'||substr(date,4,2)) as true_date FROM races) as races\n"
		"INNER JOIN horses\n"
		"WHERE horse_id=horses.id and true_date>=? and true_date<=?";
	sqlite3_stmt* res;
	int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

	char period_beginning[12], period_end[12];
	int month, day, year;
	printf("Enter period beginning:\n");
	scanf("%d.%d.%d", &month, &day, &year);
	sprintf(period_beginning, "%04d-%02d-%02d", year, month, day);

	printf("Enter period end:\n");
	scanf("%d.%d.%d", &month, &day, &year);
	sprintf(period_end, "%04d-%02d-%02d", year, month, day);

	if (rc == SQLITE_OK) {
		sqlite3_bind_text(res, 1, period_beginning, -1, 0);
		sqlite3_bind_text(res, 2, period_end, -1, 0);
	}
	else {
		fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
	}

	int step = sqlite3_step(res);
	if (step != SQLITE_ROW) {
		printf("There were no races during this period\n");
	}
	while (step == SQLITE_ROW) {
		for (int i = 0; i < sqlite3_column_count(res); ++i) {
			printf("%s = %s\n", sqlite3_column_name(res, i), sqlite3_column_text(res, i));
		}
		printf("\n");
		step = sqlite3_step(res);
	}

	sqlite3_finalize(res);
}
