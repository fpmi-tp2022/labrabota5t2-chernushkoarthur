#include "../inc/hippodrome.h"

static int callback_user_type(void* data, int argc, char** argv, char** azColName) {
	int i;
	
	for (i = 0; i < argc; i++) 
	{
		// obviously cringe (no mne len menyat)
		if (i & 1)
			printf("%s", argv[i] ? argv[i] : "(null)");
		else
			printf("%s. ", argv[i] ? argv[i] : "(null)");
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

	int rc;
	int user_type;
	char c = '0';
	char ans[22] = "0";
	int done = 0;
	while (!done)
	{
		printf("Choose user type:\n");

		sql = "SELECT * FROM user_type";
		rc = sqlite3_exec(db, sql, callback_user_type, 0, &zErrMsg);
		SQL_Error(rc, zErrMsg);


		scanf("%s", ans);
		if (strcmp(ans, "/exit") == 0)
			return;

		user_type = atoi(ans);

		switch (user_type)
		{

		case owner:
		case admin:
		case jockey:
			done = 1;
			break;

		default:
			printf("Wrong input. Try again.\n");
			break;
		}
	}
	

	printf("Enter your surname:\n");
	char surname[30];
	scanf("%s", surname);
	if (strcmp(surname, "/exit") == 0)
		return;

	printf("Enter your password:\n");
	char password[30];
	scanf("%s", password);
	if (strcmp(password, "/exit") == 0)
		return;


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

	printf("\n");
}

void LogIn(sqlite3* db, int* user_type, char* surname) {
	char password[30];
	char* sql;
	int rc;
	while (1) 
	{
		printf("Enter your surname:\n");
		scanf("%s", surname);
		if (strcmp(surname, "/exit") == 0)
			return;
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

void main_loop(sqlite3* db, int* user_type, char* surname) 
{
	while (1) 
	{
		printf("Log In:\n");
		printf("1. Log In\n");
		printf("2. Authentication\n");
		printf("3. Exit\n");
		printf("4. Clear\n");
		int choice = 10;
		char c = '0';
		if (scanf("%d", &choice) == 0)
		{
			printf("Wrong input. Try again.\n");
			while ((c = getchar()) != '\n' && c != EOF);
			continue;
		}
		switch (choice) 
		{
		case 1: 
			LogIn(db, user_type, surname);
			break;
		
		case 2: 
			Authentication(db);
			break;
		
		case 3: 
			return;

		case 4:
			printf("\033c");
			break;

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
	printf("Select races from (month.day.year): ");
	scanf("%d.%d.%d", &month, &day, &year);
	sprintf(period_beginning, "%04d-%02d-%02d", year, month, day);

	printf("Select races until (month.day.year): ");
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

void Select6(sqlite3* db, const char* jockey) {
	char* sql = "SELECT races.id, date, race_number, horse_id, taken_place, true_date FROM \n"
		"(SELECT *, (substr(date,7,4)||'-'||substr(date,1,2)||'-'||substr(date,4,2)) as true_date FROM races) as races\n"
		"INNER JOIN (SELECT id as id_jockey FROM jockeys WHERE surname = ?)\n"
		"WHERE jockey_id=id_jockey and true_date>=? and true_date<=?";
	sqlite3_stmt* res;
	int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

	char period_beginning[12], period_end[12];
	int month, day, year;
	printf("Select races from (month.day.year): ");
	scanf("%d.%d.%d", &month, &day, &year);
	sprintf(period_beginning, "%04d-%02d-%02d", year, month, day);

	printf("Select races until (month.day.year): ");
	scanf("%d.%d.%d", &month, &day, &year);
	sprintf(period_end, "%04d-%02d-%02d", year, month, day);

	if (rc == SQLITE_OK) {
		sqlite3_bind_text(res, 1, jockey, -1, 0);
		sqlite3_bind_text(res, 2, period_beginning, -1, 0);
		sqlite3_bind_text(res, 3, period_end, -1, 0);
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

void Insert(sqlite3* db) {
	char* sql = "INSERT INTO races (date, race_number, horse_id, jockey_id, taken_place)\n"
		"VALUES (?, ?, ?, ?, ?);";
	sqlite3_stmt* res;
	int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

	char date[12];
	int race_number, horse_id, jockey_id, taken_place;
	printf("Enter date:\n");
	scanf("%s", date);
	if (strcmp(date, "/exit") == 0)
		return;
	printf("Enter race number:\n");
	scanf("%d", &race_number);
	horse_id = protected_horse_id(db);
	if (horse_id == -1)
		return;
	jockey_id = protected_jockey_id(db);
	if (jockey_id == -1)
		return;
	printf("Enter taken_place:\n");
	scanf("%d", &taken_place);
	if (rc == SQLITE_OK) {
		sqlite3_bind_text(res, 1, date, -1, 0);
		sqlite3_bind_int(res, 2, race_number);
		sqlite3_bind_int(res, 3, horse_id);
		sqlite3_bind_int(res, 4, jockey_id);
		sqlite3_bind_int(res, 5, taken_place);
	}
	else {
		fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
	}
	sqlite3_step(res);
	sqlite3_finalize(res);
}

void Update(sqlite3* db) {
	char* sql = "UPDATE races SET date=?, race_number=?, horse_id=?, jockey_id=?, taken_place=?\n"
		"WHERE id=?;";
	sqlite3_stmt* res;
	int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

	char date[12];
	char id[12];
	int race_number, horse_id, jockey_id, taken_place;
	printf("Enter id:\n");
	scanf("%s", id);
	if (strcmp(id, "/exit") == 0)
		return;
	printf("Enter date:\n");
	scanf("%s", date);
	printf("Enter race number:\n");
	scanf("%d", &race_number);
	printf("Enter horse id:\n");
	scanf("%d", &horse_id);
	printf("Enter jockey id:\n");
	scanf("%d", &jockey_id);
	printf("Enter taken_place:\n");
	scanf("%d", &taken_place);
	if (rc == SQLITE_OK) {
		sqlite3_bind_text(res, 1, date, -1, 0);
		sqlite3_bind_int(res, 2, race_number);
		sqlite3_bind_int(res, 3, horse_id);
		sqlite3_bind_int(res, 4, jockey_id);
		sqlite3_bind_int(res, 5, taken_place);
		sqlite3_bind_int(res, 6, atoi(id));
	}
	else {
		fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
	}
	sqlite3_step(res);
	sqlite3_finalize(res);
}

void Delete(sqlite3* db) {
	char* sql = "DELETE FROM races WHERE id = ?;";
	sqlite3_stmt* res;
	int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

	char id[12]= "0";
	printf("Enter id:\n");
	scanf("%s", id);
	if (strcmp(id, "/exit") == 0)
		return;

	if (rc == SQLITE_OK) {
		sqlite3_bind_int(res, 1, atoi(id));
	}
	else {
		fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
	}
	sqlite3_step(res);
	sqlite3_finalize(res);
}

int protected_horse_id(sqlite3* db)
{
	while (1)
	{
		printf("Enter horse id:\n");

		char ans[22] = "0";
		scanf("%s", ans);
		if (strcmp(ans, "/exit") == 0)
			return -1;

		int horse_id = atoi(ans);

		char* sql = "SELECT id FROM horses WHERE id = ?";
		sqlite3_stmt* res;
		int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

		if (rc == SQLITE_OK) {
			sqlite3_bind_int(res, 1, horse_id);
		}
		else {
			fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
		}

		int step = sqlite3_step(res);
		if (step != SQLITE_ROW) 
		{
			printf("No such horse :/\n");
		}
		else
		{
			sqlite3_finalize(res);
			return horse_id;
		}

		sqlite3_finalize(res);
	}
	
}
int protected_jockey_id(sqlite3* db)
{
	while (1)
	{
		printf("Enter jockey id:\n");

		char ans[22] = "0";
		scanf("%s", ans);
		if (strcmp(ans, "/exit") == 0)
			return -1;

		int jokcey_id = atoi(ans);

		char* sql = "SELECT id FROM jockeys WHERE id = ?";
		sqlite3_stmt* res;
		int rc = sqlite3_prepare_v2(db, sql, -1, &res, 0);

		if (rc == SQLITE_OK) {
			sqlite3_bind_int(res, 1, jokcey_id);
		}
		else {
			fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(db));
		}

		int step = sqlite3_step(res);
		if (step != SQLITE_ROW)
		{
			printf("No such jokcey :/\n");
		}
		else
		{
			sqlite3_finalize(res);
			return jokcey_id;
		}

		sqlite3_finalize(res);
	}
}

void divide_prize (double prize)
{
	double	first = 0.5 * prize,
			second = 0.3 * prize,
			third = 0.2 * prize;

	printf("1st place: %.2f\n", first);
	printf("2nd place: %.2f\n", second);
	printf("3rd place: %.2f\n", third);

}
