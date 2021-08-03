#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include<time.h>
#include<ctype.h>
#include<string.h>
#include<windows.h>
#include "structures.h"
#pragma warning(disable:4996)
#pragma warning(disable:6031)
#define MAX_VALUE 50

//function prototype
void visitorInformationModule();
void addVisitor();
void searchVisitors();
void deleteVisitor();
void displayVisitors();
void modifyVisitors();
void highRiskReport();
void logo();
void title();
void upper(char upperChange[]);
void printRecord(visitor visitorRec);
void visitorConfirmCheck(char visitorRec[], char newRec[]);

void visitorInformationModule()
{
	int choice;
	do {
		logo();
		//menu
		printf("=====================================\n");
		printf("|1.Add Visitor Information          |\n");
		printf("|2.Search Visitor Information       |\n");
		printf("|3.Modify Visitor Information       |\n");
		printf("|4.Display Visitor Information      |\n");
		printf("|5.Delete Visitor Information       |\n");
		printf("|6.High Risk Visitor report         |\n");
		printf("|7.Back to Main Menu                |\n");
		printf("=====================================\n");


		//Allow user to enter and re-enter their choices
		printf("Enter you choice:");
		scanf("%d", &choice);
		rewind(stdin);
		system("cls");

		//move to their choices
		switch (choice)
		{
		case 1:
			addVisitor(); break;
		case 2:
			searchVisitors(); break;
		case 3:
			modifyVisitors(); break;
		case 4:
			displayVisitors(); break;
		case 5:
			deleteVisitor(); break;
		case 6:
			highRiskReport(); break;
		case 7:
			break;
		default:
			printf("Invalid choice.\n");
		};

	} while (choice != 7);
};

void addVisitor()
{
	//decalre variable
	char continueAdd;
	int dateValidity = 1;
	int categoryChoice;
	char confirmAdd;
	int i = 0, j = 0;
	int VIcount = 0;
	int venueCount = 0;
	int venueExist = 0;
	int invalid;

	//declare struct
	visitor addVisitor;
	visitor visitors[MAX_VALUE];
	venue venues[MAX_VALUE];

	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);

	// need to refresh the records by re-open file 
	do {
		//open pointer and file
		FILE* visitorFptr;
		visitorFptr = fopen("visitorInformation.txt", "r");
		FILE* venueFptr;
		venueFptr = fopen("VenueInformationRecord.txt", "r");

		//checking file is open successfully or not
		if (visitorFptr == NULL || venueFptr == NULL)
		{
			printf("Error opening file!\n");
			exit(-1);
		};

		//read records from visitorInformation.txt for validation purpose
		while (fscanf(visitorFptr, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%c|%[^|]|%d|%d|%d|%d|%d|%lf\n", &visitors[i].ID, &visitors[i].icNum, &visitors[i].name,
			&visitors[i].category, &visitors[i].area, &visitors[i].gender, &visitors[i].latestVenueVisited, &visitors[i].registerTime.hour,
			&visitors[i].registerTime.min, &visitors[i].registerDate.day, &visitors[i].registerDate.month,
			&visitors[i].registerDate.year, &visitors[i].temperature) != EOF)
		{
			i++;
		}
		VIcount = i;

		//read records from vanue.txt for validation purpose
		while (fscanf(venueFptr, "%[^|]|%[^|]|%[^|]|%[^|]|%lf|%d|%d|%d-%d-%d|%s|\n", &venues[j].ID, &venues[j].name, &venues[j].category, &venues[j].description, 
			&venues[j].areaSize, &venues[j].number.maxVisitors, &venues[j].number.currentVisitors, &venues[j].lastSanitization.day, &venues[j].lastSanitization.month, 
			&venues[j].lastSanitization.year, &venues[j].dangerLevel) != EOF)
		{
			j++;
		}
		venueCount = j;

		fclose(visitorFptr);
		fclose(venueFptr);

		//allow user to input
		printf("\nAdd Visitor Information\n");
		printf("===========================\n");
		rewind(stdin);
		printf("Enter your ID: ");
		scanf("%s", &addVisitor.ID);
		rewind(stdin);

		// to make sure visitor id entered is in upper case
		upper(addVisitor.ID);

		//validate the number of character
		while (strlen(addVisitor.ID) != 6)
		{
			printf("Invalid ID \n");
			printf("Enter your ID (6 Character): ");
			scanf("%s", &addVisitor.ID);
			rewind(stdin);
		};

		//validate with the exitsing ID
		for (i = 0; i < VIcount; i++)
		{
			if (strcmpi(addVisitor.ID, visitors[i].ID) == 0)
			{
				printf("ID already Exist.\n");
				printf("Enter your ID (6 Character): ");
				scanf("%s", &addVisitor.ID);
				rewind(stdin);
			}
		}

		do {
			invalid = 0;
			printf("Enter your IC number(XXXXXX-XX-XXXX): ");
			scanf("%[^\n]", &addVisitor.icNum);
			rewind(stdin);

			//validate the number of character
			if (strlen(addVisitor.icNum) != 14)
			{
				printf("Invalid IC Number\n");
			}
			//validate with the exitsing ic number 
			for (j = 0; j < VIcount; j++)
			{
				if (strcmpi(addVisitor.icNum, visitors[j].icNum) == 0)
				{
					printf("IC Number already Exist.\n");
					invalid = 1;
				}
			}

		} while (strlen(addVisitor.icNum) != 14 || invalid == 1);

		printf("\nEnter your name: ");
		scanf("%[^\n]", &addVisitor.name);
		rewind(stdin);

		while (strlen(addVisitor.name) > 20)
		{
			printf("Invalid Name.\n");
			printf("Enter your Name: ");
			scanf("%[^\n]", &addVisitor.name);
			rewind(stdin);
		}

		//inout category
		printf("\nEnter your category: \n");
		printf("\t 1.Student\n");
		printf("\t 2.Lecturer\n");
		printf("\t 3.Office Staff\n");
		printf("\t 4.General Worker\n");
		printf("\t 5.Outsiders\n");

		// validation and input of category
		do {
			printf("Enter your choice: ");
			scanf("%d", &categoryChoice);

			// category choice
			switch (categoryChoice)
			{
			case 1:
				strcpy(addVisitor.category, "Student"); break;
			case 2:
				strcpy(addVisitor.category, "Lecturer"); break;
			case 3:
				strcpy(addVisitor.category, "Officer Staff"); break;
			case 4:
				strcpy(addVisitor.category, "General Worker"); break;
			case 5:
				strcpy(addVisitor.category, "Outsider"); break;
			default:
				printf("Invalid Choice.\n"); break;
			}

		} while (categoryChoice < 1 || categoryChoice > 5);
		rewind(stdin);

		printf("\nEnter your living area: ");
		scanf("%[^\n]", &addVisitor.area);
		rewind(stdin);

		while (strlen(addVisitor.area) > 15)
		{
			printf("Invalid Area.\n");
			printf("Enter your Living Area: ");
			scanf("%[^\n]", &addVisitor.area);
			rewind(stdin);
		};

		printf("\nEnter your gender(M/F): ");
		scanf("%c", &addVisitor.gender);
		rewind(stdin);
		//to make gender upper case
		addVisitor.gender = toupper(addVisitor.gender);

		//validation of gender
		while (addVisitor.gender != 'M' && addVisitor.gender != 'F')
		{
			printf("Invalid gender.\n");
			printf("Enter your gender(M/F): ");
			scanf("%c", &addVisitor.gender);
			rewind(stdin);
			//to make gender upper case
			addVisitor.gender = toupper(addVisitor.gender);
		};

		printf("\nEnter latest venue visited: ");
		scanf("%[^\n]", &addVisitor.latestVenueVisited);
		rewind(stdin);

		for (j = 0; j < venueCount; j++)
		{
			//to find the vanue ID entered exist or not
			if (strcmpi(addVisitor.latestVenueVisited, venues[j].ID) == 0)
			{
				venueExist = 1;
			};
		};

		while (venueExist == 0)
		{
			printf("Venue not Exists.\n");
			printf("Enter latest venue visited: ");
			scanf("%[^\n]", &addVisitor.latestVenueVisited);
			rewind(stdin);

			for (j = 0; j < venueCount; j++)
			{
				//to find the vanue entered exist or not
				if (strcmp(addVisitor.latestVenueVisited, venues[j].ID) == 0)
				{
					venueExist = 1;
				}
			};
		};

		printf("\nEnter your temperature: ");
		scanf("%lf", &addVisitor.temperature);

		//validation on temperature
		while (addVisitor.temperature > 40 || addVisitor.temperature < 35)
		{
			printf("Invalid Temperature.\n");
			printf("Enter your temperature: ");
			scanf("%lf", &addVisitor.temperature);
		}
		rewind(stdin);

		//visitor register time
		addVisitor.registerTime.hour = sysTime.wHour;
		addVisitor.registerTime.min = sysTime.wMinute;

		//visitor register date
		addVisitor.registerDate.day = sysTime.wDay;
		addVisitor.registerDate.month = sysTime.wMonth;
		addVisitor.registerDate.year = sysTime.wYear;

		do {
			printf("\nConfirm Add ? (Y/N) ");
			scanf("%c", &confirmAdd);
			rewind(stdin);

			if (toupper(confirmAdd) == 'Y')
			{
				// re-open file to append
				FILE* addFptr;
				addFptr = fopen("visitorInformation.txt", "a");

				//append new record to txt file 
				fprintf(addFptr, "%s|%s|%s|%s|%s|%c|%s|%02d|%02d|%02d|%02d|%02d|%0.2lf\n", addVisitor.ID, addVisitor.icNum, addVisitor.name, addVisitor.category, addVisitor.area,
					addVisitor.gender, addVisitor.latestVenueVisited, addVisitor.registerTime.hour,
					addVisitor.registerTime.min, addVisitor.registerDate.day, addVisitor.registerDate.month,
					addVisitor.registerDate.year, addVisitor.temperature);

				fclose(addFptr);

				printf("Visitor added successful.\n\n");
				rewind(stdin);
			}
			else if (toupper(confirmAdd) == 'N')
			{
				printf("Visitor Add Cancelled.\n");
			}
			else
			{
				printf("Invalid Choice.\n");
			}

		} while (toupper(confirmAdd) != 'Y' && toupper(confirmAdd) != 'N');

		//ask user to continue or not
		printf("\nContinue Add? (Y/N): ");
		scanf("%c", &continueAdd);
		rewind(stdin);
		printf("\n");

		//reset data
		i = 0;
		venueExist = 0;

		system("cls");
	} while (toupper(continueAdd) == 'Y');
};

void searchVisitors()
{
	//declare variable
	int i = 0;
	int visitorCount = 0;
	int record = 0;
	int choice = 0;
	int recordExist = 0;
	int categoryChoice = 0;
	char searchID[7];
	char searchName[20];
	char searchCategory[15];
	char searchArea[10];
	char searchGender;
	char searchLVV[7];
	char continues;

	//declare struct
	visitor searchVisitor[MAX_VALUE];

	//open file pointer and file
	FILE* searchFptr;

	//checking file is open successfully or not
	searchFptr = fopen("visitorInformation.txt", "r");
	if (searchFptr == NULL)
	{
		printf("Error opening file.\n");
		exit(-1);
	};

	// read data from visitorInformation.txt to array
	while (fscanf(searchFptr, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%c|%[^|]|%d|%d|%d|%d|%d|%lf\n", &searchVisitor[i].ID, &searchVisitor[i].icNum, &searchVisitor[i].name,
		&searchVisitor[i].category, &searchVisitor[i].area, &searchVisitor[i].gender, &searchVisitor[i].latestVenueVisited, &searchVisitor[i].registerTime.hour,
		&searchVisitor[i].registerTime.min, &searchVisitor[i].registerDate.day, &searchVisitor[i].registerDate.month,
		&searchVisitor[i].registerDate.year, &searchVisitor[i].temperature)!=EOF)
	{
		i++;
	};
	visitorCount = i;

	fclose(searchFptr);

	do {
		//option for user to choose
		printf("Search Visitor by:\n");
		printf("===========================\n");
		printf("1.ID\n");
		printf("2.Name\n");
		printf("3.Category\n");
		printf("4.Living Area\n");
		printf("5.Gender\n");
		printf("6.Latest Vanue Visited\n");

		do {
			// allow user to enter choice
			printf("Enter your choice:");
			scanf("%d", &choice);
			rewind(stdin);
			printf("\n");

			if (choice == 1)
			{
				//allow user to enter ID if they choose ID to search
				printf("Enter ID to search:");
				scanf("%s", &searchID);
				rewind(stdin);
				printf("\n");
				//to found record exist or not
				for (i = 0; i < visitorCount; i++)
				{
					if (strcmpi(searchID, searchVisitor[i].ID) == 0)
					{
						recordExist = 1;
					};
				};

				if (recordExist == 0)
				{
					printf("Record Not Found.\n");
				}
				else
				{
					title();
					for (i = 0; i < visitorCount; i++)
					{
						if (strcmpi(searchID, searchVisitor[i].ID) == 0)
						{
							printRecord(searchVisitor[i]);
							record++;
						};
					};
					printf("\n\t< %d Records Listed >\n", record);
				};
			}
			else if (choice == 2)
			{
				//allow user to enter Name if they choose Name to search
				printf("Enter Name to search:");
				scanf("%[^\n]", &searchName);
				rewind(stdin);
				printf("\n");

				//to found record exist or not
				for (i = 0; i < visitorCount; i++)
				{
					if (strcmpi(searchName, searchVisitor[i].name) == 0)
					{
						recordExist = 1;
					};
				};

				if (recordExist == 0)
				{
					printf("Record Not Found.\n");
				}
				else
				{
					title();
					for (i = 0; i < visitorCount; i++)
					{
						if (strcmpi(searchName, searchVisitor[i].name) == 0)
						{
							printRecord(searchVisitor[i]);
							record++;
						};
					};
					printf("\n\t< %d Records Listed >\n", record);
				}

			}
			else if (choice == 3)
			{
				printf("Category\n");
				printf("---------------\n");
				printf("1.Student\n");
				printf("2.Lecturer\n");
				printf("3.Office Staff\n");
				printf("4.General Worker\n");
				printf("5.Outsiders\n");

				do
				{
					//allow user to choose category if they choose category to search
					printf("Enter Category to search:");
					scanf("%d", &categoryChoice);
					rewind(stdin);

					// category choice
					if (categoryChoice == 1)
						strcpy(searchCategory, "Student");
					else if (categoryChoice == 2)
						strcpy(searchCategory, "Lecturer");
					else if (categoryChoice == 3)
						strcpy(searchCategory, "Office Staff");
					else if (categoryChoice == 4)
						strcpy(searchCategory, "General Worker");
					else if (categoryChoice == 5)
						strcpy(searchCategory, "Outsider");
					else
						printf("Invalid Choice.\n");
				} while (categoryChoice < 1 || categoryChoice > 5);

				printf("\n");

				//to found record exist or not
				for (i = 0; i < visitorCount; i++)
				{
					if (strcmp(searchCategory, searchVisitor[i].category) == 0)
					{
						recordExist = 1;
					};
				};

				if (recordExist == 0)
				{
					printf("Record Not Found.\n");
				}
				else
				{
					title();
					for (i = 0; i < visitorCount; i++)
					{
						if (strcmp(searchCategory, searchVisitor[i].category) == 0)
						{
							printRecord(searchVisitor[i]);
							record++;
						};
					};
					printf("\n\t< %d Records Listed >\n", record);
				};


			}
			else if (choice == 4)
			{
				//allow user to enter area if they choose area to search
				printf("Enter Area to search:");
				scanf("%[^\n]", &searchArea);
				rewind(stdin);
				printf("\n");

				//to found record exist or not
				for (i = 0; i < visitorCount; i++)
				{
					if (strcmpi(searchArea, searchVisitor[i].area) == 0)
					{
						recordExist = 1;
					};
				};

				if (recordExist == 0)
				{
					printf("Record Not Found.\n");
				}
				else
				{
					title();
					for (i = 0; i < visitorCount; i++)
					{
						if (strcmpi(searchArea, searchVisitor[i].area) == 0)
						{
							printRecord(searchVisitor[i]);
							record++;
						};
					};
					printf("\n\t< %d Records Listed >\n", record);
				};
			}
			else if (choice == 5)
			{
				//allow user to enter gender if they choose gender to search
				printf("Enter Gender to search (M/F): ");
				scanf("%c", &searchGender);
				rewind(stdin);
				printf("\n");

				//to found record exist or not
				for (i = 0; i < visitorCount; i++)
				{
					if (toupper(searchGender) == searchVisitor[i].gender)
					{
						recordExist = 1;
					}
				};

				if (recordExist == 0)
				{
					printf("Record Not Found.\n");
				}
				else
				{
					title();
					for (i = 0; i < visitorCount; i++)
					{
						if (toupper(searchGender) == searchVisitor[i].gender)
						{
							printRecord(searchVisitor[i]);
							record++;
						};
					};
					printf("\n\t< %d Records Listed >\n", record);
				};
			}
			else if (choice == 6)
			{
				//allow user to enter Latest Venue Visited if they choose Latest Venue Visited to search
				printf("Enter Latest Venue Visited (ID) to search:");
				scanf("%[^\n]", &searchLVV);
				rewind(stdin);
				printf("\n");

				//to found record exist or not
				for (i = 0; i < visitorCount; i++)
				{
					if (strcmpi(searchLVV, searchVisitor[i].latestVenueVisited) == 0)
					{
						recordExist = 1;
					}
				};

				if (recordExist == 0)
				{
					printf("Record Not Found.\n");
				}
				else
				{
					title();
					for (i = 0; i < visitorCount; i++)
					{
						if (strcmpi(searchLVV, searchVisitor[i].latestVenueVisited) == 0)
						{
							printRecord(searchVisitor[i]);
							record++;
						};
					};
					printf("\n\t< %d Records Listed >\n", record);
				};
			}
			else
			{
				//print error massage if choice is invalid
				printf("Invalid Choice.\n");
			};

		} while (choice < 1 || choice > 6);

		//reset data  
		recordExist = 0;
		record = 0;

		printf("Continue Search ?(Y/N) ");
		scanf("%c", &continues);
		rewind(stdin);
		system("cls");

	} while (toupper(continues) == 'Y');
};

void modifyVisitors()
{
	//declare variable
	int i = 0, k = 0, j = 0;
	int VIcount, VAEcount;
	int choice;
	char confirm;
	char continueModify;
	int recordExist = 0;
	char toModify[7];
	int newCategoryChoice;
	int invalid;

	//declare struct
	visitor modifyVisitor[MAX_VALUE];
	VisitsAndExits modifyVAE[MAX_VALUE];
	visitor newModify;

	//open file pointer and file
	FILE* modifyVisitorFptr;
	modifyVisitorFptr = fopen("visitorInformation.txt", "r");
	FILE* modifyVAEFptr;
	modifyVAEFptr = fopen("VisitsAndExits.dat", "rb");

	//checking file is open successfully or not
	if (modifyVisitorFptr == NULL || modifyVAEFptr == NULL )
	{
		printf("Error opening file.\n");
		exit(-1);
	};

	// read data from visitorInformation.txt to array
	while (
		fscanf(modifyVisitorFptr, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%c|%[^|]|%d|%d|%d|%d|%d|%lf\n", &modifyVisitor[i].ID, &modifyVisitor[i].icNum, &modifyVisitor[i].name, &modifyVisitor[i].category, &modifyVisitor[i].area,
			&modifyVisitor[i].gender, &modifyVisitor[i].latestVenueVisited, &modifyVisitor[i].registerTime.hour,
			&modifyVisitor[i].registerTime.min, &modifyVisitor[i].registerDate.day, &modifyVisitor[i].registerDate.month,
			&modifyVisitor[i].registerDate.year, &modifyVisitor[i].temperature)!=EOF)
	{
		i++;
	};
	VIcount = i;
	fclose(modifyVisitorFptr);

	// read data from VisitsAndExits.txt to array
	while (fread(&modifyVAE[k], sizeof(VisitsAndExits), 1, modifyVAEFptr)!=0)
	{
		k++;
	};
	VAEcount = k;
	fclose(modifyVAEFptr);

	
	printf("Modify Visitor Module\n");
	printf("===========================\n");
	printf("Enter ID to modify: ");
	scanf("%s", &toModify);
	rewind(stdin);

	for (i = 0; i < VIcount; i++)
	{
		if (strcmpi(toModify, modifyVisitor[i].ID) == 0)
		{
			recordExist = 1;
			printf("\n");

			do {
				title();
				printRecord(modifyVisitor[i]);
				printf("\n\t< 1 Record Listed >\n");

				printf("\nData to modify\n");
				printf("-----------------\n");
				printf("1.IC Number\n");
				printf("2.Name\n");
				printf("3.Category\n");
				printf("4.Living Area\n");
				printf("5.Gender\n");
				printf("6.Temperature\n");
				printf("Enter your choice:");
				scanf("%d", &choice);
				rewind(stdin);

				printf("\n");
				switch (choice)
				{
				case 1:
					
					do{
						invalid = 0;
						printf("Enter New IC Number(XXXXXX-XX-XXXX): ");
						scanf("%[^\n]", &newModify.icNum);
						rewind(stdin);

						//validate the number of character
						if (strlen(newModify.icNum) != 14)
							printf("Invalid IC Number\n");

						//validate with the exitsing ic number 
						for (j = 0; j < VIcount; j++)
						{
							if (strcmpi(newModify.icNum, modifyVisitor[j].icNum) == 0)
							{
								printf("IC Number already Exist.\n");
								invalid = 1;
							}
						}

					} while (strlen(newModify.icNum) != 14 || invalid == 1);


					do {
						printf("Confirm Modify? (Y/N) ");
						scanf("%c", &confirm);
						rewind(stdin);

						if (toupper(confirm) == 'Y')
						{
							//to update data on visitorInformation file
							strcpy(modifyVisitor[i].icNum, newModify.icNum);

							//to update data on VisitsAndExits file if exist
							for (k = 0; k < VAEcount; k++)
							{
								if (strcmpi(toModify, modifyVAE[k].visitorID) == 0)
								{
									strcpy(modifyVAE[k].visitorICNo, newModify.icNum);
								};
							};

							// print successful massage
							printf("\nVisitor Modify Successfully.\n\n ");
						}
						else if (toupper(confirm) == 'N')
						{
							printf("\nModify Process Cancelled.\n");
						}
						else
						{
							printf("\nInvalid choice.\n");
						};

						// validate on confimation
					} while (toupper(confirm) != 'Y' && toupper(confirm) != 'N');
	
					break;

				case 2:
					
					printf("Enter New Name: ");
					scanf("%[^\n]", &newModify.name);
					rewind(stdin);

					while (strlen(newModify.name) > 20)
					{
						printf("Invalid Name.\n");
						printf("Enter New Name: ");
						scanf("%[^\n]", &newModify.name);
						rewind(stdin);
					}

					//confirm validation
					visitorConfirmCheck(modifyVisitor[i].name, newModify.name);
					break;

				case 3:
					
					
						printf("Choices Of Category\n");
						printf("---------------------------\n");
						printf("1.Student\n");
						printf("2.Lecturer\n");
						printf("3.Office Staff\n");
						printf("4.General Worker\n");
						printf("5.Outsiders\n");
					do {
						printf("Enter New category:");

						scanf("%d", &newCategoryChoice);
						rewind(stdin);

						switch (newCategoryChoice)
						{
						case 1:
							strcpy(newModify.category, "Student"); break;
						case 2:
							strcpy(newModify.category, "Lecturer"); break;
						case 3:
							strcpy(newModify.category, "Officer Staff"); break;
						case 4:
							strcpy(newModify.category, "General Worker"); break;
						case 5:
							strcpy(newModify.category, "Outsider"); break;
						default:
							printf("Invalid Choice.\n"); break;
						}
						//validate on choice
					} while (newCategoryChoice < 1 || newCategoryChoice > 5);

					//confirm validation
					visitorConfirmCheck(modifyVisitor[i].category, newModify.category);
					break;

				case 4:
					printf("Enter New Living Area: ");
					scanf("%[^\n]", &newModify.area);
					rewind(stdin);

					while (strlen(newModify.area) > 15)
					{
						printf("Invalid Area.\n");
						printf("Enter New Living Area: ");
						scanf("%[^\n]", &newModify.area);
						rewind(stdin);
					};

					//confirm validation
					visitorConfirmCheck(modifyVisitor[i].area, newModify.area);
					break;

				case 5:
					printf("Enter your gender(M/F): ");
					scanf("%c", &newModify.gender);
					rewind(stdin);
					//to make gender upper case
					newModify.gender = toupper(newModify.gender);

					//validation of gender
					while (newModify.gender != 'M' && newModify.gender != 'F')
					{
						printf("Invalid gender.\n");
						printf("Enter your gender(M/F): ");
						scanf("%c", &newModify.gender);
						rewind(stdin);
						//to make gender upper case
						newModify.gender = toupper(newModify.gender);
					};

					do {
						printf("Confirm Modify? (Y/N) ");
						scanf("%c", &confirm);
						rewind(stdin);

						if (toupper(confirm) == 'Y')
						{
							//to update data on visitor Information file
							modifyVisitor[i].gender = newModify.gender;
							printf("\nVisitor Modify Successfully.\n\n ");
						}
						else if (toupper(confirm) == 'N')
						{
							printf("\nModify Process Cancelled.\n");
						}
						else
						{
							printf("\nInvalid choice.\n");
						};

						// validate on comfimation
					} while (toupper(confirm) != 'Y' && toupper(confirm) != 'N');
					break;

				case 6:
					printf("Enter New Tempereature: ");
					scanf("%lf", &newModify.temperature);
					rewind(stdin);

					while (newModify.temperature < 35 || newModify.temperature > 40)
					{
						printf("Invalid Temperature.\n");
						printf("Enter New Tempereature: ");
						scanf("%lf", &newModify.temperature);
						rewind(stdin);
					}

					do 
					{
						printf("Confirm Modify? (Y/N) ");
						scanf("%c", &confirm);
						rewind(stdin);

						if (toupper(confirm) == 'Y')
						{
							//to update data on visitor Information file
							modifyVisitor[i].temperature = newModify.temperature;

							printf("\nVisitor Modify Successfully.\n\n ");
						}
						else if (toupper(confirm) == 'N')
						{
							printf("\nModify Process Cancelled.\n");
						}
						else
						{
							printf("\nInvalid choice.\n");
						};

						// validate on comfimation
					} while (toupper(confirm) != 'Y' && toupper(confirm) != 'N');
					break;

				default:
					printf("Invalid Choice.\n");
				};

				//allow user to continue modify data
				printf("Continue Modify This Record? (Y/N) ");
				scanf("%c", &continueModify);
				rewind(stdin);
				system("cls");

			//validation on choice
			} while (choice < 1 || choice > 6 || toupper(continueModify)=='Y');
		};
	};

	if (recordExist == 0)
	{
		printf("ID not found.\n");
		system("pause");
		system("cls");
	};

	// to update records on visitorInformation.txt file
	modifyVisitorFptr = fopen("visitorInformation.txt", "w");
	for (i = 0; i < VIcount; i++)
	{
		fprintf(modifyVisitorFptr, "%s|%s|%s|%s|%s|%c|%s|%02d|%02d|%02d|%02d|%02d|%0.2lf\n", modifyVisitor[i].ID, modifyVisitor[i].icNum, modifyVisitor[i].name, modifyVisitor[i].category, modifyVisitor[i].area,
			modifyVisitor[i].gender, modifyVisitor[i].latestVenueVisited, modifyVisitor[i].registerTime.hour,
			modifyVisitor[i].registerTime.min, modifyVisitor[i].registerDate.day, modifyVisitor[i].registerDate.month,
			modifyVisitor[i].registerDate.year, modifyVisitor[i].temperature);
	};
	fclose(modifyVisitorFptr);

	//to update records on VisitsAndExits file.
	modifyVAEFptr = fopen("VisitsAndExits.dat", "wb");
	for (k = 0; k < VAEcount; k++)
	{
		fwrite(&modifyVAE[k], sizeof(VisitsAndExits), 1, modifyVAEFptr);
	};
	fclose(modifyVAEFptr);
};

void displayVisitors()
{
	//declare variable
	int i = 0;
	int record = 0;

	//declare struct
	visitor displayVisitor;

	//open file pointer and file
	FILE* displayFptr;
	displayFptr = fopen("visitorInformation.txt", "r");

	//checking file is open successfully or not
	if (displayFptr == NULL)
	{
		printf("Error opening file.\n");
		exit(-1);
	};

	title();

	//print records
	while (fscanf(displayFptr, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%c|%[^|]|%d|%d|%d|%d|%d|%lf\n", &displayVisitor.ID, &displayVisitor.icNum, &displayVisitor.name,
		&displayVisitor.category, &displayVisitor.area, &displayVisitor.gender, &displayVisitor.latestVenueVisited, &displayVisitor.registerTime.hour,
		&displayVisitor.registerTime.min, &displayVisitor.registerDate.day, &displayVisitor.registerDate.month,
		&displayVisitor.registerDate.year, &displayVisitor.temperature)!=EOF)
	{
		printRecord(displayVisitor);
		record++;
	};

	//print number of record listed
	printf("\n\t< %d Records Listed >\n", record);
	fclose(displayFptr);

	printf("Press any key to back to Visitor Information Module.\n");
	system("pause");
	system("cls");

};

void deleteVisitor()
{
	//declare variable
	int i = 0;
	int k = 0;
	int j = 0;
	int countVI, countVAE;
	char toDelete[7];
	int recordExist = 0;
	char confirm;
	char continueDel;

	//declare struct
	visitor deleteVisitor[MAX_VALUE];
	VisitsAndExits deleteVAE[MAX_VALUE];

	//open file pointer and file
	FILE* deleteVIFptr;
	FILE* deleteVAEFptr;

	deleteVIFptr = fopen("visitorInformation.txt", "r");
	deleteVAEFptr = fopen("visitsAndExits.dat", "rb");

	//checking file is open successfully or not
	if (deleteVIFptr == NULL || deleteVAEFptr == NULL)
	{
		printf("Error opening file.\n");
		exit(-1);
	};

	// read data from visitorInformation.txt to array
	while (
		fscanf(deleteVIFptr, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%c|%[^|]|%d|%d|%d|%d|%d|%lf\n", &deleteVisitor[i].ID, &deleteVisitor[i].icNum, &deleteVisitor[i].name, &deleteVisitor[i].category, &deleteVisitor[i].area,
			&deleteVisitor[i].gender, &deleteVisitor[i].latestVenueVisited, &deleteVisitor[i].registerTime.hour,
			&deleteVisitor[i].registerTime.min, &deleteVisitor[i].registerDate.day, &deleteVisitor[i].registerDate.month,
			&deleteVisitor[i].registerDate.year, &deleteVisitor[i].temperature) != EOF)
	{
		i++;
	};
	countVI = i;
	fclose(deleteVIFptr);

	// read data from VisitsAndExits.txt to array
	while (fread(&deleteVAE[k], sizeof(VisitsAndExits), 1, deleteVAEFptr)!=0)
	{
		k++;
	};
	countVAE = k;
	fclose(deleteVAEFptr);

	do {
		printf("Delete Visitor Information\n");
		printf("================================\n");
		printf("Enter ID to delete visitor information:");
		scanf("%s", toDelete);
		rewind(stdin);
		printf("\n");
		//to check row by row
		for (i = 0; i < countVI; i++)
		{
			//to check condition
			if (strcmpi(toDelete, deleteVisitor[i].ID) == 0)
			{
				//print selected record
				title();
				printRecord(deleteVisitor[i]);
				printf("\t< 1 Record Listed >\n");

				//to record record exist
				recordExist = 1;

				do {
					printf("\n\nConfirm Delete ? (Y/N) ");
					scanf("%c", &confirm);
					rewind(stdin);

					//to replace information
					if (toupper(confirm) == 'Y')
					{
						// to identify need to delete data on visitsAndExits.dat or not
						for (k = 0; k < countVAE; k++)
						{
							if (strcmpi(deleteVAE[k].visitorID, toDelete) == 0)
							{
								//need j because dont want to disturb the k in outer loop
								for (j = k; j < countVAE - 1; j++)
								{
									deleteVAE[j] = deleteVAE[j + 1];
								};
							};
						};

						// replace information on vistorInformation.txt 
						for (i = i; i < countVI - 1; i++)
						{
							deleteVisitor[i] = deleteVisitor[i + 1];
						};

						//success delete message
						printf("Deleted Successfully.\n");

					}
					else if (toupper(confirm) == 'N')
					{
						printf("Delete Process Cancelled.\n");
					}
					else
					{
						printf("Invalid choice.\n");
					};

				//validation on comfimation
				} while (toupper(confirm) != 'Y' && toupper(confirm) != 'N');
			};
		};
		if (recordExist == 0)
		{
			printf("Record not found.\n");
		};

		// ask user to continue or not
		printf("\nContinue delete?(Y/N) ");
		scanf("%c", &continueDel);
		rewind(stdin);

		// to update count
		if (toupper(confirm) == 'Y' && toupper(continueDel) == 'Y')
		{
			countVI--;
			countVAE--;
		}

		//reset data  
		i = 0;
		k = 0;
		recordExist = 0;

		system("cls");

	} while (toupper(continueDel) == 'Y');

	//open file
	deleteVIFptr = fopen("visitorInformation.txt", "w");
	deleteVAEFptr = fopen("visitsAndExits.dat", "wb");

	// to pass data to dat file
	for (k = 0; k < countVAE - 1; k++)
	{
		fwrite(&deleteVAE[k], sizeof(VisitsAndExits), 1, deleteVAEFptr);
	};
	fclose(deleteVAEFptr);

	// to pass data to txt file
	for (i = 0; i < countVI - 1; i++)
	{
		fprintf(deleteVIFptr, "%s|%s|%s|%s|%s|%c|%s|%02d|%02d|%02d|%02d|%02d|%0.2lf\n", deleteVisitor[i].ID, deleteVisitor[i].icNum, deleteVisitor[i].name, deleteVisitor[i].category, deleteVisitor[i].area,
			deleteVisitor[i].gender, deleteVisitor[i].latestVenueVisited, deleteVisitor[i].registerTime.hour,
			deleteVisitor[i].registerTime.min, deleteVisitor[i].registerDate.day, deleteVisitor[i].registerDate.month,
			deleteVisitor[i].registerDate.year, deleteVisitor[i].temperature);
	};

	fclose(deleteVIFptr);


};

void highRiskReport()
{
	//decalre variable
	int i = 0;
	int count;
	int visitorCount = 0;
	double malePercent;
	double femalePercent;
	double highTempPercent;
	double areaPercent;

	//decalre struct
	SYSTEMTIME sysTime;
	GetLocalTime(&sysTime);
	visitor reportVisitor[MAX_VALUE];

	//open file pointer and file
	FILE* reportFptr;
	reportFptr = fopen("visitorInformation.txt", "r");

	// read data from visitorInformation.txt to array
	while (fscanf(reportFptr, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%c|%[^|]|%d|%d|%d|%d|%d|%lf\n", &reportVisitor[i].ID, &reportVisitor[i].icNum, &reportVisitor[i].name,
		&reportVisitor[i].category, &reportVisitor[i].area, &reportVisitor[i].gender, &reportVisitor[i].latestVenueVisited, &reportVisitor[i].registerTime.hour,
		&reportVisitor[i].registerTime.min, &reportVisitor[i].registerDate.day, &reportVisitor[i].registerDate.month,
		&reportVisitor[i].registerDate.year, &reportVisitor[i].temperature) != EOF)
	{
		i++;
	};
	count = i;
	fclose(reportFptr);
	system("cls");

	//show report time
	printf("Report Date: %02d/%02d/%02d \t\t Report Time: %02d:%02d \n\n", sysTime.wDay, sysTime.wMonth, sysTime.wYear, sysTime.wHour, sysTime.wMinute);
	printf("HIGH RISK REGISTERED VISITORS REPORT\n");
	printf("=========================================\n");
	printf("Total Number of Registered Visitors: %d\n", count);

	//to count male visitor
	for (i = 0; i < count; i++)
	{
		if (reportVisitor[i].gender == 'M')
			visitorCount++;
	};
	//to calculate percentage of male visitor
	malePercent = ((double)visitorCount / count) * 100;
	//to calculate percentage of female visitor
	femalePercent = 100 - malePercent;

	printf("Percentage of Male Visitors        : %0.2lf%%\n", malePercent);
	printf("Percentage of Female Visitors      : %0.2lf%%\n\n", femalePercent);

	printf("Visitors That Temperature > 37.4 \n");
	printf("--------------------------------------------\n");
	printf("%-10s %-25s %-s \n", "ID", "Name", "Temperature");
	printf("%-10s %-25s %-s \n", "========", "=======================", "=====================");

	// to reset the count
	visitorCount = 0;
	//to count and print the visitors temp > 37.4
	for (i = 0; i < count; i++)
	{
		if (reportVisitor[i].temperature > 37.4)
		{
			printf("%-10s %-25s %0.2lf\n", reportVisitor[i].ID, reportVisitor[i].name, reportVisitor[i].temperature);
			visitorCount++;
		};
	};
	printf("\n\t< %d Visitors Listed >\n", visitorCount);

	// to calculate percentage of visitor temp > 34.5
	highTempPercent = ((double)visitorCount / count) * 100;
	printf("\nPercentage of Visitors Temperature > 37.4 = %0.2lf%%\n\n", highTempPercent);
	printf("------------------------------------------------------------------\n\n");

	printf("Visitors From High Risk Area ( Puchong and Cheras )\n");
	printf("--------------------------------------------------------\n");
	printf("%-10s %-25s %-s \n", "ID", "Name", "Living Area");
	printf("%-10s %-25s %-s \n", "========", "=======================", "=====================");

	// to re-count the visitors
	visitorCount = 0;
	//to print records and count visitors from high risk area
	for (i = 0; i < count; i++)
	{
		if (strcmpi(reportVisitor[i].area, "Puchong") == 0)
		{
			printf("%-10s %-25s %-s\n", reportVisitor[i].ID, reportVisitor[i].name, reportVisitor[i].area);
			visitorCount++;
		};
	};
	// to arrange by area
	for (i = 0; i < count; i++)
	{
		if (strcmpi(reportVisitor[i].area, "Cheras") == 0)
		{
			printf("%-10s %-25s %-s\n", reportVisitor[i].ID, reportVisitor[i].name, reportVisitor[i].area);
			visitorCount++;
		};
	};

	printf("\n\t< %d Visitors Listed >\n", visitorCount);
	//to calcualte percentage of visitor from high risk area;
	areaPercent = ((double)visitorCount / count) * 100;
	printf("\nPercentage of Visitors from High Risk Area = %0.2lf%%\n\n", areaPercent);
	printf("------------------------------------------------------------------\n\n");
	printf("Press any key to back to Visitor Information Module.\n");
	system("pause");
	system("cls");
};

void logo()
{
	printf(
		" ________________________________________________________________ \n"
		"[~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~]\n"
		"[           VISITOR INFORMATION MODULE BY TAN ENG LIP            ]\n"
		"[________________________________________________________________]\n"
		" ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ \n\n"
	);
}

void title()
{
	printf("Visitor Information List\n");
	printf("============================\n");
	printf("%-10s %-20s %-25s %-15s %-15s %-10s %-25s %-17s %-17s %-s\n", "ID", "IC Number", "Name", "Category", "Area", "Gender", "Latest Vanue Visited", "Register Time", "Register Date", "Temperature");
	printf("=====================================================================================================================================================================================\n");
};

void printRecord(visitor visitorRec)
{
	printf("%-10s %-20s %-25s %-15s %-15s %-10c %-25s %02d:%02d %14d/%02d/%-11d %0.2lf\n", visitorRec.ID, visitorRec.icNum, visitorRec.name,
		visitorRec.category, visitorRec.area, visitorRec.gender, visitorRec.latestVenueVisited, visitorRec.registerTime.hour,
		visitorRec.registerTime.min, visitorRec.registerDate.day, visitorRec.registerDate.month,
		visitorRec.registerDate.year, visitorRec.temperature);
}

void upper(char upperChange[])
{
	int i;
	for (i = 0; i < strlen(upperChange); i++)
	{
		// if the character is lower, change to upper
		if (upperChange[i] >= 97 && upperChange[i] <= 122)
			upperChange[i] = upperChange[i] - 32;
	}
}

void visitorConfirmCheck(char visitorRec[], char newRec[]) 
{
	char confirm;
	int k = 0;
	do {
		printf("Confirm Modify? (Y/N) ");
		scanf("%c", &confirm);
		rewind(stdin);

		if (toupper(confirm) == 'Y')
		{
			//to update data on visitorInformation file
			strcpy(visitorRec, newRec);

			//printf successful massage
			printf("\nVisitor Modify Successfully.\n\n ");
		}
		else if (toupper(confirm) == 'N')
		{
			printf("\nModify Process Cancelled.\n");
		}
		else
		{
			printf("\nInvalid choice.\n");
		};

		// validate on confimation
	} while (toupper(confirm) != 'Y' && toupper(confirm) != 'N');
}



