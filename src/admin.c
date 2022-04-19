#include "../inc/admin.h"

void admin_loop(sqlite3* db)
{
	while (1)
	{
		printf("Admin Action:\n");
		printf("1. Read\n");
		printf("2. Update\n");
		printf("3. Delete\n");
		printf("4. Misc\n");
		printf("5. Exit\n");
		int choice = 10;
		scanf("%d", &choice);
		switch (choice) {
		case 1:
			Select5(db);
			
			break;
		case 2:
			// Upate query
			break;
		case 3:
			// Delete query
			break;
		case 4:
			admin_misc_loop(db);
			break;
		case 5:
			return;

		default:

			printf("Wrong input. Try again.\n");
			break;
		}
	}


}

void admin_read_loop(sqlite3* db)
{

}

void admin_misc_loop(sqlite3* db)
{
	while (1)
	{
		printf("Select:\n");
		printf("1. Best Horse\n");
		printf("2. Best Jockey\n");
		printf("3. Exit\n");
		int choice = 10;
		scanf("%d", &choice);
		switch (choice) {
		case 1:
			Select1(db, "*"); // blyadskij kostil no mne pohuj (maksimalno)
			break;

		case 2:
			Select2(db);
			break;

		case 3:
			return;
			
		default:

			printf("Wrong input. Try again.\n");
			break;
		}
	}
}
