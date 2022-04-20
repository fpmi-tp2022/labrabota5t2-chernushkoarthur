#include "../inc/jockey.h"

void jockey_loop(sqlite3* db, const char* surname)
{
	while (1)
	{
		printf("Jockey Action:\n");
		printf("1. Read\n");
		printf("2. Exit\n");
		int choice = 10;
		char c = '0';
		if (scanf("%d", &choice) == 0)
		{
			printf("Wrong input. Try again.\n");
			while ((c = getchar()) != '\n' && c != EOF);
			continue;
		}
		switch (choice) {
		case 1:
			jockey_read_loop(db, surname);
			break;
		case 2:
			return;

		default:

			printf("Wrong input. Try again.\n");
			break;
		}
	}


}

void jockey_read_loop(sqlite3* db, const char* surname)
{
	while (1)
	{
		printf("Read:\n");
		printf("1. My Races\n");
		printf("2. Select My Races\n");
		printf("3. Exit\n");
		int choice = 10;
		char c = '0';
		if (scanf("%d", &choice) == 0)
		{
			printf("Wrong input. Try again.\n");
			while ((c = getchar()) != '\n' && c != EOF);
			continue;
		}
		switch (choice) {
		case 1:
			Select3(db, surname);
			break;

		case 2:
			Select6(db, surname);

		case 3:
			return;

		default:
			printf("Wrong input. Try again.\n");
			break;

		}
	}
}
