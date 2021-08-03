#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <Windows.h>
#include "structures.h"
#pragma warning(disable:4996)

//========== Sanitization Records Module by Thong So Xue ==========


//============================== DEFINE CONSTANTS ==============================
#define MAX_SAN_RECORDS 100
#define MAX_SANITIZATION_COMPANIES 100
#define SURFACE_DISINFECT_PRICE 0.12
#define SPRAY_AND_COAT_PRICE 0.75
#define FOGGING_PRICE 0.20



//============================== FUNCTION DECLARATIONS ==============================
// Main Module Functions
void sanitizationRecordsModule();
void addRec(Sanitization* sanRecords, int* sanNo);
void editRec(Sanitization* sanRecords, int* sanNo);
void deleteRec(Sanitization* sanRecords, int* sanNo);
void displayRec(Sanitization* sanRecords, int* sanNo);
void searchRec(Sanitization* sanRecords, int* sanNo);
void reportRec(Sanitization* sanRecords, int* sanNo);

// Load and Save records to file
void loadSanFile(Sanitization* sanRecords, int* sanNo);
void saveSanFile(Sanitization* sanRecords, int* sanNo);

// Input validating functions
char inputYN(char question[]);
int inputMenuOpt(char question[], int min, int max);
Date inputDate(char question[]);
Time inputTime(char question[]);
int inputID(char question[]);
double inputDouble(char question[]);
void inputContactNo(char question[], char* output);
void inputVenueID(char question[], char* output);

// Misc functions
void nameCapitalize(char* name);
double calcSanPrice(char venueID[7], int sanMethod);
int calcDaysBetween(Date d1, Date d2);

// Printf Functions - Menu
void sanTitle();
void sanMenu();
void sanTypeMenu();
void sanPriceMenu();

// Printf Functions - Misc
void fill(char ch, int count);
void centerAlign(char str[], int width);
void tableHeader();
void tableRow(Sanitization san);
void tableFooter(int recordCount);
void functionTitle(char title[]);
void invalidInput();

//============================== MAIN MODULE ==============================
void sanitizationRecordsModule() {
	Sanitization sanRecords[MAX_SAN_RECORDS];
	int sanNo, menuOpt;
	loadSanFile(&sanRecords, &sanNo);	// Load records from file to the sanRecords array ONCE ONLY at the start of module

	do {
		// Reset screen and values
		menuOpt = 0;
		system("cls");

		sanTitle();		// Display Title
		sanMenu();		// Display Menu 
		menuOpt = inputMenuOpt("  Enter your option > ", 1, 7);	// Get menu option

		// Run the corresponding functions based on input
		switch (menuOpt) {
		case 1: addRec(&sanRecords, &sanNo); break;
		case 2: editRec(&sanRecords, &sanNo); break;
		case 3: deleteRec(&sanRecords, &sanNo); break;
		case 4: searchRec(&sanRecords, &sanNo); break;
		case 5: displayRec(&sanRecords, &sanNo); break;
		case 6: reportRec(&sanRecords, &sanNo); break;
		case 7:
			printf("\n  [ Returning to Main Menu... ]\n  ");
			break;
		}

		// Update records to file after every function / Before exiting module
		saveSanFile(&sanRecords, &sanNo);
		if (menuOpt != 7) {
			printf("\n  [ All records are saved to file. ]\n");
			printf("  [ Press Any key to Return to Sanitization Menu ]\n  ");
			system("pause");
		}
	} while (menuOpt != 7);

	// Exit to main menu
	return;
}



//============================== SANITIZATION MODULE MAIN FUNCTIONS ==============================
// Add record function
void addRec(Sanitization* sanRecords, int* sanNo) {
	Sanitization temp;
	char another, confirm;
	int sanTypeChoice, priceMethod, valid;

	// Add record loop, exits when user enter 'N' in "Add more records? (Y/N) > "
	do {
		system("cls");	// Clear screen for each add record loop
		sanTitle();
		functionTitle("Add Records Function");

		// Disallow to add more records when the amount of records reach MAX_RECORDS
		if (*sanNo == MAX_SAN_RECORDS) {
			printf(
				"\n  [ Already reached max records! ]\n"
				"  You cannot add more records until the file is emptied or moved to another place. \n"
			);
			another = 'N';
		}
		// Add Record Session
		else {
			// Reset values
			sanTypeChoice = 0;
			priceMethod = 0;

			// Auto generate newest record's ID
			temp.id = sanRecords[*sanNo - 1].id + 1;
			printf("\n  Enter the details for Record ID SN%04d:\n", temp.id);
			printf("  --------------------------------------\n");

			//~~~~~~~~~~ Input data for new record ~~~~~~~~~~
			temp.date = inputDate("  Date  (DD-MM-YYYY): ");		// Input Date
			temp.startTime = inputTime("  Start Time (HH:MM): ");	// Input Start Time
			do {
				valid = 1;
				temp.endTime = inputTime("  End Time   (HH:MM): ");	// Input End Time
				if (temp.endTime.hour < temp.startTime.hour ||
					(temp.endTime.hour == temp.startTime.hour && temp.endTime.min < temp.startTime.min)) {
					printf("  [ End time cannot be smaller than start time! Please re-enter. ]\n\n");
					valid = 0;
				}
			} while (!valid);

			//Input Venue ID
			printf("\n");
			inputVenueID("  Choose a Venue ID > ", &temp.venueID);

			// Input Sanitization Company's Name
			printf("  Sanitization Company's Name           : ");
			rewind(stdin);
			scanf("%[^\n]", temp.scd.name);
			nameCapitalize(&temp.scd.name);

			// Input Sanitization Company's Contact Person
			printf("  Sanitization Company's Contact Person : ");
			rewind(stdin);
			scanf("%[^\n]", temp.scd.contactPerson);
			nameCapitalize(&temp.scd.contactPerson);

			// Input Sanitization Company's Contact Number
			inputContactNo("  Sanitization Company's Contact Number : ", &temp.scd.contactNo);

			// Input type of sanitization
			printf("\n");
			sanTypeMenu();
			sanTypeChoice = inputMenuOpt("  Choose a type of sanitization > ", 1, 3);
			switch (sanTypeChoice) {
			case 1: strcpy(temp.sanitizationType, "Surface Disinfecting");	break;
			case 2: strcpy(temp.sanitizationType, "Spray and Coat");		break;
			case 3: strcpy(temp.sanitizationType, "Fogging");				break;
			default: break;
			}

			// Input Sanitization Price
			printf("\n");
			sanPriceMenu();
			priceMethod = inputMenuOpt("  Choose a method for the sanitization price > ", 1, 2);
			if (priceMethod == 1) {
				temp.price = calcSanPrice(temp.venueID, sanTypeChoice);
				printf("  The auto calculated sanitization price is RM%.2lf. \n", temp.price);
			}
			else if (priceMethod == 2) {
				temp.price = inputDouble("  Enter the new Price of Sanitization : RM");
			}

			// Preview the new record
			printf("\n  The new record looks like this : ");
			tableHeader();
			tableRow(temp);
			tableFooter(-1);
			printf("\n\n");

			// Ask for confirmation to add record
			confirm = inputYN("  Confirm to add this record? (Y/N)? > ");
			if (confirm == 'Y') {
				sanRecords[*sanNo] = temp;
				*sanNo += 1;
				printf("  [ Record added successfully. ]\n\n");

				// Update last sanitization date to the VenueInformationModule.txt

				// Open files for reading and writing
				FILE* fpVenueRead, * fpVenueWrite;
				fpVenueRead = fopen("VenueInformationRecord.txt", "r");
				if (fpVenueRead == NULL) {
					printf("  [ Error opening VenueInformationRecord.txt. ]\n  ");
					perror("Reason");
					exit(-1);
				}
				fpVenueWrite = fopen("VenueInformationRecord0.txt", "w");
				if (fpVenueWrite == NULL) {
					printf("  [ Error opening VenueInformationRecord0.txt. ]\n  ");
					perror("Reason");
					exit(-1);
				}

				venue tempVenueRec;
				// Read all from VenueInformationModule.txt
				while (fscanf(fpVenueRead, "%[^|]|%[^|]|%[^|]|%[^|]|%lf|%d|%d|%d-%d-%d|%[^|]|\n",
					tempVenueRec.ID, tempVenueRec.name, tempVenueRec.category, tempVenueRec.description,
					&tempVenueRec.areaSize, &tempVenueRec.number.maxVisitors, &tempVenueRec.number.currentVisitors,
					&tempVenueRec.lastSanitization.day, &tempVenueRec.lastSanitization.month, &tempVenueRec.lastSanitization.year,
					tempVenueRec.dangerLevel) != EOF) {
					// Update the last sanitization date
					if (strcmpi(tempVenueRec.ID, temp.venueID) == 0 &&
						(	(tempVenueRec.lastSanitization.year < temp.date.year) ||
							(tempVenueRec.lastSanitization.year == temp.date.year && tempVenueRec.lastSanitization.month < temp.date.month) || 
							(tempVenueRec.lastSanitization.year == temp.date.year && tempVenueRec.lastSanitization.month == temp.date.month && tempVenueRec.lastSanitization.day < temp.date.day)
							)) {
						tempVenueRec.lastSanitization.day = temp.date.day;
						tempVenueRec.lastSanitization.month = temp.date.month;
						tempVenueRec.lastSanitization.year = temp.date.year;
					}

					// Write to another file
					fprintf(fpVenueWrite, "%s|%s|%s|%s|%.2lf|%d|%d|%d-%d-%d|%s|\n",
						tempVenueRec.ID, tempVenueRec.name, tempVenueRec.category, tempVenueRec.description,
						tempVenueRec.areaSize, tempVenueRec.number.maxVisitors, tempVenueRec.number.currentVisitors,
						tempVenueRec.lastSanitization.day, tempVenueRec.lastSanitization.month, tempVenueRec.lastSanitization.year,
						tempVenueRec.dangerLevel);
				}
				fclose(fpVenueRead);
				fclose(fpVenueWrite);

				// Replace original file with the new file
				if (remove("VenueInformationRecord.txt")) {
					printf("  [ Error deleting file VenueInformationRecord.txt ]\n  ");
					perror("Reason");
					exit(-1);
				}
				if (rename("VenueInformationRecord0.txt", "VenueInformationRecord.txt")) {
					printf("  [ Error renaming file VenueInformationRecord0.txt ]\n  ");
					perror("Reason");
					exit(-1);
				}
			}
			else {
				printf("  [ Record Discarded.  ]\n\n");
			}

			// Ask for another
			another = inputYN("  Add another record? (Y/N)? > ");
			if (another == 'Y') {
				printf("\n  [ Creating another Add Record Session ]\n  ");
				system("pause");
			}
		}
	} while (another == 'Y');
}



// Modify record function
void editRec(Sanitization* sanRecords, int* sanNo) {
	Sanitization temp;
	char another, confirm;
	int i, searchID, searchCount, modifyNo;

	// ID Loop : Enter new ID to modify on every loop, exit by entering 'N' in "Modify another record? (Y?N) > "
	do {
		system("cls");	// Clear screen for every new ID loop
		sanTitle();
		functionTitle("Modify Records Function");

		// Reset values
		searchCount = 0;
		modifyNo = 0;

		// Get input, then search & display the record
		searchID = inputID("  Enter the Sanitization ID to modify : ");	// Get input
		for (i = 0; i < *sanNo; i++) {									// Search for the record
			if (searchID == sanRecords[i].id) {
				searchCount++;
				break;	// Break when the same unique ID is found (i's value is maintained and brought to the next phase)
			}
		}

		if (searchCount != 0) {	// If the record ID is found
			temp = sanRecords[i];

			// Record Loop : Modify fields on the same record, exit by entering 10 for fieldNo
			do {
				system("cls");
				sanTitle();
				functionTitle("Modify Records Function");
				printf("  Enter the Sanitization ID to modify : SN%04d\n", searchID);

				printf("\n  The current record looks like this : ");	// Display the current record
				tableHeader();
				tableRow(sanRecords[i]);
				tableFooter(-1);
				printf("\n\n");

				printf("  The modified record looks like this : ");	// Display the current record
				tableHeader();
				tableRow(temp);
				tableFooter(-1);
				printf("\n\n");

				printf("\n  [ Currently Modifying Record : SN%04d ]\n", temp.id);
				printf(
					"  ============================================================================================\n"
					"  |  1 - Date                                  |  6 - Sanitization Company's Contact Person  |\n"
					"  |  2 - Start Time                            |  7 - Sanitization Company's Contact Number  |\n"
					"  |  3 - End Time                              |  8 - Type of Sanitization                   |\n"
					"  |  4 - Venue ID                              |  9 - Price of Sanitization                  |\n"
					"  |  5 - Sanitization Company's Name           |  10 - Confirm Modification                  |\n"
					"  ============================================================================================\n");

				modifyNo = inputMenuOpt("  Choose a field to modify > ", 1, 10);

				// Confirm Modification & Exit Record Loop
				if (modifyNo == 10) {
					confirm = inputYN("  Confirm to modify this record? (Y/N) > ");
					if (confirm == 'Y') {
						sanRecords[i] = temp;
						printf("  [ Successfully modified record. ]\n\n");
					}
					else {
						printf("  [ Process cancelled, draft discarded. ]\n\n");
					}
				}
				else {
					printf("\n");
					if (modifyNo == 1) {		// Modify Date
						printf("  Current Date : %02d-%02d-%04d\n", temp.date.day, temp.date.month, temp.date.year);
						temp.date = inputDate("  Enter the new Date (DD-MM-YYYY) : ");
					}
					else if (modifyNo == 2) {	// Modify Start Time
						printf("  Current Start Time : %02d:%02d\n", temp.startTime.hour, temp.startTime.min);
						temp.startTime = inputTime("  Enter the new Start Time (HH:MM) > ");
						while ((temp.startTime.hour > temp.endTime.hour) || (temp.startTime.hour == temp.endTime.hour && temp.startTime.min > temp.endTime.min)) {
							printf("  [ Start time cannot be later than end time! Please re-enter. ]\n\n");
							temp.startTime = inputTime("  Enter the new Start Time (HH:MM) > ");
						}
					}
					else if (modifyNo == 3) {	// Modify End
						printf("  Current End Time : %02d:%02d\n", temp.endTime.hour, temp.endTime.min);
						temp.endTime = inputTime("  Enter the new End Time (HH:MM) > ");
						while ((temp.endTime.hour < temp.startTime.hour) || (temp.endTime.hour == temp.startTime.hour && temp.endTime.min < temp.startTime.min)) {
							printf("  [ End time cannot be earlier than start time! Please re-enter. ]\n\n");
							temp.endTime = inputTime("  Enter the new End Time (HH:MM) > ");
						}
					}
					else if (modifyNo == 4) {	// Modify Venue ID
						printf("  Current Venue ID : %s\n", temp.venueID);
						inputVenueID("  Choose a new Venue ID > ", temp.venueID);
					}
					else if (modifyNo == 5) {	// Modify Sanitization Company's Name
						printf("  Current Sanitization Company's Name : %s\n", temp.scd.name);
						printf("  Enter the new Sanitizatin Company's Name : ");
						rewind(stdin);
						scanf("%[^\n]", temp.scd.name);
						nameCapitalize(&temp.scd.name);
					}
					else if (modifyNo == 6) {	// Modify Sanitization Company's Contact Person
						printf("  Current Sanitization Company's Contact Person : %s\n", temp.scd.contactPerson);
						printf("  Enter the new Sanitizatin Company's Contact Person : ");
						rewind(stdin);
						scanf("%[^\n]", temp.scd.contactPerson);
						nameCapitalize(&temp.scd.contactPerson);
					}
					else if (modifyNo == 7) {	// Modify Sanitization Company's Contact Number
						printf("  Current Sanitization Company's Contact Number : %s\n", temp.scd.contactNo);
						inputContactNo("  Enter the new Sanitizatin Company's Contact Number : ", &temp.scd.contactNo);
					}
					else if (modifyNo == 8) {	// Modify Type of Sanitization
						int sanTypeChoice;
						printf("  Current Type of Sanitization : %s\n", temp.sanitizationType);
						sanTypeMenu();
						sanTypeChoice = inputMenuOpt("  Choose a new Type of Sanitization > ", 1, 3);
						switch (sanTypeChoice) {
						case 1: strcpy(temp.sanitizationType, "Surface Disinfecting"); break;
						case 2: strcpy(temp.sanitizationType, "Spray and Coat"); break;
						case 3: strcpy(temp.sanitizationType, "Fogging"); break;
						default: break;
						}
					}
					else if (modifyNo == 9) {	// Modify Sanitization Price
						int priceMethod;
						sanPriceMenu();
						priceMethod = inputMenuOpt("  Choose a method to modify the price > ", 1, 2);
						printf("  Current Price of Sanitization : RM%.2lf\n", temp.price);
						if (priceMethod == 1) {
							if (strcmp(temp.sanitizationType, "Surface Disinfecting") == 0)
								temp.price = calcSanPrice(temp.venueID, 1);
							else if (strcmp(temp.sanitizationType, "Spray and Coat") == 0)
								temp.price = calcSanPrice(temp.venueID, 2);
							else if (strcmp(temp.sanitizationType, "Fogging") == 0)
								temp.price = calcSanPrice(temp.venueID, 3);
							printf("  New Price of Sanitization : RM%.2lf\n", temp.price);
						}
						else if (priceMethod == 2)
							temp.price = inputDouble("  Enter the new Price of Sanitization > RM");
					}
					printf("\n  [ Field modified! ]\n  ");
					system("pause");
				}
			} while (modifyNo != 10);
		}
		else {	// If the record ID is not found
			printf("\n  [ Cannot find record with Sanitization ID of SN%04d. ]\n\n", searchID);
		}

		// Ask for another
		another = inputYN("  Modify another record? (Y/N) > ");
	} while (another == 'Y');
}

// Delete records function by entering record ID
void deleteRec(Sanitization* sanRecords, int* sanNo) {
	char another, confirm;
	int i, j, searchID, found;

	do {
		// Reset screen and values
		system("cls");
		sanTitle();
		functionTitle("Delete Records Function");
		found = 0;

		// Get input then search and display the record
		searchID = inputID("  Enter the Sanitization ID to Delete > ");
		for (i = 0; i < *sanNo; i++) {
			if (searchID == sanRecords[i].id) {
				found = 1;
				printf("\n  Found the sanitization record with ID SN%04d", searchID);
				tableHeader();
				tableRow(sanRecords[i]);
				tableFooter(1);
				confirm = inputYN("  Do you want to delete this record? (Y/N) > ");	// Get confirmation to delete
				if (confirm == 'Y') {					// Delete record
					for (j = i; j < *sanNo - 1; j++) {
						sanRecords[j] = sanRecords[j + 1];
					}
					*sanNo -= 1;
					printf("  [ Record deleted successfully. ]\n");
				}
				else {	// Skip delete record
					printf("  [ Process cancelled. ]\n");
				}
				break;	// Break loop to prevent redundant looping
			}
		}

		if (!found) {	// If record ID not found
			printf("\n  [ Cannot find record with Sanitization ID of SN%04d. ]\n", searchID);
		}

		// Ask for another
		printf("\n");
		another = inputYN("  Delete another record? (Y/N) > ");
	} while (another == 'Y');
}

// Display all records function (order by record ID)
void displayRec(Sanitization* sanRecords, int* sanNo) {
	system("cls");
	sanTitle();
	functionTitle("Display Records Function");

	tableHeader();
	for (int i = 0; i < *sanNo; i++)
		tableRow(sanRecords[i]);
	tableFooter(*sanNo);
}

// Search for records function
void searchRec(Sanitization* sanRecords, int* sanNo) {
	Sanitization temp;
	int i, fieldNo, searchCount, searchMethod;

	// Search loop: Choose a field to search on every loop, exit by entering 0 in "Please choose a field to search on (0 to quit) > "
	do {
		// Reset values to avoid false inputs
		searchCount = 0;
		fieldNo = -1;

		system("cls");
		sanTitle();
		functionTitle("Search Records Function");
		printf("\n"
			"  ===============================================\n"
			"  |  1 - Sanitization ID                        |\n"
			"  |  2 - Date                                   |\n"
			"  |  3 - Start Time                             |\n"
			"  |  4 - End Time                               |\n"
			"  |  5 - Venue ID / Name                        |\n"
			"  |  6 - Sanitization Company's Name            |\n"
			"  |  7 - Sanitization Company's Contact Person  |\n"
			"  |  8 - Sanitization Company's Contact Number  |\n"
			"  |  9 - Type of Sanitization                   |\n"
			"  | 10 - Price of Sanitization                  |\n"
			"  ===============================================\n"
		);
		// Get which field to search
		fieldNo = inputMenuOpt("  Please choose a field to search on (0 to quit) > ", 0, 10);
		printf("\n");

		// Prompt and accept the corresponding inputs, then search and display the matching records
		if (fieldNo == 1) {			// Search by ID
			temp.id = inputID("  Enter the Record ID to search (eg: SN0001): ");
			tableHeader();
			for (i = 0; i < *sanNo; i++) {
				if (temp.id == sanRecords[i].id) {
					tableRow(sanRecords[i]);
					searchCount++;
				}
			}
		}
		else if (fieldNo == 2) {	// Search by date
			printf(""
				"  ===================================\n"
				"  | 1 - Search by a single date     |\n"
				"  | 2 - Search by a range of date   |\n"
				"  ===================================\n"
			);
			searchMethod = inputMenuOpt("  Please choose a search method > ", 1, 2);
			if (searchMethod == 1) {
				temp.date = inputDate("  Please enter the date to search (DD-MM-YYYY) : ");
				tableHeader();
				for (i = 0; i < *sanNo; i++) {
					if (temp.date.day == sanRecords[i].date.day &&
						temp.date.month == sanRecords[i].date.month &&
						temp.date.year == sanRecords[i].date.year) {
						tableRow(sanRecords[i]);
						searchCount++;
					}
				}
			}
			if (searchMethod == 2) {
				Date minDate, maxDate;
				minDate = inputDate("  Please enter the minimum date to search (DD-MM-YYYY) : ");
				maxDate = inputDate("  Please enter the maximum date to search (DD-MM-YYYY) : ");
				tableHeader();
				for (i = 0; i < *sanNo; i++) {
					if ((sanRecords[i].date.year > minDate.year || 
						(sanRecords[i].date.year == minDate.year && sanRecords[i].date.month > minDate.month) || 
						(sanRecords[i].date.year == minDate.year && sanRecords[i].date.month == minDate.month && sanRecords[i].date.day >= minDate.day))
						&&
						(sanRecords[i].date.year < maxDate.year ||
							(sanRecords[i].date.year == maxDate.year && sanRecords[i].date.month < maxDate.month) ||
							(sanRecords[i].date.year == maxDate.year && sanRecords[i].date.month == maxDate.month && sanRecords[i].date.day <= maxDate.day))) {
						tableRow(sanRecords[i]);
						searchCount++;
					}
				}
			}
		}
		else if (fieldNo == 3) {	// Search by start time
			printf(""
				"  ===================================\n"
				"  | 1 - Search by a single time     |\n"
				"  | 2 - Search by a range of time   |\n"
				"  ===================================\n"
			);
			searchMethod = inputMenuOpt("  Please choose a search method > ", 1, 2);
			if (searchMethod == 1) {
				temp.startTime = inputTime("  Please enter the start time to search for (HH:MM) : ");
				tableHeader();
				for (i = 0; i < *sanNo; i++) {
					if (temp.startTime.hour == sanRecords[i].startTime.hour &&
						temp.startTime.min == sanRecords[i].startTime.min) {
						tableRow(sanRecords[i]);
						searchCount++;
					}
				}
			}
			else if (searchMethod == 2) {
				Time minStartTime, maxStartTime;
				minStartTime = inputTime("  Please enter the minimum start time to search for (HH:MM) : ");
				maxStartTime = inputTime("  Please enter the maximum start time to search for (HH:MM) : ");
				tableHeader();
				for (i = 0; i < *sanNo; i++) {
					if ((sanRecords[i].startTime.hour > minStartTime.hour || 
						(sanRecords[i].startTime.hour == minStartTime.hour && sanRecords[i].startTime.min >= minStartTime.min))
						&&
						(sanRecords[i].startTime.hour < maxStartTime.hour ||
							(sanRecords[i].startTime.hour == maxStartTime.hour && sanRecords[i].startTime.min <= maxStartTime.min))) {
						tableRow(sanRecords[i]);
						searchCount++;
					}
				}
			}
		}
		else if (fieldNo == 4) {	// Search by end time
			printf(""
				"  ===================================\n"
				"  | 1 - Search by a single time     |\n"
				"  | 2 - Search by a range of time   |\n"
				"  ===================================\n"
			);
			searchMethod = inputMenuOpt("  Please choose a search method > ", 1, 2);
			if (searchMethod == 1) {
				temp.endTime = inputTime("  Please enter the end time to search for (HH:MM) : ");
				tableHeader();
				for (i = 0; i < *sanNo; i++) {
					if (temp.endTime.hour == sanRecords[i].endTime.hour &&
						temp.endTime.min == sanRecords[i].endTime.min) {
						tableRow(sanRecords[i]);
						searchCount++;
					}
				}
			}
			else if (searchMethod == 2) {
				Time minEndTime, maxEndTime;
				minEndTime = inputTime("  Please enter the minimum start time to search for (HH:MM) : ");
				maxEndTime = inputTime("  Please enter the maximum start time to search for (HH:MM) : ");
				tableHeader();
				for (i = 0; i < *sanNo; i++) {
					if ((sanRecords[i].endTime.hour > minEndTime.hour ||
						(sanRecords[i].endTime.hour == minEndTime.hour && sanRecords[i].endTime.min >= minEndTime.min))
						&&
						(sanRecords[i].endTime.hour < maxEndTime.hour ||
							(sanRecords[i].endTime.hour == maxEndTime.hour && sanRecords[i].endTime.min <= maxEndTime.min))) {
						tableRow(sanRecords[i]);
						searchCount++;
					}
				}
			}
		}
		else if (fieldNo == 5) {	// Search by Venue ID
			inputVenueID("  Please choose the Venue ID to search for > ", &temp.venueID);
			tableHeader();
			for (i = 0; i < *sanNo; i++) {
				if (strcmp(temp.venueID, sanRecords[i].venueID) == 0) {
					tableRow(sanRecords[i]);
					searchCount++;
				}
			}
		}
		else if (fieldNo == 6) {	// Search by Sanitization Company's Name
			printf("  Please enter the Sanitization Company's Name to search for : ");
			rewind(stdin);
			scanf("%[^\n]", temp.scd.name);
			nameCapitalize(&temp.scd.name);
			tableHeader();
			for (i = 0; i < *sanNo; i++) {
				if (strcmp(temp.scd.name, sanRecords[i].scd.name) == 0) {
					tableRow(sanRecords[i]);
					searchCount++;
				}
			}
		}
		else if (fieldNo == 7) {	// Search by Sanitization Company's contact person
			printf("  Please enter the Sanitization Company's Contact Person to search for : ");
			rewind(stdin);
			scanf("%[^\n]", temp.scd.contactPerson);
			nameCapitalize(&temp.scd.contactPerson);
			tableHeader();
			for (i = 0; i < *sanNo; i++) {
				if (strcmp(temp.scd.contactPerson, sanRecords[i].scd.contactPerson) == 0) {
					tableRow(sanRecords[i]);
					searchCount++;
				}
			}
		}
		else if (fieldNo == 8) {	// Search by Sanitization company's contact number
			inputContactNo("  Please enter the Sanitization Company's Contact Number to search for : ", &temp.scd.contactNo);
			tableHeader();
			for (i = 0; i < *sanNo; i++) {
				if (strcmp(temp.scd.contactNo, sanRecords[i].scd.contactNo) == 0) {
					tableRow(sanRecords[i]);
					searchCount++;
				}
			}
		}
		else if (fieldNo == 9) {	// Search by Type of Sanitization
			int sanTypeChoice;
			sanTypeMenu();
			sanTypeChoice = inputMenuOpt("  Please choose a type of sanitization to search for > ", 1, 3);
			switch (sanTypeChoice) {
			case 1: strcpy(temp.sanitizationType, "Surface Disinfecting"); break;
			case 2: strcpy(temp.sanitizationType, "Spray and Coat"); break;
			case 3: strcpy(temp.sanitizationType, "Fogging"); break;
			}
			tableHeader();
			for (i = 0; i < *sanNo; i++) {
				if (strcmp(temp.sanitizationType, sanRecords[i].sanitizationType) == 0) {
					tableRow(sanRecords[i]);
					searchCount++;
				}
			}
		}
		else if (fieldNo == 10) {	// Search by sanitization price
			printf(""
				"  ===================================\n"
				"  | 1 - Search by a single price    |\n"
				"  | 2 - Search by a range of price  |\n"
				"  ===================================\n"
			);
			searchMethod = inputMenuOpt("  Please choose a search method > ", 1, 2);
			if (searchMethod == 1) {		// Search by a single price
				printf("\n");
				temp.price = inputDouble("  Please enter the Sanitization Price to search on : RM");
				tableHeader();
				for (i = 0; i < *sanNo; i++) {
					if (temp.price == sanRecords[i].price) {
						tableRow(sanRecords[i]);
						searchCount++;
					}
				}
			}
			else if (searchMethod == 2) {	// Search by a range of price
				double minPrice, maxPrice;
				printf("\n");
				minPrice = inputDouble("  Please enter the minimum Sanitization Price to search on : RM");
				maxPrice = inputDouble("  Please enter the maximum Sanitization Price to search on : RM");
				tableHeader();
				for (i = 0; i < *sanNo; i++) {
					if (sanRecords[i].price >= minPrice && sanRecords[i].price <= maxPrice) {
						tableRow(sanRecords[i]);
						searchCount++;
					}
				}
			}
		}

		// Table footer
		if (fieldNo != 0) {
			tableFooter(searchCount);
			printf("  ");
			system("pause");
		}
	} while (fieldNo != 0);
}

// Produce reports from the stored records, allow user to choose from 3 types of reports to print
void reportRec(Sanitization* sanRecords, int* sanNo) {
	int i, reportOpt;
	FILE* fpVenue;

	// Report Loop, exits when user enters 4 to Quit Viewing Reports
	do {
		// Reset screen and values
		system("cls");
		sanTitle();
		functionTitle("Report Records Function");
		reportOpt = 0;

		// Get menu option
		printf(
			"  ==============================================================\n"
			"  | 1 - Past Details on Hiring Sanitization Companies          |\n"
			"  | 2 - Number of Times of Sanitizations on Each Venue         |\n"
			"  | 3 - Venues that require Sanitizations                      |\n"
			"  | 4 - Quit Viewing Reports                                   |\n"
			"  ==============================================================\n\n"
		);
		reportOpt = inputMenuOpt("  Choose a report to print > ", 1, 4);

		// Open venue file for certain report uses
		fpVenue = fopen("VenueInformationRecord.txt", "r");
		if (fpVenue == NULL) {
			printf("\n  [ Error opening file \"VenueInformationRecord.txt\". ]\n  ");
			perror("Reason");
			exit(-1);
		}

		// Report 1 : Past Details on Hiring Sanitization Companies
		if (reportOpt == 1) {
			//~~~~~~~~~~ Fetch data for Report 1 (Copy all the distinct company details into the 'companies' array) ~~~~~~~~~~
			int j, companyExists, totalCompanies = 0, totalTimesHired = 0;
			double totalMoneySpent = 0;
			struct scdPastDetails {
				char companyName[31];
				double moneySpent;
				int timesHired;
				double avgMoneyPerHire;
			} companies[MAX_SANITIZATION_COMPANIES];

			// Loop through every record in sanRecords and extract the required information
			for (i = 0; i < *sanNo; i++) {
				// Check if this record's company is in the "companies" array or not
				companyExists = 0;
				for (j = 0; j < totalCompanies; j++) {
					if (strcmp(sanRecords[i].scd.name, companies[j].companyName) == 0)
						companyExists = 1;
				}
				// Add the current record's company into the "companies" array if it isn't in the array
				if (!companyExists) {
					strcpy(companies[totalCompanies].companyName, sanRecords[i].scd.name);
					companies[totalCompanies].moneySpent = 0;
					companies[totalCompanies].timesHired = 0;
					totalCompanies++;
				}
				// Update the price and timesHired fields
				// *This loop must be run after the loops aboves to avoid records being ignored in the process
				for (j = 0; j < totalCompanies; j++) {
					if (strcmp(sanRecords[i].scd.name, companies[j].companyName) == 0) {
						companies[j].moneySpent += sanRecords[i].price;
						companies[j].timesHired++;
						totalMoneySpent += sanRecords[i].price;
						totalTimesHired++;
					}
				}
			}

			// Print Report
			printf("\n\n  ");
			centerAlign("<--- Past Details on Hiring Sanitization Companies --->", 78);
			printf("\n\n"
				"  ==============================================================================\n"
				"  |          Sanitization          |   Total Money  |  Times  | Average Amount |\n"
				"  |         Company's Name         |    Spent(RM)   |  Hired  |  per Hire(RM)  |\n"
				"  ==============================================================================\n"
			);
			for (i = 0; i < totalCompanies; i++) {
				companies[i].avgMoneyPerHire = (double)companies[i].moneySpent / companies[i].timesHired;
				printf("  | %-30s | %14.2f |  %5d  | %14.2f |\n",
					companies[i].companyName, companies[i].moneySpent, companies[i].timesHired, companies[i].avgMoneyPerHire);
			}
			printf("  ==============================================================================\n");
			printf("  | TOTAL                          | %14.2f |  %5d  |                |\n", totalMoneySpent, totalTimesHired);
			printf("  ==============================================================================\n");
		}

		// Report 2 : Number of Times of Sanitizations on Each Venue
		else if (reportOpt == 2) {
			char venueID[7], tempStr[18];;
			int sanCount, totalSanCount = 0;
			double moneySpent, totalMoneySpent = 0;

			printf("\n\n  ");
			centerAlign("<--- Number of Times of Sanitizations on Each Venue --->", 63);
			printf("\n\n"
				"  ===============================================================\n"
				"  |                      |  Number of Times  |  Money Spent on  |\n"
				"  |       Venue ID       |  of Sanitization  |  Sanitizing(RM)  |\n"
				"  ===============================================================\n"
			);
			while (fscanf(fpVenue, "%[^|]|%*[^|]|%*[^|]|%*[^|]|%*[^|]|%*[^|]|%*[^|]|%*[^|]|%*[^|]|\n", venueID) != EOF) {	// For each venue ID:
				// Reset values
				sanCount = 0;
				moneySpent = 0;

				// Calculate their total price by looping the sanRecords array
				for (i = 0; i < *sanNo; i++) {
					if (strcmp(sanRecords[i].venueID, venueID) == 0) {
						sanCount++;
						moneySpent += sanRecords[i].price;
						totalSanCount++;
						totalMoneySpent += sanRecords[i].price;
					}
				}

				// And print the row
				printf("  | %-20s | ", venueID);
				itoa(sanCount, tempStr, 10);
				centerAlign(tempStr, 17);
				printf(" |  %14.2f  |\n", moneySpent);
			}
			printf("  ===============================================================\n");
			printf("  | TOTAL:               | ");
			itoa(totalSanCount, tempStr, 10);
			centerAlign(tempStr, 17);
			printf(" |  %14.2f  |\n", totalMoneySpent);
			printf("  ===============================================================\n");
		}

		// Report 3 : Venues that require Sanitizations
		else if (reportOpt == 3) {
			// Get today's date
			SYSTEMTIME today;
			GetLocalTime(&today);
			Date lastSanDate, todayDate = { today.wDay, today.wMonth, today.wYear };

			char venueID[7];
			int daysBetween, sanitizedBefore;

			printf("\n\n  ");
			centerAlign("<--- Venues that require Sanitizations --->", 64);
			printf("\n\n"
				"  ================================================================\n"
				"  |                      | Last Sanitization |    Days since     |\n"
				"  |       Venue ID       |       Date        | Last Sanitization |\n"
				"  ================================================================\n"
			);

			// For each Venue ID,
			while (fscanf(fpVenue, "%[^|]|%*[^|]|%*[^|]|%*[^|]|%*[^|]|%*[^|]|%*[^|]|%*[^|]|%*[^|]|\n", venueID) != EOF) {
				// Reset values 
				lastSanDate.day = 1;
				lastSanDate.month = 1;
				lastSanDate.year = 1900;
				sanitizedBefore = 0;

				// Find the last sanitization date
				for (i = 0; i < *sanNo; i++) {
					if (strcmp(sanRecords[i].venueID, venueID) == 0) {
						if ((sanRecords[i].date.year > lastSanDate.year) ||
							(sanRecords[i].date.year == lastSanDate.year && sanRecords[i].date.month > lastSanDate.month) ||
							(sanRecords[i].date.year == lastSanDate.year && sanRecords[i].date.month == lastSanDate.month && sanRecords[i].date.day > lastSanDate.day)) {
							lastSanDate = sanRecords[i].date;
							sanitizedBefore = 1;
						}
					}
				}
				// Display this venue ID's data
				if (sanitizedBefore) {	// If the Venue is sanitized before
					daysBetween = calcDaysBetween(lastSanDate, todayDate);
					printf("  | %-20s |     %02d-%02d-%04d    | ",
						venueID, lastSanDate.day, lastSanDate.month, lastSanDate.year);
					char tempStr[18];
					itoa(daysBetween, tempStr, 10);
					centerAlign(tempStr, 17);
					printf(" |\n");
				}
				else {					// If the Venue is never sanitized before
					printf("  | %-20s |  Never Sanitized  | ", venueID);
					centerAlign("N/A", 17);
					printf(" |\n");
				}
			}
			printf("  ================================================================\n");
		}

		fclose(fpVenue);

		// 4 - Quit Viewing Reports
		if (reportOpt != 4) {
			printf("\n  ");
			system("pause");
		}
	} while (reportOpt != 4);
}



//============================== LOAD AND SAVE TO FILE FUNCTIONS ==============================
// Load / Read all the records from sanitizationRecords.bin into the sanRecords array
void loadSanFile(Sanitization* sanRecords, int* sanNo) {
	FILE* fsan;
	*sanNo = 0;
	fsan = fopen("sanitizationRecords.bin", "rb");
	if (fsan == NULL) {
		printf("  [ Error opening file \"sanitizationRecords.bin\". ]\n  ");
		perror("Reason");
		exit(-1);
	}
	while (fread(&sanRecords[*sanNo], sizeof(Sanitization), 1, fsan) != 0 && *sanNo < MAX_SAN_RECORDS)
		*sanNo += 1;
	fclose(fsan);
}

// Save / Write all the records from the sanRecords array into the sanitizationRecords.bin file
void saveSanFile(Sanitization* sanRecords, int* sanNo) {
	FILE* fsan;
	int i;
	fsan = fopen("sanitizationRecords.bin", "wb");
	if (fsan == NULL) {
		printf("  [ Error opening file \"sanitizationRecords.bin\". ]\n  ");
		perror("Reason");
		exit(-1);
	}
	fwrite(sanRecords, sizeof(Sanitization), *sanNo, fsan);
	fclose(fsan);
}



//=========================== USER-DEFINED FUNCTIONS================================

//============================== Input Functions ==============================
// Get input of either Y or N
char inputYN(char question[]) {
	char input;
	do {
		printf("%s", question);
		rewind(stdin);
		scanf("%c", &input);
		if (toupper(input) != 'Y' && toupper(input) != 'N') {
			invalidInput();
		}
	} while (toupper(input) != 'Y' && toupper(input) != 'N');
	return toupper(input);
}

// Get input between a min and max value (used to get menu options)
int inputMenuOpt(char question[], int min, int max) {
	int input;
	do {
		printf("%s", question);
		rewind(stdin);
		scanf("%d", &input);
		if (input < min || input > max) {
			invalidInput();
		}
	} while (input < min || input > max);
	return input;
}

// Accept and validate string input and convert it into Date structure format
Date inputDate(char question[]) {
	Date temp;
	while (1) {
		printf("%s", question);
		rewind(stdin);
		scanf("%d%*c%d%*c%d", &temp.day, &temp.month, &temp.year);
		if (temp.month > 0 && temp.month <= 12 && temp.day > 0 && temp.year >= 1900 && temp.year <= 9999) {
			if (temp.month == 2 && ((temp.year % 4 == 0 && temp.day <= 29) || (temp.year % 4 != 0 && temp.day <= 28))) {
				return temp;
			}
			else if (temp.month != 2) {
				if (temp.month <= 7 && ((temp.month % 2 == 1 && temp.day <= 31) || (temp.month % 2 == 0 && temp.day <= 30))) {
					return temp;
				}
				else if (temp.month > 7 && ((temp.month % 2 == 0 && temp.day <= 31) || (temp.month % 2 == 1 && temp.day <= 30))) {
					return temp;
				}
			}
		}
		printf("  [ Invalid date format, Please re-enter. ]\n\n");
	}
}

// Accept and validate string input in 24-hour format and convert it into Time structure format
Time inputTime(char question[]) {
	Time temp;
	while (1) {
		printf("%s", question);
		rewind(stdin);
		scanf("%d%*c%d", &temp.hour, &temp.min);
		if (temp.hour >= 0 && temp.hour < 24 && temp.min >= 0 && temp.min < 60)
			return temp;
		else
			printf("  [ Invalid time format, Please re-enter. ]\n\n");
	}
}

// Accept and validate string input of ID (eg: "SN0001") and convert into integer record ID
int inputID(char question[]) {
	char input[20];
	int tempID = 0;

	while (1) {
		printf("%s", question);
		rewind(stdin);
		scanf("%s", input);
		if (strlen(input) == 6 && toupper(input[0]) == 'S' && toupper(input[1]) == 'N'
			&& isdigit(input[2]) && isdigit(input[3]) && isdigit(input[4]) && isdigit(input[5])) {
			tempID += (input[2] - '0') * 1000;
			tempID += (input[3] - '0') * 100;
			tempID += (input[4] - '0') * 10;
			tempID += (input[5] - '0') * 1;
			return tempID;
		}
		else {
			printf("  [ Invalid Sanitization ID format, Please re-enter. ]\n");
			printf("  [ Example Sanitization ID: SN0001 ]\n\n");
		}
	}
}

// Accept and validate string input of phone numbers
void inputContactNo(char question[], char* output) {
	char input[20];
	int i, valid;

	do {
		printf("%s", question);
		rewind(stdin);
		scanf("%[^\n]", input);
		valid = 1;
		// Invalid if first number is not 0 or string length < 10
		if (input[0] != '0' || strlen(input) < 10)
			valid = 0;
		else {
			for (i = 0; i < strlen(input); i++) {
				if (!isdigit(input[i]))
					valid = 0;
			}
		}

		if (valid)
			strcpy(output, input);
		else
			invalidInput();
	} while (!valid);
}

// Accept and validate input of doubles
double inputDouble(char question[]) {
	double input;
	do {
		printf("%s", question);
		rewind(stdin);
		scanf("%lf", &input);
		if (input < 0)
			invalidInput();
	} while (input < 0);
	return input;
}

void inputVenueID(char question[], char* output) {
	FILE* fpVenue;
	fpVenue = fopen("VenueInformationRecord.txt", "r");
	if (fpVenue == NULL) {
		printf("  [ Error opening VenueInformationRecord.txt. ]\n  ");
		perror("Reason");
		exit(-1);
	}
	int i = 0;
	char tempVenueID[7];

	// Print Menu of Venue IDs
	printf("  =============================\n");
	printf("  |     List of Venue IDs     |\n");
	printf("  |---------------------------|\n");
	while (fscanf(fpVenue, "%[^|]|%*[^|]|%*[^|]|%*[^|]|%*[^|]|%*[^|]|%*[^|]|%*[^|]|%*[^|]|\n", tempVenueID) != EOF) {
		printf("  | %2d - %-20s |\n", i + 1, tempVenueID);
		i++;
	}
	printf("  =============================\n");
	fclose(fpVenue);

	// Get and validate input
	int input = inputMenuOpt(question, 1, i);

	// Convert int input to char venueID
	fpVenue = fopen("VenueInformationRecord.txt", "r");
	if (fpVenue == NULL) {
		printf("  [ Error opening VenueInformationRecord.txt. ]\n  ");
		perror("Reason");
		exit(-1);
	}
	for (i = 0; i < input; i++)
		fscanf(fpVenue, "%[^|]|%*[^|]|%*[^|]|%*[^|]|%*[^|]|%*[^|]|%*[^|]|%*[^|]|%*[^|]|\n", tempVenueID);
	fclose(fpVenue);
	strcpy(output, tempVenueID);
}



//============================== Misc Functions ==============================
// Will capitalize the first letter in every word
void nameCapitalize(char* name) {
	int i;
	name[0] = toupper(name[0]);
	for (i = 0; i < strlen(name); i++) {
		if (name[i] == ' ' && (i + 1) < strlen(name))
			name[i + 1] = toupper(name[i + 1]);
	}
}

// It will auto calculate the sanitization price based on the venue's size area
double calcSanPrice(char venueID[7], int sanMethod) {
	FILE* fpVenue;
	fpVenue = fopen("VenueInformationRecord.txt", "r");
	if (fpVenue == NULL) {
		printf("  [ Error opening VenueInformationRecord.txt. ]\n  ");
		perror("Reason");
		exit(-1);
	}

	char tempVenueID[7];
	double tempSize;
	while (fscanf(fpVenue, "%[^|]|%*[^|]|%*[^|]|%*[^|]|%lf|%*[^|]|%*[^|]|%*[^|]|%*[^|]|\n", tempVenueID, &tempSize) != EOF) {
		if (strcmp(venueID, tempVenueID) == 0)
			break;	// tempSize's value is maintained by breaking the loop here
	}
	fclose(fpVenue);
	if (sanMethod == 1)
		return tempSize * SURFACE_DISINFECT_PRICE;
	else if (sanMethod == 2)
		return tempSize * SPRAY_AND_COAT_PRICE;
	else if (sanMethod == 3)
		return tempSize * FOGGING_PRICE;
}

// Calculate the days between two given date
int calcDaysBetween(Date d1, Date d2) {
	int i, result = 0;

	// Calculate days between both years
	for (i = d1.year; i < d2.year; i++) {
		result += 365;
		if (i % 4 == 0)
			result += 1;
	}

	// Subtract days from the first date's month
	for (i = 1; i < d1.month; i++) {
		if (i == 2) {
			if (d1.year % 4 == 0)
				result -= 29;
			else
				result -= 28;
		}
		else {
			if (i % 2 == 1)
				result -= 31;
			else if (i % 2 == 0)
				result -= 30;
		}
	}
	// Subtract days from the first date's day
	result -= d1.day;

	// Add days from the second date's month
	for (i = 1; i < d2.month; i++) {
		if (i == 2) {
			if (d2.year % 4 == 0)
				result += 29;
			else
				result += 28;
		}
		else {
			if (i % 2 == 1)
				result += 31;
			else if (i % 2 == 0)
				result += 30;
		}
	}
	// Add days from the second date's day
	result += d2.day;
	return result;
}



//============================== Printf Functions - Menus ==============================
// Prints the Sanitization Records Module title
void sanTitle() {
	printf(
		"   ________________________________________________________________ \n"
		"  [~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~]\n"
		"  [           SANITIZATION RECORDS MODULE by Thong So Xue          ]\n"
		"  [________________________________________________________________]\n"
		"   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ \n"
	);
}

// Prints the main menu of the sanitization records module
void sanMenu() {
	printf(
		"  ============================\n"
		"  | 1. Add Records           |\n"
		"  | 2. Modify Records        |\n"
		"  | 3. Delete Records        |\n"
		"  | 4. Search for Records    |\n"
		"  | 5. Display All Records   |\n"
		"  | 6. Produce Reports       |\n"
		"  | 7. Back to Main Menu     |\n"
		"  ============================\n\n"
	);
}

// Prints the Type of Sanitization menu
void sanTypeMenu() {
	printf(
		"  ==============================================\n"
		"  |    Sanitization Method    | Price per sqft |\n"
		"  |--------------------------------------------|\n"
		"  | 1 - Surface Disinfecting  |     RM %.2lf    |\n"
		"  | 2 - Spray and Coat        |     RM %.2lf    |\n" 
		"  | 3 - Fogging               |     RM %.2lf    |\n"
		"  ==============================================\n"
		, SURFACE_DISINFECT_PRICE, SPRAY_AND_COAT_PRICE, FOGGING_PRICE
	);
}

// Prints the Methods to input Sanitization Price menu
void sanPriceMenu() {
	printf(
		"  ===========================================\n"
		"  | 1 - Auto-calculate Based on Venue Size  |\n"
		"  | 2 - Manually Enter the price            |\n"
		"  ===========================================\n"
	);
}



//============================== Printf Functions - Misc ==============================
// Repeatedly printf a given char for a given amount of times
void fill(char ch, int count) {
	int i;
	for (i = 0; i < count; i++)
		printf("%c", ch);
}

// Center align a given string within a given width
void centerAlign(char str[], int width) {
	int l = strlen(str);
	int indent = (width - l) / 2;

	fill(' ', indent);
	printf("%s", str);
	if ((width - l) % 2 == 1)
		indent++;
	fill(' ', indent);
}

// Prints the table header for sanitization records
void tableHeader() {
	printf("\n"
		"  =========================================================================================================================================================================\n"
		"  |        |            |       |       |                      |                    SANITIZATION COMPANY DETAILS                     |                      |             |\n"
		"  |   ID   |    DATE    | START |  END  |       VENUE ID       |---------------------------------------------------------------------|   SANITIZATION TYPE  |  PRICE(RM)  |\n"
		"  |        |            |  TIME |  TIME |                      |              NAME              |    CONTACT PERSON    | CONTACT NO  |                      |             |\n"
		"  =========================================================================================================================================================================\n"
	);
}

// Prints a given sanitization record in the table row format
void tableRow(Sanitization san) {
	printf("  | SN%04d | %02d-%02d-%04d | %02d:%02d | %02d:%02d | %-20s | %-30s | %-20s | %-11s | %-20s | %11.2lf |\n",
		san.id,
		san.date.day, san.date.month, san.date.year,
		san.startTime.hour, san.startTime.min, san.endTime.hour, san.endTime.min,
		san.venueID, san.scd.name, san.scd.contactPerson, san.scd.contactNo,
		san.sanitizationType, san.price);
}

// Prints the table footer for sanitization records, if recordCount == -1, the table footer will not print the number of records found
void tableFooter(int recordCount) {
	printf("  ");
	fill('=', 169);

	if (recordCount != -1) {
		printf("\n\n");
		fill(' ', 72);
		if (recordCount > 0)
			printf("< Total %d record(s) listed. >\n", recordCount);
		else if (recordCount == 0)
			printf("< No records found. >\n");
	}
}

// Prints the title of a main function (eg: "Add Records Function")
void functionTitle(char title[]) {
	printf("  ");
	fill('=', strlen(title) + 8);
	printf("  \n");
	printf("  |   %s   |\n", title);
	printf("  ");
	fill('=', strlen(title) + 8);
	printf("  \n");
}

// Prints the invalid input error message
void invalidInput() {
	printf("  [ Invalid input, Please re-enter. ]\n\n");
}
