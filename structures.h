#ifndef STRUCTURES_HEADER
#define STRUCTURES_HEADER
// This header file contains all the structures to be used by each module

// Date
typedef struct
{
	int day;
	int month;
	int year;
}Date;

// Time
typedef struct
{
	int hour;
	int min;
}Time;

// Visitor structure
typedef struct
{
	char ID[7];
	char icNum[15];
	char name[21];
	char category[15];
	char area[16];
	char gender;
	char latestVenueVisited[7];
	Time registerTime;
	Date registerDate;
	double temperature;
}visitor;

// Visits and Exits structure
typedef struct {
	char visitorID[7];
	char visitorICNo[14];
	Date dateEntered;
	Time timeEntered;
	char venueID[10];
	char visitPurpose[30];
	double visitorTemperature;
	Time timeExited;
	Time durationOfStay;
}VisitsAndExits;

// Venue sub-structure
typedef struct
{
	int maxVisitors;
	int currentVisitors;
}visit;

// Venue structure
typedef struct
{
	char ID[7];
	char name[10];
	char category[20];
	char description[30];
	Date lastSanitization;
	visit number;
	double areaSize;
	char dangerLevel[10];
} venue;

// Administrative staff structure
typedef struct
{
	char staffId[5];
	char name[30];
	char position[20];
	char faculty[7];
	char password[20];
	char contactNumber[14];
	double salary;
	Date joinDate;
} Admin;

// Sanitization sub-structure
typedef struct {
	char name[31], contactPerson[21], contactNo[12];
} SanitizationCompanyDetails;

// Sanitization structure
typedef struct {
	int id;
	Date date;
	Time startTime, endTime;
	char venueID[7];
	SanitizationCompanyDetails scd;
	char sanitizationType[21];
	double price;
} Sanitization;

#endif