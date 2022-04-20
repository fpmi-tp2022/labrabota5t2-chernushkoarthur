#include "../inc/owner.h"

void owner_loop(sqlite3* db, const char* surname)
{
	while (1)
	{
		printf("Owner Action:\n");
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
			owner_read_loop(db, surname);
			break;
		case 2:
			return;

		default:

			printf("Wrong input. Try again.\n");
			break;
		}
	}


}

void owner_read_loop(sqlite3* db, const char* surname)
{
	while (1)
	{
		printf("Read:\n");
		printf("1. My Horses and Races\n");
		printf("2. My Best Horse\n");
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
			Select4(db, surname);
			break;

		case 2:
			Select1(db, surname);
			break;

		case 3:
			return;

		default:
			printf("Wrong input. Try again.\n");
			break;

		}
	}
}
