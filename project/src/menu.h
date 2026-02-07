#ifndef menu_h
#define menu_h

int inputMenuChoice(void);

int intputValidcourseID();
int inputValidStudentID(void);
const char *inputValidateSemester(void);

int inputProf();

void printStudentManagment();


void searchStudentMENU();

void printCourseManagment();

void searchCourseMenu();
void printProfessorManagment();
void searchProfMENU();
void printEnrollmentManagment();

void printGradetManagment();
void printReportsManagment();
void printSystemManagment();

int loadAlldata();


int saveAlldata();

int backupAllData();
void freeAll();

void mainMenu();

#endif