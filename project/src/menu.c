#include"menu.h"
#include<stdio.h>
#include"student.h"
#include"courses.h"
#include"professor.h"
#include"utils.h"
#include"enrollment.h"
#include"grade.h"
#include <float.h>
#include <string.h> 

//Reads a numeric menu choice from the user.
int inputMenuChoice(void)
{
    int choice;

    printf("Enter your choice: ");
    if (scanf("%d", &choice) != 1) {
        clear_input_buffer();
        printf("Invalid input. Please enter a number.\n");
        return -1;
    }
    clear_input_buffer();

    return choice;   
}


//Reads a course ID with basic validation.
int intputValidcourseID(){
    int id ;
    int ok  = 0;
         do {
        printf("Enter course ID :");

        if (scanf("%d", &id) != 1) {
            printf("Invalid input. Please enter numbers only.\n");
          clear_input_buffer();
            continue;
        }
        clear_input_buffer();
           if(id<= 0){
            printf("Invalid course id. Please try again :");
            continue;
        }
        else {
            ok = 1;
        }

    } while (!ok);
    return id ;
}

//Reads a student ID with validation.
 //* Uses validateid() to check YEAR### format and year range (2020-2024).
int inputValidStudentID(void)
{
    int student_id;
    int ok = 0;

    do {
        printf("Enter Student ID (YEAR###, e.g. 2020001): ");

        if (scanf("%d", &student_id) != 1) {
            printf("Invalid input. Please enter numbers only.\n");
            clear_input_buffer();
            continue;
        }

        if (!validateid(student_id)) {
            printf("Invalid Student ID! Must be YEAR### and year between 2020-2024.\n");
        } else {
            ok = 1;
        }

    } while (!ok);

    return student_id;
}

 // Reads and validates semester input.
const char *inputValidateSemester(void)
{
    static char semester[10];

    while (1) {
        printf("Enter semester (YYYY-FALL / YYYY-SPRING / YYYY-SUMMER): ");

        if (scanf("%9s", semester) != 1) {
            clear_input_buffer();
            printf("Invalid input.\n");
            continue;
        }
        clear_input_buffer();

        if (!validateSemester(semester)) {
            printf("Invalid semester format. Use YYYY-FALL, YYYY-SPRING or YYYY-SUMMER.\n");
            continue;
        }

        return semester;
    }
}

//Reads professor ID in the allowed range (5000-5999).
int inputProf(){
    int id ;

    while(1){

        printf("Enter professor ID (5000-5999) : ");
        if(scanf("%d",&id) != 1){
            clear_input_buffer();
            printf("Invalid input. Please enter numbers only.\n");
            continue;
        }
        clear_input_buffer();

        if(id<5000 || id>5999){
            printf("Invalid professor ID. Must be between 5000 and 5999\n");
            continue;
        } 
        return id ;
    }
}


// Runs the Student Management submenu.
 //* Handles add/delete/update/search/transcript/GPA operations for students.
void printStudentManagment(){
     int choice ;
    do{
    printf("\n================ Student Management ================\n");
    printf(" 1. Add Student\n");
    printf(" 2. Edit Student Infortmation \n");
    printf(" 3. Delete Student \n");
    printf(" 4. Search for student\n");
    printf(" 5. Display student details and transcript\n");
    printf(" 6. Calculate and Display student Gpa \n");
    printf(" 0. Back\n");
    printf("====================================================\n");
    
            choice = inputMenuChoice();
            if(choice == -1)continue;
    switch (choice)
    {
        case 1: {
                if(!arrStudents){
                    printf("Students not initialized. Please load students first.\n");
                    break ;
                    }
            Students s = createStudent();
            if(addStudent(&s)){
                printf("Student added succesfullt \n");
            }else{
                printf("Failed to add student. Returning  to menu\n");
            }
            break;
        }
        case 2 :
                {           
                    if (!arrStudents) {
                    printf("Students not initialized. Please load students first.\n");
                    break;}
                    int student_id  = inputValidStudentID();         
                    updateStudent(student_id);
                    break ;
                }

        case 3 :    
                    {
                    if (!arrStudents) {
                    printf("Students not initialized. Please load students first.\n");
                    break;}
                    int student_id = inputValidStudentID();
                    delete_Student(student_id);
                    break ;
                    }
        case 4 :    searchStudentMENU();
                     break ;
        

        case 5 :
                   {  if (!arrStudents) {
                    printf("Students not initialized. Please load students first.\n");
                    break;} 
                    int student_id = inputValidStudentID();
                    Students *s = search_student_id(student_id);
                 if(!s) {
                     printf("There is no such student registered in the system. \n");  break ;}  
                    showStudent(s);
                    displayStudentTranskript(student_id);
                    break;
                }
        case 6 :
                  {  if(!arrStudents) {
                    printf("Students not initialized. Please load students first.\n");
                    break;}
                     int student_id = inputValidStudentID();
                    Students *s = search_student_id(student_id);
                 if(!s) {
                     printf("There is no such student registered in the system. \n");  break ;}  
                    
                     double gpa = calculate_GPA(student_id);
                    printf("Student gpa is %.2f\n", gpa);
                    break;
                    }
        case 0 :
                    break; 
                    default :
                    printf("Please enter valid choice . \n");
                    break;
                
                }



}while(choice != 0 );
}

 //* Student search submenu.
 //* Allows search by ID, full name, or email.
void searchStudentMENU(){
  int ch;

    do{
printf("\n----------Search Student----------\n");
printf(" 1. Search by ID\n");
printf(" 2. Search by Student Name \n");
printf(" 3. Search by Student Mail\n ");
printf(" 0. Back\n");
                 ch = inputMenuChoice();
                if(ch == -1)continue;
                        switch (ch)
                        {
                        case 1 : {

                            int student_id  = inputValidStudentID();
                            Students *s = search_student_id(student_id);
                            if(!s)  printf("Student not found. \n");
                            else    showStudent(s);
                            break;
                        }
                        case 2 :{
                                char firstN[30];
                                char lastN[30];
                        printf("Enter first name: ");
                         fgets(firstN, sizeof(firstN), stdin);
                        clean_newLine(firstN);

                        printf("Enter last name: ");
                        fgets(lastN, sizeof(lastN), stdin);
                        clean_newLine(lastN);

                            Students *s = search_student_name(firstN,lastN);
                                  if(!s)    printf("Student not found. \n");

                            else    showStudent(s);
                            break;
                        }
                            case 3 :{
                                    char mail[40];
                                     printf("Enter mail: ");
                                    fgets(mail, sizeof(mail), stdin);
                                        clean_newLine(mail);
                                        if(strchr(mail, '@') == NULL ||  strstr(mail, "@university.edu") == NULL){
                                                printf("Email must be a university email (must include @university.edu).\n");
                                                break ;
                                        }
                                     

                              Students *s = search_student_email(mail);
                              if(!s) {
                                printf("There is no such student registered in the system. \n");  break ;}          
                                else showStudent(s);
                                break ;
                            }
                            case 0:
                            printf("Returning...\n");
                              break;
                        default :
                        printf("Invalid choice \n");
                        }

}while(ch != 0 );

}


//Runs the Course Management submenu.
 //* Handles add/update/delete/search/capacity/prerequisite/roster for courses.
 
void printCourseManagment(){    

    int select ;
    do{
    printf("\n================ Course Management ================\n");
    printf(" 1. Add Course \n");
    printf(" 2. Edit Course İnformation \n ");
    printf(" 3. Delete Course\n");
    printf(" 4. Search for courses\n");
    printf(" 5. Set Course capacity\n");
    printf(" 6. Assign Prerquistis\n");
    printf(" 7. Display Course Roster\n");
    printf(" 0. Back\n");
    printf("====================================================\n");

        select =inputMenuChoice();
        if(select == -1)continue;
        switch (select)
        {
        case  1 :{
                  if(!arrCourses){
                    printf("Courses not initialized. Please load courses first.\n");
                    break ;
                    }
                    courses c = createCourse(1);
                    if(addCourse(&c)){
                        printf("Courses added succesfully \n");
                    }
                    else{
                    printf("Failed to add courses. Returning  to menu\n");
                    }
                    break ;

        } 
        case 2 :
               
                    {
                     if(!arrCourses){
                    printf("Courses not initialized. Please load courses first.\n");
                    break ;
                    
                    } 
                    int id = intputValidcourseID();
                        
                    if(updateCourse(id)){
                        printf("Course updated succesfully \n");
                    }
                    else{
                        printf("Failed to update course. Returning menu \n");
                    }
                    break; 
               
                }
        
                
                case 3 : 
                            {
                                 if(!arrCourses){
                    printf("Courses not initialized. Please load courses first.\n");
                    break ;
                    }
                                int id  = intputValidcourseID();
                            if(delete_Course(id)){
                                printf("Course deleted succesfully \n");
                            }
                            else{
                                printf("Failed to delete course.Returning menu \n");
                            }
                            break;
                            }
                            
                case 4 :{
                             if(!arrCourses){
                    printf("Courses not initialized. Please load courses first.\n");
                    break ;
                    }   
                    searchCourseMenu();
                    break;
                }
            
                case 5 : {
                    if(!arrCourses){
                    printf("Courses not initialized. Please load courses first.\n");
                    break ;      }  
                    int capasite ;
                            int id = intputValidcourseID();
                            printf("Enter course capacity\n ");
                                    if (scanf("%d",&capasite) != 1) {
                                            clear_input_buffer();
                                            printf("Invalid capacity.\n");
                                            break;
                                            }
                                    clear_input_buffer();
                                    if(setCourse(id,capasite)){
                                            printf("Course capacity updated susccessfully \n");
                                    }
                                    else{
                                        printf("Failed to set course capasity\n");
                                    }
                                    break;
                            
                                }
                    case 6 : {

                               if(!arrCourses){
                                 printf("Courses not initialized. Please load courses first.\n");
                                break ; }  

                            char prequistis[30];
                            int id = intputValidcourseID();
                            printf("Enter prerquistis :(or 0 for none)");
                            fgets(prequistis, sizeof(prequistis), stdin);
                            clean_newLine(prequistis);
                            if(addPrerquistis(id,prequistis)){
                                printf("Add prequistis is succesfully \n");
                            }
                            else{
                                printf("Failed to prequistis . Retuning menu \n");
                            }
                            break;
                    }
                    case 7 :
                                {
                                       if(!arrCourses){
                                printf("Courses not initialized. Please load courses first.\n");
                                        break ;      }  

                                int id = intputValidcourseID();
                                const char *semester = inputValidateSemester();
                                displayCourseRoster(id, semester);
                                break;
                                }     
                                case 0 :
                                break;
                                default :
                                    printf("Please enter valid choice \n");
                                    break;
                            }
    }while(select != 0 );

}


//Course search submenu.
 //* Allows search by ID, course name, or course code.
void searchCourseMenu(){
 int ch;

    do{
printf("\n----------Search Course----------\n");
printf(" 1. Search by ID\n");
printf(" 2. Search by  Name \n");
printf(" 3. Search by  Course Code\n ");
printf(" 0. Back\n");
                 ch = inputMenuChoice();
                if(ch == -1)continue;
                        switch (ch)
                        {
                        case 1 : {

                            int id = intputValidcourseID();
                            courses *c = search_course_id(id);
                            if(!c)  printf("Course not found. \n");
                            else    showCourse(c);
                            break;
                        }
                        case 2 :{
                                char course[40];;
                        printf("Enter course name: ");
                         fgets(course, sizeof(course), stdin);
                        clean_newLine(course);


                            courses *c = search_course_name(course);
                                  if(!c)    
                                  printf("Course not found. \n");
                                    else  showCourse(c);
                            break;
                        }
                            case 3 :{
                                        char code[10];
                                 printf("Enter course code : ");
                                scanf("%9s",code);
                                clear_input_buffer();
                             courses *c = search_course_code(code);
                             if(!c){
                                printf("Course not found \n");
                             } 
                             else{
                                showCourse(c);                          
                               }
                            break; 
                        }
                        case 0 :
                                break;
                        default :
                                printf("Please enter valid choice :\n");
                                break;
                    }
}while(ch != 0 );


}



 //Runs the Professor Management submenu.
 //* Handles professor CRUD operations and course load reporting.
void printProfessorManagment(){
    int choice ;

    do{
    printf("\n================ Professor Management ================\n");
    printf(" 1. Add Professor\n");
    printf(" 2. Edit Professor Information\n");
    printf(" 3. Delete Professor \n");
    printf(" 4. Search for professor\n");
    printf(" 5. Assign Course to Professor \n");
    printf(" 6. Display Professor Course Load\n");
    printf(" 0. Back \n");
    printf("====================================================\n");
            
            choice = inputMenuChoice();
            if(choice ==-1)continue;

                switch(choice){

                    case 1: {
                            if(!arrProf){
                    printf("Professors not initialized. Please load professor first.\n");
                    break ;
                    }
                    professor p = createProf(1,0);
                    if(addProf(&p)){
                        printf(" Professor added  succesfully \n");
                    }
                    else{
                        printf("Failed to add professor. \n");
                    }
                    
                    break;

                    }

                    case 2: {
                    if(!arrProf){
                    printf("Professors not initialized. Please load professor first.\n");
                    break ;
                    }
                    int prof_id = inputProf();
                    if(updateProf(prof_id)){
                        printf("Professor updated succesfully\n");
                    }else{
                        printf("Failed to updated professor .Returning menu\n");
                    }
                    break;

                    }
                    case 3: 
                    {
                                if(!arrProf){
                                printf("Professors not initialized. Please load professor first.\n");
                                break ;
                                }
                            int prof_id = inputProf();
                            if(delete_prof(prof_id)){
                                printf("Professor %d deleted succesfully\n",prof_id);
                            }
                            else{
                                printf("Failed to delete professor .Returning menu\n");
                            }
                            break;




                    }
                    case 4 :{
                                       if(!arrProf){
                    printf("Professors not initialized. Please load professor first.\n");
                    break ;}
                        searchProfMENU();
                                        break;
                    

                    }
                    case 5 :{
                                       if(!arrProf){
                            printf("Professors not initialized. Please load professor first.\n");
                                break ;
                                    }
                                    int prof_id = inputProf();
                                    int course_id = intputValidcourseID();
                                    const char *semester = inputValidateSemester();
                                    if(assignCourse_Professor(course_id,semester,prof_id)){
                                        printf("Professor assign the course succesfully\n");
                                    }else{
                                        printf("Failed to assign professor.Returning menu \n");
                                    }
                                    break;
                    }   
                    case 6 :
                    {
                    if(!arrProf){
                    printf("Professors not initialized. Please load professor first.\n");
                    break ;
                    }   
                       if(!arrEnrollments){
                    printf("Enrollment not initialized. Please load enrollment first.\n");
                    break ;
                    }   

                    int prof_id = inputProf();
                    printf("1. All semesters\n");
                    printf("2. Specific semester\n");
                    printf("Choose: ");

                    int ch = inputMenuChoice();
                        if(ch == -1) break;

                    if(ch == 1){
                        displayProfesorCourseload(prof_id, NULL);
                    }       
                    else if(ch == 2){
                    const char *semester = inputValidateSemester();
                    displayProfesorCourseload(prof_id, semester);
                        }
                    else{
                        printf("Invalid choice.\n");
                    }

                   break;
                    }

                    case 0 :
                    break;

                    default :
                    printf("Please enter valid choice \n");
                }
    }while(choice!= 0);
}


//Professor search submenu.
 //* Allows search by ID, full name, or email.
void searchProfMENU(){
    int ch ;
    do{
printf("\n----------Search Professor----------\n");
printf(" 1. Search by ID\n");
printf(" 2. Search by Name \n");
printf(" 3. Search by  Mail\n ");
printf(" 0. Back\n");
        ch = inputMenuChoice();
        if(ch == -1)continue;

            switch (ch)
            {
            case 1:{
                    int id = inputProf();
                    professor *p = search_prof_id(id);
                    if(!p)printf("No such a professor.\n");
                    else showProf(p);
                    break;
            }
            case 2 :
                    {
                    char firstN[30];
                     char lastN[30];
                        printf("Enter first name: ");
                        if (!fgets(firstN, sizeof(firstN), stdin)) {
                    printf("Input error.\n");
                            break;
}   
        clean_newLine(firstN);

                        printf("Enter last name: ");
                        fgets(lastN, sizeof(lastN), stdin);
                        clean_newLine(lastN);

                            professor *s = search_prof_name(firstN,lastN);
                                  if(!s)    printf("No such a professor\n");
                            else    showProf(s);
                            break;

                    }    
                    
                    case 3 :
                    {
                         char mail[40];
                                     printf("Enter mail: ");
                                    fgets(mail, sizeof(mail), stdin);
                                        clean_newLine(mail);
                                        if(strchr(mail, '@') == NULL ||  strstr(mail, "@university.edu") == NULL){
                                                printf("Email must be a university email (must include @university.edu).\n");
                                                break ;
                                        }
                        professor *p = search_prof_email(mail);
                        if(!p)printf("No such a professor \n");
                        else showProf(p);
                         
                        break;

                    }
                    case 0 :
                    break;

                    default :
                    printf("Plase enter valid choice\n");

            }
}while(ch != 0 );

}

 //Runs the Enrollment Management submenu.
 //* Handles enroll/drop/list student enrollments/course roster operations.

void printEnrollmentManagment(){

        int choice ;
do{
    printf("\n================ Enrollment Management ================\n");
    printf(" 1. Enroll Student in Course\n");
    printf(" 2. Drop Student from Course\n");
    printf(" 3. Display Student Enrollments \n");
    printf(" 4. Display Course Roster \n");
    printf(" 0. Back \n");
    printf("====================================================\n");
    choice = inputMenuChoice();
    if(choice == -1)continue;

    switch (choice)
    {
    case 1:
                {
                    if(!arrEnrollments || !arrStudents || !arrCourses || !arrProf){
    printf("Required data not loaded (enrollments/students/courses/professors).\n");
    break;
}
                    enrollment e = createEnrollment();
                    if(addEnrollment(&e)){
                        printf("Student enrolled  is successful\n");
                    }
                    else{
                        printf("Failed to  enroll student.Returning menu \n");
                    }
                    break;
                }
     case 2: {

                if(!arrEnrollments || !arrStudents || !arrCourses){
                 printf("Required data not loaded (enrollments/students/courses).\n");
             break;
        }

                    int student_id = inputValidStudentID();
                    int course_id = intputValidcourseID();
                    const char *semester = inputValidateSemester();
                    if(dropStudentFromCOurse(student_id,course_id,semester)){
                        printf("Drop student from course is successful\n");
                    }else{
                        printf("Failed to drop student from course.Returning menu \n");
                    
                    }
                    break;

     }
     case 3 :
     {
        if(!arrEnrollments){
                    printf("Enrollment not initialized. Please load enrollment first.\n");
                    break ;
                    }   
                        int student_id = inputValidStudentID();
                                if(search_student_id(student_id) == NULL){
                    printf("Student not found. ID: %d\n", student_id);
                    break;
                }

                    printf("1. All semesters\n");
                    printf("2. Specific semester\n");
                    printf("Choose: ");

                    int ch = inputMenuChoice();
                        if(ch == -1) break;

                    if(ch == 1){
                        listStudentEnrollment(student_id, NULL);
                    }       
                    else if(ch == 2){
                    const char *semester = inputValidateSemester();
                    listStudentEnrollment(student_id,semester);
                        }
                    else{
                        printf("Invalid choice.\n");
                    }

                   break;


     }

    case 4 :
                    {
                        if(!arrEnrollments || !arrCourses){
                printf("Required data not loaded (enrollments/courses).\n");
                break;
            }

                    int course_id = intputValidcourseID();
                     const char *semester = inputValidateSemester();
                    displayCourseRoster(course_id,semester);
                    break;
        }
    case 0 : 
    break;
    default :
    printf("Please enter valid choice \n");
         break;
    }


}while(choice!= 0 );
}

//Runs the Grade Management submenu.
 //* Handles add/update/statistics/distribution operations for grades
void printGradetManagment(){
        int ch ;

        do{

    printf("\n================ Grade Management ================\n");
    printf(" 1. Record Grade \n");
    printf(" 2. Update Grade \n");
    printf(" 3. Display Course Grade Statistics \n");
    printf(" 4. Display Grade Distribution \n");
    printf(" 0. Back \n");
    printf("====================================================\n");

        ch = inputMenuChoice();

            switch (ch)
            {
                case 1 : 
                {
                    if(!arrGrades || !arrEnrollments || !arrCourses){
                    printf("Required data not loaded (grades/enrollments/courses).\n");
                             break;}   
                            grades g = createGrade();
                            if(addGrade(&g)){
                                printf("Grade added succesfully grade \n");
                            }else{
                                printf("Failed to record grade Returning menu\n");
                            }
                            break;
                }

                case 2 :
                {
                        if(!arrGrades || !arrEnrollments || !arrCourses){
                            printf("Required data not loaded (grades/enrollments/courses).\n");
                            break; }

 printf("\n--- UPDATE GRADE ---\n");
    printf("Update is performed using ENROLLMENT ID.\n");
    printf("If you don't know the Enrollment ID, go to:\n");
    printf("Enrollment Management -> Display Student Enrollments\n");
    printf("and get the EnrollID from the list.\n\n");

    int enrollment_id ;
    char letter[8];
    double num ;

    printf("Enter Enrollment ID (0 to go back):");
    while (scanf("%d", &enrollment_id) != 1) {
        clear_input_buffer();
        printf("Invalid. Enter Enrollment ID (0 to go back) :  ");
    }
    clear_input_buffer();
                            
if(enrollment_id == 0){
    printf("Returning to Grade Management  menu ...\n");
    break;
}

printf("Enter new letter grade ( A , A- ,B+,B ,B-,C+,C)\n");
    fgets(letter,sizeof(letter),stdin);
    clean_newLine(letter);

    printf("Please enter numeric grade (0-100)\n");
    while(scanf("%lf",&num) != 1){
        clear_input_buffer();
        printf("Invalid . Enter numeric grade :");
    }
clear_input_buffer();


if(updateGradeByEnrollment(enrollment_id,letter,num)){
    printf("Grade updated succesfully\n");
}else{
    printf("Update failed . Please check enrollment Id and try again \n");
}
break;

                        }

                case 3 :
                        {
                        if(!arrGrades || !arrEnrollments){
                        printf("Required data not loaded (grades/enrollments).\n");
                        break;
                    }

            printf("This part shows average/min/max numeric grades for a course.\n");


                    int course_id = intputValidcourseID();
                    const char *semester = inputValidateSemester();
                    double avg,min, max ;
                    courses *c = search_course_id(course_id);
                    if(!c){
                        printf("No such a course. Returning Menu. \n");
                        break;
                    }
                    printf("\n--- COURSE STATISTICS ---\n");

                    if(course_statistics(course_id,semester,&avg,&min,&max)){

                             printf("\nStatistics for Course ID %d (%s)\n", course_id, semester);
                                printf("----------------------------------\n");
                                printf("Average Grade : %.2f\n", avg);
                                printf("Minimum Grade : %.2f\n", min);
                                printf("Maximum Grade : %.2f\n", max);

                    }else{
                        printf("Failed to calculate course statistics.Returning Menu\n");
                    }

                        break;

                            }
                case 4 : {
                 if(!arrGrades || !arrEnrollments || !arrCourses){
                printf("Required data not loaded (grades/enrollments/courses).\n");
                break; }


                     int course_id = intputValidcourseID();
                    const char *semester = inputValidateSemester();
                    courses *c = search_course_id(course_id);
                    if(!c){
                        printf("No such a course. Returning Menu. \n");
                        break;
                    }
            printf("\n--- GRADE DISTRIBUTION ---\n");
            grade_distribution(course_id, semester);

                    break;
                }
                case 0 :
                    break;

                default :
                printf("Please enter valid choice\n");
            }


}while(ch != 0);




}


//Runs the Reports submenu.
 //* Includes transcript, roster, professor course load, and grade reports.
void printReportsManagment(){

        int ch ;
do{
    printf("\n==================== Reports =======================\n");
    printf(" 1. Student Transcript\n");
    printf(" 2. Course Roster\n");
    printf(" 3. Professor Course Load\n");
    printf(" 4. Grade Statistics\n");
    printf(" 0. Back\n");
    printf("====================================================\n");

    ch = inputMenuChoice();
    if(ch == -1)continue;

        switch (ch)
        {
        case 1:
                    {
        if(!arrEnrollments || !arrStudents || !arrCourses || !arrProf){
            printf("Required data not loaded .\n");
             break;
                    }
                    int student_id = inputValidStudentID();
                    displayStudentTranskript(student_id);
                    break;
        }

        case 2 :
                {
 if(!arrEnrollments || !arrStudents || !arrCourses || !arrProf){
            printf("Required data not loaded \n");
             break;
                }
                    int course_id = intputValidcourseID();
                    const char *semester = inputValidateSemester();
                    displayCourseRoster(course_id,semester);
                    break;
                        }



        case 3 : {
            if(!arrEnrollments || !arrStudents || !arrCourses || !arrProf){
            printf("Required data not loaded .\n");
             break;
                }
                    int prof_id = inputProf();
                    printf("1. All semesters\n");
                    printf("2. Specific semester\n");
                    printf("Choose: ");
                    int ch = inputMenuChoice();
                        if(ch == -1) break;

                    if(ch == 1){
                        displayProfesorCourseload(prof_id, NULL);
                    }       
                    else if(ch == 2){
                    const char *semester = inputValidateSemester();
                    displayProfesorCourseload(prof_id,semester);
                        }
                    else{
                        printf("Invalid choice.\n");
                    }

                   break;

        }
        case 4 :
                    {
                        if(!arrGrades || !arrEnrollments){
                        printf("Required data not loaded (grades/enrollments).\n");
                        break;
                    }

            printf("This part shows average/min/max numeric grades for a course.\n");


                    int course_id = intputValidcourseID();
                    const char *semester = inputValidateSemester();
                    double avg,min, max ;
                    courses *c = search_course_id(course_id);
                    if(!c){
                        printf("No such a course. Returning Menu. \n");
                        break;
                    }
                    printf("\n--- COURSE STATISTICS ---\n");

                    if(course_statistics(course_id,semester,&avg,&min,&max)){

                             printf("\nStatistics for Course ID %d (%s)\n", course_id, semester);
                                printf("----------------------------------\n");
                                printf("Average Grade : %.2f\n", avg);
                                printf("Minimum Grade : %.2f\n", min);
                                printf("Maximum Grade : %.2f\n", max);
                             printf("\n--- GRADE DISTRIBUTION ---\n");
                            grade_distribution(course_id, semester);
                    }else{
                        printf("Failed to calculate course statistics.Returning Menu\n");
                    }

                        break;

                            }

                    case 0 :
                    break; 

                    default :
                    printf("Please enter valid choice\n");

}

}while(ch!=0);
}

//Runs the System Management submenu.
 //* Handles load/save/backup operations for all data.
void printSystemManagment(){
    int ch ;
do{
    printf("\n================ System Management ================\n");
    printf(" 1. Load All Data\n");
    printf(" 2. Save All Data\n");
    printf(" 3. Backup Data\n");
    printf(" 0. Back \n");
    printf("====================================================\n");

        ch = inputMenuChoice();
        if(ch == -1)continue; 

            switch(ch){

                case 1 :{
                        if(loadAlldata())
                            printf("All data loaded succesfully \n");
                        else printf("Loaded failed. \n");
                    break;

                }
        
        case 2:
        {
                if(saveAlldata()) printf("All data saved successfully.\n");
                else printf("Save failed: some files could not be saved.\n");
                break;

        }


        case 3:{
    if(backupAllData())
        printf("Backup created successfully.\n");
    else
        printf("Backup failed.\n");
    break;
        }
    case 0 :
    break; 
         default:
    printf("Please enter valid choice \n");


            }
}while(ch != 0);
}


 //* Initializes modules if needed and loads all CSV data.
 //* If any load operation fails, ok is set to 0.

int loadAlldata(){
    int ok = 1 ;

    if(!arrStudents) 
    init_Students();
    
    if(!arrCourses)
    init_Courses();
    
    if(!arrGrades)
    init_Grades();

    if(!arrEnrollments)
    init_Enrollments();

    if(!arrProf)
    init_Prof();

if(loadStudent(studentPath) < 0) ok =0 ;
if(loadCourse(coursePath)<0) ok = 0 ;
if(loadGrades(gradePath)<0) ok = 0 ;
if(loadEnrollment(enrollmentPath)<0) ok = 0 ;
if(loadPorf(profPath)<0) ok = 0 ;

return ok ;

}

//Saves all data into their respective CSV files.
 //* If any save operation fails, ok is set to 0.
int saveAlldata(){
    int ok = 1 ;

    if(save_student(studentPath)<0) ok = 0;
    if(saveCourses(coursePath)<0) ok = 0;
    if(saveGrade(gradePath)<0) ok = 0;
    if(saveEnrollment(enrollmentPath)<0) ok = 0;
    if(save_Prof(profPath)<0) ok = 0;
    return ok;

}


//Creates timestamped backups of all data files under backup/ directory.
 //* Uses copyFile for each file; if any copy fails, ok becomes 0.
int backupAllData(void)
{
    char ts[32];
    makeTimestamp(ts, sizeof(ts));

    char dst[256];
    int ok = 1;

    snprintf(dst, sizeof(dst), "backup/%s_students.csv", ts);
    if(!copyFile("data/students.csv", dst)) ok = 0;

    snprintf(dst, sizeof(dst), "backup/%s_courses.csv", ts);
    if(!copyFile("data/courses.csv", dst)) ok = 0;

    snprintf(dst, sizeof(dst), "backup/%s_professors.csv", ts);
    if(!copyFile("data/professors.csv", dst)) ok = 0;

    snprintf(dst, sizeof(dst), "backup/%s_enrollments.csv", ts);
    if(!copyFile("data/enrollments.csv", dst)) ok = 0;

    snprintf(dst, sizeof(dst), "backup/%s_grades.csv", ts);
    if(!copyFile("data/grades.csv", dst)) ok = 0;

    if(ok) printf("Backup files created under backup/ with timestamp %s\n", ts);
    else   printf("Backup failed for one or more files.\n");

    return ok;
}


 //* Frees all dynamically allocated module data.
 //* Called on exit to avoid memory leaks.
void freeAll(){

        freeCourses();
        freeEnrollment();
        freeGrade();
        freeProf();
        freeStudent();

}

//* Runs the main menu.
 //* Redirects to submenus based on user choice.
 //* On exit, saves data and frees memory.
void mainMenu(){

    int choice; 
        do{   
    printf("\n========================================\n");
    printf("  STUDENT INFORMATION SYSTEM\n");
    printf("========================================\n");
    printf(" 1. Student Management\n");
    printf(" 2. Course Management\n");
    printf(" 3. Professor Management\n");
    printf(" 4. Enrollment Management\n");
    printf(" 5. Grade Management\n");
    printf(" 6. Reports\n");
    printf(" 7. System Options\n");
    printf(" 0. Exit\n");
    printf("----------------------------------------\n");

    choice = inputMenuChoice();
    if(choice == -1)continue;


                switch (choice)
                {
                case 1 :
                        printStudentManagment();
                        break;
                case 2 :
                        printCourseManagment();
                            break;

                case 3 :
                        printProfessorManagment();
                    break;


                case 4 :
                        printEnrollmentManagment();
                        break;

                case 5 :
                        printGradetManagment();
                        break;
                
                case 6 :
                        printReportsManagment();
                        break;
                case 7 :
                        printSystemManagment();
                        break;
                case 0 :
                        saveAlldata();
                        freeAll();
                        printf("Exiting ...\n\n");
                        break;
                default:
                printf("Please ente valid choice :\n");
                    break;

                    }
}while(choice != 0);

}