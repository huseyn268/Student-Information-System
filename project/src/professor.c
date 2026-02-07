#include<stdio.h>
#include<stdlib.h>
#include"utils.h"
#include<string.h>
#include"professor.h"
#include <ctype.h>
#include"enrollment.h"
#include"grade.h"
professor* arrProf = NULL ; // Dynamic array that stores all professor records.
int prof_count = 0 ;//Number of professors currently stored in memory.
int maxProf = 30 ; //Initial capacity for the professor array.

//Ensures the professor array has enough capacity.
 //* If full, grows the array by doubling its size (realloc).
static int ensureProfCapacity()
{
    if (prof_count< maxProf) return 1;
    int newCap = maxProf * 2;
    professor *tmp = realloc(arrProf, newCap * sizeof(professor));
    if (!tmp) {
        printf("Error: realloc failed for Professor\n");
        return 0;
    }
    arrProf = tmp;
    maxProf = newCap;
    return 1;
}

/*

  Initializes the Professor module.
 Allocates initial memory and resets counters.
 */
void init_Prof(){
    maxProf = 30 ;
    arrProf = (professor*)malloc(maxProf*sizeof(professor));
    prof_count = 0 ;
    if(!arrProf){
        printf("Memory allocation is failed ");
        exit(1);
    }
}


//Loads professor records from a CSV file.
int loadPorf(const char* path ){

      if (!arrProf) {
    printf("arrProf is NULL. Call init_Prof first.\n");
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
    //Update prof_count during loading so capacity check works correctly.
             prof_count = i ;
          if (!ensureProfCapacity()) {
            fclose(fp);
            return -1;
        }

    
        int a = sscanf(line,"%d,%49[^,],%49[^,],%49[^,],%49[^,],%49[^,],%49[^,],%49[^,\n]",
            &arrProf[i].prof_id,
            arrProf[i].first_name,
            arrProf[i].last_name,
            arrProf[i].p_mail,
            arrProf[i].p_phone,
            arrProf[i].department,
            arrProf[i].title,
            arrProf[i].office
        );
        if(a==8) {
clean_newLine(arrProf[i].first_name);
clean_newLine(arrProf[i].last_name);
clean_newLine(arrProf[i].p_mail);
clean_newLine(arrProf[i].p_phone);
clean_newLine(arrProf[i].department);
clean_newLine(arrProf[i].title);
clean_newLine(arrProf[i].office);
       i++;  }
}
      prof_count = i ;

fclose(fp);
 return i ;

}


//* Creates a professor struct by reading user input.
 //* If askt_id=1, ID is asked from user.
 //* If askt_id=0, update_id is used (updateProf scenario).

professor createProf(int askt_id , int update_id){
    professor p ;
    int error;
   clear_input_buffer();
    do{
        if(askt_id){
                    printf("Enter id : ");
                    while(scanf("%d",&p.prof_id)!= 1){
            clear_input_buffer();
                printf("Invalid input. Enter id again :  \n");
                    }
                    clear_input_buffer();
       
                }
     else{
        p.prof_id = update_id ;
     }
        printf("Enter first name :");
        fgets(p.first_name,sizeof(p.first_name),stdin);
        clean_newLine(p.first_name);

        printf("Enter last name ");
        fgets(p.last_name,sizeof(p.last_name),stdin);
        clean_newLine(p.last_name);
        

        printf("Enter mail :");
        fgets(p.p_mail,sizeof(p.p_mail),stdin);
        clean_newLine(p.p_mail);
     
        printf("Enter phone :");
        fgets(p.p_phone,sizeof(p.p_phone),stdin);

        clean_newLine(p.p_phone);
        

        printf("Enter department ");
        fgets(p.department,sizeof(p.department),stdin);
        clean_newLine(p.department);

        printf("Enter title :");
        fgets(p.title,sizeof(p.title),stdin);
        clean_newLine(p.title);

        printf("Enter office :");
        fgets(p.office,sizeof(p.office),stdin);
        clean_newLine(p.office);
        error = validate_prof(&p);
        if(error != PROF_OK){
            printf("%s\n",prof_Error_MSG(error));
        }
    }while(error != PROF_OK);
return p ;
}
//Prints a professor record to the console.
void showProf(professor *p){
    printf("Proffessor information : \n");
    printf("Id :%d \n",p->prof_id);
    printf("Fist Name: %s \n",p->first_name);
    printf("Last Name :%s\n",p->last_name);
    printf("Mail : %s\n",p->p_mail);
    printf("Phone : %s\n",p->p_phone);
    printf("Department : %s\n",p->department);
    printf("Title : %s\n",p->title);
    printf("Office : %s\n",p->office);

}


//Frees memory allocated for professor array.

void freeProf(){
    free(arrProf);
    arrProf = NULL;
    prof_count = 0 ;
    maxProf = 0 ;
}

//Performs a linear search by prof_id.
professor* search_prof_id(const int prof_id ){
       int i = 0 ;
            for( i = 0 ; i<prof_count ; i++){
               if(arrProf[i].prof_id == prof_id){
                  return  &arrProf[i];
               }
            }
          
               return NULL ;
}

//Searches professor by first and last name (exact strcmp).
professor* search_prof_name(const char* first_name ,const  char* last_name){
int i = 0 ;
      for(i = 0 ; i<prof_count ; i++ ){
         if(strcmp(arrProf[i].first_name, first_name) == 0  &&  strcmp(arrProf[i].last_name , last_name) == 0){
            return &arrProf[i];
         }
      }

return NULL ;

} 

//Searches professor by email.
professor* search_prof_email(const char *mail){
   for(int i = 0 ; i<prof_count ; i++){
      if(strcmp(arrProf[i].p_mail , mail) == 0){
         return &arrProf[i];
      }
   }
          

      return NULL ;
}

//Adds a new professor record.
int addProf(const professor* s){
         if(!arrProf || !s){printf("arrProf or s  is NULL..\n");return 0 ;}
         
        if (!ensureProfCapacity()) return 0;
        

    Prof_Error err = validate_prof(s);     
    if (err != PROF_OK) {
        printf("%s\n", prof_Error_MSG(err));
        return 0;
    }
                 if(search_prof_id(s->prof_id) != NULL){ printf("Please enter diffirent professor id \n"); return 0 ;}  
          if(search_prof_email(s->p_mail) != NULL){ printf("Please enter diffirent professor mail \n"); return 0 ;}
 
arrProf[prof_count] = *s ;
prof_count++;
   return 1 ;

}

//Saves all professor records into a CSV file.
 //* Writes header line and all records
int save_Prof(const char *path){
     FILE* fp = fopen(path,"w");
    int i = 0 ;

   if(!fp){
      printf("File could not open\n");
      return 0 ;
   }
//Header .
   fprintf(fp,"professor_id,first_name,last_name,email,phone,department,title,office\n");

   for(i = 0 ; i<prof_count ; i++){
    fprintf(fp,"%d,%s,%s,%s,%s,%s,%s,%s\n",  
            arrProf[i].prof_id,
            arrProf[i].first_name,
            arrProf[i].last_name,
            arrProf[i].p_mail,
            arrProf[i].p_phone,
            arrProf[i].department,
            arrProf[i].title,
            arrProf[i].office);

   }//professor_id,first_name,last_name,email,phone,department,title,office

   fclose(fp);
   return 1 ;

}




/*

 Deletes a professor.
  If there are enrollments assigned to this professor, warns the user.
  If confirmed:
  - deletes related enrollments
  - related grades are also handled inside deleteEnolmment_Prof
 */
int delete_prof(int prof_id){

   if(!arrProf) return 0 ;

professor *p = search_prof_id(prof_id);
if(!p){
    printf("Professor not found . \n");
    return 0 ;

}

int prof_counter_incourse = countEnrollment_Prof(prof_id);
        if(prof_counter_incourse>0){
            printf("Warninng !! \n");
            printf("if you delete professor , all enrollment assigned to  this professor  %d record deleted."
                "Also grades linked to those enrollments will be also deleted.\n",prof_counter_incourse);
                printf("If you want to change profesor for a course , use Assign courses to professors \n");
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
int removed = deleteEnolmment_Prof(prof_id);
printf("Deleted %d enrollments link to professor %d \n",removed,prof_id);
        }
            


   for(int i = 0 ; i<prof_count ; i++){
      if(arrProf[i].prof_id == prof_id){
            arrProf[i] = arrProf[prof_count -1 ];
            prof_count-- ;
            
            return 1 ;
      }
   }
      return 0 ;

}

// Updates professor information.
int updateProf(int id){

    professor *prof = search_prof_id(id);
        if(prof ==NULL){
            printf("No such a proffesor :"); 
            return 0 ;
        }


        printf("\n------Current proffessor details : ------\n");
        showProf(prof);

    professor newProf = createProf(0,id);


    professor* bymail = search_prof_email(newProf.p_mail);
        if(bymail != NULL && bymail->prof_id != id ){
            printf("This email is already used by another professor\n");
            return 0 ;
        }

*prof = newProf ;
    

        return 1 ;
}

/*
  Validates professor fields and returns an error code.
  Checks:
  - ID range 5000-5999
  - names cannot be empty and must pass is_valid_name
  - email must contain '@' and '@university.edu'
  - phone format: "555-####" (total length 8)
  - department cannot be empty
 - title must be one of allowed values
  - office format must include '-' and have minimum length (e.g., ENG-302)
 */
Prof_Error validate_prof(const professor *prof){
    if(prof == NULL){return ERR_PROF_NULL ;}
    if(prof->prof_id <5000 || prof->prof_id >5999){ return ERR_PROF_ID;}
    if(strlen(prof->first_name)==0 || strlen(prof->last_name)==0 ){
        return ERR_PROF_NAME;
    }  
       if (!is_valid_name(prof->first_name)) return ERR_PROF_NAME;       
      if (!is_valid_name(prof->last_name)) return ERR_PROF_NAME;
      if (strchr(prof->p_mail, '@') == NULL) return ERR_PROF_MAIL;

       if (strstr(prof->p_mail, "@university.edu") == NULL) {
        return ERR_PROF_MAIL ;
    }
 if (strlen(prof->p_phone) != 8) { 
                 return ERR_PROF_PHONE;
    }
      if(prof->p_phone[0] != '5' || prof->p_phone[1] != '5' || prof->p_phone[2] != '5' || prof->p_phone[3] != '-')return ERR_PROF_PHONE ; 
for (int i = 4; i <= 7; i++) {  
    if (!isdigit((unsigned char)prof->p_phone[i])) {
        return ERR_PROF_PHONE;
    }}
if(strlen(prof->department) == 0 ){
    return ERR_PROF_DEPARTMENT;
}
if(strcmp(prof->title,"Professor") != 0 &&
    strcmp(prof->title ,"Associate Professor") != 0 &&
    strcmp(prof->title ,"Assistant Professor") != 0 ) { return ERR_PROF_TITLE ;}
    if(strlen(prof->office) <5 || strchr(prof->office,'-') == NULL)
        return ERR_PROF_OFFICE ;
return PROF_OK ;
}

 //* Converts professor validation error codes to user-friendly messages.
 //* Used to explain errors returned by validate_prof.
const char* prof_Error_MSG(Prof_Error a){

    switch (a)
    {   
    case PROF_OK : return " No Error ";
    case ERR_PROF_NULL : return "Professor data is Null ";
    case ERR_PROF_ID :  return " Invalid Professor ID  , Id must be in range 5000-5999(5###)" ;
    case ERR_PROF_NAME : return "Invalid Professor name , Only alphapetic charcahters are allowed ";
    case  ERR_PROF_MAIL :   return "Invalid Professor mail. Mail must be univesity mail (name@university.edu.)";
    case  ERR_PROF_PHONE :      return "Invalid Professor phone number , Format bust be 555 -####";
    case  ERR_PROF_DEPARTMENT : return "Department can not be empty  ";
    case  ERR_PROF_TITLE :  return " Invalid acedemic title. Allowed values  : Professor , Associate Professor , Assistant Professor ";
    case  ERR_PROF_OFFICE :  return "Invalid office format. ENG-302 ";
     
default :
        return "Unkown professor error .";

    }

}

//Updates professor_id for enrollments of a given course in a given semester.
// * Used by "Assign Course to Professor" menu option.
int assignCourse_Professor(int course_id ,const char *semester,int professor_id){

if (!arrEnrollments || enrollment_count <= 0) {        printf("No enrollments loaded.\n");
        return 0;
 }if (!semester || semester[0] == '\0') {
       printf("Invalid semester.\n");      return 0;
    }


            if(search_prof_id(professor_id) == NULL){
                printf("Error : Professor ID %d ,not found \n",professor_id);
                return 0 ;
            }
int changed  = 0 ;
int i = 0 ;
            for(i = 0 ;i<enrollment_count ; i++){
                if(arrEnrollments[i].course_id == course_id && strcmp(arrEnrollments[i].semester,semester) == 0){
                    arrEnrollments[i].professor_id = professor_id;
                    changed++;
                }
            }
if(changed == 0){
    printf("No enrollment found for course %d,in %s\n",course_id,semester);
    return 0 ;
}

printf("OK: Assigned course %d in %s  to professor  id : %d (updated %d enrollments).\n",
           course_id, semester, professor_id, changed);

           return 1 ;

}

