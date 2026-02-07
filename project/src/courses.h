#ifndef courses_h
#define courses_h

#define coursePath "/home/huseyn/Desktop/project/data/courses.csv" 

typedef struct {
    int course_id;
    char course_code[10];
    char course_name[30];
    int credit;
    int capasity;
    char department[30];
    char prerquistis[30] ;

}courses;

typedef enum  {
    COURSE_OK = 0 ,
    ERR_COURSE_NULL =1 ,
    ERR_COURSE_ID =2 ,
    ERR_COURSE_CODE_EMPTY =3,  
    ERR_COURSE_CODE_LEVEL =4,  
    ERR_COURSE_NAME_EMPTY=5,
    ERR_COURSE_CREDIT =6,
    ERR_COURSE_CAPACITY =7,
    ERR_COURSE_DEPARTMENT =8,
    ERR_COURSE_PREQUISSITE  = 9,
}Error_Code;
extern int maxCourse ;
static int ensureCourseCapacity();
void init_Courses();

extern courses* arrCourses;
extern int course_count ;

int loadCourse(const char *path);

courses createCourse(int ask_id);

void showCourse(const courses *c);

void freeCourses();

courses *search_course_id(int course_id);

courses * search_course_name(const char * name);

courses *search_course_code(const char * code);

int addCourse(const courses*c);

int saveCourses(const char* path);

int delete_Course(int course_id);

int updateCourse(int course_id);
void displayProfesorCourseload(int prof_id , const char *semester_orNull);


Error_Code validate_Course(const courses *c);

const char* validation_errorMSG(Error_Code error);

int setCourse(int course_id ,int new_capasity);
int validatePrequis(const courses *c) ;
int addPrerquistis(int course_id ,const char* preq);
int checkCourse_capacity(int course_id , const char *semester);
void displayCourseRoster(int course_id, const char *semester);
int remaingCourseCapasty(int course_id , const char  *semester);
#endif