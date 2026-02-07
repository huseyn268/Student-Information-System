#include"grade.h"
#include"enrollment.h"
#include"utils.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <float.h>
#include"student.h"
#include<time.h>
#include"courses.h"




int grade_count ;  //Number of grade records currently stored in memory.
grades* arrGrades = NULL ;   //Dynamic array that stores all grade records.
int maxGrade  = 103; //Initial capacity for the grades array.




//Ensures the grade array has enough capacity.
 //* If full, grows the array by doubling its size (realloc).
static int ensureGradeCapacity()
{
    if (grade_count< maxGrade) return 1;

    int newCap = maxGrade * 2;

    grades *tmp = realloc(arrGrades, newCap * sizeof(grades));
    if (!tmp) {
        printf("Error: realloc failed for Grades\n");
        return 0;
    }
    arrGrades = tmp;
    maxGrade = newCap;
    return 1;
}
void init_Grades(){
    maxGrade = 103 ;
    arrGrades = (grades*)malloc(maxGrade*sizeof(grades));
    grade_count = 0 ;
    if(!arrGrades){
        printf("Memory allocation is failed");
        exit(1);
    }
}


/*
  Loads grade records from a CSV file.
 Skips the header line, parses each row using sscanf into arrGrades.
 */
int loadGrades(const char * path){

     if(!arrGrades){
        printf("arrGrade is null plese call init_grade();\n");
        return -1 ;
    }
        FILE* fp = fopen(path,"r");
    char line[260];
    int i = 0 ;

      if(!fp){
         printf("File could not be opened \n");
         return -1 ;

      }
   

fgets(line, sizeof(line), fp); // For header


      while(fgets(line,sizeof(line),fp)){

                   grade_count = i ;
          if (!ensureGradeCapacity()) {
            fclose(fp);
            return -1;
        }



        int a =sscanf(line,"%d,%d,%d,%d,%9[^,],%lf,%19[^,\n]",
            &arrGrades[i].grade_id,
            &arrGrades[i].enrollment_id,
            &arrGrades[i].student_id,
            &arrGrades[i].course_id,
            arrGrades[i].letter_grade,
            &arrGrades[i].numeric_grade,
            arrGrades[i].semester
        );
        if(a==7){
           clean_newLine(arrGrades[i].semester);
    clean_newLine(arrGrades[i].letter_grade);
    i++;
}
      }
    
      grade_count = i ;
      fclose(fp);
return i ;

}

//Frees memory allocated for the grades array.
 //* Used during shutdown or reset.
void freeGrade(){

    free(arrGrades);
    arrGrades = NULL;
    grade_count = 0 ;
    maxGrade = 0 ;
}


 //* Creates a grade struct by reading user input.
 //* Repeats input until validateGrade returns GRADE_OK.
grades createGrade(){

    grades g ;
    int err ;


    do{
        printf("Enter grade id :");
        while (scanf("%d",&g.grade_id) != 1) { 
    clear_input_buffer(); printf("Invalid. Enter grade id  : "); }
        printf("Enter enrollment id : ");
            while (scanf("%d",&g.enrollment_id) != 1) { 
    clear_input_buffer(); printf("Invalid. Enter enrollment id  : "); }

        printf("Enter student id :");
            while (scanf("%d",&g.student_id) != 1) { 
    clear_input_buffer(); printf("Invalid. Enter student id  : "); }

        printf("Enter course id :");
            while (scanf("%d",&g.course_id) != 1) { 
    clear_input_buffer(); printf("Invalid. Enter course id  : "); }

        printf("Enter letter grade :");
            scanf(" %10s", g.letter_grade);
        
        printf("Enter numeric grade :");
            while (scanf("%lf",&g.numeric_grade) != 1) { 
    clear_input_buffer(); printf("Invalid. Enter numeric grade  : "); }

printf("Enter semester (YYYY-FALL / YYYY-SPRING / YYYY-SUMMER): ");
        while (scanf(" %20s", g.semester) != 1) {   
            clear_input_buffer();
            printf("Invalid. Enter semester: ");
        }
        clear_input_buffer();
            err = validateGrade(&g);
            if(err!= GRADE_OK){
                printf("%s",validationGrade_msg(err));
            }
    }while(err != GRADE_OK);
return g ;
}

 //Saves grade records into a CSV file.
 //* Writes the header line and all grade rows.
int saveGrade(const char * path){
    FILE* fp = fopen(path,"w");

   if(!fp){
      printf("File could not open\n");
      return 0 ;
   }
   //header :
   fprintf(fp,"grade_id,enrollment_id,student_id,course_id,letter_grade,numeric_grade,semester\n");

   int i = 0 ;
   for(i = 0 ;i<grade_count ;i++)
    fprintf(fp,"%d,%d,%d,%d,%s,%.2lf,%s\n",
            arrGrades[i].grade_id,
            arrGrades[i].enrollment_id,
            arrGrades[i].student_id,
            arrGrades[i].course_id,
            arrGrades[i].letter_grade,
            arrGrades[i].numeric_grade,
            arrGrades[i].semester);


fclose(fp);
return 1 ;

}


// Searches a grade by enrollment_id.
grades* seachGrade_byEnrollmentid(const int  enrollmentid ){
       int i = 0 ;
            for(i = 0 ; i<grade_count ; i++){
               if(arrGrades[i].enrollment_id == enrollmentid){
                  return  &arrGrades[i];
               }
            }
          
               return NULL ;
}



//Searches a grade by grade_id.
grades* search_grade_id(const int  gradeID ){
       int i = 0 ;
            for(i = 0 ; i<grade_count ; i++){
               if(arrGrades[i].grade_id == gradeID){
                  return  &arrGrades[i];
               }
            }
          
               return NULL ;
}

 //Converts a letter grade to 4.0 GPA scale.
 //* Used for GPA calculations.

float letterTo_gpa(const char *letter){

        if(strcmp(letter,"A") == 0)  return 4 ;
        if(strcmp(letter,"A-") == 0) return 3.7 ;
        if(strcmp(letter,"B+") == 0) return 3.3 ;
        if(strcmp(letter,"B") == 0) return 3 ;
        if(strcmp(letter,"B-") == 0) return 2.7 ;
        if(strcmp(letter,"C+") == 0) return 2.3 ;
        if(strcmp(letter,"C") == 0) return 2.0 ;

return -1.0f; // Unknown letter grade.
}

// Adds a new grade record.
int addGrade(const grades *g){
    if(!arrGrades || !g){
        printf("arrGrades  or pointer is Null \n");
        return 0 ;
    }
         if (!ensureGradeCapacity()) return 0;


int err = validateGrade(g);
    if (err != GRADE_OK) {
        printf("%s\n", validationGrade_msg(err));
        return 0;
    }
if(!checkletter_numeric(g->letter_grade, g->numeric_grade)){
    printf("Error: Numeric grade %.2lf does not match letter grade %s\n",
           g->numeric_grade, g->letter_grade);
    return 0;
}
if(seachGrade_byEnrollmentid(g->enrollment_id) != NULL){
    printf("Error A grade already exist for enrollment id %d \n",g->enrollment_id);
    return 0 ;
}
if(search_grade_id(g->grade_id)!= NULL ){
    printf("Error : Grade ID : %d already exist. \n",g->grade_id);
    return 0;
}

enrollment *e = search_enrollment_id(g->enrollment_id);
if(!e){
    printf("Error Enrollment ID %d does not exist \n",g->enrollment_id);
return 0 ;
}
if(strcmp(e->status,"Enrolled") != 0){
   printf("Error: Enrollment status is %s. If it is Completed, use Update Grade instead of Add.\n", e->status);
    return 0 ;
}
    if(e->student_id != g->student_id){
        printf("Error : Student ID does not match enrollment record. \n");
        return 0 ;
    }
if(strcmp(e->semester,g->semester) != 0){
    printf("Error : Semester does not match enrollment record. \n");
    return 0 ;
}
if(e->course_id != g->course_id){
    printf("Error: Course ID does not match enrollment record.\n");
    return 0;
}





if(!updateEnrollmentStatus(e->enrollment_id, "Completed")){
    printf("Error: Failed to set enrollment to Completed.\n");
  
    return 0;
}
arrGrades[grade_count] = *g;
grade_count++;

        return 1 ;

}

// Updates a grade by grade_id.
int updateGRade(int grade_id, const char *letter, double new_numeric){

if(!arrGrades){
    printf("Error : arr grades is null , call init_grades()\n");
return 0 ;
        }
        grades *g = search_grade_id(grade_id);
        if(!g){
    printf("Error  : Grade with id : %d nod found . \n",grade_id);
    return 0 ;
        }
  if (!letter || letter[0] == '\0') {
        printf("Error: letter grade is empty\n");
        return 0;
    }
    if(!(strcmp(letter,"A")== 0 ||  strcmp(letter,"A-")== 0 || strcmp(letter,"B+")== 0 ||strcmp(letter,"B")== 0 ||
strcmp(letter,"B-")== 0  || strcmp(letter,"C+")== 0 || strcmp(letter,"C")== 0) )
        { printf("Error: Invalid letter grade. Use: A, A-, B+, B, B-, C+, C\n"); 
        return 0 ;}

if (new_numeric < 0.0f || new_numeric> 100.0f){
printf("Invalid numeric grade , grade must be 0<grade<100\n");        
return 0 ;}

    if(!checkletter_numeric(letter,new_numeric)){
        printf("Error : Numerix grade %.2lf does'not match letter grade %s\n",new_numeric,letter
        );return 0 ;
    }

strncpy(g->letter_grade, letter, sizeof(g->letter_grade) - 1);
    g->letter_grade[sizeof(g->letter_grade) - 1] = '\0';
    g->numeric_grade = new_numeric;
printf("Grade updated succesfully \n ");
return 1 ;
    }




 //Updates a grade by enrollment_id.
 //* Finds the grade for that enrollment and then calls updateGRade().   
int updateGradeByEnrollment(int enrollment_id,const char *letter,double new_numeric){


  if(!arrGrades || !arrEnrollments){
        printf("Required data not loaded (grades/enrollments).\n");
        return 0;
    }

    grades *g = seachGrade_byEnrollmentid(enrollment_id);
    if(!g){
        printf("No grade found for enrollment ID :%d",enrollment_id);
        return 0;
    }
return updateGRade(g->grade_id,letter,new_numeric);
}


//Validates if numeric score matches the given letter grade range.
int  checkletter_numeric(const char *letter, double num){
        if(strcmp(letter,"A") == 0 ) return num>=95.0 && num<=100 ;
        if(strcmp(letter,"A-" ) == 0) return num>=90.0 && num<95 ;
        if(strcmp(letter,"B+" ) == 0)return num>=85.0 && num<90;
        if(strcmp(letter,"B") == 0 )return num>=80.0 && num<85 ;
        if(strcmp(letter,"B-"  )== 0)return num>=75 && num<80 ;
        if(strcmp(letter,"C+" )== 0 )return num>=70 && num<75 ;
        if(strcmp(letter,"C" )== 0 )return num>=60 && num<70 ;
return 0 ;
}

//Calculates statistics (avg/min/max) for a course in a given semester.
 //* avg/min/max are output parameters written through pointers.
int course_statistics(int course_id, const char *semester,double *avg, double *min, double *max)
{
 if(!arrGrades) {printf("Error: arrGrades is NULL. Call init_Grades() first.\n");return 0;
    }
if(!semester || semester[0] == '\0') {printf("Error: semester is empty.\n");
   return 0;
    }
if(!avg|| !min || !max) {
printf("Error: avg/min/max pointers cannot be NULL.\n");
return 0;
 }
double sum = 0.0;    double mn = DBL_MAX;
double mx = -DBL_MAX;
 int cnt = 0;
int i = 0 ;
for (i = 0; i < grade_count; i++) {
if (arrGrades[i].course_id == course_id &&
strcmp(arrGrades[i].semester, semester) == 0)
        {
                double x = arrGrades[i].numeric_grade;
            sum += x;
if(x < mn) {mn = x;}
if(x > mx) {mx = x;}

       cnt++;
   }
    }
 if(cnt == 0) {
  printf("No grades found for course %d in %s\n", course_id, semester);        return 0;
    }
*avg = sum / cnt;
*min = mn;
*max = mx;

return 1; // başarı
}


//Prints letter grade distribution for a course in a given semester.
 //* Counts A, A-, B+, B, B-, C+, C.
void grade_distribution(int course_id, const char *semester){

    if (!arrGrades) {
        printf("Error: arrGrades is NULL. Call init_Grades() first.\n"); return;  }
    if (!semester || semester[0] == '\0') {
        printf("Error: semester is empty.\n"); return; }
    int A_counter=0, A_ncounter=0, B_pcounter=0, B_counter=0, B_ncounter=0, C_pcounter=0, C_counter=0, Total=0;
int i = 0;
    for (i = 0; i<grade_count;i++) { if (arrGrades[i].course_id == course_id &&
            strcmp(arrGrades[i].semester, semester) == 0)
        {
            Total++;
            if(strcmp(arrGrades[i].letter_grade, "A")  == 0) A_counter++;
            else if(strcmp(arrGrades[i].letter_grade, "A-") == 0) A_ncounter ++;
            else if(strcmp(arrGrades[i].letter_grade, "B+") == 0) B_pcounter++;
            else if(strcmp(arrGrades[i].letter_grade, "B")  == 0) B_counter++;
            else if(strcmp(arrGrades[i].letter_grade, "B-") == 0) B_ncounter++;
            else if(strcmp(arrGrades[i].letter_grade, "C+") == 0) C_pcounter++;
            else if(strcmp(arrGrades[i].letter_grade, "C")  == 0) C_counter++; } }
    if (Total == 0) {
        printf("No grades found for course %d in %s\n", course_id, semester); return; }
    printf("Grade Distribution | Course %d | %s | Total=%d\n", course_id, semester, Total);
        printf("A : %d\n", A_counter);
        printf("A-: %d\n", A_ncounter);
    printf("B+: %d\n", B_pcounter);
        printf("B : %d\n", B_counter);
    printf("B-: %d\n", B_ncounter);
        printf("C+: %d\n", C_pcounter);
     printf("C : %d\n", C_counter);
}


//grade_id 5-digit range
 //- enrollment_id positive
  //- student_id format and year range
  //- course_id positive
 //- semester format
 //- letter grade valid
 //* - numeric range and letter/numeric consistency 
Grade_Error validateGrade(const grades *g ){
    if(!g) return GRADE_NULL ;
    if(g->grade_id<10000 || g->grade_id>99999)
    return GRADE_ID;

       if (g->enrollment_id <= 0) {
        return GRADE_ENROLLMENT;
    }
if (g->student_id < 1000000 || g->student_id > 9999999) return GRADE_STUDENT;
   int year = g->student_id / 1000 ;
    if(year <2020 || year >2024 ){ return GRADE_STUDENT ;}

      if(g->course_id<= 0){
        return  GRADE_COURSE;
    }
if (!validateSemester(g->semester))
    return GRADE_SEMESTER;
    if(!(strcmp(g->letter_grade,"A")== 0 ||  strcmp(g->letter_grade,"A-")== 0 
    || strcmp(g->letter_grade,"B+")== 0 ||strcmp(g->letter_grade,"B")== 0 ||
    strcmp(g->letter_grade,"B-")== 0  || strcmp(g->letter_grade,"C+")== 0
     || strcmp(g->letter_grade,"C")== 0) )
        return GRADE_LETTER ;
if(g->numeric_grade < 0.0 || g->numeric_grade > 100.0)
    return GRADE_NUMERIC;
    if(!checkletter_numeric(g->letter_grade,g->numeric_grade))
    return GRADE_NUMERIC;
       return  GRADE_OK;
}


/*
  Validates semester format.
 Expected: "YYYY-FALL" / "YYYY-SPRING" / "YYYY-SUMMER"
 Also checks year range (2020-2024).
 */
int validateSemester(const char *semester){
    if(!semester) return 0 ;
     if (semester[0] == '\0') return 0;


 if (!(strlen(semester) == 9 || strlen(semester)== 11)) return 0;

    int i = 0;
    for(i = 0 ;i<4 ; i++){
        if(semester[i]<'0' ||   semester[i] >'9')
        return 0;

    }   
        if (semester[4] != '-')
        return 0;

   int  sem_year = (semester[0]-'0')*1000 + (semester[1]-'0')*100 + (semester[2]-'0')*10  + (semester[3]-'0');
    if(sem_year <2020 || sem_year >2024 ){ return 0 ;}
 if(!(strcmp(semester + 5 , "FALL") == 0 || strcmp(semester + 5 , 
        "SPRING") == 0 ||strcmp(semester + 5 , "SUMMER") == 0 ))
        return 0;

return 1 ;
}

//Returns rank such that SPRING < SUMMER < FALL.
 int term_rank(char *term){

        if(!term){
                return 50 ;
        }

 if(strcmp(term,"SPRING")== 0) return 1 ;
if(strcmp(term,"SUMMER") == 0 )  return 2 ;
if(strcmp(term,"FALL") == 0 ) return 3 ;

return 100 ;
}


/*
 Enrollment comparator: first by semester, then by enrollment_id.
Ordering:
 year ascending
 within year: SPRING < SUMMER < FALL
 within same semester: enrollment_id ascending
 */
int cmp_enrollments_by_semester_then_id(const void *pa, const void *pb)
{
    const enrollment *a = (const enrollment*)pa;
    const enrollment *b = (const enrollment*)pb;

    int ya=0, yb=0;
    char ta[16]={0}, tb[16]={0};

    if (sscanf(a->semester, "%d-%15s", &ya, ta) != 2) { ya = -1; ta[0] = '\0'; }
    if (sscanf(b->semester, "%d-%15s", &yb, tb) != 2) { yb = -1; tb[0] = '\0'; }

    if (ya != yb) return (ya < yb) ? -1 : 1;

    int ra = term_rank(ta);
    int rb = term_rank(tb);
    if (ra != rb) return (ra < rb) ? -1 : 1;

    if (a->enrollment_id < b->enrollment_id) return -1;
    if (a->enrollment_id > b->enrollment_id) return 1;
    return 0;
}


// Deletes a grade record by enrollment_id.
int deleteGradeBy_EnrollmentId(int enrollment_id){


    if(!arrGrades) return 0 ;
int i = 0 , j = 0  ;
    for(i = 0 ; i<grade_count ; i++){
        if(arrGrades[i].enrollment_id == enrollment_id){
            for(j = i ; j<grade_count-1 ; j++){
                arrGrades[j] =arrGrades[j+1];
            }
            grade_count -- ;
            return 1 ;

        }
  

    }


      return 0 ;

}



//Deletes all grade records of a given student_id.
int deleteGradeBy_StudentId(int student_id){


    if(!arrGrades) return 0 ;
int i = 0 , j = 0  ;
int deleted = 0;
    for(i = 0 ; i<grade_count ;){
        if(arrGrades[i].student_id == student_id){
            for(j = i ; j<grade_count-1 ; j++){
                arrGrades[j] =arrGrades[j+1];
            }
            grade_count -- ;
            deleted ++ ;
    
        }
  else{
    i++;
  }

    }


      return deleted ;

}


//Converts grade validation error codes into user-friendly messages.
 // Used to explain errors returned by validateGrade.
const char * validationGrade_msg(Grade_Error err){
    switch (err)
    {
    case GRADE_NULL :  return "Grade pointer is null";
    case GRADE_OK  :    return "Grade data is valid ";
    case GRADE_ID :       return "Grade must be 5 digit and >0";  
    case GRADE_ENROLLMENT :       return "Enrollment id must be greater than 0";
    case GRADE_STUDENT :  return"Invalid student id , id must be 7 digit and 2020-2024";
    case GRADE_COURSE :    return "Invalid course id , id must be grater than 0";
    case GRADE_LETTER :     return"Invalid letter grade , A , A- , B+, B- ,B, C+,C";
    case GRADE_NUMERIC :      return"Invalid numeric grade , grade must be 0<grade<100";
    case GRADE_SEMESTER :   return "Invalid semester format , (YYYY-FALL) or (YYYY-SUMMER), (YYYY-SPRING)";
    default:        return "Unknwn enrollment error" ;
    }

}

//Counts how many grades belong to a course_id 
int countGradeCourse(int courseID){
    if(!arrGrades) return 0;

int counter = 0 ;
int i = 0 ;
for(i = 0 ; i<grade_count ; i++){
    if(arrGrades[i].course_id == courseID) counter++;
}
return counter ;

}


//Deletes all grade records for a given course_id.
int deleteGRades_Course(int course_id){
    if(!arrGrades)return 0 ;
    int delete_counter = 0 ;

int i = 0 ;
int j = 0 ;
for(i = 0 ;i<grade_count ;){
    if(arrGrades[i].course_id == course_id){

            for(j = i ; j<grade_count  -1 ; j++){
                arrGrades[j] = arrGrades[j+1];
            }
            grade_count--;
            delete_counter++; }
    else{
        i++;
    }
}
    return delete_counter;
}
