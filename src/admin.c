#include "../inc/admin.h"

void admin_loop(sqlite3* db)
{
	while (1)
	{
		printf("Admin Action:\n");
		printf("1. Insert Race\n");
		printf("2. Update Race\n");
		printf("3. Delete Race\n");
		printf("4. Output Races\n");
		printf("5. Misc\n");
		printf("6. Exit\n");
		printf("7. Clear\n");
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
			Insert(db);
			break;

		case 2:
			Update(db);
			break;

		case 3:
			Delete(db);
			break;

		case 4:
			Select5(db);

		case 5:
			admin_misc_loop(db);
			break;

		case 6:
			return;

		case 7:
			printf("\033c");
			break;

		default:

			printf("Wrong input. Try again.\n");
			break;
		}
	}


}


void admin_misc_loop(sqlite3* db)
{
	while (1)
	{
		printf("Select:\n");
		printf("1. Best Horse\n");
		printf("2. Best Jockey\n");
		printf("3. Divide prize pool\n");
		printf("4. Exit\n");
		printf("5. Clear\n");

		double prize = 0;
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
			Select1(db, "*"); // kostil no mne pohuj (maksimalno)
			break;

		case 2:
			Select2(db);
			break;

		case 3:
			printf("Input prize pool: ");
			if (scanf("%lf", &prize) == 0)
			{
				printf("Wrong input. Try again.\n");
				while ((c = getchar()) != '\n' && c != EOF);
				break;
			}

			divide_prize(prize);
			printf("\n\n");
			break;

			

		case 4:
			return;

		case 5:
			printf("\033c");
			break;
			
		default:

			printf("Wrong input. Try again.\n");
			break;
		}
	}
}
