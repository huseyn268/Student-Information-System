#ifndef student_h 
#define student_h
#define studentPath "/home/huseyn/Desktop/project/data/students.csv"
typedef struct {
    int student_id;
    char first_name[30];
    char last_name[30];
    char s_mail[30];
    char s_phone[30];
    int enrollment_year;
    char major[50];
    double gpa;

}Students;

typedef enum {
    ERROR_NULL = -1 ,
    ERROR_OK = 0,
    ERROR_STUDENT_ID = 1,
    ERROR_STUDENT_NAME = 2,
    ERROR_STUDENT_MAIL = 3,
    ERROR_STUDENT_PHONE = 4,
    ERROR_STUDENT_YEAR = 5,
    ERROR_STUDENT_GPA = 6,
    ERROR_STUDENT_MAJOR = 7
} ErrorCode;
extern int maxStudent;
extern int student_count; 
static int ensureStudentCapacity();
extern Students* arrStudents;

void init_Students();

int loadStudent(const char* path );

Students createStudent();

void showStudent(Students *s);

void freeStudent();

Students* search_student_id(const int studentid );

Students* search_student_name(char* first_name , char* last_name);

Students* search_student_email(const char *mail);

int addStudent(const Students* s);

int save_student(const char* path);

int delete_Student(int student_id);


int updateStudent(int id);
int validateStudent(const Students *s);
const char* validation_error_msg(ErrorCode e) ;

double calculate_GPA(int student_id);
int validateid(int student_id);
void displayStudentTranskript(int student_id);


#endif 