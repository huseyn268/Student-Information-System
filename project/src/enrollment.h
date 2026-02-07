#ifndef enrollment_h 
#define enrollment_h
#define enrollmentPath "/home/huseyn/Desktop/project/data/enrollments.csv"
typedef struct {
    int enrollment_id;
    int student_id ;
    int course_id;
    int professor_id;
    char semester[30];
    char enrollment_date[30];
    char status[30];

}enrollment ;


typedef enum{
    ENROLLMENT_OK = 0 ,
    ENROLLMENT_NULL ,
    ENROLLMENT_ID,
    ENROLLMENT_STUDENT_ID,
    ENROLLMENT_COURSE_ID,
    ENROLLMENT_PROFESSOR_ID,
    ENROLLMENT_SEMESTER,
    ENROLLMENT_DATE,
    ENROLLMENT_STATUS,
}Enrollment_error ;

extern int  enrollment_count;

extern enrollment* arrEnrollments ;
extern int maxEnrollment ;
static int ensureEnrollmentCapacity();
void init_Enrollments();

int loadEnrollment(const char *path);

void freeEnrollment();


enrollment createEnrollment();

void showEnrollment(enrollment *e);

enrollment* search_enrollment_id(const int  enrollmentid );

int checkStudent_Enrollment(int  studentID, int courseID ,const char *semester);

int addEnrollment(const enrollment *e);

int saveEnrollment(const char * path);

int deleteEnrollment(int enrollment_id);

int count_enrolledCourse(int course_id ,const char *semester);

int check_prerquisties(int student_id ,int couurse_id);

int updateEnrollmentStatus(int enrollment_id,const char*newStatus);

void listStudentEnrollment(int student_id ,const char *semesterOptinal);
int validate_date(const char *s);

int validateEnrollment(const enrollment *e);

const char* validation_enrollment_msg(Enrollment_error err);



int dropStudentFromCOurse(int student_id ,int course_id ,const char *semster);

int countEnrollmentsBycourse(int courseID);

int deleteEnrollment_Course(int course_id);

int deleteEnrollment_Student(int student_id);
int deleteEnolmment_Prof(int professor_id);

int countEnrollment_Prof(int prof_id);

void sortEnrollment(enrollment *list , int n);

#endif