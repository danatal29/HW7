#include "linked-list.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>


typedef struct list list;
typedef struct node node;

struct grades{
	list *student_list;
};

typedef struct {
	int id;
	char *name;
	list *grade_sheet;
}student_t;

typedef struct {
	char *name;
	int grade;
}course_t;



/**
 * @brief User function, Destroys “student”.
 * @always succeed 
 */
void student_destroy(void *student){
	student_t *student_temp=(student_t*)student;
	list_destroy(student_temp->grade_sheet);
	free(student_temp->name);
	free(student_temp);
}


/**
 * @brief User function, Destroys “course”.
 * @always succeed 
 */
void course_destroy(void *course) {
	course_t *course_temp=(course_t*)course;
	free(course_temp->name);
	free(course_temp);
}


/**
 * @brief User function, clone struct “student” to “clone”.
 * @Returns 0 on success
 * @note Failes if "student" is invalid 
 */
static list* grade_sheet_clone(list *source ,list *dest ){
	if ((source == NULL)||(dest==NULL)){
		return NULL;
	}

	for(node *it = list_begin(source) ; it != NULL ; it=list_next(it)){
		if(!list_push_back(dest,list_get(it))){
			list_destroy(dest);
			return NULL;
		}

		
	}
	return dest;

}


/**
 * @brief User function, clone struct “course” to “clone”.
 * @Returns 0 on success
 * @note Failes if "course" is invalid 
 */
int course_clone(void *course, void **clone){
	if ((course==NULL)||(clone==NULL)){
		return 1;
	}
	course_t *course_temp=(course_t*)course;
	course_t* new_course = (course_t*) malloc (sizeof(course_t));
	if (new_course == NULL){
		return 1;
	}

	new_course->name=(char*)malloc(strlen(course_temp->name)+1);
	if(new_course->name==NULL){
		course_destroy(new_course);
		return 1;
	}
	strcpy(new_course->name,course_temp->name);

	new_course->grade = course_temp->grade;

	*clone=new_course;
	
	return 0;
}



/**
 * @brief User function, clone struct “student” to “clone”.
 * @Returns 0 on success
 * @note Failes if "student" is invalid 
 */
int student_clone(void *student, void **clone) {
	if (student == NULL){
		return 1;
	}
	student_t *student_temp=(student_t*)student;
	student_t* new_student = (student_t*) malloc (sizeof(student_t));
	if (new_student == NULL){
		return 1;
	}
	
	new_student->name=(char*)malloc(strlen(student_temp->name)+1);
	if(new_student->name==NULL){
		student_destroy(new_student);
		return 1;
	}
	strcpy(new_student->name,student_temp->name);

	new_student->id=student_temp->id;

	list* new_grade_sheet = list_init(course_clone,course_destroy);
	if (new_grade_sheet == NULL){
		student_destroy(new_student);
		return 1;
	}

	new_student->grade_sheet=grade_sheet_clone(student_temp->grade_sheet,
													new_grade_sheet);
	if(new_student->grade_sheet==NULL){
		list_destroy(new_grade_sheet);
		student_destroy(new_student);
		return 1;
	}
	*clone=new_student;
	return 0;
}





/**
 * @brief serches for student with id in grades
 * @returns pointer to student with id on success, NULL if no such student
 * @note Failes if "grades" is invalid
 */
static student_t* find_student(struct grades *grades,int id){
	if(grades==NULL){
		return NULL;
	}
	for(node *it=list_begin(grades->student_list);it!=NULL;it=list_next(it)){
		student_t *student_temp=(student_t*)list_get(it);	
		if(student_temp->id==id){
			return student_temp;
		}
	}
	return NULL;
}


/**
 * @brief serches for student with id in grades
 * @returns 0 on success
 * @note Failes if "grades" is invalid
 */
static bool id_exists(struct grades *grades,int id){
	if(grades==NULL){
		exit(1);
	}
	for(node *it=list_begin(grades->student_list);it!=NULL;it=list_next(it)){
		student_t *student_temp=(student_t*)list_get(it);	
		if(student_temp->id==id){
			return true;
		}
	}
	return false;

}


/**
 * @brief serches for "course" in student->grade_sheet
 * @returns 0 on success
 * @note Failes if "student" is invalid
 */
static bool course_exists(student_t *student,const char *course){
	if(student==NULL){
		exit(1);
	}

	list *temp_sheet= student->grade_sheet;
	for(node *it=list_begin(temp_sheet) ; it!=NULL ; it=list_next(it)){
		course_t *course_temp=(course_t*)list_get(it);
		const char *temp_name = course_temp->name;
		/////////complete according to answer on forum//////
		if((temp_name==NULL)&&(course==NULL)){
			return true;
		}
		if(!strcmp(temp_name,course)){
			return true;
		}
	}
	return false;
}






/**
 * @prints course data 
 * @note Failes if "course" is invalid
 */
static void print_course(course_t* course){
	if(course==NULL){
		return ;
	}
	if(course->name==NULL){
		printf(" %d", course->grade);
	}
	else {
	printf(" %s %d", course->name, course->grade);
	}
	return;
}


/**
 * @prints students grade_sheet accordung to given format
 * @note Failes if "grade_sheet" is invalid
 */
static void print_grade_sheet(list *grade_sheet){
	if(grade_sheet==NULL){
		return ;
	}
	
	for(node *it=list_begin(grade_sheet)  ;it!=NULL;   it=list_next(it)){
		course_t *course_temp=(course_t*)list_get(it);	
		print_course(course_temp);
		/* prevent an extra comma at the end of the line */
		if(it != list_end(grade_sheet)){ 
			printf(",");
		}
	}
	return;
}






/**
 * @brief initializing struct course_t with "name" and "grade"
 * @returns pointer to course on success.
 * @note returns NULL on faliure to allocate
 */
static course_t* create_course(const char *name, int grade){

	course_t *new_course = (course_t*) malloc (sizeof(course_t));
	if (new_course==NULL){
		return NULL;
	}

	new_course->grade = grade;
	if (name!=NULL){
		char *new_name= (char*)malloc(strlen(name)+1);
		if (new_name==NULL){
			free(new_course);
			return NULL;
		}
		strcpy(new_name,name);
		new_course->name=new_name;
	}
	else if (name==NULL){
		new_course->name = NULL;
	}

	return new_course;
}


/**
 * @brief initializing struct student with "name" and "id"
 * @returns pointer to student on success
 * @note Failes if "name" is invalid 
 */
static student_t* create_student(const char *name, int id){

	student_t *new_student = (student_t*) malloc (sizeof(student_t));
	if (new_student==NULL){
		return NULL;
	}

	new_student->id=id;
	if (name == NULL){
		new_student -> name = NULL;
	}

	else if (name!=NULL){
		char *new_name= (char*)malloc(strlen(name)+1);
		if (new_name==NULL){
			free(new_student);
			return NULL;
		}
		strcpy(new_name,name);
		new_student->name=new_name;
	}

	new_student->grade_sheet=list_init(course_clone, course_destroy);
	if(new_student->grade_sheet==NULL){
		free(new_student->name);
		free(new_student);
		return NULL;
	}
	return new_student;
}



/**
 * @brief Initializes the "grades" data-structure.
 * @returns A pointer to the data-structure, of NULL in case of an error
 */
struct grades* grades_init(){
	
	struct grades* new_grades = (struct grades*) malloc(sizeof(struct grades)); 
	if (new_grades==NULL){
		return NULL;
	}

	new_grades->student_list= list_init(student_clone, student_destroy);

	return new_grades;
}


/**
 * @brief Destroys "grades", de-allocate all memory!
*/
void grades_destroy(struct grades *grades){

	if (grades == NULL){
		return ;
	}

	list_destroy(grades->student_list);
	free (grades);

	return;
}


/**
 * @brief Adds a student with "name" and "id" to "grades"
 * @returns 0 on success
 * @note Failes if "grades" is invalid, or a student with 
 * the same "id" already exists in "grades"
 */
int grades_add_student(struct grades *grades, const char *name, int id){
	/* name might be NULL*/
	if (grades==NULL || id_exists(grades,id) ){ 
		return 1;
	}

	student_t *new_student = create_student(name,id);
	if (new_student==NULL){
		return 1;
	}

	list_push_back(grades->student_list, new_student);
	student_destroy(new_student);
	return 0;
}




/**
 * @brief Adds a course with "name" and "grade" to the student with "id"
 * @return 0 on success
 * @note Failes if "grades" is invalid, if a student with "id" does not exist
 * in "grades", if the student already has a course with "name", or if "grade"
 * is not between 0 to 100.
 */
int grades_add_grade(struct grades *grades, const char *name,int id,
					int grade){

	if ((grades==NULL) || (grade<0) || (grade>100) || (!id_exists(grades,id))){
		return 1;
	}
	
	student_t *student_temp = find_student(grades, id);
	if(course_exists(student_temp,name)){
		return 1;
	}
	
	course_t *new_course=create_course(name,grade);
	if(new_course==NULL){
		return 1;
	}

	list_push_back(student_temp->grade_sheet, new_course);
	course_destroy(new_course);
	return 0;

}




/**
 * @brief Calcs the average of the student with "id" in "grades".
 * @param[out] out This method sets the variable pointed by "out" to the
 * student's name. Needs to allocate memory. The user is responsible for
 * freeing the memory.
 * @returns The average, or -1 on error
 * @note Fails if "grades" is invalid, or if a student with "id" does not exist
 * in "grades".
 * @note If the student has no courses, the average is 0.
 * @note On error, sets "out" to NULL.
 */
float grades_calc_avg(struct grades *grades, int id, char **out){

	if ((grades == NULL)|| !id_exists(grades, id)){
		return -1;
	}

	student_t* temp_student = find_student(grades, id);


	char *name_copy= (char*)malloc(strlen(temp_student->name)+1);
	if (name_copy==NULL){
		return -1;
	}
	strcpy(name_copy,temp_student->name);
	*out=name_copy;
	

	if (list_begin(temp_student->grade_sheet) == NULL){
		return 0;
	}

	unsigned int count = 0;
	float tot = 0;

	for(node *it=list_begin(temp_student->grade_sheet) ; it!=NULL ; it=list_next(it)){
		course_t *temp_course=(course_t*)list_get(it);
		tot = tot + temp_course->grade;
		count++;
	}
	return tot/count;
}






/**
 * @brief Prints the courses of the student with "id" in the following format:
 * STUDENT-NAME STUDENT-ID: COURSE-1-NAME COURSE-1-GRADE, [...]
 * @returns 0 on success
 * @note Fails if "grades" is invalid, or if a student with "id" does not exist
 * in "grades".
 * @note The courses should be printed according to the order 
 * in which they were inserted into "grades"
 */
int grades_print_student(struct grades *grades, int id){
	if((grades==NULL)||(!id_exists(grades,id))){
		return 1;
	}
	student_t *student_temp= find_student( grades, id);
	if (student_temp == NULL){
		return 1;
	}
	printf("%s %d:", student_temp->name, student_temp->id);
	print_grade_sheet(student_temp->grade_sheet);
	printf ("\n"); //////////////////////////////////////////maybe \n\r
	return 0;
}


/**
 * @brief Prints all students in "grade", in the following format:
 * STUDENT-1-NAME STUDENT-1-ID: COURSE-1-NAME COURSE-1-GRADE, [...]
 * STUDENT-2-NAME STUDENT-2-ID: COURSE-1-NAME COURSE-1-GRADE, [...]
 * @returns 0 on success
 * @note Fails if "grades" is invalid
 * @note The students should be printed according to the order 
 * in which they were inserted into "grades"
 * @note The courses should be printed according to the order 
 * in which they were inserted into "grades"
 */
int grades_print_all(struct grades *grades){
	if(grades==NULL){
		return 1;
	}

	for(node *it=list_begin(grades->student_list);it!=NULL; it=list_next(it)){
		student_t* student_temp	= (student_t*)list_get(it);
		/* if such student doesnt exist everything ok */
		grades_print_student(grades,student_temp->id);
		
	}
	return 0;
}


