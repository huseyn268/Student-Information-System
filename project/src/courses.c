#include"courses.h"
#include"student.h"
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include"utils.h"
#include"enrollment.h"
#include"grade.h" 
#include"professor.h"



//Maximum allocated capacity for courses array.
int maxCourse  = 20; 

// Dynamic array that stores all course records.
courses* arrCourses = NULL ;

//Number of courses currently stored in the system.
int course_count = 0 ;

//Forward declaration for prerequisite validation function.
int validatePrequis(const courses *c);

/**
  Ensures enough capacity in course array.
  If full, reallocates memory.
 */
static int ensureCourseCapacity(void)
{
    if (course_count < maxCourse) return 1;

    int newCap = maxCourse * 2;

    courses *tmp = realloc(arrCourses, newCap * sizeof(courses));
    if (!tmp) {
        printf("Error: realloc failed for Courses\n");
        return 0;
    }
    arrCourses = tmp;
    maxCourse = newCap;
    return 1;
}


/*
  Initializes the course module.
  Allocates initial memory for course array.
 */
void init_Courses(){
    maxCourse = 20 ;
     arrCourses = (courses*)malloc(maxCourse* sizeof(courses));
    if(!arrCourses){
        printf("Memory allecation is failed");
        exit(1);
    }
    course_count = 0;
}



/*
  Loads course data from a CSV file.
  Each line is parsed and stored as a course record.
 */
int loadCourse(const char *path){


      if (!arrCourses) {
    printf("arrCourses is NULL.  Call init_course() first.\n");
    return -1;
}
    FILE* fp = fopen(path,"r");
    char line[260];
    int i = 0 ;

      if(!fp){
         printf("File could not be opened \n");
         return -1 ;

      }
   
fgets(line, sizeof(line), fp); // For header

while(fgets(line, sizeof(line), fp )){

         /*
         * Update course_count during loading so ensureCourseCapacity works correctly.
         */

        course_count = i ;
          if (!ensureCourseCapacity()) {
            fclose(fp);
            return -1;
        }

int a = sscanf(line,"%d,%9[^,],%29[^,],%d,%d,%29[^,],%29[^\n]",
    &arrCourses[i].course_id,
    arrCourses[i].course_code,
    arrCourses[i].course_name,
    &arrCourses[i].credit,
    &arrCourses[i].capasity,
    arrCourses[i].department,
  arrCourses[i].prerquistis
);

if (a == 7 || a == 6) {
if (a == 6) arrCourses[i].prerquistis[0] = '\0';
clean_newLine(arrCourses[i].course_name);
clean_newLine(arrCourses[i].department);
clean_newLine(arrCourses[i].prerquistis);
    i++;
}



}
      course_count = i ;

fclose(fp);
 return i ;

}


/*
  Creates a courses struct by reading user input.
  If ask=1, course_id and course_code are also requested.
 If ask=0, those fields are not asked (used in updateCourse).
 */
courses createCourse(int ask){

        courses c = {0};

        if(ask){
    printf("Enter course id : ");
    scanf("%d",&c.course_id);
    clear_input_buffer();
    printf("Enter course code : ");
    scanf("%9s",c.course_code);
    clear_input_buffer();
        
}
    printf("Enter course name : ");
    fgets(c.course_name ,sizeof(c.course_name),stdin);
    clean_newLine(c.course_name);

    printf("Enter credit : ");
    scanf("%d",&c.credit);
    clear_input_buffer();

    printf("Enter capasity : " ) ;
    scanf("%d",&c.capasity);
    clear_input_buffer();
    printf("Enter course department : ");
    fgets(c.department ,sizeof(c.department),stdin);
    clean_newLine(c.department);

    /*
     * If user enters "0", prerequisites are treated as empty.
     */
    printf("Enter prerquistis :(or 0 for none)");
  fgets(c.prerquistis, sizeof(c.prerquistis), stdin);
  clean_newLine(c.prerquistis);
   
            if(strcmp(c.prerquistis ,"0")== 0){
                    c.prerquistis[0] = '\0';
   
             }
return c ;
}




/*
 
  Displays all information of a course.
 */
void showCourse(const courses *c ){
        printf("\nCourse information : ");
        printf("\nId : %d",c->course_id);
        printf("\nCode : %s",c->course_code);
        printf("\nName : %s",c->course_name);
        printf("\nCredit : %d",c->credit);
        printf("\nCapasity : %d",c->capasity);
        printf("\nDepartment : %s",c->department);
        printf("\nPrerquistis : %s",c->prerquistis);

}


/*
  Frees memory allocated for course array.
  Used during shutdown or reload.
 */
void freeCourses(){
    free(arrCourses);
    arrCourses = NULL ;
    course_count = 0 ;
    maxCourse = 0 ;
}



/*
  Searches for a course by ID.
  Returns pointer to course if found.
 */
courses * search_course_id(int course_id){
    int i = 0 ;
      for( i = 0 ; i<course_count ; i++){
               if(arrCourses[i].course_id == course_id){
                  return  &arrCourses[i];
               }
            }
          
               return NULL ;
}


/*
  Performs a linear search by course_name.
  Uses strcmp for exact match (case-sensitive).
 */

courses * search_course_name(const char * name){
    int i = 0 ;
      for( i = 0 ; i<course_count ; i++ ){
         if(strcmp(arrCourses[i].course_name, name) == 0  ){
            return &arrCourses[i];
         }
      }
         
return NULL ;

}



/*
  Performs a linear search by course_code.
  Course code is assumed to be unique (e.g., CS101).
 */
courses * search_course_code(const char *code ){
        int i = 0 ;
      for( i = 0 ; i<course_count ; i++ ){
         if(strcmp(arrCourses[i].course_code,code ) == 0  ){
            return &arrCourses[i];
         }
      }
         
return NULL ;

}




/*
  Adds a new course.
  Before adding: capacity check, unique ID/code, and validate_Course are applied.
  On success, the course is appended and course_count is incremented.
 */
int addCourse(const  courses* c ){

    if(!arrCourses || !c ){printf("arrCourses or c is null "); return 0 ;}
      if (!ensureCourseCapacity()) return 0;

    if(search_course_id(c->course_id )!= NULL){ printf("Please enter diffirent course id :"); return 0 ;}
    if(search_course_code(c->course_code)!= NULL ){printf("Please enter diffirent course code : ");return 0 ;}
        
    Error_Code  Error = validate_Course(c);
                if(Error != COURSE_OK){
                    printf("Invalid input : %s",validation_errorMSG(Error));
                    return 0 ; 
                }

   
    arrCourses[course_count] = *c ;
    course_count ++;
    return 1 ;

}

/*
  Saves all courses to a CSV file.
  Writes a header line first, then dumps all course records.
 */
int saveCourses(const char* path){
    int i = 0 ;
          FILE* fp = fopen(path,"w");

   if(!fp){
      printf("File could not open\n");
      return 0 ;
   }
   fprintf(fp,"course_id,course_code,course_name,credits,capacity,department,prerequisites\n");
    for( i = 0 ; i<course_count ; i++ ){
        fprintf(fp,"%d,%s,%s,%d,%d,%s,%s\n",
            arrCourses[i].course_id,
            arrCourses[i].course_code,
            arrCourses[i].course_name,
            arrCourses[i].credit,
            arrCourses[i].capasity,
            arrCourses[i].department,
            arrCourses[i].prerquistis        
        ); 
        //course_id,course_code,course_name,credits,capacity,department,prerequisites
    }
    fclose(fp);
    return 1 ;

}






/*
  Deletes a course.
  If there are no related enrollments/grades, deletes directly.
  If related records exist, asks confirmation and deletes them too.
 */
int delete_Course(int course_id){
    courses* course  = search_course_id(course_id);
    if(!course){
        printf("No such a course .%d \n",course_id);
        return 0 ;
    }
 
     
    int i = 0 ;
    int j = 0 ; 

int enrollmentCourse_counter = countEnrollmentsBycourse(course_id);
int gradeCourse_counter = countGradeCourse(course_id);


    // If no related records: only remove from courses array.
            if(enrollmentCourse_counter == 0 && gradeCourse_counter == 0 ){
                
    for(i = 0; i < course_count; i++){
        if(arrCourses[i].course_id == course_id){
            for( j = i; j < course_count - 1; j++){
                arrCourses[j] = arrCourses[j+1];
            }
            course_count--;
            return 1;
        }
    }
    return 0 ;
            }
        //If related records exist, warn user.
        //Ask confirmation because transcript/GPA can be affected.
        printf("Warning !!\n");
      printf("If you delete course %d, %d enrollment record and %d grade record will be deleted.\n",
       course_id, enrollmentCourse_counter, gradeCourse_counter);

            printf("This situation may affect transcript/GPA calculations.\n");
            printf("Press 1 if you want this, or 0 if you don't.\n");

            int choice ;
        
            if(scanf("%d", &choice) != 1){
                clear_input_buffer();
                printf("Invalid input .\n");
                return 0 ;
            }
        clear_input_buffer();
        if(choice != 0 && choice != 1){
    printf("Invalid choice. Cancelled.\n");
    return 0;
}
if(choice != 1) {
printf("Process canceled.\n");
return 0;
    }
       // If confirmed: delete enrollments/grades first, then delete the course.
    int deltedCourseEnrollment = deleteEnrollment_Course(course_id);
    int deltedCourseGrade = deleteGRades_Course(course_id);
            printf("%d enrollment and  %d  grade deleted.\n",deltedCourseEnrollment , deltedCourseGrade);

    for(i = 0; i < course_count; i++){
        if(arrCourses[i].course_id == course_id){
            for( j = i; j < course_count - 1; j++){
                arrCourses[j] = arrCourses[j+1];
            }
            course_count--;
            return 1;
        }
    }
    
    return 0 ;

}



/*
 Updates a course record.
 ID and course_code remain unchanged, other fields are updated.
 validate_Course is called again to ensure new data is valid.
 */
int updateCourse(int course_id){
    courses *c  = search_course_id(course_id);
        if(c == NULL){
            printf("Course not found \n :");
           return 0 ; 
        }
        printf("\n------Current course details : ------\n");
        showCourse(c);
        printf("-----Enter New Data ( ID and Code will not change )-----\n");
        courses intput = createCourse(0);
        courses  new =  *c ; 
   strncpy(new.course_name , intput.course_name,sizeof(new.course_name) -1 );
   new.course_name[sizeof(new.course_name) -1 ] = '\0';
new.credit = intput.credit;
new.capasity = intput.capasity;

   strncpy(new.department , intput.department,sizeof(new.department) -1 );
   new.department[sizeof(new.department) -1 ] = '\0';

 strncpy(new.prerquistis , intput.prerquistis,sizeof(new.prerquistis) -1 );
   new.prerquistis[sizeof(new.prerquistis) -1 ] = '\0';
         
   
     // Checks if another course already has the same name.
     
   courses *temp = search_course_name(new.course_name);
         if(temp != NULL  && temp !=c){
            printf("Duplicate course name : \n");
            return 0 ;            
         }
         
    Error_Code e = validate_Course(&new);
    if(e  != COURSE_OK)
    {
        printf("%s\n",validation_errorMSG(e));
        return 0 ;
    }   
         *c = new;
         return 1 ;

        }





/*
 Prints a professor's course load to the console.
 Optionally filters by a specific semester if provided.
 */
void displayProfesorCourseload(int prof_id , const char *semester_orNull){
        if(!arrEnrollments){
            printf("Enrollments not initialized \n");
            return  ;

        }       

        if(!arrProf){
            printf("Proffessors not initialized \n");
            return  ;
        }

professor *p = search_prof_id(prof_id);
if(!p){
    printf("Professor nof found %d\n",prof_id);
    return ;

}
 
     // If semester_orNull is not NULL and not empty, semester filtering is enabled.
     
int semester = (semester_orNull && semester_orNull[0] != '\0');

printf("\n========================================\n");
    printf(" PROFESSOR COURSE LOAD: %s %s (ID:%d)\n",
           p->first_name, p->last_name, prof_id);
    if(semester) printf(" Semester: %s\n", semester_orNull);
    else printf(" Semester: ALL\n");
    printf("========================================\n");

    printf("+--------+---------+------------------------------+----------+-----------+\n");
    printf("| CID    | Code    | Course Name                   | Enrolled | Completed |\n");
    printf("+--------+---------+------------------------------+----------+-----------+\n");

int print = 0 ;


int i , k ,j ;

//Iterate over enrollments to find courses taught by the professor.
for(i = 0; i<enrollment_count ; i++){
    if(arrEnrollments[i].professor_id != prof_id)continue;
    if(semester && strcmp(arrEnrollments[i].semester , semester_orNull) != 0 )continue;

    int course_id = arrEnrollments[i].course_id;
     
 
//Same course may appear multiple times due to multiple enrollments.
//         * Prevent printing duplicate course rows in the table.
    int printed_before = 0 ;

    for(k = 0 ; k< i ; k++){
        if(arrEnrollments[k].professor_id != prof_id)continue;
        if(semester && strcmp(arrEnrollments[k].semester,semester_orNull) != 0 )continue;
        if(arrEnrollments[k].course_id == course_id){
            printed_before = 1 ;
            break ;
        }

    }

if(printed_before)continue;


int enrolled_counter = 0 ;
int completed_counter = 0 ;
    //Count how many students are Enrolled vs Completed for this course.
        for(j = 0 ;j<enrollment_count ; j++){
            if(arrEnrollments[j].professor_id !=prof_id)continue;
            if(arrEnrollments[j].course_id != course_id)continue;
            if(semester && strcmp(arrEnrollments[j].semester,semester_orNull) != 0)continue;

            if(strcmp(arrEnrollments[j].status,"Enrolled") == 0) enrolled_counter++;
            else if (strcmp(arrEnrollments[j].status,"Completed") == 0)completed_counter ++;

        }


courses *c = search_course_id(course_id);
const char *code = c ? c->course_code  : "-";
const char *name = c ? c->course_name : "UNKNOWN";



printf("| %-6d | %-7s | %-28s | %-8d | %-9d |\n",
               course_id, code, name,enrolled_counter , completed_counter);



print = 1 ;


}
printf("+--------+---------+------------------------------+----------+-----------+\n");
if(!print){
    if(semester_orNull) printf("No courses found for this professor in %s \n",semester_orNull);
    else printf("No courses found for this professor\n");
}




}


/*
 Validates a course record and returns an error code.
 Called in critical operations like addCourse/updateCourse.
 */
Error_Code validate_Course(const courses *c)
{

    if(c == NULL){  return ERR_COURSE_NULL; }

    if(c->course_id<= 0){
        return  ERR_COURSE_ID;
    }
    if(c->course_code[0] == '\0'){
   return   ERR_COURSE_CODE_EMPTY;
    }
    // Course level check (100 - 400);
   
   
    int level =0 ;  
    int level_faund = 0 ; 
    int i = 0 ;
      /* indicates whether numeric level was found */
    for( i = 0 ; c->course_code[i] != '\0' ;i++){
        if(c->course_code[i]>='0' && c->course_code[i]<='9'){
            level = atoi(&c->course_code[i]);
            level_faund = 1 ;
            break ;
        }
    }

if(!level_faund ||level <100 || level>400 ){return ERR_COURSE_CODE_LEVEL; }  

if(c->course_name[0] == '\0'){
   return  ERR_COURSE_NAME_EMPTY;
}
//Credits are allowed only as 3 or 4.
if(!(c->credit == 3  || c->credit == 4)){
return  ERR_COURSE_CREDIT;
}

    if(c->capasity<= 0 ){
   return ERR_COURSE_CAPACITY;
    }
//Department field cannot be empty.
        if(c->department[0] == '\0'){
        return    ERR_COURSE_DEPARTMENT;
            
        }
    if(c->prerquistis[0] =='\0')
    return COURSE_OK ;
//If prerequisites is not empty, validate its format and logic.
            if(!validatePrequis(c))
            return ERR_COURSE_PREQUISSITE;




return COURSE_OK ;


}






/*
 Converts validation error codes into user-friendly messages.
  Used to explain codes returned by functions like validate_Course.
 */
const char* validation_errorMSG(Error_Code error){
    switch (error)
    {
    case COURSE_OK :
            return "\nCourse data is valid :\n";
    case   ERR_COURSE_NULL: 
            return "\nCourse pointer is NULL\n";
    case ERR_COURSE_ID :
            return  "\nInvalid course id\n " ;

    case ERR_COURSE_CODE_EMPTY:
            return "\nCourse code cannot be empty \n";
        case   ERR_COURSE_CODE_LEVEL :
            return "\nCourse level must be between 100 and 400\n";
        case ERR_COURSE_NAME_EMPTY :
            return "\nCourse name cannot be empty\n" ;
        case ERR_COURSE_CREDIT : 
            return "\nCourse credit must be 3 or 4 \n";
        case ERR_COURSE_CAPACITY :
            return "\nCourse capasity must be greater than zero ";
        case  ERR_COURSE_DEPARTMENT :
            return "\nCourse department can not be empty";
        case ERR_COURSE_PREQUISSITE:
            return "\nInvalid prerquisite format \n";
        default :
            return "\nUnkown validation error\n:";
    }

}


//Updates the capacity of a course.
// Used as a helper function for capacity adjustment in the menu.
int setCourse(int course_id ,int new_capasity){
        if(!arrCourses ){
            printf("Please load courses datas. \n");
            return 0 ;
        }
        courses *c = search_course_id(course_id);

        if(!c){
            printf("Course not found ID : %d\n",course_id);
            return 0;
        }
        if(new_capasity<=0){
            printf("Capacity must be greater than zero \n");
            return 0 ;
        }

    c->capasity = new_capasity ;
    return 1; 
}




/*

  Validates the prerequisites field of a course.
 - Expects comma-separated course codes: "CS101,MATH101"
 - Ensures prerequisite level is lower than the course level (e.g., CS101 before CS102).
 */
int validatePrequis(const courses *c) {
       if(c->prerquistis[0] =='\0')
    return 1 ;
int i = 0 ;
int level = 0 ;
int level_faund = 0 ; 

    for( i = 0 ; c->course_code[i] != '\0' ;i++){
        if(c->course_code[i]>='0' && c->course_code[i]<='9'){
            level = atoi(&c->course_code[i]);
            level_faund = 1 ;
            break ; } }
if(!level_faund ||level <100 || level>400 ){return 0; }  
//We copy the string because strtok modifies the original buffer.
char prerquistis[60];
strcpy(prerquistis, c->prerquistis);
            char *tok = strtok(prerquistis,",");
            if(tok == NULL) return 0;
            while(tok){   
                while(*tok == ' ')tok++;   //Trims leading/trailing spaces of each token.  
    int len = strlen(tok);
    while (len > 0 && tok[len-1] == ' ') tok[--len] = '\0';
    courses *cours = search_course_code(tok);
    if(cours == NULL) return 0 ;

    int pre_level = 0 ;
    int found = 0 ;
        for(i = 0 ; tok[i] != '\0'; i++){
            if(tok[i]>='0' && tok[i]<= '9'){
                pre_level = atoi(&tok[i]);
                found = 1 ;
                break ;
            }
        }
            if(!found )return 0 ;
            if(pre_level>=level)return 0 ;
            tok = strtok(NULL,",");
            }
return 1 ;
}


//* Adds or clears prerequisites for an existing course.
// * If preq is NULL/empty/"0", prerequisites are cleared.
int addPrerquistis(int course_id ,const char* preq){

    if(!arrCourses){
        printf(" Please load courses data.\n");
        return 0 ;
    }
    courses *c = search_course_id(course_id);
    
    if(!c){
        printf("Course not found. \n");
        return 0 ;
    }
    courses temp = *c ;

        if(!preq || preq[0] == '\0' || strcmp(preq,"0") == 0){
    c->prerquistis[0] = '\0';
    printf("Prequistis cleaned.\n");
    return 1 ;
}
        strncpy(temp.prerquistis, preq, sizeof(temp.prerquistis) - 1);
        temp.prerquistis[sizeof(temp.prerquistis) - 1] = '\0';  


if(!validatePrequis(&temp)){
    printf("Invalid prerquisite format \n"); 
    return 0 ;
}//If valid, copy it into the real course record.
strncpy(c->prerquistis, preq, sizeof(c->prerquistis) - 1);
c->prerquistis[sizeof(c->prerquistis) - 1] = '\0';
return 1 ;

}





/*
 Checks whether a course still has available capacity in a given semester.
 Returns 1 if enroll_course < course_size, otherwise returns 0.
 */


int checkCourse_capacity(int course_id , const char *semester){
                if(!semester) return 0;

    courses *course = search_course_id(course_id);
    if(!course) {printf("Course not found (id : %d)\n",course_id);
        return 0;}

    int course_size = course->capasity ;
    int enroll_course = count_enrolledCourse(course_id,semester);

    return (enroll_course<course_size) ? 1 : 0 ;


}


 /*
 Displays the course roster for a specific semester.
 Reads enrollments and prints student info in a table format.
 */
void displayCourseRoster(int course_id, const char *semester){

if(!arrEnrollments || enrollment_count<=0){
    printf("No enrollments loaded \n");
    return ;

}

if(!semester || semester[0]  == '\0'){
    printf("Invalid semester .\n");
    return ;
}
    if(!validateSemester(semester)){
    printf("Invalid semester format , (YYYY-FALL) or (YYYY-SUMMER), (YYYY-SPRING)\n");
    return ;
}
courses *c = search_course_id(course_id);
if(!c){printf("Course not found  id :%d\n" ,course_id);
return ;}
        int remain = remaingCourseCapasty(course_id,semester);
 printf("\n========================================\n");
    printf(" COURSE ROSTER: %s - %s (ID:%d)\n", c->course_code, semester, course_id);
    printf(" Capacity: %d | Enrolled: %d  | Remaing : %d\n", c->capasity, count_enrolledCourse(course_id, semester),remain);
    printf("========================================\n");

    printf("+----------+-----------+-------------------------+----------+\n");
    printf("| EnrollID | StudentID | Student Name            | Status   |\n");
    printf("+----------+-----------+-------------------------+----------+\n");

int found = 0;

int i = 0 ;
        for(i = 0 ; i<enrollment_count ; i++){
            if(arrEnrollments[i].course_id != course_id)continue; 
            if(strcmp(arrEnrollments[i].semester,semester) != 0)continue;



Students * s = search_student_id(arrEnrollments[i].student_id);

char fullname[64] = "UNKNOWN";
 if (s) {
     snprintf(fullname, sizeof(fullname), "%s %s", s->first_name, s->last_name);
    }

            printf("| %-8d | %-9d | %-23s | %-8s |\n",
               arrEnrollments[i].enrollment_id,
               arrEnrollments[i].student_id,
               fullname,
               arrEnrollments[i].status);

             found = 1;
        }


printf("+----------+-----------+-------------------------+----------+\n");

if(!found){
    printf("No students found for this course in this semester \n");
}

}


/*
 Calculates remaining capacity for a course in a given semester.
  If result becomes negative, clamps it to 0 (safety).
 */
int remaingCourseCapasty(int course_id , const char  *semester){



    if(!semester || semester[0] == '\0')return -1 ;

    courses *c  = search_course_id(course_id);
    if(!c) return -1 ;

    int enrolled = count_enrolledCourse(course_id , semester);
    int remain_course = c->capasity - enrolled ;

if(remain_course<0) remain_course = 0 ;
return remain_course ;

}














