#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include "structures.h"
#pragma warning(disable:4996)
#pragma warning(disable:6031)
#pragma warning(disable:6054)
#define MAX_SIZE 50

// Function Declaration
int dateValidationCheckofVisitsAndExits(int day, int month, int year);
int timeValidationCheckofVisitsAndExits(int hour, int minute);
int venueValidation(char venueid[]);
int visitsAndExitsMenu();
void displayVisitsAndExitsRecords();
void addVisitsAndExitsRecords();
void searchVisitsAndExitsRecords();
void modifyVisitsAndExitsRecords();
void deleteVisitsAndExitsRecords();
void reportVisitsAndExitsRecords();

void visitsAndExitsModule() {
	int option;

	// call visitsAneExitsMenu function to display menu and get user option
	option = visitsAndExitsMenu();

	switch (option) {
	case 1:
		displayVisitsAndExitsRecords();
		break;
	case 2:
		addVisitsAndExitsRecords();
		break;
	case 3:
		searchVisitsAndExitsRecords();
		break;
	case 4:
		modifyVisitsAndExitsRecords();
		break;
	case 5:
		deleteVisitsAndExitsRecords();
		break;
	case 6:
		reportVisitsAndExitsRecords();
		break;
	case 7:
		return;
	}
	system("pause");
}

int visitsAndExitsMenu() {
	int option;
	printf("\n _______________________________________________________________________\n");
	printf("[~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~]\n");
	printf("[        VISITS AND EXITS RECORDS MODULE  by Wang Shu Wei               ]\n");
	printf("[_______________________________________________________________________]\n");
	printf(" ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

	printf("\n===========================================\n");
	printf("| 1. Display Visits and Exits Records     |\n");
	printf("| 2. Add Visits and Exits Records         |\n");
	printf("| 3. Search Visits and Exits Records      |\n");
	printf("| 4. Modify Visits and Exits Records      |\n");
	printf("| 5. Delete Visits and Exits Records      |\n");
	printf("| 6. Visits and Exits Summary Report      |\n");
	printf("| 7. Back to Main Menu                    |\n");
	printf("===========================================\n");

	printf("\nOption (1-7) > ");
	scanf("%d", &option);

	while (option < 1 || option > 7) {
		printf("\nInvalid input!! Please enter again.\n");
		printf("Option (1-7) > ");
		scanf("%d", &option);
	}

	return option;
}

void displayVisitsAndExitsRecords() {
	FILE* pDisplay;
	VisitsAndExits records;
	int count;
	char quit;

	// Open file for reading
	pDisplay = fopen("visitsAndExits.dat", "rb");
	// Check whether the file can open or not
	if (!pDisplay) {
		printf("Unable to open visitsAndExits.dat.\n");
		exit(-1);
	}

	system("cls");
	printf("\n=========================================\n");
	printf("|    DISPLAY Visits And Exits Records   |\n");
	printf("=========================================\n");

	printf("\n------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
	printf("Visitor ID  Visitor IC Number  Date Entered  Time Entered  Time Exited  Duration of Stay  Visitor Temperature  Venue ID    Visit Purpose\n");
	printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

	count = 0;
	// Read all the records from the file
	while (fread(&records, sizeof(VisitsAndExits), 1, pDisplay) != 0) {
		/* Display the records of who haven't exit */
		// Time exited equals to -1 means that the visitor still haven't exit from the venue, so there are no time exited and duration
		if (records.timeExited.hour == -1) {
			printf("%-10s  %-16s   %02d/%02d/%04d    %02d:%02d         --:--          --              %-20.1f  %-10s  %-30s\n",
				records.visitorID, records.visitorICNo, records.dateEntered.day, records.dateEntered.month, records.dateEntered.year,
				records.timeEntered.hour, records.timeEntered.min, records.visitorTemperature, records.venueID,
				records.visitPurpose);
		}
		/* Display the records of who have already exit */
		else {
			printf("%-10s  %-16s   %02d/%02d/%04d    %02d:%02d         %02d:%02d        %2d hrs %2d min     %-20.1f  %-10s  %-30s\n",
				records.visitorID, records.visitorICNo, records.dateEntered.day, records.dateEntered.month, records.dateEntered.year,
				records.timeEntered.hour, records.timeEntered.min, records.timeExited.hour, records.timeExited.min,
				records.durationOfStay.hour, records.durationOfStay.min, records.visitorTemperature, records.venueID,
				records.visitPurpose);
		}
		count++;
	}
	printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
	printf("\t< %d record(s) listed >\n", count);

	// Close the file
	fclose(pDisplay);

	printf("\nPress y to close display function > ");
	rewind(stdin);
	scanf("%c", &quit);

	while (tolower(quit) != 'y') {
		printf("Invalid input. Please enter again!!\n");
		printf("\nPress y to close display function > ");
		rewind(stdin);
		scanf("%c", &quit);
	}

	// Back to visits and exits main page
	system("cls");
	return;
}

void addVisitsAndExitsRecords() {
	SYSTEMTIME t;
	FILE* pAdd;
	FILE* pVisitor;
	VisitsAndExits addVisit, records[MAX_SIZE];
	visitor visitorRecords[MAX_SIZE];
	int duration, option, count, i, checkRecordFound, countVisitor;
	char choice, icNo[15];

	system("cls");

	GetLocalTime(&t);
	do {
		printf("\n========================================\n");
		printf("|     ADD Visits And Exits Records     |\n");
		printf("========================================\n");
		printf("Current time: %02d-%02d-%4d %02d:%02d\n", t.wDay, t.wMonth, t.wYear, t.wHour, t.wMinute);
		printf("\n*** Make sure your ic number has been registed as visitor before adding the records.\n");

		// Let user choose either he/she want to visit or exit
		printf("\n------------------------\n");
		printf("| Visit or Exit?       |\n");
		printf("|                      |\n");
		printf("| Enter 1 for visit.   |\n");
		printf("| Enter 2 for exit.    |\n");
		printf("------------------------\n");

		printf("\nOption > ");
		scanf("%d", &option);

		// Validation for user input
		while (option != 1 && option != 2) {
			printf("\nInvalid input!! Please enter again.\n");
			printf("Option > ");
			scanf("%d", &option);
		}

		//Visit
		if (option == 1) {

			printf("\n------------------\n");
			printf("ADD Visits Records\n");
			printf("------------------\n");

			if (t.wHour >= 0 && t.wHour < 6) {
				printf("\nFailed to add visits records...\n");
				printf("**Visit can only made at 06:00 until 11:59\n");
			}
			else {
				// open visitor file for reading
				pVisitor = fopen("visitorinformation.txt", "r");

				// check whether the file can open or not
				if (!pVisitor) {
					printf("Unable to open visitorinformation.txt.\n");
					exit(-1);
				}

				// read records from visitor file and count the number of records
				i = 0;
				while (fscanf(pVisitor, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%c|%[^|]|%d|%d|%d|%d|%d|%lf\n", visitorRecords[i].ID,
					visitorRecords[i].icNum, visitorRecords[i].name, visitorRecords[i].category, visitorRecords[i].area, &visitorRecords[i].gender,
					visitorRecords[i].latestVenueVisited, &visitorRecords[i].registerTime.hour, &visitorRecords[i].registerTime.min,
					&visitorRecords[i].registerDate.day, &visitorRecords[i].registerDate.month, &visitorRecords[i].registerDate.year,
					&visitorRecords[i].temperature) != EOF) {
					i++;
				}
				countVisitor = i;

				// close visitor file
				fclose(pVisitor);

				printf("\nEnter IC number (XXXXXX-XX-XXXX): ");
				rewind(stdin);
				scanf("%s", addVisit.visitorICNo);

				checkRecordFound = 0;

				for (i = 0; i < countVisitor; i++) {
					if (strcmp(addVisit.visitorICNo, visitorRecords[i].icNum) == 0) {

						checkRecordFound = 1;

						do {
							strcpy(addVisit.visitorID, visitorRecords[i].ID);

							// Record the date and time entered by using system date instead of user input.
							addVisit.dateEntered.day = t.wDay;
							addVisit.dateEntered.month = t.wMonth;
							addVisit.dateEntered.year = t.wYear;
							addVisit.timeEntered.hour = t.wHour;
							addVisit.timeEntered.min = t.wMinute;

							printf("Enter venue ID: ");
							rewind(stdin);
							scanf("%[^\n]", addVisit.venueID);

							while (venueValidation(addVisit.venueID) == 0) {
								printf("\nNo such venue ID !! Please enter again..\n");
								printf("Enter venue ID: ");
								rewind(stdin);
								scanf("%[^\n]", addVisit.venueID);
							}

							printf("Enter visit purpose: ");
							rewind(stdin);
							scanf("%[^\n]", addVisit.visitPurpose);

							printf("Enter temperature: ");
							scanf("%lf", &addVisit.visitorTemperature);

							// Display the information inputed by the user for checking
							printf("\n---------------------------------------------------------------------------------------------------------------------------------\n");
							printf("Visitor ID\tVisitor IC Number\tDate Entered\tTime Entered\tVisitor Temperature\tVenue ID  \tVisit Purpose\n");
							printf("---------------------------------------------------------------------------------------------------------------------------------\n");
							printf("%-10s\t%-16s\t%02d/%02d/%04d  \t%02d:%02d       \t%-20.1f\t%-10s\t%-30s\n", addVisit.visitorID, addVisit.visitorICNo, addVisit.dateEntered.day, addVisit.dateEntered.month,
								addVisit.dateEntered.year, addVisit.timeEntered.hour, addVisit.timeEntered.min, addVisit.visitorTemperature, addVisit.venueID,
								addVisit.visitPurpose);
							printf("---------------------------------------------------------------------------------------------------------------------------------\n");
							printf("\nPlease ensure your information entered is correct. If yes, enter y > ");
							rewind(stdin);
							scanf("%c", &choice);

						} while (tolower(choice) != 'y');

						// As the visitor haven't exit from the venue, so there are no time exited and duration. Therefore, let them assign to -1 first
						addVisit.timeExited.hour = -1;
						addVisit.timeExited.min = -1;
						addVisit.durationOfStay.hour = -1;
						addVisit.durationOfStay.min = -1;

						// Update data to visitor information
						strcpy(visitorRecords[i].latestVenueVisited, addVisit.venueID);
						visitorRecords[i].temperature = addVisit.visitorTemperature;

						// Open file for writing
						pAdd = fopen("visitsAndExits.dat", "ab");
						// Check whether the file can open or not
						if (!pAdd) {
							printf("Unable to open visitsAndExits.dat.\n");
							exit(-1);
						}
						// Write the records into the file
						fwrite(&addVisit, sizeof(VisitsAndExits), 1, pAdd);
						// Close the file
						fclose(pAdd);

						printf("\nThe new visit information has been sucessfully recorded.\n");
					}
				}

				if (checkRecordFound == 0) {
					printf("This ic number haven't been registered yet.\n");
				}
				else {
					// open visitor file for writing
					pVisitor = fopen("visitorinformation.txt", "w");

					// check whether the file can open or not
					if (!pVisitor) {
						printf("Unable to open visitorinformation.txt.\n");
						exit(-1);
					}

					// Update visitor information text file
					for (i = 0; i < countVisitor; i++) {
						fprintf(pVisitor, "%s|%s|%s|%s|%s|%c|%s|%d|%d|%d|%d|%d|%0.2f\n", visitorRecords[i].ID,
							visitorRecords[i].icNum, visitorRecords[i].name, visitorRecords[i].category, visitorRecords[i].area, visitorRecords[i].gender,
							visitorRecords[i].latestVenueVisited, visitorRecords[i].registerTime.hour, visitorRecords[i].registerTime.min,
							visitorRecords[i].registerDate.day, visitorRecords[i].registerDate.month, visitorRecords[i].registerDate.year,
							visitorRecords[i].temperature);
					}

					// Close the file
					fclose(pVisitor);
				}
			}
		}

		//Exit
		else {
			// Open file for reading
			pAdd = fopen("visitsAndExits.dat", "rb");
			// Check wheater the file can open or not
			if (!pAdd) {
				printf("Unable to open visitsAndExits.dat.\n");
				exit(-1);
			}

			// Read the records from the file and also count the number of records found in the file
			count = 0;
			fread(&records[count], sizeof(VisitsAndExits), 1, pAdd);
			while (!feof(pAdd)) {
				count++;
				fread(&records[count], sizeof(VisitsAndExits), 1, pAdd);
			}
			// Close the file
			fclose(pAdd);


			printf("\n-----------------\n");
			printf("ADD Exits Records\n");
			printf("-----------------\n");

			printf("\nEnter IC number: ");
			rewind(stdin);
			scanf("%s", icNo);

			checkRecordFound = 0; // Equals to 0 means that the record is not found.

			//Searching the records by IC number inputed by the user
			for (i = 0; i < count; i++) {
				if (strcmp(icNo, records[i].visitorICNo) == 0 && records[i].timeExited.hour == -1) {  // time exited == -1 means that user haven't record the time exited yet.
					printf("\n------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
					printf("Visitor ID\tVisitor IC Number\tDate Entered\tTime Entered\tVisitor Temperature\tVenue ID  \tVisit Purpose\n");
					printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
					printf("%-10s\t%-19s\t%02d/%02d/%04d  \t%02d:%02d       \t%-20.1f\t%-10s\t%-30s\n", records[i].visitorID, records[i].visitorICNo, records[i].dateEntered.day, records[i].dateEntered.month,
						records[i].dateEntered.year, records[i].timeEntered.hour, records[i].timeEntered.min, records[i].visitorTemperature, records[i].venueID,
						records[i].visitPurpose);
					checkRecordFound = 1; // Equals to 1 means that the record is found.
					printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
					break;
				}
			}	

			if (checkRecordFound == 1) {
				printf("\nIs your visit's information? Press y to record exit time > ");
				rewind(stdin);
				scanf("%c", &choice);

				if (tolower(choice) == 'y') {
					// Record the time exited by system date instead of user input
					records[i].timeExited.hour = t.wHour;
					records[i].timeExited.min = t.wMinute;

					// Calculate duration stayed
					duration = (records[i].timeExited.hour * 60 + records[i].timeExited.min) - (records[i].timeEntered.hour * 60 + records[i].timeEntered.min);
					records[i].durationOfStay.hour = duration / 60;
					records[i].durationOfStay.min = duration % 60;

					// Open file for writing
					pAdd = fopen("visitsAndExits.dat", "wb");
					// Check whether ther file can open or not
					if (!pAdd) {
						printf("Unable to open visitsAndExits.dat.\n");
						exit(-1);
					}
					// Rewrite all the records from the array into the file again
					fwrite(&records, sizeof(VisitsAndExits), count, pAdd);
					// Close the file
					fclose(pAdd);
					printf("\nThe exit information has been successfully recorded.\n");
				}
			}
			else {
				printf("This ic number is not found in the records or the time exited of this ic number has already been recorded.\n");
				choice = 'x'; // Since the choice must be initailized, so simply assign a value to the choice.
			}
		}

		printf("\nAdd more records? (enter y = yes) > ");
		rewind(stdin);
		scanf("%c", &choice);

		system("cls");

	} while (tolower(choice) == 'y');

	// Back to visits and exits main page
	return;
}

void searchVisitsAndExitsRecords() {
	FILE* pSearch;
	VisitsAndExits records[MAX_SIZE];
	int count, i, option, day, month, year, recordCount;
	char visitorID[7], icNo[15], venueID[10], choice;
	double minTemperature, maxTemperature;

	// Open file for reading
	pSearch = fopen("visitsAndExits.dat", "rb");
	// Check whether the file can open or not
	if (!pSearch) {
		printf("Unable to open visitsAndExits.dat.\n");
		exit(-1);
	}

	// Read records from file can also count the number of records found in the file
	count = 0;
	fread(&records[count], sizeof(VisitsAndExits), 1, pSearch);
	while (!feof(pSearch)) {
		count++;
		fread(&records[count], sizeof(VisitsAndExits), 1, pSearch);
	}
	// Close the file
	fclose(pSearch);

	system("cls");

	do {
		printf("\n===========================================\n");
		printf("|     SEARCH Visits And Exits Records     |\n");
		printf("===========================================\n");

		printf("\n-----------------------------\n");
		printf("| Search by                 |\n");
		printf("| 1. Visitor ID             |\n");
		printf("| 2. Visitor IC number      |\n");
		printf("| 3. Date entered           |\n");
		printf("| 4. Visitor temperature    |\n");
		printf("| 5. Venue ID               |\n");
		printf("-----------------------------\n");
		printf("\nEnter your option (1-5) > ");
		scanf("%d", &option);

		// Validation of user input
		while (option < 1 || option > 5) {
			printf("\nInvalid input!! Please enter again.\n");
			printf("Enter your option (1-5) > ");
			scanf("%d", &option);
		}

		recordCount = 0;

		/* Search by visitor id */
		if (option == 1) {
			printf("\nEnter Visitor ID: ");
			rewind(stdin);
			scanf("%s", visitorID);
			printf("\n------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
			printf("Visitor ID  Visitor IC Number  Date Entered  Time Entered  Time Exited  Duration of Stay  Visitor Temperature  Venue ID    Visit Purpose\n");
			printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

			for (i = 0; i < count; i++) {
				if (strcmp(visitorID, records[i].visitorID) == 0) {
					/* Display the records of who haven't exit */
					// Time exited equals to -1 means that the visitor still haven't exit from the venue, so there are no time exited and duration
					if (records[i].timeExited.hour == -1) {
						printf("%-10s  %-16s   %02d/%02d/%04d    %02d:%02d         --:--          --              %-20.1f  %-10s  %-30s\n",
							records[i].visitorID, records[i].visitorICNo, records[i].dateEntered.day, records[i].dateEntered.month, records[i].dateEntered.year,
							records[i].timeEntered.hour, records[i].timeEntered.min, records[i].visitorTemperature, records[i].venueID,
							records[i].visitPurpose);
					}
					/* Display the records of who have already exit */
					else {
						printf("%-10s  %-16s   %02d/%02d/%04d    %02d:%02d         %02d:%02d        %2d hrs %2d min     %-20.1f  %-10s  %-30s\n",
							records[i].visitorID, records[i].visitorICNo, records[i].dateEntered.day, records[i].dateEntered.month, records[i].dateEntered.year,
							records[i].timeEntered.hour, records[i].timeEntered.min, records[i].timeExited.hour, records[i].timeExited.min,
							records[i].durationOfStay.hour, records[i].durationOfStay.min, records[i].visitorTemperature, records[i].venueID,
							records[i].visitPurpose);
					}
					recordCount++;
				}
			}
			printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
			printf("\t< %d record(s) found >\n", recordCount);
		}

		/* Search by ic number */
		else if (option == 2) {
			printf("\nEnter IC number (XXXXXX-XX-XXXX): ");
			rewind(stdin);
			scanf("%s", icNo);
			printf("\n------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
			printf("Visitor ID  Visitor IC Number  Date Entered  Time Entered  Time Exited  Duration of Stay  Visitor Temperature  Venue ID    Visit Purpose\n");
			printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

			for (i = 0; i < count; i++) {
				if (strcmp(icNo, records[i].visitorICNo) == 0) {
					/* Display the records of who haven't exit */
					// Time exited equals to -1 means that the visitor still haven't exit from the venue, so there are no time exited and duration
					if (records[i].timeExited.hour == -1) {
						printf("%-10s  %-16s   %02d/%02d/%04d    %02d:%02d         --:--          --              %-20.1f  %-10s  %-30s\n",
							records[i].visitorID, records[i].visitorICNo, records[i].dateEntered.day, records[i].dateEntered.month, records[i].dateEntered.year,
							records[i].timeEntered.hour, records[i].timeEntered.min, records[i].visitorTemperature, records[i].venueID,
							records[i].visitPurpose);
					}
					/* Display the records of who have already exit */
					else {
						printf("%-10s  %-16s   %02d/%02d/%04d    %02d:%02d         %02d:%02d        %2d hrs %2d min     %-20.1f  %-10s  %-30s\n",
							records[i].visitorID, records[i].visitorICNo, records[i].dateEntered.day, records[i].dateEntered.month, records[i].dateEntered.year,
							records[i].timeEntered.hour, records[i].timeEntered.min, records[i].timeExited.hour, records[i].timeExited.min,
							records[i].durationOfStay.hour, records[i].durationOfStay.min, records[i].visitorTemperature, records[i].venueID,
							records[i].visitPurpose);
					}
					recordCount++;
				}
			}
			printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
			printf("\t< %d record(s) found >\n", recordCount);
		}

		/* Search by date entered */
		else if (option == 3) {
			printf("\nEnter date entered (dd/mm/yyyy): ");
			scanf("%d/%d/%d", &day, &month, &year);

			while (dateValidationCheckofVisitsAndExits(day, month, year) == 0) {
				printf("\nInvalid date!! Please enter again.\n");
				printf("Enter date entered (dd/mm/yyyy): ");
				scanf("%d/%d/%d", &day, &month, &year);
			}
			printf("\n------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
			printf("Visitor ID  Visitor IC Number  Date Entered  Time Entered  Time Exited  Duration of Stay  Visitor Temperature  Venue ID    Visit Purpose\n");
			printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

			for (i = 0; i < count; i++) {
				if (day == records[i].dateEntered.day && month == records[i].dateEntered.month && year == records[i].dateEntered.year) {
					/* Display the records of who haven't exit */
					// Time exited equals to -1 means that the visitor still haven't exit from the venue, so there are no time exited and duration
					if (records[i].timeExited.hour == -1) {
						printf("%-10s  %-16s   %02d/%02d/%04d    %02d:%02d         --:--          --              %-20.1f  %-10s  %-30s\n",
							records[i].visitorID, records[i].visitorICNo, records[i].dateEntered.day, records[i].dateEntered.month, records[i].dateEntered.year,
							records[i].timeEntered.hour, records[i].timeEntered.min, records[i].visitorTemperature, records[i].venueID,
							records[i].visitPurpose);
					}
					/* Display the records of who have already exit */
					else {
						printf("%-10s  %-16s   %02d/%02d/%04d    %02d:%02d         %02d:%02d        %2d hrs %2d min     %-20.1f  %-10s  %-30s\n",
							records[i].visitorID, records[i].visitorICNo, records[i].dateEntered.day, records[i].dateEntered.month, records[i].dateEntered.year,
							records[i].timeEntered.hour, records[i].timeEntered.min, records[i].timeExited.hour, records[i].timeExited.min,
							records[i].durationOfStay.hour, records[i].durationOfStay.min, records[i].visitorTemperature, records[i].venueID,
							records[i].visitPurpose);
					}
					recordCount++;
				}
			}
			printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
			printf("\t< %d record(s) found >\n", recordCount);
		}

		/* Search by temperature */
		else if (option == 4) {
			printf("\nEnter minimum temperature: ");
			scanf("%lf", &minTemperature);
			printf("Enter maximum temperature: ");
			scanf("%lf", &maxTemperature);
			printf("\n------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
			printf("Visitor ID  Visitor IC Number  Date Entered  Time Entered  Time Exited  Duration of Stay  Visitor Temperature  Venue ID    Visit Purpose\n");
			printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

			for (i = 0; i < count; i++) {
				if (records[i].visitorTemperature >= minTemperature && records[i].visitorTemperature <= maxTemperature) {
					/* Display the records of who haven't exit */
					// Time exited equals to -1 means that the visitor still haven't exit from the venue, so there are no time exited and duration
					if (records[i].timeExited.hour == -1) {
						printf("%-10s  %-16s   %02d/%02d/%04d    %02d:%02d         --:--          --              %-20.1f  %-10s  %-30s\n",
							records[i].visitorID, records[i].visitorICNo, records[i].dateEntered.day, records[i].dateEntered.month, records[i].dateEntered.year,
							records[i].timeEntered.hour, records[i].timeEntered.min, records[i].visitorTemperature, records[i].venueID,
							records[i].visitPurpose);
					}
					/* Display the records of who have already exit */
					else {
						printf("%-10s  %-16s   %02d/%02d/%04d    %02d:%02d         %02d:%02d        %2d hrs %2d min     %-20.1f  %-10s  %-30s\n",
							records[i].visitorID, records[i].visitorICNo, records[i].dateEntered.day, records[i].dateEntered.month, records[i].dateEntered.year,
							records[i].timeEntered.hour, records[i].timeEntered.min, records[i].timeExited.hour, records[i].timeExited.min,
							records[i].durationOfStay.hour, records[i].durationOfStay.min, records[i].visitorTemperature, records[i].venueID,
							records[i].visitPurpose);
					}
					recordCount++;
				}
			}
			printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
			printf("\t< %d record(s) found >\n", recordCount);
		}

		/* Search by venue id */
		else {
			printf("\nEnter venue ID: ");
			rewind(stdin);
			scanf("%[^\n]", venueID);
			printf("\n------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
			printf("Visitor ID  Visitor IC Number  Date Entered  Time Entered  Time Exited  Duration of Stay  Visitor Temperature  Venue ID    Visit Purpose\n");
			printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

			for (i = 0; i < count; i++) {
				if (strcmp(venueID, records[i].venueID) == 0) {
					/* Display the records of who haven't exit */
					// Time exited equals to -1 means that the visitor still haven't exit from the venue, so there are no time exited and duration
					if (records[i].timeExited.hour == -1) {
						printf("%-10s  %-16s   %02d/%02d/%04d    %02d:%02d         --:--          --              %-20.1f  %-10s  %-30s\n",
							records[i].visitorID, records[i].visitorICNo, records[i].dateEntered.day, records[i].dateEntered.month, records[i].dateEntered.year,
							records[i].timeEntered.hour, records[i].timeEntered.min, records[i].visitorTemperature, records[i].venueID,
							records[i].visitPurpose);
					}
					/* Display the records of who have already exit */
					else {
						printf("%-10s  %-16s   %02d/%02d/%04d    %02d:%02d         %02d:%02d        %2d hrs %2d min     %-20.1f  %-10s  %-30s\n",
							records[i].visitorID, records[i].visitorICNo, records[i].dateEntered.day, records[i].dateEntered.month, records[i].dateEntered.year,
							records[i].timeEntered.hour, records[i].timeEntered.min, records[i].timeExited.hour, records[i].timeExited.min,
							records[i].durationOfStay.hour, records[i].durationOfStay.min, records[i].visitorTemperature, records[i].venueID,
							records[i].visitPurpose);
					}
					recordCount++;
				}
			}
			printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
			printf("\t< %d record(s) found >\n", recordCount);
		}

		printf("\nContinue searching? (enter y = yes) > ");
		rewind(stdin);
		scanf("%c", &choice);

		system("cls");

	} while (tolower(choice) == 'y');

	// Back to visits and exits main page
	return;
}

void modifyVisitsAndExitsRecords() {
	FILE* pModify;
	FILE* pVisitor;
	FILE* pVenue;
	VisitsAndExits records[MAX_SIZE];
	visitor visitorRecords[MAX_SIZE];
	int count, countVisitor, i, j, option, recordCount, arr[MAX_SIZE], duration;
	char search[15], choice, newVenue[7];
	double newTemperature;

	// Open file for reading
	pModify = fopen("visitsAndExits.dat", "rb");
	// Check whether the file can open or not
	if (!pModify) {
		printf("Unable to open visitsAndExits.dat.\n");
		exit(-1);
	}

	// Read records from file can also count the number of records found in the file
	count = 0;
	fread(&records[count], sizeof(VisitsAndExits), 1, pModify);
	while (!feof(pModify)) {
		count++;
		fread(&records[count], sizeof(VisitsAndExits), 1, pModify);
	}
	// Close the file
	fclose(pModify);


	// open visitor file for reading
	pVisitor = fopen("visitorinformation.txt", "r");

	// check whether the file can open or not
	if (!pVisitor) {
		printf("Unable to open visitorinformation.txt.\n");
		exit(-1);
	}

	// read records from visitor file and count the number of records
	i = 0;
	while (fscanf(pVisitor, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%c|%[^|]|%d|%d|%d|%d|%d|%lf\n", visitorRecords[i].ID,
		visitorRecords[i].icNum, visitorRecords[i].name, visitorRecords[i].category, visitorRecords[i].area, &visitorRecords[i].gender,
		visitorRecords[i].latestVenueVisited, &visitorRecords[i].registerTime.hour, &visitorRecords[i].registerTime.min,
		&visitorRecords[i].registerDate.day, &visitorRecords[i].registerDate.month, &visitorRecords[i].registerDate.year,
		&visitorRecords[i].temperature) != EOF) {
		i++;
	}
	countVisitor = i;

	// close visitor file
	fclose(pVisitor);

	system("cls");

	do {
		printf("\n===========================================\n");
		printf("|     MODIFY Visits And Exits Records     |\n");
		printf("===========================================\n");
		printf("\nEnter visitor ID or IC number (XXXXXX-XX-XXXX) of the record that you want to modify : ");
		rewind(stdin);
		scanf("%s", search);

		printf("\n------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
		printf("No.  Visitor ID  Visitor IC Number  Date Entered  Time Entered  Time Exited  Duration of Stay  Visitor Temperature  Venue ID    Visit Purpose\n");
		printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

		// Display the records found by the ic number and also count the records.
		recordCount = 0;
		for (i = 0; i < count; i++) {
			if (strcmp(search, records[i].visitorICNo) == 0 || strcmp(search, records[i].visitorID) == 0) {
				/* Display the records of who haven't exit */
				// Time exited equals to -1 means that the visitor still haven't exit from the venue, so there are no time exited and duration
				if (records[i].timeExited.hour == -1) {
					printf("%3d  %-10s  %-17s   %02d/%02d/%04d    %02d:%02d         --:--          --              %-20.1f  %-10s  %-30s\n", recordCount + 1,
						records[i].visitorID, records[i].visitorICNo, records[i].dateEntered.day, records[i].dateEntered.month, records[i].dateEntered.year,
						records[i].timeEntered.hour, records[i].timeEntered.min, records[i].visitorTemperature, records[i].venueID,
						records[i].visitPurpose);
				}
				/* Display the records of who have already exit */
				else {
					printf("%3d  %-10s  %-17s   %02d/%02d/%04d    %02d:%02d         %02d:%02d        %2d hrs %2d min     %-20.1f  %-10s  %-30s\n", recordCount + 1,
						records[i].visitorID, records[i].visitorICNo, records[i].dateEntered.day, records[i].dateEntered.month, records[i].dateEntered.year,
						records[i].timeEntered.hour, records[i].timeEntered.min, records[i].timeExited.hour, records[i].timeExited.min,
						records[i].durationOfStay.hour, records[i].durationOfStay.min, records[i].visitorTemperature, records[i].venueID,
						records[i].visitPurpose);
				}
				// Store the sequeance of records into an array
				arr[recordCount] = i;

				recordCount++;
			}
		}
		printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
		// Display total number of the records found.
		printf("\t< %d record(s) found >\n\n", recordCount);

		// Start modify only if there are the records found.
		if (recordCount != 0) {

			i = 0;

			// If the records found is more than 1, it will allow the user to choose either 1 to modify
			if (recordCount > 1) {
				printf("Select the record's no. that you want to modify (1-%d) > ", recordCount);
				scanf("%d", &option);

				// Validation of user's option
				while (option < 1 || option > recordCount) {
					printf("\nInvalid input. Please enter again.\n");
					printf("Select the record's no. that you want to delete (1-%d) > ", recordCount);
					scanf("%d", &option);
				}

				i = option - 1;

				printf("\n------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
				printf("Visitor ID  Visitor IC Number  Date Entered  Time Entered  Time Exited  Duration of Stay  Visitor Temperature  Venue ID    Visit Purpose\n");
				printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

				/* Display the records of who haven't exit */
				// Time exited equals to -1 means that the visitor still haven't exit from the venue, so there are no time exited and duration
				if (records[arr[i]].timeExited.hour == -1) {
					printf("%-10s  %-17s   %02d/%02d/%04d    %02d:%02d         --:--          --              %-20.1f  %-10s  %-30s\n",
						records[arr[i]].visitorID, records[arr[i]].visitorICNo, records[arr[i]].dateEntered.day, records[arr[i]].dateEntered.month, records[arr[i]].dateEntered.year,
						records[arr[i]].timeEntered.hour, records[arr[i]].timeEntered.min, records[arr[i]].visitorTemperature, records[arr[i]].venueID,
						records[arr[i]].visitPurpose);
				}
				/* Display the records of who have already exit */
				else {
					printf("%-10s  %-17s   %02d/%02d/%04d    %02d:%02d         %02d:%02d        %2d hrs %2d min     %-20.1f  %-10s  %-30s\n",
						records[arr[i]].visitorID, records[arr[i]].visitorICNo, records[arr[i]].dateEntered.day, records[arr[i]].dateEntered.month, records[arr[i]].dateEntered.year,
						records[arr[i]].timeEntered.hour, records[arr[i]].timeEntered.min, records[arr[i]].timeExited.hour, records[arr[i]].timeExited.min,
						records[arr[i]].durationOfStay.hour, records[arr[i]].durationOfStay.min, records[arr[i]].visitorTemperature, records[arr[i]].venueID,
						records[arr[i]].visitPurpose);
				}
				printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
			}
			printf("\nAre you sure to modify this record? (y = yes) > ");
			rewind(stdin);
			scanf("%c", &choice);

			if (tolower(choice) == 'y') {
				do {
					printf("\n------------------------------------------\n");
					printf("| Which field would you want to modify?  |\n");
					printf("| 1. Date entered                        |\n");
					printf("| 2. Time entered                        |\n");
					printf("| 3. Time exited                         |\n");
					printf("| 4. Temperature                         |\n");
					printf("| 5. Venue ID                            |\n");
					printf("| 6. Visit purpose                       |\n");
					printf("------------------------------------------\n");
					printf("\nEnter your option (1-6) > ");
					scanf("%d", &option);

					// User is not allow to modify the time exited if the time exited is still haven't recorded.
					while (records[arr[i]].timeExited.hour == -1 && option == 3) {
						printf("\nTime exited cannot be modify as this record doesn't include time exited yet. \n");
						printf("Enter your option (1-6, except 3) > ");
						scanf("%d", &option);
					}

					// Validation of user input
					while (option > 6 || option < 1) {
						printf("\nInvalid input!! Please enter again.\n");
						printf("Enter your option (1-6) > ");
						scanf("%d", &option);
					}

					// Let temperature and venue of the visitor information stored into the temporary variables
					for (j = 0; j < countVisitor; j++) {
						if (strcmp(records[arr[i]].visitorID, visitorRecords[j].ID) == 0) {
							newTemperature = visitorRecords[j].temperature;
							strcpy(newVenue, visitorRecords[j].latestVenueVisited);
						}
					}

					switch (option) {

						// Modify date entered
					case 1:
						printf("\nEnter date entered (dd/mm/yyyy): ");
						scanf("%d/%d/%d", &records[arr[i]].dateEntered.day, &records[arr[i]].dateEntered.month, &records[arr[i]].dateEntered.year);

						// Date validation check
						while (dateValidationCheckofVisitsAndExits(records[arr[i]].dateEntered.day, records[arr[i]].dateEntered.month,
							records[arr[i]].dateEntered.year) == 0) {
							printf("\nInvalid date!! Please enter again.\n");
							printf("Enter date entered (dd/mm/yyyy): ");
							scanf("%d/%d/%d", &records[arr[i]].dateEntered.day, &records[arr[i]].dateEntered.month, &records[arr[i]].dateEntered.year);
						}
						break;

						// Modify time entered
					case 2:
						printf("\nEnter time entered (hh:mm): ");
						scanf("%d:%d", &records[arr[i]].timeEntered.hour, &records[arr[i]].timeEntered.min);

						// Time validation check
						while (timeValidationCheckofVisitsAndExits(records[arr[i]].timeEntered.hour, records[arr[i]].timeEntered.min) == 0) {
							printf("\nInvalid time!! Please enter again.\n");
							printf("Enter time entered (hh:mm): ");
							scanf("%d:%d", &records[arr[i]].timeEntered.hour, &records[arr[i]].timeEntered.min);
						}

						// Calculate new duration
						duration = (records[arr[i]].timeExited.hour * 60 + records[arr[i]].timeExited.min) -
							(records[arr[i]].timeEntered.hour * 60 + records[arr[i]].timeEntered.min);
						records[arr[i]].durationOfStay.hour = duration / 60;
						records[arr[i]].durationOfStay.min = duration % 60;
						break;

						// Modify time exited
					case 3:
						printf("\nEnter time exited (hh:mm): ");
						scanf("%d:%d", &records[arr[i]].timeExited.hour, &records[arr[i]].timeExited.min);

						// Time validation check
						while (timeValidationCheckofVisitsAndExits(records[arr[i]].timeExited.hour, records[arr[i]].timeExited.min) == 0) {
							printf("\nInvalid time!! Please enter again.\n");
							printf("Enter time exited (hh:mm): ");
							scanf("%d:%d", &records[arr[i]].timeExited.hour, &records[arr[i]].timeExited.min);
						}

						// Calculate new duration
						duration = (records[arr[i]].timeExited.hour * 60 + records[arr[i]].timeExited.min) -
							(records[arr[i]].timeEntered.hour * 60 + records[arr[i]].timeEntered.min);
						records[arr[i]].durationOfStay.hour = duration / 60;
						records[arr[i]].durationOfStay.min = duration % 60;
						break;

						// Modify temperature
					case 4:
						printf("\nEnter temperature: ");
						scanf("%lf", &records[arr[i]].visitorTemperature);

						// Update tempororay variable by the temperature entered by user
						newTemperature = records[arr[i]].visitorTemperature;
						break;

						// Modify venue ID
					case 5:
						printf("\nEnter venue ID: ");
						rewind(stdin);
						scanf("%[^\n]", &records[arr[i]].venueID);

						// Validate venue id
						while (venueValidation(records[arr[i]].venueID) == 0) {
							printf("\nNo such venue ID !! Please enter again...\n");
							printf("Enter venue ID: ");
							rewind(stdin);
							scanf("%[^\n]", &records[arr[i]].venueID);

						}

						// Update tempororay variable by the venue entered by user
						strcpy(newVenue, records[arr[i]].venueID);
						break;

						// Modify visit purpose
					case 6:
						printf("\nEnter visit purpose: ");
						rewind(stdin);
						scanf("%[^\n]", &records[arr[i]].visitPurpose);
						break;
					}

					// Show the record after modifying
					printf("\nThis is the record after modifying.");
					printf("\n------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
					printf("Visitor ID  Visitor IC Number  Date Entered  Time Entered  Time Exited  Duration of Stay  Visitor Temperature  Venue ID    Visit Purpose\n");
					printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
					/* Display the records of who haven't exit */
						// Time exited equals to -1 means that the visitor still haven't exit from the venue, so there are no time exited and duration
					if (records[arr[i]].timeExited.hour == -1) {
						printf("%-10s  %-17s   %02d/%02d/%04d    %02d:%02d         --:--          --              %-20.1f  %-10s  %-30s\n",
							records[arr[i]].visitorID, records[arr[i]].visitorICNo, records[arr[i]].dateEntered.day, records[arr[i]].dateEntered.month, records[arr[i]].dateEntered.year,
							records[arr[i]].timeEntered.hour, records[arr[i]].timeEntered.min, records[arr[i]].visitorTemperature, records[arr[i]].venueID,
							records[arr[i]].visitPurpose);
					}
					/* Display the records of who have already exit */
					else {
						printf("%-10s  %-17s   %02d/%02d/%04d    %02d:%02d         %02d:%02d        %2d hrs %2d min     %-20.1f  %-10s  %-30s\n",
							records[arr[i]].visitorID, records[arr[i]].visitorICNo, records[arr[i]].dateEntered.day, records[arr[i]].dateEntered.month, records[arr[i]].dateEntered.year,
							records[arr[i]].timeEntered.hour, records[arr[i]].timeEntered.min, records[arr[i]].timeExited.hour, records[arr[i]].timeExited.min,
							records[arr[i]].durationOfStay.hour, records[arr[i]].durationOfStay.min, records[arr[i]].visitorTemperature, records[arr[i]].venueID,
							records[arr[i]].visitPurpose);
					}
					printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
					printf("\nPlease ensure the records is modify correctly. If yes, press y to store it > ");
					rewind(stdin);
					scanf("%c", &choice);

					if (tolower(choice) == 'y') {
						// Open file for writing
						pModify = fopen("visitsAndExits.dat", "wb");
						// Check whether ther file can open or not
						if (!pModify) {
							printf("Unable to open visitsAndExits.dat.\n");
							exit(-1);
						}
						// Rewrite all the records from the array into the file again
						fwrite(&records, sizeof(VisitsAndExits), count, pModify);
						// Close the file
						fclose(pModify);


						// Store again venue and temperature back to visitor information from the temporary variables 
						for (j = 0; j < countVisitor; j++) {
							if (strcmp(records[arr[i]].visitorID, visitorRecords[j].ID) == 0) {
								visitorRecords[j].temperature = newTemperature;
								strcpy(visitorRecords[j].latestVenueVisited, newVenue);
							}
						}

						// open visitor file for writing
						pVisitor = fopen("visitorinformation.txt", "w");

						// check whether the file can open or not
						if (!pVisitor) {
							printf("Unable to open visitorinformation.txt.\n");
							exit(-1);
						}

						// Update visitor information text file
						for (j = 0; j < countVisitor; j++) {
							fprintf(pVisitor, "%s|%s|%s|%s|%s|%c|%s|%d|%d|%d|%d|%d|%0.2f\n", visitorRecords[j].ID,
								visitorRecords[j].icNum, visitorRecords[j].name, visitorRecords[j].category, visitorRecords[j].area, visitorRecords[j].gender,
								visitorRecords[j].latestVenueVisited, visitorRecords[j].registerTime.hour, visitorRecords[j].registerTime.min,
								visitorRecords[j].registerDate.day, visitorRecords[j].registerDate.month, visitorRecords[j].registerDate.year,
								visitorRecords[j].temperature);
						}

						// Close the file
						fclose(pVisitor);
						printf("\nThe modified information has been successfully recorded.\n");
					}


					printf("\nWould you want to continue modify this record? (enter y = yes) > ");
					rewind(stdin);
					scanf("%c", &choice);

				} while (tolower(choice) == 'y');

			}
		}

		printf("\nContinue to modify other records? (enter y = yes) > ");
		rewind(stdin);
		scanf("%c", &choice);

		system("cls");

	} while (tolower(choice) == 'y');

	// Back to visits and exits main page
	return;
}

void deleteVisitsAndExitsRecords() {
	FILE* pDelete;
	VisitsAndExits records[MAX_SIZE];
	int i, j, k, count, recordCount, arr[MAX_SIZE], option;
	char choice, search[15];

	// Open the file for reading
	pDelete = fopen("visitsAndExits.dat", "rb");

	// Check whether the file can open or not
	if (!pDelete) {
		printf("Unable to open visitsAndExits.dat.\n");
		exit(-1);
	}

	// Read records from file can also count the number of records found in the file
	count = 0;
	fread(&records[count], sizeof(VisitsAndExits), 1, pDelete);
	while (!feof(pDelete)) {
		count++;
		fread(&records[count], sizeof(VisitsAndExits), 1, pDelete);
	}
	// Close the file
	fclose(pDelete);

	system("cls");

	do {
		printf("\n===========================================\n");
		printf("|     DELETE Visits And Exits Records     |\n");
		printf("===========================================\n");
		printf("\nEnter visitor ID or IC number (XXXXXX-XX-XXXX) of the record that you want to delete : ");
		rewind(stdin);
		scanf("%s", search);

		printf("\n------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
		printf("No.  Visitor ID  Visitor IC Number  Date Entered  Time Entered  Time Exited  Duration of Stay  Visitor Temperature  Venue ID    Visit Purpose\n");
		printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

		// Display the records found by the ic number and also count the records.
		recordCount = 0;
		for (i = 0; i < count; i++) {
			if (strcmp(search, records[i].visitorICNo) == 0 || strcmp(search, records[i].visitorID) == 0) {
				/* Display the records of who haven't exit */
				// Time exited equals to -1 means that the visitor still haven't exit from the venue, so there are no time exited and duration
				if (records[i].timeExited.hour == -1) {
					printf("%3d  %-10s  %-16s   %02d/%02d/%04d    %02d:%02d         --:--          --              %-20.1f  %-10s  %-30s\n", recordCount + 1,
						records[i].visitorID, records[i].visitorICNo, records[i].dateEntered.day, records[i].dateEntered.month, records[i].dateEntered.year,
						records[i].timeEntered.hour, records[i].timeEntered.min, records[i].visitorTemperature, records[i].venueID,
						records[i].visitPurpose);
				}
				/* Display the records of who have already exit */
				else {
					printf("%3d  %-10s  %-16s   %02d/%02d/%04d    %02d:%02d         %02d:%02d        %2d hrs %2d min     %-20.1f  %-10s  %-30s\n", recordCount + 1,
						records[i].visitorID, records[i].visitorICNo, records[i].dateEntered.day, records[i].dateEntered.month, records[i].dateEntered.year,
						records[i].timeEntered.hour, records[i].timeEntered.min, records[i].timeExited.hour, records[i].timeExited.min,
						records[i].durationOfStay.hour, records[i].durationOfStay.min, records[i].visitorTemperature, records[i].venueID,
						records[i].visitPurpose);
				}

				// Store the sequeance of records into array
				arr[recordCount] = i;

				recordCount++;
			}
		}
		printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
		// Display total number of the records found.
		printf("\t< %d record(s) found >\n\n", recordCount);

		// Start modify only if there are the records found.
		if (recordCount != 0) {

			i = 0;

			// If the records found is more than 1, it will allow the user to choose either 1 to delete
			if (recordCount > 1) {
				printf("Select the record's no. that you want to delete (1-%d) > ", recordCount);
				scanf("%d", &option);

				// Validation of user's option
				while (option < 1 || option > recordCount) {
					printf("\nInvalid input. Please enter again.\n");
					printf("Select the record's no. that you want to delete (1-%d) > ", recordCount);
					scanf("%d", &option);
				}

				i = option - 1;

				printf("\n------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
				printf("Visitor ID  Visitor IC Number  Date Entered  Time Entered  Time Exited  Duration of Stay  Visitor Temperature  Venue ID    Visit Purpose\n");
				printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
				/* Display the records of who haven't exit */
					// Duration equals to -1 means that the visitor still haven't exit from the venue, so there are no time exited and duration
				if (records[arr[i]].timeExited.hour == -1) {
					printf("%-10s  %-16s   %02d/%02d/%04d    %02d:%02d         --:--          --              %-20.1f  %-10s  %-30s\n",
						records[arr[i]].visitorID, records[arr[i]].visitorICNo, records[arr[i]].dateEntered.day, records[arr[i]].dateEntered.month, records[arr[i]].dateEntered.year,
						records[arr[i]].timeEntered.hour, records[arr[i]].timeEntered.min, records[arr[i]].visitorTemperature, records[arr[i]].venueID,
						records[arr[i]].visitPurpose);
				}
				/* Display the records of who have already exit */
				else {
					printf("%-10s  %-16s   %02d/%02d/%04d    %02d:%02d         %02d:%02d        %2d hrs %2d min     %-20.1f  %-10s  %-30s\n",
						records[arr[i]].visitorID, records[arr[i]].visitorICNo, records[arr[i]].dateEntered.day, records[arr[i]].dateEntered.month, records[arr[i]].dateEntered.year,
						records[arr[i]].timeEntered.hour, records[arr[i]].timeEntered.min, records[arr[i]].timeExited.hour, records[arr[i]].timeExited.min,
						records[arr[i]].durationOfStay.hour, records[arr[i]].durationOfStay.min, records[arr[i]].visitorTemperature, records[arr[i]].venueID,
						records[arr[i]].visitPurpose);
				}
				printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
			}
			printf("\nConfirm delete this record? (enter y = yes) > ");
			rewind(stdin);
			scanf("%c", &choice);

			// Start deleting the selected array of the records
			if (tolower(choice) == 'y') {
				for (j = 0; j < count; j++) {
					if (j == arr[i]) {
						for (k = j; k < count - 1; k++) {
							records[k] = records[k + 1];
						}
					}
				}
				printf("\nThe record has been successfully deleted.\n");
				// Total number of records minus 1
				count -= 1;
			}
		}

		printf("\nContinue to delete other records? (enter y = yes) > ");
		rewind(stdin);
		scanf("%c", &choice);

		system("cls");

	} while (tolower(choice) == 'y');

	// Open file for writing
	pDelete = fopen("visitsAndExits.dat", "wb");
	// Check whether ther file can open or not
	if (!pDelete) {
		printf("Unable to open visitsAndExits.dat.\n");
		exit(-1);
	}
	// Rewrite all the records from the array into the file again
	fwrite(&records, sizeof(VisitsAndExits), count, pDelete);
	// Close the file
	fclose(pDelete);

	// Back to visits and exits main page
	return;
}

void reportVisitsAndExitsRecords() {
	SYSTEMTIME t;
	FILE* pReport;
	VisitsAndExits records[MAX_SIZE];
	Date today;
	int i, count, option, totalVisits, todaysVisits, previousVisits, currentVisits, highRiskVisits, morningVisits, afternoonVisits, eveningVisits;
	double previousVisitsPct, currentVisitsPct, highRiskVisitsPct, morningVisitsPct, afternoonVisitsPct, eveningVisitsPct;
	char quit;

	// Open the file for reading
	pReport = fopen("visitsAndExits.dat", "rb");

	// Check whether the file can open or not
	if (!pReport) {
		printf("Unable to open visitsAndExits.dat.\n");
		exit(-1);
	}

	// Read records from file can also count the number of records found in the file
	count = 0;
	fread(&records[count], sizeof(VisitsAndExits), 1, pReport);
	while (!feof(pReport)) {
		count++;
		fread(&records[count], sizeof(VisitsAndExits), 1, pReport);
	}
	// Close the file
	fclose(pReport);

	// Get system date and time
	GetLocalTime(&t);

	// Total number of visits is equals to the total number of records
	totalVisits = count;

	// Let all variable assign to 0 first
	morningVisits = 0;
	afternoonVisits = 0;
	eveningVisits = 0;
	todaysVisits = 0;
	previousVisits = 0;
	currentVisits = 0;
	highRiskVisits = 0;

	// Calculation
	for (i = 0; i < count; i++) {

		// Find the total number today's visits
		if (records[i].dateEntered.day == t.wDay && records[i].dateEntered.month == t.wMonth && records[i].dateEntered.year == t.wYear) {

			todaysVisits++;

			// Find the total number of current visits and previous visits
			if (records[i].timeExited.hour == -1)
				currentVisits++;
			else
				previousVisits++;

			// Find the total number of high risk visits (which the temperature of visitor is 37.5 or higher)
			if (records[i].visitorTemperature >= 37.5)
				highRiskVisits++;
		}

		// Find the total number of visits at each period of time for overall visits
		if (records[i].timeEntered.hour >= 6 && records[i].timeEntered.hour < 12)
			morningVisits++;
		else if (records[i].timeEntered.hour >= 12 && records[i].timeEntered.hour < 18)
			afternoonVisits++;
		else if (records[i].timeEntered.hour >= 18 && records[i].timeEntered.hour <= 23)
			eveningVisits++;
	}

	// Calculate percentages
	previousVisitsPct = (double)previousVisits / todaysVisits * 100;
	currentVisitsPct = (double)currentVisits / todaysVisits * 100;
	highRiskVisitsPct = (double)highRiskVisits / todaysVisits * 100;
	morningVisitsPct = (double)morningVisits / totalVisits * 100;
	afternoonVisitsPct = (double)afternoonVisits / totalVisits * 100;
	eveningVisitsPct = (double)eveningVisits / totalVisits * 100;

	system("cls");
	// Display report
	printf("\n===========================================\n");
	printf("|     Visits And Exits Summary Report     |\n");
	printf("===========================================\n");

	// Display report date and time
	printf("\nReport Date: %d/%d/%d\n", t.wDay, t.wMonth, t.wYear);
	printf("Report Time: %02d:%02d\n", t.wHour, t.wMinute);

	// Report on today's visits
	printf("\nNumber of Today's Visits : %d\n", todaysVisits);
	printf("-----------------------------------------------------------------------------------\n");
	printf(" Number of Current Visits                        : %d (%.2f%% of today's visits)\n", currentVisits, currentVisitsPct);
	printf(" Number of Previous Visits                       : %d (%.2f%% of today's visits)\n", previousVisits, previousVisitsPct);
	printf(" Number of High Risk Visits (Temperature > 37.4) : %d (%.2f%% of today's visits)\n", highRiskVisits, highRiskVisitsPct);
	printf("-----------------------------------------------------------------------------------\n");

	// Report on overall visits
	printf("\nTotal Number of Visits : %d\n", totalVisits);
	printf("-----------------------------------------------------------------------------------\n");
	printf("Percentage of visits for each time frame: \n");
	printf("  Morning   (06:00 - 11:59) : %0.2f%% of total visits\n", morningVisitsPct);
	printf("  Afternoon (12:00 - 17:59) : %0.2f%% of total visits\n", afternoonVisitsPct);
	printf("  Evening   (18:00 - 23:59) : %0.2f%% of total visits\n", eveningVisitsPct);
	printf("-----------------------------------------------------------------------------------\n\n");

	printf("\nPress y to close the report > ");
	rewind(stdin);
	scanf("%c", &quit);

	while (tolower(quit) != 'y') {
		printf("\nInvalid input. Please enter again!!\n");
		printf("\nPress y to close the report > ");
		rewind(stdin);
		scanf("%c", &quit);
	}

	// Back to visits and exits main page
	system("cls");
	return;
}

int dateValidationCheckofVisitsAndExits(int day, int month, int year) {

	int valid = 0; // valid equals 0 means invalid

	if (month >= 1 && month <= 12) {
		switch (month) {
		case 1: case 3: case 5: case 7: case 8: case 10: case 12:
			if (day >= 1 && day <= 31)
				valid = 1; // valid equals 1 means valid
			break;
		case 2:
			if (month % 4 == 0) {
				if (day >= 1 && day <= 29)
					valid = 1;
			}
			else {
				if (day >= 1 && day <= 28)
					valid = 1;
			}
			break;
		case 4: case 6: case 9: case 11:
			if (day >= 1 && day <= 30)
				valid = 1;
			break;
		}
	}

	return valid;
}

int timeValidationCheckofVisitsAndExits(int hour, int minute) {

	int valid = 0; // means invalid

	if (hour >= 0 && hour < 24) {
		if (minute >= 0 && minute < 60)
			valid = 1; // means valid
	}

	return valid;
}

int venueValidation(char venueid[]) {
	FILE* pVenue;
	venue venueRecords[MAX_SIZE];
	int i, countVenues, valid;

	// open venue file for reading
	pVenue = fopen("VenueInformationRecord.txt", "r");

	// check whether the file can open or not
	if (!pVenue) {
		printf("Unable to open VenueInformationRecord.txt.\n");
		exit(-1);
	}

	// read records from venue file and count the number of records
	i = 0;
	while (fscanf(pVenue, "%[^|]|%[^|]|%[^|]|%[^|]|%d-%d-%d|%d|%d|%lf|%[^|]|\n", venueRecords[i].ID, venueRecords[i].name, venueRecords[i].category,
		venueRecords[i].description, &venueRecords[i].lastSanitization.day, &venueRecords[i].lastSanitization.month,
		&venueRecords[i].lastSanitization.year, &venueRecords[i].number.maxVisitors, &venueRecords[i].number.currentVisitors,
		&venueRecords[i].areaSize, &venueRecords[i].dangerLevel) != EOF)
	{
		i++;
	}
	countVenues = i;

	// close venue file
	fclose(pVenue);

	// Validate venue id whether is it found in venue records
	valid = 0; // equals to 0 means invalid
	for (i = 0; i < countVenues; i++) {
		if (strcmp(venueid, venueRecords[i].ID) == 0) {
			valid = 1; // equals to 1 means valid
		}
	}
	
	return valid;
}