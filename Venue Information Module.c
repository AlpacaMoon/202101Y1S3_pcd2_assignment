#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <windows.h>
#include <math.h>
#include "structures.h"
#pragma warning (disable:4996)
#define MAX_VALUE 50

//Function prototype
void venueInformationModule();
void venueMenu(int *choice);
void addVenue();
void searchVenue();
void modifyVenue();
void displayVenue();
void deleteVenue();
void reportVenue();
void drawALine();


//Venue information module function
void venueInformationModule()
{
	int choice, selection;
	do
	{
		//Menu
		//Allow user to enter their choices
		venueMenu(&choice);

		while (choice < 1 || choice > 7)
		{
			//Allow user to re-enter their choices
			printf("\nINVALID CHOICE!!!\n");
			printf("Please enter your choice again!!!\n");
			venueMenu(&choice);
		}
		
		drawALine();

		//Move to their choices
		switch (choice)
		{
		case 1:addVenue(); break;
		case 2:searchVenue(); break;
		case 3:modifyVenue(); break;
		case 4:displayVenue(); break;
		case 5:deleteVenue(); break;
		case 6:reportVenue(); break;
		case 7:exit(0); break;
		}
		
	} while (choice != 8);

}

void venueMenu(int *choice)
{
	printf("Venue Information Record\n");
	printf("1. Add Venue Record\n");
	printf("2. Search Venue Record\n");
	printf("3. Modify Venue Record\n");
	printf("4. Display Venue Reocrd\n");
	printf("5. Report Venue Record\n");
	printf("6. Delete Venue Record\n");
	printf("7. Exit Venue Information\n\n");
	printf("Please enter your choice : ");
	rewind(stdin);
	scanf("%d", choice);
}

void addVenue()
{
	//Declare variables
	int i = 0, vCount = 0, valid;
	char option, addConfirm;

	//Declare Struct
	venue v[MAX_VALUE];
	venue addV;
	SYSTEMTIME t;
	GetLocalTime(&t);

	//Allow user to add new venue information
	do
	{
		//Open file pointer
		FILE* fptr;
		fptr = fopen("VenueInformationRecord.txt", "r");

		//Checking the file whether is open successfully or not
		if (fptr == NULL)
		{
			printf("Unable to open the file!");
			exit(-1);
		}

		//Read records from VenueInformationRecord.txt for validation purpose
		while (fscanf(fptr, "%[^|]|%[^|]|%[^|]|%[^|]|%0.2lf|%d|%d|%d-%d-%d|%s|\n", &v[i].ID, &v[i].name, &v[i].category, &v[i].description, &v[i].areaSize, &v[i].number.maxVisitors, &v[i].number.currentVisitors, &v[i].lastSanitization.day, &v[i].lastSanitization.month, &v[i].lastSanitization.year, &v[i].dangerLevel) != EOF)
		{
			i++;
		}
		vCount = i;

		fclose(fptr);

		//Allow user to input
		printf("\nPlease enter the venue ID : ");
		rewind(stdin);
		scanf("%[^\n]", &addV.ID);

		valid = 1;
		for (i = 0; i < vCount; i++)
		{
			if (strcmp(addV.ID, v[i].ID) == 0)
			{
				valid = 0;
			}
		}
		while (!valid) {
			printf("VENUE ID ALREADY EXISTS!\n");
			printf("Please enter the venue ID again! : ");
			rewind(stdin);
			scanf("%[^\n]", &addV.ID);

			valid = 1;
			for (i = 0; i < vCount; i++)
			{
				if (strcmp(addV.ID, v[i].ID) == 0)
				{
					valid = 0;
				}
			}
		}

		//To make venue ID become upper case
		for (int j = 0; j < strlen(addV.ID); j++)
			addV.ID[j] = toupper(addV.ID[j]);

		//Validate with the existing venue ID
		while (strlen(addV.ID) > 7)
		{
			printf("INVALID INPUT!\n");
			printf("Please enter the venue ID again! : ");
			rewind(stdin);
			scanf("%[^\n]", &addV.ID);
		}
		printf("Please enter the venue name : ");
		rewind(stdin);
		scanf("%[^\n]", &addV.name);

		//Validate with the existing venue name
		while (strlen(addV.name) > 10)
		{
			printf("INVALID INPUT!\n");
			printf("Please enter the venue name again! : ");
			rewind(stdin);
			scanf("%[^\n]", &addV.name);
		}
		printf("Please enter the venue category : ");
		rewind(stdin);
		scanf("%[^\n]", &addV.category);

		//Validate with the existing venue category
		while (strlen(addV.name) > 15)
		{
			printf("INVALID INPUT!\n");
			printf("Please enter the venue category again! : ");
			rewind(stdin);
			scanf("%[^\n]", &addV.category);
		}
		printf("Please enter the venue description : ");
		rewind(stdin);
		scanf("%[^\n]", &addV.description);

		//Validate with the existing venue description
		while (strlen(addV.description) > 20)
		{
			printf("INVALID INPUT!\n");
			printf("Please enter the venue description again! : ");
			rewind(stdin);
			scanf("%[^\n]", &addV.description);
		}
		printf("Enter the venue area size (in sqft) : ");
		rewind(stdin);
		scanf("%lf", &addV.areaSize);

		//Validate with the existing venue area size (in sqft)
		while (addV.areaSize < 0)
		{
			printf("INVALID INPUT!\n");
			printf("Please enter the venue area size again! : ");
			rewind(stdin);
			scanf("%lf", &addV.areaSize);
		}
		if (addV.areaSize > 3000)
		{
			addV.number.maxVisitors = 50;
		}
		else if (addV.areaSize > 1000)
		{
			addV.number.maxVisitors = 35;
		}
		else
		{
			addV.number.maxVisitors = 20;
		}

		printf("Please enter the number of the current visitors : ");
		rewind(stdin);
		scanf("%d", &addV.number.currentVisitors);
		while (addV.number.currentVisitors < 0) {
			printf("INVALID INPUT!\n");
			printf("Please enter the number of the current visitors again! : ");
			rewind(stdin);
			scanf("%d", &addV.number.currentVisitors);
		}

		printf("Please enter the last sanitization date in (DD-MM-YYYY) form : ");
		scanf("%d %d %d", &addV.lastSanitization.day, &addV.lastSanitization.month, &addV.lastSanitization.year);

		//Validate with the existing date
		while ((addV.lastSanitization.year > t.wYear)
			|| (addV.lastSanitization.year == t.wYear && addV.lastSanitization.month > t.wMonth)
			|| (addV.lastSanitization.year == t.wYear && addV.lastSanitization.month == t.wMonth && addV.lastSanitization.day > t.wDay))
		{
			printf("INVALID INPUT!\n");
			printf("Please enter the last sanitization date again! : ");
			rewind(stdin);
			scanf("%d %d %d", &addV.lastSanitization.day, &addV.lastSanitization.month, &addV.lastSanitization.year);
		}
		if (addV.number.maxVisitors * 0.5 > addV.number.currentVisitors)
		{
			strcpy(addV.dangerLevel, "LOW");
		}
		else if (addV.number.maxVisitors * 0.5 <= addV.number.currentVisitors && addV.number.maxVisitors >= addV.number.currentVisitors)
		{
			strcpy(addV.dangerLevel, "MODERATE");
		}
		else if (addV.number.maxVisitors < addV.number.currentVisitors)
		{
			strcpy(addV.dangerLevel, "HIGH");
		}

		do
		{
			printf("\nAre you confirm to add venue information ? (Y/N) : ");
			rewind(stdin);
			scanf("%c", &addConfirm);

			if (addConfirm == 'y' || addConfirm == 'Y')
			{
				//Reopen the file pointer to append
				FILE* addfptr;
				addfptr = fopen("VenueInformationRecord.txt", "a");

				//Append new record to txt file
				fprintf(addfptr, "|%s|%s|%s|%s|%0.2lf|%d|%d|%d-%d-%d|%s|\n", addV.ID, addV.name, addV.category, addV.description, addV.areaSize, addV.number.maxVisitors, addV.number.currentVisitors, addV.lastSanitization.day, addV.lastSanitization.month, addV.lastSanitization.year, addV.dangerLevel);

				fclose(addfptr);

				printf("\nVenue information added successfully!\n");
			}
			else if (addConfirm == 'n' || addConfirm == 'N')
			{
				printf("\nVenue information cancelled.\n");
			}
			else
			{
				printf("\nINVALID CHOICE!\n");
			}
			rewind(stdin);

		} while (addConfirm != 'y' && addConfirm != 'Y' && addConfirm != 'n' && addConfirm != 'N');

		drawALine();

		printf("Do you want to add new venue information ? : ");
		rewind(stdin);
		scanf("%c", &option);
	} while (option == 'y' || option == 'Y');
}

void searchVenue()
{
	int i = 0;
	char option;

	//Declare struct
	venue searchVenue[MAX_VALUE];

	//Open file pointer and the file
	FILE* searchFptr;

	//Checking the file whether is open successfully or not
	searchFptr = fopen("VenueInformationRecord.txt", "r");
	if (searchFptr == NULL)
	{
		printf("Unable to open the file!\n");
		exit(-1);
	};

	//Read data from VenueInformationRecord.txt
	while (fscanf(searchFptr, "%[^|]|%[^|]|%[^|]|%[^|]|0.2%lf|%d|%d|%d-%d-%d|%s|\n", &searchVenue[i].ID, &searchVenue[i].name, &searchVenue[i].category, &searchVenue[i].description, &searchVenue[i].areaSize, &searchVenue[i].number.maxVisitors, &searchVenue[i].number.currentVisitors, &searchVenue[i].lastSanitization.day, &searchVenue[i].lastSanitization.month, &searchVenue[i].lastSanitization.year, &searchVenue[i].dangerLevel) != EOF)
	{
		i++;
	}

	fclose(searchFptr);

	do
	{
		printf("Do you want to continue? (Y/N) : ");
		rewind(stdin);
		scanf("%c", &option);
	} while (option == 'y' || option == 'Y');
}

void modifyVenue()
{
	int i;
	venue displayVenue;
}

void displayVenue()
{
	//Declare variables
	int i = 0;
	int display = 0;
	
	//Declare struct
	venue v;

	//Open file pointer and text file
	FILE *displayFptr;
	displayFptr = fopen("VenueInformationRecord.txt", "r");

	//Checking the file whether is open successfully or not
	if (displayFptr == NULL)
	{
		printf("Unable to open the file.\n");
		exit(-1);
	}

	printf("\t\t\t\t\t\t\t\t\t\t\t============================\n");
	printf("\t\t\t\t\t\t\t\t\t\t\t   Venue Information List\n");
	printf("\t\t\t\t\t\t\t\t\t\t\t============================\n\n");
	printf("+======================================================================================================================================================================================================+\n");
	printf("| %-10s | %-15s | %-20s | %-30s | %-15s | %-20s | %-20s | %-20s | %-20s |\n", "ID", "Name", "Category", "Description", "Area Size(sqft)", "Maximum Visitors", "Current Visitors", "Last Sanitization Date", "Danger Level");
	printf("|============+=================+======================+================================+=================+======================+======================+========================+======================|\n");

	//Print records
	while (fscanf(displayFptr, "%[^|]|%[^|]|%[^|]|%[^|]|%lf|%d|%d|%d-%d-%d|%[^|]|\n", &v.ID, &v.name, &v.category, &v.description, &v.areaSize, &v.number.maxVisitors, &v.number.currentVisitors , &v.lastSanitization.day, &v.lastSanitization.month, &v.lastSanitization.year, &v.dangerLevel) != EOF)
	{
		printf("| %-10s | %-15s | %-20s | %-30s |        %-8.2lf |           %-10d |           %-10d |       %2d/%2d/%4d       |         %-10s   |\n", v.ID, v.name, v.category, v.description, v.areaSize, v.number.maxVisitors, v.number.currentVisitors, v.lastSanitization.day, v.lastSanitization.month, v.lastSanitization.year, v.dangerLevel);
		printf("+======================================================================================================================================================================================================+\n");
		display++;
	}

	//print number of record listed
	printf("\n\t\t\t\t\t\t\t\t\t\t\t %d RECORDS LISTED \n\n", display);
	fclose(displayFptr);

	return;
}

void deleteVenue()
{
	int i;
	venue v[MAX_VALUE];
}

void reportVenue()
{
	int i;
	venue v[MAX_VALUE];
}

void drawALine()
{
	printf("\n");
	for (int l = 0; l <= 210; l++)
	{
		printf("=");
	}
	printf("\n");
}
