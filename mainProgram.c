#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#pragma warning(disable:4996)

void visitorInformationModule();
void administrativeStaffModule();
void visitsAndExitsModule();
void sanitizationRecordsModule();
//void venueInformationModule();

void test1() {
	printf("\n\n"
		"  Start Time (HH:MM): 15:30\n\n"
		"  Start Time (HH:MM): 15.30\n\n"
		"  Start Time (HH:MM): 15 30\n\n"
	);
}

void main() {
	int menuOpt;
	do {
		// Print logo
		printf("\n"
			"  ______________________________________________________________________________________________________________________________________________________________\n"
			"  |[~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~]|\n"
			"  |[  _____                 ___       _      _   _                   ___         _           _      _____            _             ___         _              ]|\n"
			"  |[ |_   _|_ _ _ _ _  _ __/ __| ___ (_)__ _| |_| |_ ___ _ _ __ _   / __|___ _ _| |_ __ _ __| |_ __|_   _| _ __ _ __(_)_ _  __ _  / __|_  _ __| |_ ___ _ __   ]|\n"
			"  |[   | |/ _` | '_| || / _\\__ \\/ -_)| / _` | ' \\  _/ -_) '_/ _` | | (__/ _ \\ ' \\  _/ _` / _|  _|___|| || '_/ _` / _| | ' \\/ _` | \\__ \\ || (_-<  _/ -_) '  \\  ]|\n"
			"  |[   |_|\\__,_|_|  \\_,_\\__|___/\\___|/ \\__,_|_||_\\__\\___|_| \\__,_|  \\___\\___/_||_\\__\\__,_\\__|\\__|    |_||_| \\__,_\\__|_|_||_\\__, | |___/\\_, /__/\\__\\___|_|_|_| ]|\n"
			"  |[                               |__/                                                                                    |___/       |__/                   ]|\n"
			"  |[__________________________________________________________________________________________________________________________________________________________]|\n"
			"   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
			"  =====================================\n"
			"  | 1 - Visitor Information Module    |\n"
			"  | 2 - Venue Information Module      |\n"
			"  | 3 - Administration Staff Module   |\n"
			"  | 4 - Visits and Exits Module       |\n"
			"  | 5 - Sanitization Records Module   |\n"
			"  | 6 - Exit Program                  |\n"
			"  =====================================\n\n"
		);

		// Accept and validate menu option
		do {
			printf("  Choose a module > ");
			rewind(stdin);
			scanf("%d", &menuOpt);
			if (menuOpt < 1 || menuOpt > 6) {
				printf("  [ Invalid input, Please re-enter. ]\n\n");
			}

			if (menuOpt == 7) {
				test1();
			}
		} while (menuOpt < 1 || menuOpt > 6);

		if (menuOpt != 6) {
			system("cls");
		}

		// Run according modules
		switch (menuOpt) {
		case 1: visitorInformationModule(); break;
		//case 2: venueInformationModule(); break;
		case 3: administrativeStaffModule(); break;
		case 4: visitsAndExitsModule();  break;
		case 5: sanitizationRecordsModule(); break;
		case 6: 
			printf("\n  [ Program Ended. ]\n");
			exit(0); 
			break;
		default: break;
		}

		// Pause and clear screen after every module
		system("cls");
	
	} while (menuOpt != 6);
}
