#ifndef professor_h
#define professor_h


#define profPath "/home/huseyn/Desktop/project/data/professors.csv"
typedef struct {
    int prof_id ;
    char first_name[50];
    char last_name[50];
    char p_mail[50];
    char p_phone[50];
    char department[50];
    char title[50];
    char office[50];

}professor ;

typedef enum {

    PROF_OK = 0 ,
    ERR_PROF_NULL ,
    ERR_PROF_ID,
    ERR_PROF_NAME ,
    ERR_PROF_MAIL,
    ERR_PROF_PHONE,
    ERR_PROF_DEPARTMENT, 
    ERR_PROF_TITLE,
    ERR_PROF_OFFICE,
}Prof_Error ;


extern professor* arrProf ;

extern int prof_count ;
extern int maxProf ;
static int ensureProfCapacity();
void init_Prof();

int loadPorf(const char* path ) ;

professor createProf(int askt_id ,int update_id) ;

void showProf(professor *p) ;

void freeProf();

professor* search_prof_id(const int prof_id );

professor* search_prof_name(const char* first_name ,const  char* last_name) ;

professor* search_prof_email(const char *mail) ;

int addProf(const professor* s) ;

int save_Prof(const char *path) ;

int delete_prof(int prof_id);

int updateProf(int id);

Prof_Error validate_prof(const professor *prof);

const char* prof_Error_MSG(Prof_Error a);
int assignCourse_Professor(int course_id ,const char *semester,int professor_id);
#endif 