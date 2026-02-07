#ifndef grade_h
#define grade_h

#define gradePath "/home/huseyn/Desktop/project/data/grades.csv"

typedef struct {
    int grade_id;
    int enrollment_id;
    int student_id;
    int course_id ;
    char letter_grade[10];
    double numeric_grade;
    char semester[20];

}grades ;

typedef enum{
    GRADE_NULL = 0 ,
    GRADE_OK ,
    GRADE_ID,
    GRADE_ENROLLMENT,
    GRADE_STUDENT,
    GRADE_COURSE,
    GRADE_LETTER,
    GRADE_NUMERIC,
    GRADE_SEMESTER,
}Grade_Error ;
extern grades* arrGrades ;
extern int grade_count ;
extern int maxGrade ;
static int ensureGradeCapacity();
void init_Grades();

int loadGrades(const char * path);

void freeGrade();

grades createGrade();

int saveGrade(const char * path);

grades* seachGrade_byEnrollmentid(const int  enrollmentid );

grades* search_grade_id(const int  gradeID );

float letterTo_gpa(const char *letter);


int addGrade(const grades *g);

int updateGRade(int grade_id, const char *letter, double new_numeric);
int updateGradeByEnrollment(int enrollment_id,const char *letter,double new_numeric);

int  checkletter_numeric(const char *letter, double num);

int course_statistics(int course_id, const char *semester,double *avg, double *min, double *max);

void grade_distribution(int course_id, const char *semester);

Grade_Error validateGrade(const grades *g );

int validateSemester(const char *semester);

 int term_rank(char *term);

 int cmp_enrollments_by_semester_then_id(const void *pa, const void *pb);




int deleteGradeBy_EnrollmentId(int enrollment_id);

int deleteGradeBy_StudentId(int student_id);

const char * validationGrade_msg(Grade_Error err);

int countGradeCourse(int courseID);

int deleteGRades_Course(int course_id);
#endif