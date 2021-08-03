#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <Windows.h>
#include "structures.h"
#pragma warning (disable :4996)
#define MAX 100


int loginScreen();
int staffMenu();
void addStaff();
void searchStaff();
void modifyStaff();
void displayStaff();
void staffReport();


void administrativeStaffModule()
{
	int choice;
	if (loginScreen() == -1)
	{
		return 0;
	}

	SYSTEMTIME t; // structure for date, time etc
	GetLocalTime(&t); // get date and time from OS 


	do {
		choice = staffMenu();

		switch (choice) {
		case 1:
			addStaff();
			break;
		case 2:
			searchStaff();
			break;
		case 3:
			modifyStaff();
			break;
		case 4:
			displayStaff();
			break;
		case 5:
			staffReport();
			break;
		case 6:
		{
			printf("\tTHANK YOU FOR USING THIS PROGRAM\n");
			printf("Last Login at %d-%d-%d %02d:%02d\n\n", t.wDay, t.wMonth, t.wYear, t.wHour, t.wMinute);
			break;
		}
		default:
			printf("\nPlease Enter Valid Option.\n\n");
			system("pause");
			break;
		}
	} while (choice != 6);




	return;
}

int loginScreen()
{
	system("cls");
	//declare an array to store data 
	Admin login[MAX];

	int i = 0;
	//import all data from txt file
	FILE* fp;
	fp = fopen("staff.txt", "r");
	if (fp == NULL)
	{
		printf("Error of file.\n");
		exit(-1);
	}

	//scan txt file and store into array
	while (fscanf(fp, "%[^\,],", login[i].staffId) != EOF)
	{
		fscanf(fp, "%[^\,],", login[i].name);
		fscanf(fp, "%[^\,],", login[i].position);
		fscanf(fp, "%[^\,],", login[i].faculty);
		fscanf(fp, "%[^\,],", login[i].password);
		fscanf(fp, "%[^\,],", login[i].contactNumber);
		fscanf(fp, "%lf,", &login[i].salary);
		fscanf(fp, "%d/%d/%d\n", &login[i].joinDate.day, &login[i].joinDate.month, &login[i].joinDate.year);
		i++;
	}

	fclose(fp);

	//variable declaration for login check 
	char loginId[20];
	char loginPass[20];
	char loginAgain;

	//this is to check the data in the array
	/*
	for (i = 0; i < 10; i++)
		printf("%s, %s, %s, %s, %s, %s, %.2f, %s \n\n", login[i].staffId, login[i].name, login[i].position,
			login[i].faculty, login[i].password, login[i].contactNumber, login[i].salary, login[i].joinDate
		); */



	printf("\tAdministration Staff\n");
	printf("\t        Login\n");
	printf("=========================================\n");
	do {
		rewind(stdin);
		printf("\nSTAFF ID : ");
		scanf("%[^\n]", loginId);
		rewind(stdin);
		printf("Password : ");
		scanf("%[^\n]", loginPass);
		rewind(stdin);



		for (int j = 0; j < MAX; j++)
		{
			if (strcmp(login[j].staffId, loginId) == 0 && strcmp(loginPass, login[j].password) == 0)
			{
				printf("\nLogin Successful.\n\n");
				printf("Welcome %s !\n\n", login[j].name);
				system("pause");
				return 0;
			}

		}

		printf("ID or password Invalid! Please Try Again!\n");
		printf("Do you want to login again? ( y = yes ) > ");
		scanf("%c", &loginAgain);

	} while (toupper(loginAgain) == 'Y');
	printf("\nThank you for using this module. Glad to see you soon !\n\n");
	return -1;
}

int staffMenu()
{

	int option;
	system("cls");
	printf("\tAdministration Staff\n");
	printf("\t        Module\n");
	printf("=========================================\n");
	printf("1. Add record\n");
	printf("2. Search record \n");
	printf("3. Modify record \n");
	printf("4. Display record \n");
	printf("5. Display Staff Report \n");
	printf("6. Exit\n");
	printf("Your Option > ");
	scanf("%d", &option);
	printf("\n");
	return option;
	system("cls");
}

void addStaff()
{
	char confirmation; // confirm add record
	char continueAdding; // prompt user to continue or not
	int a;	//a stand for add
	int count = 0;	// count total record added

	char newStaffId[5]; // store New Staff Id
	char newName[30];
	char newPosition[20];
	char newFaculty[7];
	char newPassword[20];
	char newContactNumber[14];
	double newSalary;
	Date newJoinDate;

	//declare an array to store data 
	Admin add[MAX];

	int i = 0;

	//open pointer
	FILE* fp;
	fp = fopen("staff.txt", "r");

	//check text file 
	if (fp == NULL)
	{
		printf("Error of file.\n");
		exit(-1);
	}

	// scan text file and store to array
	while (fscanf(fp, "%[^\,],", add[i].staffId) != EOF)
	{
		fscanf(fp, "%[^\,],", add[i].name);
		fscanf(fp, "%[^\,],", add[i].position);
		fscanf(fp, "%[^\,],", add[i].faculty);
		fscanf(fp, "%[^\,],", add[i].password);
		fscanf(fp, "%[^\,],", add[i].contactNumber);
		fscanf(fp, "%lf,", &add[i].salary);
		fscanf(fp, "%d/%d/%d\n", &add[i].joinDate.day, &add[i].joinDate.month, &add[i].joinDate.year);
		i++;
	}

	a = i; //let a continue the number of the array

	//close pointer 
	fclose(fp);

	//open file to write
	FILE* addPtr;
	addPtr = fopen("staff.txt", "w");
	//check text file 
	if (addPtr == NULL)
	{
		printf("Error of file.\n");
		exit(-1);
	}
	system("cls");
	printf("\tAdministration Staff\n");
	printf("\t        Add\n");
	printf("=========================================\n");
	do {
		rewind(stdin);

		printf(" Enter New Staff Id     > ");
		scanf("%[^\n]", newStaffId);
		rewind(stdin);

		printf(" Enter Staff Name       > ");
		scanf("%[^\n]", newName);
		rewind(stdin);

		printf(" Enter Staff Position   > ");
		scanf("%[^\n]", newPosition);
		rewind(stdin);

		printf(" Enter Staff Faculty    > ");
		scanf("%[^\n]", newFaculty);
		rewind(stdin);

		printf(" Enter Staff Password   > ");
		scanf("%[^\n]", newPassword);
		rewind(stdin);

		printf(" Enter Staff Contact    > ");
		scanf("%[^\n]", newContactNumber);
		rewind(stdin);

		printf(" Enter Salary           > ");
		scanf("%lf", &newSalary);
		rewind(stdin);

		printf(" Enter Staff Join Date  > ");
		scanf("%d/%d/%d", &newJoinDate.day, &newJoinDate.month, &newJoinDate.year);
		rewind(stdin);


		//confirm adding
		printf("Are you sure you want to enter this record? (y = yes) >");
		scanf("%c", &confirmation);

		if (toupper(confirmation) == 'Y')
		{
			//copy new record to array
			strcpy(add[a].staffId, newStaffId);
			strcpy(add[a].name, newName);
			strcpy(add[a].position, newPosition);
			strcpy(add[a].faculty, newFaculty);
			strcpy(add[a].password, newPassword);
			strcpy(add[a].contactNumber, newContactNumber);
			add[a].salary = newSalary;
			add[a].joinDate.day = newJoinDate.day;
			add[a].joinDate.month = newJoinDate.month;
			add[a].joinDate.year = newJoinDate.year;


			a++;
			count++;

			printf("\nAdministration Staff Record has been added SUCCESSFULLY.\n");
		}
		rewind(stdin);
		printf("Do you want to add another record? ( y = yes ) > ");
		scanf("%c", &continueAdding);

	} while (toupper(continueAdding) == 'Y');
	system("pause");
	system("cls");
	//print everything in array to text file 
	for (int j = 0; j < a; j++)
	{

		fprintf(addPtr, "%s,", add[j].staffId);
		fprintf(addPtr, "%s,", add[j].name);
		fprintf(addPtr, "%s,", add[j].position);
		fprintf(addPtr, "%s,", add[j].faculty);
		fprintf(addPtr, "%s,", add[j].password);
		fprintf(addPtr, "%s,", add[j].contactNumber);
		fprintf(addPtr, "%0.2f,", add[j].salary);
		fprintf(addPtr, "%d/%d/%d\n", add[j].joinDate.day, add[j].joinDate.month, add[j].joinDate.year);

	}

	//close pointer
	fclose(addPtr);

	//display record added count
	printf("\n%d record(s) has been added.\n\n ", count);
	system("pause");
	printf("\n\n");
	return;

}

void searchStaff()
{
	int searchOption;
	char keyword[20];
	double searchSalaryMin, searchSalaryMax;
	char tryAgain;
	char another;
	char searchContact[5];
	Date searchDate;

	//declare an array to store data 
	Admin search[MAX];

	int i = 0;
	int s = 0;
	int countRecord = 0;

	//open pointer
	FILE* fp;
	fp = fopen("staff.txt", "r");

	//check text file 
	if (fp == NULL)
	{
		printf("Error of file.\n");
		exit(-1);
	}

	// scan text file and store to array
	while (fscanf(fp, "%[^\,],", search[i].staffId) != EOF)
	{
		fscanf(fp, "%[^\,],", search[i].name);
		fscanf(fp, "%[^\,],", search[i].position);
		fscanf(fp, "%[^\,],", search[i].faculty);
		fscanf(fp, "%[^\,],", search[i].password);
		fscanf(fp, "%[^\,],", search[i].contactNumber);
		fscanf(fp, "%lf,", &search[i].salary);
		fscanf(fp, "%d/%d/%d\n", &search[i].joinDate.day, &search[i].joinDate.month, &search[i].joinDate.year);
		i++;
	}
	fclose(fp);

	system("cls");
	printf("\tAdministration Staff\n");
	printf("\t        Search\n");
	printf("=========================================\n");
	do
	{
		// user choose to search with what data field
		rewind(stdin);
		printf("Please select the data field you would like to search.\n");
		printf("1. Staff ID\n2. Staff Name\n3. Staff Position\n4. Staff Faculty\n5. Staff Contact Number\n6. Staff Salary\n7. Staff Join Date.\n\n Data Field > ");
		scanf("%d", &searchOption);

		if (searchOption == 1)

			//checking array purpose(developer use)
			/*for (s = 0; s < i; s++)
				printf("%s, %s, %s, %s, %s, %s, %.2f, %s \n\n", search[s].staffId, search[s].name, search[s].position,
					search[s].faculty, search[s].password, search[s].contactNumber, search[s].salary, search[s].joinDate);*/
		{
			do
			{
				//enter staff ID
				rewind(stdin);
				printf("\nPlease Enter Staff ID > ");
				scanf("%[^\n]", keyword);
				printf("\n");
				// show what user entered
				printf("SEARCHED > %s\n\n", keyword);
				// search the word in array
				for (s = 0; s < i; s++)
				{
					if (strcmp(search[s].staffId, keyword) == 0)
					{
						//print record that fulfilled user needs
						printf("Staff ID        : %s\n", search[s].staffId);
						printf("Staff Name      : %s\n", search[s].name);
						printf("Staff Position  : %s\n", search[s].position);
						printf("Staff Faculty   : %s\n", search[s].faculty);
						printf("Staff Contact   : %s\n", search[s].contactNumber);
						printf("Staff Salary    : RM %.2f\n", search[s].salary);
						printf("Staff Join Date : %d/%d/%d\n\n", search[s].joinDate.day, search[s].joinDate.month, search[s].joinDate.year);
						countRecord++;
					}
				}

				rewind(stdin);

				//display total records found
				printf("  %d record(s) has been found!\n\n", countRecord);

				//reset the count for next search
				countRecord = 0;

				// search again
				printf("Do you want to search with staff ID again? ( y = yes ) > ");
				scanf("%c", &tryAgain);

			} while (toupper(tryAgain) == 'Y');
		}
		else if (searchOption == 2)
		{
			do
			{
				rewind(stdin);
				printf("Please Enter Staff Name > ");
				scanf("%[^\n]", keyword);
				printf("\n");
				printf("SEARCHED > %s\n\n", keyword);
				for (s = 0; s < i; s++)
				{
					if (strcmp(search[s].name, keyword) == 0)
					{
						printf("Staff ID        : %s\n", search[s].staffId);
						printf("Staff Name      : %s\n", search[s].name);
						printf("Staff Position  : %s\n", search[s].position);
						printf("Staff Faculty   : %s\n", search[s].faculty);
						printf("Staff Contact   : %s\n", search[s].contactNumber);
						printf("Staff Salary    : RM %.2f\n", search[s].salary);
						printf("Staff Join Date : %d/%d/%d\n\n", search[s].joinDate.day, search[s].joinDate.month, search[s].joinDate.year);
						countRecord++;
					}
				}

				rewind(stdin);


				printf("  %d record(s) has been found!\n\n", countRecord);
				countRecord = 0;
				printf("Do you want to search with staff name again? ( y = yes ) >");
				scanf("%c", &tryAgain);

			} while (toupper(tryAgain) == 'Y');
		}
		else if (searchOption == 3)
		{
			do
			{
				rewind(stdin);
				printf("Please Enter Staff Position > ");
				scanf("%[^\n]", keyword);
				printf("\n");
				printf("SEARCHED > %s\n\n", keyword);
				for (s = 0; s < i; s++)
				{
					if (strcmp(search[s].position, keyword) == 0)
					{
						printf("Staff ID        : %s\n", search[s].staffId);
						printf("Staff Name      : %s\n", search[s].name);
						printf("Staff Position  : %s\n", search[s].position);
						printf("Staff Faculty   : %s\n", search[s].faculty);
						printf("Staff Contact   : %s\n", search[s].contactNumber);
						printf("Staff Salary    : RM %.2f\n", search[s].salary);
						printf("Staff Join Date : %d/%d/%d\n\n", search[s].joinDate.day, search[s].joinDate.month, search[s].joinDate.year);
						countRecord++;
					}
				}

				rewind(stdin);


				printf("  %d record(s) has been found!\n\n", countRecord);
				countRecord = 0;
				printf("Do you want to search with staff Position again? ( y = yes ) >");
				scanf("%c", &tryAgain);


			} while (toupper(tryAgain) == 'Y');
		}
		else if (searchOption == 4)
		{
			do
			{
				rewind(stdin);
				printf("\nPlease Enter Staff faculty > ");
				scanf("%[^\n]", keyword);
				printf("\n");
				printf("SEARCHED > %s\n\n", keyword);
				for (s = 0; s < i; s++)
				{
					if (strcmp(search[s].faculty, keyword) == 0)
					{

						printf("Staff ID        : %s\n", search[s].staffId);
						printf("Staff Name      : %s\n", search[s].name);
						printf("Staff Position  : %s\n", search[s].position);
						printf("Staff Faculty   : %s\n", search[s].faculty);
						printf("Staff Contact   : %s\n", search[s].contactNumber);
						printf("Staff Salary    : RM %.2f\n", search[s].salary);
						printf("Staff Join Date : %d/%d/%d\n\n", search[s].joinDate.day, search[s].joinDate.month, search[s].joinDate.year);
						countRecord++;
					}
				}

				rewind(stdin);


				printf("  %d record(s) has been found!\n\n", countRecord);
				countRecord = 0;
				printf("Do you want to search with faculty again? ( y = yes ) > ");
				scanf("%c", &tryAgain);
			} while (toupper(tryAgain) == 'Y');
		}
		else if (searchOption == 5)
		{
			do
			{
				rewind(stdin);
				printf("Please Enter first three number of Contact ( 01X ) > ");
				scanf("%[^\n]", searchContact);
				printf("\n");
				printf("SEARCHED > %s\n\n", searchContact);
				for (s = 0; s < i; s++)
				{
					if (search[s].contactNumber[0] == searchContact[0])
					{
						if (search[s].contactNumber[1] == searchContact[1])
						{
							if (search[s].contactNumber[2] == searchContact[2])
							{
								printf("Staff ID        : %s\n", search[s].staffId);
								printf("Staff Name      : %s\n", search[s].name);
								printf("Staff Position  : %s\n", search[s].position);
								printf("Staff Faculty   : %s\n", search[s].faculty);
								printf("Staff Contact   : %s\n", search[s].contactNumber);
								printf("Staff Salary    : RM %.2f\n", search[s].salary);
								printf("Staff Join Date : %d/%d/%d\n\n", search[s].joinDate.day, search[s].joinDate.month, search[s].joinDate.year);
								countRecord++;

							}
						}
					}
				}

				rewind(stdin);


				printf("  %d record(s) has been found!\n\n", countRecord);
				countRecord = 0;
				printf("Do you want to search with contact number again? ( y = yes ) > ");
				scanf("%c", &tryAgain);

			} while (toupper(tryAgain) == 'Y');
		}
		else if (searchOption == 6)
		{
			do
			{
				rewind(stdin);
				printf("Please Enter Staff Salary (Min) > ");
				scanf("%lf", &searchSalaryMin);
				printf("Please Enter Staff Salary (Max) > ");
				scanf("%lf", &searchSalaryMax);
				rewind(stdin);

				printf("searched min > %.2f\n", searchSalaryMin);
				printf("searched max > %.2f\n\n", searchSalaryMin);
				for (s = 0; s < i; s++)
				{
					if (search[s].salary >= searchSalaryMin && search[s].salary <= searchSalaryMax)
					{

						printf("Staff ID        : %s\n", search[s].staffId);
						printf("Staff Name      : %s\n", search[s].name);
						printf("Staff Position  : %s\n", search[s].position);
						printf("Staff Faculty   : %s\n", search[s].faculty);
						printf("Staff Contact   : %s\n", search[s].contactNumber);
						printf("Staff Salary    : RM %.2f\n", search[s].salary);
						printf("Staff Join Date : %d/%d/%d\n\n", search[s].joinDate.day, search[s].joinDate.month, search[s].joinDate.year);
						countRecord++;
					}
				}

				rewind(stdin);


				printf("  %d record(s) has been found!\n\n", countRecord);
				countRecord = 0;
				printf("Do you want to search with salary again? ( y = yes ) > ");
				scanf("%c", &tryAgain);

			} while (toupper(tryAgain) == 'Y');
		}
		else if (searchOption == 7)
		{
			do
			{
				rewind(stdin);
				printf("Please Enter Staff Join Date (DD/MM/YYYY) > ");
				scanf("%d/%d/%d", &searchDate.day, &searchDate.month, &searchDate.year);
				printf("\n");
				printf("SEARCHED > %s\n\n", searchDate);
				for (s = 0; s < i; s++)
				{
					if (search[s].joinDate.day == searchDate.day && search[s].joinDate.month == searchDate.month && search[s].joinDate.year == searchDate.year)
					{
						printf("Staff ID        : %s\n", search[s].staffId);
						printf("Staff Name      : %s\n", search[s].name);
						printf("Staff Position  : %s\n", search[s].position);
						printf("Staff Faculty   : %s\n", search[s].faculty);
						printf("Staff Contact   : %s\n", search[s].contactNumber);
						printf("Staff Salary    : RM %.2f\n", search[s].salary);
						printf("Staff Join Date : %d/%d/%d\n\n", search[s].joinDate.day, search[s].joinDate.month, search[s].joinDate.year);
						countRecord++;
					}
				}

				rewind(stdin);


				printf("  %d record(s) has been found!\n\n", countRecord);
				countRecord = 0;
				printf("Do you want to search with Join Date again? ( y = yes ) > ");
				scanf("%c", &tryAgain);

			} while (toupper(tryAgain) == 'Y');
		}
		else
			printf("\n\nInvalid Data field. \n\n");

		rewind(stdin);
		printf("Would you like to try another data field? ( y = yes ) > ");
		scanf("%c", &another);
	} while (toupper(another) == 'Y');

	printf("\n");
}

void modifyStaff()
{
	//declare an array to store data 
	Admin modify[MAX];

	char searchId[6];
	int i = 0;
	int m = 0;
	int p;
	char answer;
	char confirmUpdate;
	char modifyStaffId[5]; // store New Staff Id
	char modifyName[30];
	char modifyPosition[20];
	char modifyFaculty[7];
	char modifyContactNumber[14];
	double modifySalary;
	Date modifyJoinDate;
	char another;
	char otherData;
	int option;

	//import all data from txt file
	FILE* fp;
	fp = fopen("staff.txt", "r");
	if (fp == NULL)
	{
		printf("Error of file.\n");
		exit(-1);
	}

	//scan txt file and store into array
	while (fscanf(fp, "%[^\,],", modify[i].staffId) != EOF)
	{
		fscanf(fp, "%[^\,],", modify[i].name);
		fscanf(fp, "%[^\,],", modify[i].position);
		fscanf(fp, "%[^\,],", modify[i].faculty);
		fscanf(fp, "%[^\,],", modify[i].password);
		fscanf(fp, "%[^\,],", modify[i].contactNumber);
		fscanf(fp, "%lf,", &modify[i].salary);
		fscanf(fp, "%d/%d/%d\n", &modify[i].joinDate.day, &modify[i].joinDate.month, &modify[i].joinDate.year);
		i++;
	}
	fclose(fp);


	//open file to write
	FILE* modPtr;
	modPtr = fopen("staff.txt", "w");
	//check text file 
	if (modPtr == NULL)
	{
		printf("Error of file.\n");
		exit(-1);
	}

	system("cls");
	printf("\tAdministration Staff\n");
	printf("\t        Modify\n");
	printf("=========================================\n");
	do {
		do
		{
			rewind(stdin);
			printf("Please Enter the staff ID that you would like to modify > ");
			scanf("%[^\n]", searchId);
			printf("\n");

			rewind(stdin);
			// search the entire result in array
			for (m = 0; m < i; m++)
			{
				if (strcmp(modify[m].staffId, searchId) == 0)
				{
					//print record that fulfilled user needs
					printf("Staff ID        : %s\n", modify[m].staffId);
					printf("Staff Name      : %s\n", modify[m].name);
					printf("Staff Position  : %s\n", modify[m].position);
					printf("Staff Faculty   : %s\n", modify[m].faculty);
					printf("Staff Password  : ******\n");
					printf("Staff Contact   : %s\n", modify[m].contactNumber);
					printf("Staff Salary    : RM %.2f\n", modify[m].salary);
					printf("Staff Join Date : %d/%d/%d\n\n", modify[m].joinDate.day, modify[m].joinDate.month, modify[m].joinDate.year);
					p = m;
				}
			}

			printf("Do you want to modify this staff record? ( y = yes ) > ");
			scanf("%c", &answer);
			printf("\n");
		} while (toupper(answer) != 'Y');

		do {

			rewind(stdin);
			printf("\n\nSelect the data you would like to modify >\n");
			printf("1. Staff Name\n");
			printf("2. Staff Position\n");
			printf("3. Staff Faculty\n");
			printf("4. Staff Contact\n");
			printf("5. Staff Salary\n");
			printf("6. Staff Joined Date\n\n");
			printf(" NOTICE\n*You are not allowed to modify \n1. Staff ID\n2. Staff Password\n");
			printf("\n*Please contact your faculty if you wished to modify them.\n\n\n");
			printf("Your Option > ");
			scanf("%d", &option);
			rewind(stdin);
			if (option == 1)
			{
				printf(" Enter Updated Staff Name     > ");
				scanf("%[^\n]", modifyName);
				rewind(stdin);
				//confirm modifying
				printf("Are you sure you want to update this record? ( y = yes ) > ");
				scanf("%c", &confirmUpdate);

				if (toupper(confirmUpdate) == 'Y')
				{
					//copy new record to array
					strcpy(modify[p].name, modifyName);
				}
			}
			else if (option == 2)
			{
				printf(" Enter Updated Staff Position > ");
				scanf("%[^\n]", modifyPosition);
				rewind(stdin);
				//confirm modifying
				printf("Are you sure you want to update this record? ( y = yes ) > ");
				scanf("%c", &confirmUpdate);

				if (toupper(confirmUpdate) == 'Y')
				{
					//copy new record to array
					strcpy(modify[p].position, modifyPosition);
				}
			}
			else if (option == 3)
			{
				printf(" Enter Updated Staff Faculty  > ");
				scanf("%[^\n]", modifyFaculty);
				rewind(stdin);

				//confirm modifying
				printf("Are you sure you want to update this record? ( y = yes ) > ");
				scanf("%c", &confirmUpdate);

				if (toupper(confirmUpdate) == 'Y')
				{
					//copy new record to array
					strcpy(modify[p].faculty, modifyFaculty);
				}
			}
			else if (option == 4)
			{
				printf(" Enter Updated Staff Contact  > ");
				scanf("%[^\n]", modifyContactNumber);
				rewind(stdin);
				//confirm modifying
				printf("Are you sure you want to update this record? ( y = yes ) > ");
				scanf("%c", &confirmUpdate);

				if (toupper(confirmUpdate) == 'Y')
				{
					//copy new record to array
					strcpy(modify[p].contactNumber, modifyContactNumber);
				}
			}
			else if (option == 5)
			{
				printf(" Enter Updated Salary         > ");
				scanf("%lf", &modifySalary);
				rewind(stdin);
				//confirm modifying
				printf("Are you sure you want to update this record? ( y = yes ) > ");
				scanf("%c", &confirmUpdate);

				if (toupper(confirmUpdate) == 'Y')
				{
					//copy new record to array
					modify[p].salary = modifySalary;
				}
			}

			else if (option == 6)
			{

				printf(" Enter Updated Join Date   > ");
				scanf("%d/%d/%d", &modifyJoinDate.day, &modifyJoinDate.month, &modifyJoinDate.year);
				rewind(stdin);
				//confirm modifying
				printf("Are you sure you want to update this record? ( y = yes ) > ");
				scanf("%c", &confirmUpdate);

				if (toupper(confirmUpdate) == 'Y')
				{
					//copy new record to array
					modify[p].joinDate.day = modifyJoinDate.day;
					modify[p].joinDate.month = modifyJoinDate.month;
					modify[p].joinDate.year = modifyJoinDate.year;
				}
			}
			else
				printf("\n\nInvalid Option. \n\n");

			rewind(stdin);
			printf("\nDo you want to modify other data of Staff ID ---- %s ? ( y = yes ) > ", searchId);
			scanf("%c", &otherData);
		} while (toupper(otherData) == 'Y');

		rewind(stdin);
		printf("\n\n");
		printf("Do you want to modify another staff record? ( y = yes ) > ");
		scanf("%c", &another);
	} while (toupper(another) == 'Y');

	system("cls");

	//print everything in array to text file 
	for (int j = 0; j < i; j++)
	{

		fprintf(fp, "%s,", modify[j].staffId);
		fprintf(fp, "%s,", modify[j].name);
		fprintf(fp, "%s,", modify[j].position);
		fprintf(fp, "%s,", modify[j].faculty);
		fprintf(fp, "%s,", modify[j].password);
		fprintf(fp, "%s,", modify[j].contactNumber);
		fprintf(fp, "%0.2f,", modify[j].salary);
		fprintf(fp, "%d/%d/%d\n", modify[j].joinDate.day, modify[j].joinDate.month, modify[j].joinDate.year);

	}
	printf("Record has been updated!\n\n");

	system("pause");
	fclose(modPtr);
}

void displayStaff()
{
	int d;
	int countRecord = 0;
	int option;
	char again;
	//declare an array to store data 
	Admin display[MAX];

	int i = 0;

	//open pointer
	FILE* fp;
	fp = fopen("staff.txt", "r");

	//check text file 
	if (fp == NULL)
	{
		printf("Error of file.\n");
		exit(-1);
	}

	// scan text file and store to array
	while (fscanf(fp, "%[^\,],", display[i].staffId) != EOF)
	{
		fscanf(fp, "%[^\,],", display[i].name);
		fscanf(fp, "%[^\,],", display[i].position);
		fscanf(fp, "%[^\,],", display[i].faculty);
		fscanf(fp, "%[^\,],", display[i].password);
		fscanf(fp, "%[^\,],", display[i].contactNumber);
		fscanf(fp, "%lf,", &display[i].salary);
		fscanf(fp, "%d/%d/%d\n", &display[i].joinDate.day, &display[i].joinDate.month, &display[i].joinDate.year);
		i++;
	}
	fclose(fp);
	system("cls");
	printf("\tAdministration Staff\n");
	printf("\t        Display\n");
	printf("=========================================\n");

	do {
		printf("Which type of output would you like to choose ?\n");
		printf("1. With Subject.\n");
		printf("2. Without Subject. ( As in text file )\n");
		printf("3. Table Form.\n");
		printf("Your Option > ");
		scanf("%d", &option);
		printf("\n");
		if (option == 1)
		{
			for (d = 0; d < i; d++)
			{
				printf("Staff ID        : %s\n", display[d].staffId);
				printf("Staff Name      : %s\n", display[d].name);
				printf("Staff Position  : %s\n", display[d].position);
				printf("Staff Faculty   : %s\n", display[d].faculty);
				printf("Staff Contact   : %s\n", display[d].contactNumber);
				printf("Staff Salary    : RM %.2f\n", display[d].salary);
				printf("Staff Join Date : %d/%d/%d\n\n", display[d].joinDate.day, display[d].joinDate.month, display[d].joinDate.year);
				countRecord++;
			}
		}
		else if (option == 2)
		{
			for (d = 0; d < i; d++)
				printf("%s, %s, %s, %s, %s, %.2f, %d/%d/%d \n", display[d].staffId, display[d].name, display[d].position,
					display[d].faculty, display[d].contactNumber, display[d].salary, display[d].joinDate.day, display[d].joinDate.month, display[d].joinDate.year);
		}
		else if (option == 3)
		{
			printf("==========+================================+==================+=========+================+=========+=============+\n");
			printf(" Staff ID | Staff Name\t\t\t   | Position         | Faculty | Contact Number | Salary  | Joined Date | \n");
			printf("==========+================================+==================+=========+================+=========+=============+\n");
			for (d = 0; d < i; d++)
			{
				printf(" %-7s  | %-30s | %-16s | %-7s | %-14s | %-5.2f | %02d/%02d/%04d  |\n", display[d].staffId, display[d].name, display[d].position, display[d].faculty, display[d].contactNumber, display[d].salary, display[d].joinDate.day, display[d].joinDate.month, display[d].joinDate.year);
				printf("----------+--------------------------------+------------------+---------+----------------+---------+-------------+\n");
			}
			printf("\n");
		}
		else
		{
			printf("Invalid Option\n");
		}

		rewind(stdin);
		printf("\nWould you like to display again? ( y = yes ) > ");
		scanf("%c", &again);
		system("cls");
	} while (toupper(again) == 'Y');

}

void staffReport()
{

	Admin report[MAX];
	char try;
	int i = 0;
	int r;
	int reportOption;
	double totalSalary = 0;

	//open pointer
	FILE* fp;
	fp = fopen("staff.txt", "r");

	//check text file 
	if (fp == NULL)
	{
		printf("Error of file.\n");
		exit(-1);
	}

	// scan text file and store to array
	while (fscanf(fp, "%[^\,],", report[i].staffId) != EOF)
	{
		fscanf(fp, "%[^\,],", report[i].name);
		fscanf(fp, "%[^\,],", report[i].position);
		fscanf(fp, "%[^\,],", report[i].faculty);
		fscanf(fp, "%[^\,],", report[i].password);
		fscanf(fp, "%[^\,],", report[i].contactNumber);
		fscanf(fp, "%lf,", &report[i].salary);
		fscanf(fp, "%d/%d/%d\n", &report[i].joinDate.day, &report[i].joinDate.month, &report[i].joinDate.year);
		i++;
	}
	fclose(fp);

	system("cls");
	printf("\tAdministration Staff\n");
	printf("\t        Report\n");
	printf("=========================================\n");

	do
	{

		printf("Select The Report Category > \n");
		printf("1. All Staff Name. \n");
		printf("2. All Staff Name and Contact Number. \n");
		printf("3. All Staff Name and Salary. \n");
		printf("4. All Staff Name and JoinDate. \n");
		printf("\nYour Option > ");
		scanf("%d", &reportOption);

		if (reportOption == 1)
		{
			printf("\tAdministrative Staff Report\n");
			printf("===+===============================+\n");
			printf(" No| Staff Name\t\t\t   |\n");
			printf("===+===============================+\n");
			for (r = 0; r < i; r++)
			{
				printf("%2d | %-30s|\n", r + 1, report[r].name);
				printf("---+-------------------------------+\n");
			}
			printf("\n");
		}
		else if (reportOption == 2)
		{
			printf("\tAdministrative Staff Report\n");
			printf("===+===============================+================+\n");
			printf(" No| Staff Name\t\t\t   | Contact Number |\n");
			printf("===+===============================+================+\n");
			for (r = 0; r < i; r++)
			{
				printf("%2d | %-30s| %-14s |\n", r + 1, report[r].name, report[r].contactNumber);
				printf("---+-------------------------------+----------------+\n");
			}
			printf("\n");
		}
		else if (reportOption == 3)
		{
			printf("\tAdministrative Staff Report\n");
			printf("===+===============================+=========+\n");
			printf(" No| Staff Name\t\t\t   | Salary  |\n");
			printf("===+===============================+=========+\n");
			for (r = 0; r < i; r++)
			{
				printf("%2d | %-30s| %6.2f |\n", r + 1, report[r].name, report[r].salary);
				printf("---+-------------------------------+---------+\n");
				totalSalary = totalSalary + report[r].salary;
			}
			printf("\nTotal Salary > RM %.2f", totalSalary);
			printf("\n");
		}
		else if (reportOption == 4)
		{
			printf("\tAdministrative Staff Report\n");
			printf("===+===============================+=============+\n");
			printf(" No| Staff Name\t\t\t   | Joined Date |\n");
			printf("===+===============================+=============+\n");
			for (r = 0; r < i; r++)
			{
				printf("%2d | %-30s| %02d/%02d/%04d  |\n", r + 1, report[r].name, report[r].joinDate.day, report[r].joinDate.month, report[r].joinDate.year);
				printf("---+-------------------------------+-------------+\n");
			}
			printf("\n");
		}
		else
			printf("Invalid Option.");

		rewind(stdin);
		printf(" Do you want to read another report ? ( y = yes ) ");
		scanf("%c", &try);
		system("cls");
	} while (toupper(try) == 'Y');

	printf("\n");
	system("pause");
	return;

}


