#include"student.h"
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include <ctype.h>
#include"utils.h"
#include<time.h>
#include"courses.h"
#include"grade.h"
#include"enrollment.h"
   /*     int student_id;
    char first_name[20];
    char last_name[20];
    char s_mail[30];
    char s_phone[20];
    int enrollment_year;
    char major[20];
    double gpa;*/
int maxStudent = 100 ;  //initial capacity of the students array (100 for sample CSV data).           
int student_count = 0  ; //Number of students currently stored in memory.
Students* arrStudents = NULL ;    //Dynamic array that stores all student records.


//Ensures the student array has enough capacity.
 //* If full, grows the array by doubling its size (realloc).
static int ensureStudentCapacity(void)
{
    if (student_count < maxStudent) return 1;

    int newCap = maxStudent * 2;

    Students *tmp = realloc(arrStudents, newCap * sizeof(Students));
    if (!tmp) {
        printf("Error: realloc failed for Students\n");
        return 0;
    }

    arrStudents = tmp;
    maxStudent = newCap;
    return 1;
}

//Initializes the Student module.
 //* Uses calloc so allocated memory is zero-initialized.
void init_Students(){
    maxStudent = 100;
     arrStudents = (Students*)calloc(maxStudent,sizeof(Students));
     student_count =0 ;
     if(!arrStudents ){
        printf("Memory allocation failed. ");
        exit(1);

     }
}
 

//* Loads student records from a CSV file.
 //* Skips header line, parses each record using sscanf.
int loadStudent(const char* path ){

      if (!arrStudents) {
    printf("arrStudents is NULL. Call init_Students() first.\n");
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
//Update student_count while loading so capacity check works correctly.

      student_count = i ;
          if (!ensureStudentCapacity()) {
            fclose(fp);
            return -1;
        }


   int a = sscanf(line,"%d,%29[^,],%29[^,],%29[^,],%29[^,],%d,%49[^,],%lf",&arrStudents[i].student_id, arrStudents[i].first_name, arrStudents[i].last_name,
   arrStudents[i].s_mail,arrStudents[i].s_phone,&arrStudents[i].enrollment_year,arrStudents[i].major,&arrStudents[i].gpa);
   if(a==8){ clean_newLine(arrStudents[i].first_name);
clean_newLine(arrStudents[i].last_name);
clean_newLine(arrStudents[i].s_mail);
clean_newLine(arrStudents[i].s_phone);
clean_newLine(arrStudents[i].major);
      i++ ;}
}
      student_count = i ;

fclose(fp);
 return i ;

}


//Reads new student data for update (does not ask for ID).
 //* Used inside updateStudent().
Students createStudentforUpdate(){

      Students s= {0};
      clear_input_buffer();
   printf("Enter first name : ");
   fgets(s.first_name,sizeof(s.first_name),stdin);
   clean_newLine(s.first_name);
   
   printf("Enter last name : ");
   fgets(s.last_name,sizeof(s.last_name),stdin);
   clean_newLine(s.last_name);
   
   printf("Enter student mail  : ");
   fgets(s.s_mail,sizeof(s.s_mail),stdin);
   clean_newLine(s.s_mail);

   printf("Enter  student phone (format 555-####) : ");
    fgets(s.s_phone, sizeof(s.s_phone), stdin);
    clean_newLine(s.s_phone);
   
    printf("Enter enrollment year :");
   while (scanf("%d", &s.enrollment_year) != 1) {
        clear_input_buffer();
        printf("Invalid year. Try again: ");
    }
   clear_input_buffer();
   printf("Enter student major : ");
   fgets(s.major, sizeof(s.major), stdin);
clean_newLine(s.major);

return s ;

}

 //Creates a new student record (asks for ID and all fields).
 //* Re-prompts until validateStudent returns ERROR_OK.
Students createStudent(){
      Students s  = {0};
      int err ;
   do{   
printf("Enter student id: ");
   if (scanf("%d", &s.student_id) != 1) {
    clear_input_buffer();
    printf("Invalid input.\n");
      continue;
}
   clear_input_buffer();
printf("Enter first name : ");
   fgets(s.first_name,sizeof(s.first_name),stdin);
   clean_newLine(s.first_name);
   printf("Enter last name : ");
   fgets(s.last_name,sizeof(s.last_name),stdin);
   clean_newLine(s.last_name);
   

   printf("Enter student mail  : ");
   fgets(s.s_mail,sizeof(s.s_mail),stdin);
   clean_newLine(s.s_mail);

   printf("Enter  student phone (format 555-####) : ");
    fgets(s.s_phone, sizeof(s.s_phone), stdin);
    clean_newLine(s.s_phone);

   printf("Enter enrollment year : ");
while (scanf("%d", &s.enrollment_year) != 1) {
        clear_input_buffer();
        printf("Invalid year. Try again: ");
    }

   clear_input_buffer();
   printf("Enter student major : ");
   fgets(s.major, sizeof(s.major), stdin);
   clean_newLine(s.major);
      s.gpa = 0 ;
//GPA starts at 0 when created (updated after grades are recorded).

      err = validateStudent(&s);
      if(err!=  ERROR_OK){
         printf("%s\n", validation_error_msg(err));
         printf("Pleas try again \n");
      }

   }while(err != ERROR_OK);
return s ;

}


// Prints all fields of a student record.
void showStudent(Students *s){
if (!s) return;
    printf("Student information:\n");
    printf("Student ID:       %d\n", s->student_id);
    printf("First Name:       %s\n", s->first_name);
    printf("Last Name:        %s\n", s->last_name);
    printf("Mail:             %s\n", s->s_mail);
    printf("Phone:            %s\n", s->s_phone);
    printf("Enrollment year:  %d\n", s->enrollment_year);
    printf("Major:            %s\n", s->major);
    printf("Gpa:              %.2f\n", s->gpa);
}

//Frees memory allocated for student array.
void freeStudent(){
   free(arrStudents);
   arrStudents = NULL ;
   student_count = 0 ;
   maxStudent = 0 ;

}

//Searches student by student_id (linear search).
Students* search_student_id(const int  studentid ){
      validateid(studentid);
            for(int i = 0 ; i<student_count ; i++){
               if(arrStudents[i].student_id == studentid){
                  return  &arrStudents[i];
               }
            }
          
               return NULL ;
}

// Searches student by first and last name (exact strcmp).
Students* search_student_name(char* first_name , char* last_name){

      for(int i = 0 ; i<student_count ; i++ ){
         if(strcmp(arrStudents[i].first_name, first_name) == 0  &&  strcmp(arrStudents[i].last_name , last_name) == 0){
            return &arrStudents[i];
         }
      }
         

return NULL ;

} 


// Searches student by email.
Students* search_student_email(const char *mail){
   for(int i = 0 ; i<student_count ; i++){
      if(strcmp(arrStudents[i].s_mail , mail) == 0){
         return &arrStudents[i];
      }
   }

      return NULL ;
}



//Adds a new student record.
int addStudent(const Students* s){
         if(!arrStudents || !s){printf("arrStudents or s  is NULL..\n");return 0 ;}
         
         if (!ensureStudentCapacity()) return 0;
         if(search_student_email(s->s_mail) != NULL){ printf("Please enter diffirent student mail \n"); return 0 ;}
         if(search_student_id(s->student_id) != NULL){ printf("Please enter diffirent student id \n"); return 0 ;}   
   
int err = validateStudent(s);
if(err != ERROR_OK) return 0; 
         
         arrStudents[student_count] = *s ;


   student_count++;
   return 1 ;

}


//* Saves student data to a CSV file.
 //* Writes header line and all student records.
int save_student(const char* path){
      FILE* fp = fopen(path,"w");
   if(!fp){
      printf("File could not open\n");
      return 0 ;
   }
int i = 0;

    fprintf(fp, "student_id,first_name,last_name,email,phone,enrollment_year,major,gpa\n");
      for( i = 0 ; i<student_count ; i++){
         fprintf(fp,"%d,%s,%s,%s,%s,%d,%s,%.2lf\n",
            arrStudents[i].student_id,
            arrStudents[i].first_name,
            arrStudents[i].last_name,
            arrStudents[i].s_mail,
            arrStudents[i].s_phone,
            arrStudents[i].enrollment_year,
            arrStudents[i].major,
            arrStudents[i].gpa);
      } // student_id,first_name,last_name,email,phone,enrollment_year,major,gpa
      fclose(fp);
      return 1 ;
      }

  //   * Deletes a student from the system.
// * - deletes student's grade records
 //* - deletes student's enrollment records
 //* Then removes student from array (swap with last element).
 
int delete_Student(int student_id){

   if(!validateid(student_id)){
      printf("Student ID must be YEAR### (e.g., 2020001) and year must be 2020-2024.\n");
      return 0 ;
   }

   if(!arrStudents) { printf("Error. Plese load students. \n");return 0 ;}
   Students *a = search_student_id(student_id);
if(!a){
   printf("No such a student . \n");
   return 0 ;
}

int deletedGrade = deleteGradeBy_StudentId(student_id);
int deletedEnrollmet = deleteEnrollment_Student(student_id);
int found = 0 ;
   for(int i = 0 ; i<student_count ; i++){
      if(arrStudents[i].student_id == student_id){
            arrStudents[i] = arrStudents[student_count -1 ];
            student_count-- ;
            found = 1 ;
            break;
      }
   }
   if(!found){     
            printf("Student could not be deleted.\n");
            return 0 ;
         }

printf("Deleted student. Remove %d  grades , and %d enrollment record\n",deletedGrade,deletedEnrollmet);
return 1 ;
}

 //* Updates student information.
 //* ID does not change, new data is read via createStudentforUpdate().
 //* Checks email uniqueness.



//Keep the ID and preserve current GPA.

int updateStudent(int id){

      if(!arrStudents ){
         printf("Students not loaded .\n");
         return 0 ;
      }
Students* s = search_student_id(id);

if(s == NULL){
               printf("No such student :"); return 0  ;
}
   printf("\n-----Current student detais: -----\n");
   showStudent(s);
   printf("\n-----Enter  NEW data ( ID will Not change)-----\n");
   Students newData = createStudentforUpdate();
Students* byEmail = search_student_email(newData.s_mail);
       if (byEmail != NULL && byEmail->student_id != id) {
        printf(" This email is already used by another student.\n");
        return 0;
    }

newData.student_id = id ;
newData.gpa = s->gpa; 

int err = validateStudent(&newData);
if(err != ERROR_OK){
printf("%s\n", validation_error_msg(err));
return 0;
    }


*s = newData ;
    printf(" Student updated successfully \n");
    return 1 ;


}


/*

 Validates student fields and returns an error code.
  Checks:
  - student_id must be 7 digits and year range 2020-2024
  - names cannot be empty and must pass is_valid_name
  - email must contain '@' and '@university.edu'
  - phone format: 555-#### (8 chars)
  - enrollment_year range 1900-2100
  - gpa range 0.0-4.0
  - major must be one of allowed majors
 */
int validateStudent(const Students *s){
             if(s == NULL){ return  ERROR_NULL;}
      int year = s->student_id / 1000 ;
      if(s->student_id <1000000 || s->student_id >9999999 )return  ERROR_STUDENT_ID ;
         if(year <2020 || year >2024 ){ return ERROR_STUDENT_ID ;}
      if(strlen(s->first_name)  == 0  || strlen(s->last_name)== 0 ) return ERROR_STUDENT_NAME;
   if (!is_valid_name(s->first_name) || !is_valid_name(s->last_name))
    return ERROR_STUDENT_NAME;
      if (strchr(s->s_mail, '@') == NULL) return ERROR_STUDENT_MAIL;
       if (strstr(s->s_mail, "@university.edu") == NULL) {
        return ERROR_STUDENT_MAIL ;
    }
        if (strlen(s->s_phone) != 8) {  
                 return ERROR_STUDENT_PHONE;
    }
// Burada ilk  3 index 5 5 5 - olmalı onun kontrolu.
      if(s->s_phone[0] != '5' 
|| s->s_phone[1] != '5' || s->s_phone[2] != '5' || s->s_phone[3] != '-')return ERROR_STUDENT_PHONE ; 
         //Son 4 index rakam olmalı.
for (int i = 4; i <= 7; i++) {  
    if (!isdigit((unsigned char)s->s_phone[i])) {
        return ERROR_STUDENT_PHONE;}
   }
   if (s->enrollment_year < 1900 || s->enrollment_year > 2100) {
        return ERROR_STUDENT_YEAR; }
          if (s->gpa < 0.0 || s->gpa > 4.0) {
        return ERROR_STUDENT_GPA;
    }    
    if (strcmp("Computer Engineering", s->major) != 0 &&
    strcmp("Industrial Engineering", s->major) != 0 &&
    strcmp("Mechanical Engineering", s->major) != 0 &&
    strcmp("Electrical Engineering", s->major) != 0 &&
    strcmp("Computer Science", s->major) != 0){
    return ERROR_STUDENT_MAJOR;
}
return ERROR_OK;

   }

   //Converts student validation error codes to user-friendly messages.
 // Used to explain errors returned by validateStudent.
const char* validation_error_msg(ErrorCode e) {
    switch (e) {
         case  ERROR_NULL : return " Student is null try again. \n.";
        case ERROR_OK: return "OK";
        case ERROR_STUDENT_ID: return "Student ID must be YEAR### (e.g., 2020001) and year must be 2020-2024.";
        case ERROR_STUDENT_NAME: return "First/Last name cannot be empty and must be invalid..";
        case ERROR_STUDENT_MAIL: return "Email must be a university email (must include @university.edu).";
        case ERROR_STUDENT_PHONE: return "Phone must be exactly in format 555-#### (e.g., 555-0101).";
        case ERROR_STUDENT_YEAR: return "Enrollment year must be between 1900 and 2100.";
        case ERROR_STUDENT_GPA: return "GPA must be between 0.0 and 4.0.";
        case ERROR_STUDENT_MAJOR: return "Major must be one of allowed majors (e.g., Computer Engineering).";
        default: return "Unknown validation error.";
    }
}



//Calculates a student's GPA (weighted by course credits).
 //* Only enrollments with status "Completed" and a recorded grade are counted.
double calculate_GPA(int student_id){

      if(!arrGrades || !arrStudents || !arrEnrollments ) return 0 ;
      if(validateid(student_id) != 1){
         printf("Student ID must be YEAR### (e.g., 2020001) and year must be 2020-2024.");
return 0 ;
      }

double sum_credit= 0 ;
int total_credi = 0;
         int i = 0 , j = 0;
            for(i = 0 ; i<grade_count; i++){
               if(arrGrades[i].student_id !=student_id) continue; ;
            
               int completed = 0;
               for(j = 0 ;j<enrollment_count ; j++){
                  if(arrEnrollments[j].enrollment_id == arrGrades[i].enrollment_id){
                  if (strcmp(arrEnrollments[j].status, "Completed") == 0) {
                     completed = 1;
                }
                break;
               }
            }
      if(!completed) continue; 

      courses* c = search_course_id((arrGrades[i].course_id));
      if(!c) continue; 
             double gp = arrGrades[i].numeric_grade / 25.0; 
            sum_credit += gp * c->credit ;
            total_credi += c->credit ; 

}
         if(total_credi == 0)return 0 ;

   
return sum_credit/(double)total_credi;

}





//Validates student ID format (YEAR### and year range 2020-2024).
int validateid(int student_id){
        

      if(student_id <1000000 || student_id >9999999 )return 0 ;
    int year = student_id / 1000 ;
         if(year <2020 || year >2024 ){ return 0 ;}
      
         return 1 ;
      }

//Prints the student's academic transcript.
 //* Student enrollments are sorted by semester order.
 //* Lists courses per semester and calculates semester GPA.
 //* Calculates cumulative GPA at the end.
void displayStudentTranskript(int student_id){

Students *s = search_student_id(student_id);
        if(!s){
            printf("Student not found.  ID : %d \n",student_id);
            return;
        }


        if(!arrEnrollments || !arrGrades || !arrStudents){
                printf("\nGrades , Enrollment and students not loaded .\n");
                return ;
            }


//Copy the student's enrollments into a separate list.
enrollment *list = (enrollment*)malloc(sizeof(enrollment) * enrollment_count);
if(!list){
    printf("Memory error . \n");
    return;
}

int n = 0 ;
int i = 0 ;
for(i = 0; i<enrollment_count ; i++){

    if(arrEnrollments[i].student_id !=student_id) continue;
    if(!validateSemester(arrEnrollments[i].semester)) continue; ;
    list[n++] = arrEnrollments[i];
}

if(n== 0){
    printf("Student has no valid Enrollment \n");
    free(list);    return ;
}

//sortEnrollment is used to order by semester (and then ID).
sortEnrollment(list,n);

//Print transcript generation date.
char gen_date[64];
time_t now = time(NULL);
struct tm *ti = localtime(&now);
strftime(gen_date, sizeof(gen_date), "%b %d %Y", ti);
printf("\n");
printf("================================================================================\n");
printf("                          ACADEMIC TRANSCRIPT                                   \n");
printf("================================================================================\n");
printf("Student: %s %s (ID:%d)\n", s->first_name, s->last_name, s->student_id);
printf("Major: %s\n", s->major);
printf("Generated: %s\n", gen_date);
printf("================================================================================\n\n");



char curentSemester[20] = "";

//semester gpa
int sem_credit = 0 ;
double semQualty = 0 ;

//Cumulative gpa
int Cum_credi = 0 ;
double cumQualty = 0 ;

for(i = 0 ; i<n  ; i++){

    enrollment *e = &list[i];


if(curentSemester[0] == '\0' || strcmp(curentSemester, e->semester ) != 0 ){

        if(curentSemester[0]!= '\0'){
  printf("--------------------------------------------------------------------------------\n");
double sem_gpa = (sem_credit == 0) ? 0.0 : (semQualty / (double)sem_credit);
printf("Semester GPA : %.2f\n\n",sem_gpa);
        }
        // New semester.
strcpy(curentSemester,e->semester);
sem_credit = 0 ;
semQualty = 0 ;

printf("Semester : %s\n",curentSemester);

printf("%-11s  %-38s  %-5s  %-6s  %-7s  %-12s\n",
"Course Code", "Course Name", "Cred", "Ltr", "Num", "Status");
printf("--------------------------------------------------------------------------------\n");
}


//If Completed, try to find grade; otherwise show "-" placeholders.
courses *c = search_course_id(e->course_id);
const char *code = c ? c->course_code : "N/A";
const char *name = c ? c->course_name : "Unknown";
int credit = c ? c->credit : 0;


grades *g = NULL ;
const char *ltr = "-";
double num = -1.0;



if(strcmp(e->status,"Completed")== 0){

    g = seachGrade_byEnrollmentid(e->enrollment_id);
    if(g){
        ltr = g->letter_grade;
        num = g->numeric_grade;
    }

}//If numeric grade exists, print it; otherwise print "-"
if (num >= 0.0) {
printf("%-11s  %-38.38s  %-5d  %-6s  %-7.1f  %-12s\n",
code, name, credit, ltr, num, e->status);
 } 
 else {
    printf("%-11s  %-38.38s  %-5d  %-6s  %-7s  %-12s\n",
                   code, name, credit, ltr, "-", e->status);
        }




//GPA : Completed + valid letter +credit 

if(g&&strcmp(e->status,"Completed") == 0){
 double gp = g->numeric_grade / 25.0;

            if(gp>=0 && credit>0){
                sem_credit = sem_credit + credit;
                semQualty = semQualty  + gp * (double)credit;
                
                Cum_credi = Cum_credi +  credit;
                cumQualty = cumQualty + gp *  (double)credit;
            }
}

}


double cum_gpa = 0.0;
if (Cum_credi != 0)
    cum_gpa = cumQualty / (double)Cum_credi;

//Print GPA for the last semester as well.
if(curentSemester[0] != '\0') {
    printf("--------------------------------------------------------------------------------\n");
    double sem_gpa = 0.0;
    if (sem_credit != 0) sem_gpa = semQualty / (double)sem_credit;
    printf("Semester GPA : %.2f\n\n", sem_gpa);
}

printf("================================================================================\n");
    printf("                              SUMMARY                                           \n");
    printf("================================================================================\n");
    printf("Cumulative GPA: %.2f\n", cum_gpa);
    printf("================================================================================\n");

    free(list);
}
