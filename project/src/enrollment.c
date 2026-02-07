#include"enrollment.h"
#include"utils.h"
#include"courses.h"
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include"professor.h"
#include"student.h"
#include"grade.h"
int enrollment_count = 0 ; //Number of enrollments currently stored in memory.
enrollment* arrEnrollments = NULL ; // Dynamic array that stores all enrollment records.
int maxEnrollment =121 ; //  Initial capacity for the enrollment array.

/*
  Ensures the enrollment array has enough capacity.
  If full, grows the array by doubling its size (realloc).
 */
static int ensureEnrollmentCapacity()
{
    if (enrollment_count< maxEnrollment) return 1;

    int newCap = maxEnrollment * 2;

    enrollment *tmp = realloc(arrEnrollments, newCap * sizeof(enrollment));
    if (!tmp) {
        printf("Error: realloc failed for Enrollments\n");
        return 0;
    }
    arrEnrollments = tmp;
    maxEnrollment = newCap;
    return 1;
}
//Allocates initial memory and resets the counter.
void init_Enrollments(){
    maxEnrollment = 121 ;
    enrollment_count = 0 ;
    arrEnrollments = (enrollment*)malloc(maxEnrollment * sizeof(enrollment));
    if(!arrEnrollments){
        printf("Memory allocation is failed\n");
    exit(1);
    }
}

/*
  Loads enrollment records from a CSV file.
  Header line is skipped, each row is parsed using sscanf.
 */
int loadEnrollment(const char *path){
    if(!arrEnrollments){
        printf("arrEnrollment is NULL . Call init_Enrollments");
        return -1 ;
    }
        FILE* fp = fopen(path,"r");
    char line[260];
    int i = 0 ;

      if(!fp){
         printf("File could not be opened \n");
         return -1 ;

      }
   
if (!fgets(line, sizeof(line), fp)) {   // header
    printf("Empty file or missing header.\n");
    fclose(fp);
    return -1;
}


      while(fgets(line,sizeof(line),fp)){
// Update enrollment_count while loading so capacity check is correct.
    
           enrollment_count = i ;
          if (!ensureEnrollmentCapacity()) {
            fclose(fp);
            return -1;
        }

        int a =sscanf(line,"%d,%d,%d,%d,%29[^,],%29[^,],%29[^,]",
            &arrEnrollments[i].enrollment_id,
            &arrEnrollments[i].student_id,
            &arrEnrollments[i].course_id,
            &arrEnrollments[i].professor_id,
            arrEnrollments[i].semester,
            arrEnrollments[i].enrollment_date,
            arrEnrollments[i].status
        
        );
        if(a==7){ // If the row is parsed successfully, trim newline and spaces.
           clean_newLine(arrEnrollments[i].semester);
    clean_newLine(arrEnrollments[i].enrollment_date);
    clean_newLine(arrEnrollments[i].status);
i++;}

      }
    
      enrollment_count = i ;
      fclose(fp);
return i ;


}



 //Frees memory allocated for the enrollments array.
 // Used during shutdown or data reset.
void freeEnrollment(){
        free(arrEnrollments);
        arrEnrollments = NULL;
        enrollment_count = 0;
        maxEnrollment = 0 ;
}






/*
 Creates a new enrollment record from user input.
 Keeps asking until validateEnrollment returns ENROLLMENT_OK.
 */
enrollment createEnrollment(){
    enrollment e ;
    int err ;


    do{ 
    printf("Enter enrollment_id :");
       
    while (scanf("%d",&e.enrollment_id) != 1) { 
    clear_input_buffer(); printf("Invalid. Enter enrollment id : "); }
            clear_input_buffer();  
    printf("Enter student_id : ");
  while (scanf("%d",&e.student_id) != 1) { 
    clear_input_buffer(); printf("Invalid. Enter student id  : "); }



printf("Enter course id :");
  while (scanf("%d",&e.course_id) != 1) { 
    clear_input_buffer(); printf("Invalid. Enter Course id  : "); }

printf("Enter  professor id :");
      while (scanf("%d",&e.professor_id) != 1) { 
    clear_input_buffer(); printf("Invalid. Enter Professor id  : "); }
    // Clear input buffer before fgets because scanf leaves a newline.
printf("Enter semester : ");
clear_input_buffer();

if (!fgets(e.semester, sizeof(e.semester), stdin)) {
    printf("Input error while reading semester.\n");
    continue;               
}
clean_newLine(e.semester);


printf("Enter enrollment date : ");
if (!fgets(e.enrollment_date, sizeof(e.enrollment_date), stdin)) {
    printf("Input error while reading date.\n");
    continue;
}
clean_newLine(e.enrollment_date);

printf("Enter status : ");
if (!fgets(e.status, sizeof(e.status), stdin)) {
    printf("Input error while reading status.\n");
    continue;
}
clean_newLine(e.status);

    err = validateEnrollment(&e);
        if(err!= ENROLLMENT_OK){
        printf("%s\n", validation_enrollment_msg(err));
         printf("Pleas try again \n");
        }
    }while(err != ENROLLMENT_OK);

return e ;

}

//Prints an enrollment record in a readable format.
void showEnrollment(enrollment *e){

    if (!e) {printf("Enrollment is NULL\n") ;return; }
printf("\n=====Enrollment Information=====\n");
printf("Enrollment ID  :    %d\n",e->enrollment_id);
printf("Student ID     :    %d\n",e->student_id);
printf("Course ID      :    %d\n",e->course_id);
printf("Professor ID   :    %d\n",e->professor_id);
printf("Semester       :    %s\n",e->semester);
printf("Enrollment Date:    %s\n",e->enrollment_date);
printf("Status         :    %s\n",e->status);
}


//Performs a linear search by enrollment_id.
enrollment* search_enrollment_id(const int  enrollmentid ){
       int i = 0 ;
            for(i = 0 ; i<enrollment_count ; i++){
               if(arrEnrollments[i].enrollment_id == enrollmentid){
                  return  &arrEnrollments[i];
               }
            }
          
               return NULL ;
}



/* 
Checks whether a student is already enrolled in the same course
for the same semester (excluding withdrawn/dropped records).
 */

int checkStudent_Enrollment(int  studentID, int courseID ,const char *semester){

    if(!semester) return 0 ;

int i = 0 ;
for(i = 0 ;i<enrollment_count ; i++){

    if(arrEnrollments[i].student_id == studentID && arrEnrollments[i].course_id == courseID 
    && strcmp(arrEnrollments[i].semester ,semester ) == 0
&&  (strcmp(arrEnrollments[i].status , "Enrolled")==0 || strcmp(arrEnrollments[i].status,"Completed")==0 ))
    return 1 ;

        }

    return 0 ;

}




  //Adds a new enrollment record.
int addEnrollment(const enrollment *e){
    if(!arrEnrollments || !e  ) {
        printf("arrEnrollment or e is Null  \n");
        return 0 ;
    }
      if (!ensureEnrollmentCapacity()) return 0;
int err = validateEnrollment(e);
    if (err != ENROLLMENT_OK) {
        printf("%s\n", validation_enrollment_msg(err));
        return 0;
    }
    if(search_enrollment_id(e->enrollment_id) !=NULL){
    printf("Duplicate enrollment \n");
    return 0;
}
if(search_student_id(e->student_id) == NULL){
    printf("Student not found . ID : %d\n",e->student_id);
    return 0 ;
}
if(search_course_id(e->course_id) == NULL){
    printf("Course not found  ,  ID %d\n ",e->course_id);
    return 0 ;}
if(search_prof_id(e->professor_id) == NULL){
    printf("Professor not found, ID   %d \n",e->professor_id);
    return 0 ;}
if(checkStudent_Enrollment(e->student_id,e->course_id,e->semester)){
    printf("This student already enrolled int this course for this semester \n");
    return 0;
}
if(!check_prerquisties(e->student_id,e->course_id)){
    printf("Prerquisties are not satisfied.\n");
    return 0 ;
}
if(!checkCourse_capacity(e->course_id,e->semester)){
    printf("Course capacity is full\n");
    return 0 ;}
    //New enrollment cannot start as Completed.
     //* If status is Completed, it is automatically changed to Enrolled.
        enrollment  temp = *e;
        if(strcmp(temp.status,"Completed") == 0 ){
            printf("Warning : New enrollment can not start as Completed. \n");
            printf("Student status change Enrolled. If you want to Completed you need to add grade.\n");
            strcpy(temp.status ,"Enrolled");
        }
arrEnrollments[enrollment_count] = temp;
enrollment_count++;
printf("Enrollment added \n");
printf("Remaing capacity : %d \n",remaingCourseCapasty(e->course_id,e->semester));
return 1 ;
}
int saveEnrollment(const char * path){
          FILE* fp = fopen(path,"w");

   if(!fp){
      printf("File could not open\n");
      return 0 ;
   }
   //Header :
   fprintf(fp,"enrollment_id,student_id,course_id,professor_id,semester,enrollment_date,status\n");
int i = 0;
   for(i = 0 ;i<enrollment_count ; i++){
    fprintf(fp,"%d,%d,%d,%d,%s,%s,%s\n",
    arrEnrollments[i].enrollment_id,
    arrEnrollments[i].student_id,
    arrEnrollments[i].course_id,
    arrEnrollments[i].professor_id,
    arrEnrollments[i].semester,
    arrEnrollments[i].enrollment_date,
    arrEnrollments[i].status           
);

   }
fclose(fp);
return 1 ;
}


//Delete enrollment
int deleteEnrollment(int enrollment_id){

    int i = 0 , j = 0 ;
    for(i = 0 ; i<enrollment_count ; i++)
    {
            if(arrEnrollments[i].enrollment_id == enrollment_id){
                for(j = i ; j<enrollment_count-1 ; j++){
                    arrEnrollments[j] = arrEnrollments[j +1];

                }
                enrollment_count--;
                return  1;

            }



    }
            return 0 ;

}


//* Counts how many "Enrolled" records exist for a course in a semester.
 //* Used for capacity checks.
int count_enrolledCourse(int course_id ,const char *semester){
    if (!arrEnrollments || !semester) return 0;
int i = 0 ;
int count = 0 ;
    for(i = 0 ; i<enrollment_count ; i++){
        if(arrEnrollments[i].course_id == course_id && 
            strcmp(arrEnrollments[i].semester , semester) == 0 && (strcmp(arrEnrollments[i].status, "Enrolled") == 0 ))
            count ++;
    }
return count;

}



//Checks whether the student has satisfied prerequisites for a course.
int check_prerquisties(int student_id ,int couurse_id){
    courses *course = search_course_id(couurse_id);
        if(!course) return 0;

    if(course->prerquistis[0] == '\0')
    return 1 ;
char prerquistis[50];
strcpy(prerquistis,course->prerquistis);
char*token = strtok(prerquistis,",");
int i = 0 ;        

while(token){
    // Clear space.
        while (*token == ' ' || *token == '\t') token++;
int len = strlen(token);
while (len > 0 && (token[len-1] == ' ' || token[len-1] == '\t')) token[--len] = '\0';
        courses *p = search_course_code(token);
           if(!p)  return 0 ;
        int completed = 0;
        for(i = 0 ; i<enrollment_count; i++){
            if(arrEnrollments[i].student_id == student_id &&
            arrEnrollments[i].course_id == p->course_id && strcmp(arrEnrollments[i].status ,"Completed") == 0 )
            {   completed = 1 ;
            break ;}
        }
            if(!completed)
            return 0 ;
        token = strtok(NULL,",");
 
    }
return 1 ;

}


/*

  Updates enrollment status (Enrolled <-> Completed).
  Rules:
 - Completed -> Enrolled: delete grade record if exists.
 - Enrolled -> Completed: grade must exist first.
 */
int updateEnrollmentStatus(int enrollment_id,const char*newStatus){
        if(!arrEnrollments){
            printf("Enrollments not initialized \n");
            return 0 ;
        }
        if(!newStatus || newStatus[0] ==   '\0' ){
            printf("New statüs is null\n");
            return 0 ;
        }
    if(!(strcmp(newStatus,"Enrolled") == 0 || strcmp(newStatus,"Completed") == 0)){
        printf("Invalid status, Only  Enrolled  or Completed \n");
        return 0 ;
    }
        enrollment *e = search_enrollment_id(enrollment_id);
        if(!e){
                printf("Enrollment id is not found  (id : %d )\n",enrollment_id);
                return 0 ;
                        }

                if(strcmp(e->status, newStatus)==0){
            printf("This status already %s\n",newStatus);
                                return 1;
                            }
                                // for Completed --> Enrolled .
    if(strcmp(e->status,"Completed") == 0 && strcmp(newStatus,"Enrolled") == 0){
        courses *c = search_course_id(e->course_id);
        if(!c){
            printf("Course not found \n");
            return 0 ;
        }
            int enroll_Now = count_enrolledCourse(e->course_id , e->semester);
            if(enroll_Now>=c->capasity){
                printf("Cannot set to Enrolled : course is full  , capasity : %d\n",c->capasity);
                return 0 ;
            }

            if(seachGrade_byEnrollmentid(e->enrollment_id)!= NULL){
                if(!deleteGradeBy_EnrollmentId(e->enrollment_id)){
                    printf("Error : Failed to delete grade for enrollment id\n");
                    return 0;
                }
            }

    snprintf(e->status, sizeof(e->status), "%s", "Enrolled");
    printf("Enrollment %d updated: Completed -> Enrolled (grade deleted)\n", enrollment_id);
    return 1;

    }
                    //For enrollled --> Completed.

    if(strcmp(newStatus,"Completed") == 0 && strcmp(e->status,"Enrolled") == 0){

            if(seachGrade_byEnrollmentid(e->enrollment_id) == NULL){
                printf("Cannot set to  Completed, No grade recorded for enrollment id : %d\n",enrollment_id);
                printf("First add grade , then set status tuo Completed \n");
                return 0 ;
            }
    snprintf(e->status, sizeof(e->status), "%s", "Completed");
    printf("Enrollment %d updated: Enrolled -> Completed\n", enrollment_id);
    return 1;       

    }
printf("Status transition not allowed: %s -> %s\n", e->status, newStatus);
return 0;
    }

 
    //Prints the enrollment list of a student.
void listStudentEnrollment(int student_id ,const char *semesterOptinal){

        if(!arrEnrollments){
            printf("Printf arrEnrollment is null . Call init arrEnrollment .\n");
          return ; }
        int found = 0 ;
int i = 0 ;
printf("\n==== Student Enrollments (student id : %d ) ====\n",student_id);
        if (semesterOptinal && *semesterOptinal)
        printf("Semester filter: %s\n", semesterOptinal);
 printf("+----------+--------+----------+-----------------+------------+------------+-----------+\n");
    printf("| EnrollID | CID    | ProfID   | Course (Code)   | Semester    | Date       | Status    |\n");
    printf("+----------+--------+----------+-----------------+------------+------------+-----------+\n");

for(i = 0 ; i<enrollment_count ; i++){

    if(arrEnrollments[i].student_id != student_id) 
    continue; 

    if(semesterOptinal && semesterOptinal[0] !='\0'){
        if(strcmp(arrEnrollments[i].semester,semesterOptinal) != 0 )
        continue; 
    }

courses *c = search_course_id(arrEnrollments[i].course_id);

const char *code = (c ? c->course_code : "-");

printf("| %-8d | %-6d | %-8d | %-15s | %-10s | %-10s | %-9s |\n",
       arrEnrollments[i].enrollment_id,
       arrEnrollments[i].course_id,
       arrEnrollments[i].professor_id,
       code,
       arrEnrollments[i].semester,
       arrEnrollments[i].enrollment_date,
       arrEnrollments[i].status);
    found = 1 ;

}printf("+----------+--------+----------+-----------------+------------+------------+-----------+\n");
    if(!found){
        if(semesterOptinal && semesterOptinal[0] != '\0')
        printf("No enrollments found for this stdudent in semester %s \n",semesterOptinal);
        else{
            printf("No enrollments found for this student \n");
        }
    }
}

//Validates date format: YYYY-MM-DD
int validate_date(const char *s)
{
    
    if(!s) return 0 ;

    if(strlen(s) != 10)return 0 ;
if (s[4] != '-' || s[7] != '-') return 0;

   for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) continue;
        if (s[i] < '0' || s[i] > '9') return 0;
    }
int year = (s[0]-'0')*1000 + (s[1]-'0')*100 + (s[2]-'0')*10  + (s[3]-'0');
    int month = (s[5]-'0')*10 + (s[6]-'0')*1;
    int day = (s[8]-'0')*10 + (s[9]-'0')*1;

if(year<1900 || year>2100) return 0 ;
if(month>12 || month <1) return 0;
if(day > 31 || day<1) return 0 ;
    return 1 ;

}

/*
  Validates enrollment data and returns an error code.
 Checks:
  - ID ranges
  - student_id year range (2020-2024)
  professor_id range (5000-5999)
  - semester format and year range
  date format
  - allowed status values
 */
int validateEnrollment(const enrollment *e)
{

    if(e== NULL){return   ENROLLMENT_NULL ; }
      if (e->enrollment_id <= 0) {
        return ENROLLMENT_ID;
    }
    if(e->student_id <1000000 || e->student_id >9999999 )return  ENROLLMENT_STUDENT_ID ;

    int year = e->student_id / 1000 ;
    if(year <2020 || year >2024 ){ return ENROLLMENT_STUDENT_ID ;}
      
  if(e->course_id<= 0){
        return  ENROLLMENT_COURSE_ID;
    }
      if(e->professor_id <5000 || e->professor_id >5999){ return ENROLLMENT_PROFESSOR_ID;}

       if (e->semester[0] == '\0') return ENROLLMENT_SEMESTER;


 if (!(strlen(e->semester) == 9 || strlen(e->semester)== 11)) return ENROLLMENT_SEMESTER;

    int i = 0;
    for(i = 0 ;i<4 ; i++){
        if(e->semester[i]<'0' || e->semester[i] >'9')
        return ENROLLMENT_SEMESTER;

    }   
        if (e->semester[4] != '-')
        return ENROLLMENT_SEMESTER;


   int  sem_year = (e->semester[0]-'0')*1000 + (e->semester[1]-'0')*100 + (e->semester[2]-'0')*10  + (e->semester[3]-'0');
    if(sem_year <2020 || sem_year >2024 ){ return ENROLLMENT_SEMESTER ;}
 if(!(strcmp(e->semester + 5 , "FALL") == 0 || strcmp(e->semester + 5 , 
        "SPRING") == 0 ||strcmp(e->semester + 5 , "SUMMER") == 0 ))
        return ENROLLMENT_SEMESTER;



        if(!validate_date(e->enrollment_date)) return ENROLLMENT_DATE;

if (!(strcmp(e->status, "Enrolled") == 0 || strcmp(e->status, "Completed") == 0)) return ENROLLMENT_STATUS;

return ENROLLMENT_OK;
}
//Converts enrollment validation error codes to user-friendly messages.
 //* Used to describe errors returned by validateEnrollment.
const char* validation_enrollment_msg(Enrollment_error err)
{
    switch (err)
    {
        case ENROLLMENT_OK : return"Enrollment data is valid ";
        case ENROLLMENT_NULL:   return "Erollment pointer is null";
        case ENROLLMENT_ID :    return "Enrollment id must be greater than 0";
        case ENROLLMENT_STUDENT_ID :   return"Invalid student id , id must be 7 digit and 2020-2024";
        case ENROLLMENT_COURSE_ID:  return "Invalid course id , id must be grater than 0";
        case ENROLLMENT_PROFESSOR_ID :  return "Invalid proffessor ID , ID must be between(5000 -5999)";
        case ENROLLMENT_SEMESTER :      return "Invalid enrollment format , (YYYY-FALL) or (YYYY-SUMMER), (YYYY-SPRING)";
        case ENROLLMENT_DATE :  return  "Invalid enrollment date (use YYYY-MM-DD)";
        case ENROLLMENT_STATUS: return "Invalid status , status must be Enrlled or  Completed";
        default : return "Unknwn enrollment error" ;
        
    }

}


 //* Drops a student from a specific course in a specific semester.
 //* Deletes the enrollment record, and if a grade exists for that enrollment, deletes the grade first.
int dropStudentFromCOurse(int student_id ,int course_id ,const char *semster){
            if(!arrEnrollments){

                printf("Enrollment not initialized \n");
                return 0 ;
            }

            if(course_id<0){
                printf("Invalid course ID \n");
                return 0 ;
            }

        if(!validateid(student_id)){
            printf("Student ID must be YEAR### (e.g., 2020001) and year must be 2020-2024.\n");
            return 0 ;
        }

            if(!validateSemester(semster)){
    printf("Invalid semester format , (YYYY-FALL) or (YYYY-SUMMER), (YYYY-SPRING)\n");
    return 0 ;
}

int i = 0 ;
    for(i = 0 ;i<enrollment_count ; i++){
        if(arrEnrollments[i].student_id == student_id && arrEnrollments[i].course_id == course_id && 
            strcmp(arrEnrollments[i].semester ,semster) == 0 ){
                int enrollment_id = arrEnrollments[i].enrollment_id ;


                //if student have grade, delete grade.
                if(seachGrade_byEnrollmentid(enrollment_id) != NULL){
                    deleteGradeBy_EnrollmentId(enrollment_id);
                }

                //delete enrollment.
                if(!deleteEnrollment(enrollment_id)){
                    printf("Failed to delete enrollment id \n");
                    return 0 ;
                }
                printf("Dropped : student %d ,from course %d  %s\n",student_id,course_id,semster);
                return 1 ;
            }
    }
        printf("No enrollment found for student %d , in course %d %s\n",student_id,course_id,semster);
        return 0 ;

}


//Counts total enrollments for a given course_id
//Used in delete warnings to know how many dependent records exist
int countEnrollmentsBycourse(int courseID){
if(!arrEnrollments) return 0;

int counter = 0 ;
int i = 0 ;
for(i = 0 ; i<enrollment_count ; i++){
    if(arrEnrollments[i].course_id == courseID) counter++;
}
return counter ;

}

//Deletes all enrollment records for a given course_id.
int deleteEnrollment_Course(int course_id){
    if(!arrEnrollments)return 0 ;
    int delete_counter = 0 ;

int i = 0 ;
int j = 0 ;
for(i = 0 ;i<enrollment_count ;){
    if(arrEnrollments[i].course_id == course_id){

            for(j = i ; j<enrollment_count  -1 ; j++){
                arrEnrollments[j] = arrEnrollments[j+1];
            }
            enrollment_count--;
            delete_counter++; }
    else{
        i++;
    }
}
    return delete_counter;
}

// Deletes all enrollment records for a given student_id.
int deleteEnrollment_Student(int student_id){
    if(!arrEnrollments)return 0 ;
    int delete_counter = 0 ;
    
int i = 0 ;
int j = 0 ;
for(i = 0 ;i<enrollment_count ;){
    if(arrEnrollments[i].student_id == student_id){

            for(j = i ; j<enrollment_count  -1 ; j++){
                arrEnrollments[j] = arrEnrollments[j+1];
            }
            enrollment_count--;
            delete_counter++; }
    else{
        i++;
    }
}
    return delete_counter;
}


// * Deletes all enrollment records for a given professor_id.
 //* If a grade exists for an enrollment, delete the grade first
int deleteEnolmment_Prof(int professor_id){

    if(!arrEnrollments) return 0 ;
int i = 0 ;
    int deleted  = 0 ;

    for(i = 0 ; i<enrollment_count ; ){
    if(arrEnrollments[i].professor_id == professor_id){
            
        if(seachGrade_byEnrollmentid(arrEnrollments[i].enrollment_id) != NULL){
                if(!deleteGradeBy_EnrollmentId(arrEnrollments[i].enrollment_id)){
                    printf("Failed to grade for enrollment id \n");
                    return 0 ;
                }
        }
        for(int j  = i ; j<enrollment_count-1 ; j++){
            arrEnrollments[j] =  arrEnrollments[j+1];
        }
        enrollment_count--;
        deleted++;
    }

else{
    i++;
}

}
return deleted ;


}


//Counts how many enrollment records belong to a professor.
int countEnrollment_Prof(int prof_id){

        if(!arrEnrollments)return 0 ;

    int i = 0 ;
    int counter = 0 ;

    for(i = 0 ; i<enrollment_count ; i++){
        if(arrEnrollments[i].professor_id == prof_id)
        counter ++ ;
    }

return counter ;
}

//* Sorts an enrollment list using cmp_enrollments_by_semester_then_id.
 //* Ordering is by semester first, then by ID (as defined in comparator).
 //* (Uses bubble sort; acceptable for small n.)
 
void sortEnrollment(enrollment *list , int n){
int i = 0 ;
int j = 0 ;
        for(i = 0 ; i<n-1 ; i++){
            for(j = 0 ; j<n-1 -i ; j++){
                if((cmp_enrollments_by_semester_then_id(&list[j],&list[j+1]))>0){
                    enrollment temp = list[j];
                    list[j] = list[j+1];
                    list[j +1] = temp ;
                }
            }
        }
}

