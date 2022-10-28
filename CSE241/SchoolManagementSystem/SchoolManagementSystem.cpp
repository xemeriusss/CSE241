/**
 * It adds a student to a course.
 */
#include <iostream>
#include "SchoolManagementSystem.h"

using namespace std;

SchoolManagementSystem::SchoolManagementSystem(){ }

SchoolManagementSystem::~SchoolManagementSystem(){

	for (int i = 0; i < student_size ; ++i){
        delete studentArr[i];
    }

    delete[] studentArr;

    for (int i = 0 ; i < course_size ; ++i){
        delete courseArr[i];
    }
    delete[] courseArr;  
}

int SchoolManagementSystem::getStudentSize() const{

	return student_size;
}

int SchoolManagementSystem::getCourseSize() const{

	return course_size;
}

void SchoolManagementSystem::listStudent(){

	for(int i = 0; i < student_size ; i++){

		cout << endl;
		cout << "Student name: " << studentArr[i]->getStudentName() << endl;
		cout << "Student id: " << studentArr[i]->getStudentId() << endl;
		cout << endl;
	}
}

/**
 * This function lists all the courses in the system
 */
void SchoolManagementSystem::listCourse(){

	for(int i = 0; i < course_size ; i++){

		cout << endl;
		cout << i+1 << " " << courseArr[i]->getCourseCode() << " ";
		cout << courseArr[i]->getCourseName() << endl;
		cout << endl;
	}
}

/**
 * It checks if the student id is already in the system
 * 
 * @param id the id of the student
 */
bool SchoolManagementSystem::checkStudent(int id){

	for(int i=0 ; i<student_size ; i++){

        if(studentArr[i]->getStudentId() == id){

            return true; // there is same id
        }
    }

    return false;
 }

/**
 * It checks if the course name or code already exists in the course array
 * 
 * @param name The name of the course
 * @param code
 * 
 * @return a boolean value.
 */
bool SchoolManagementSystem::checkCourse(string name, string code){

	for(int i=0 ; i<course_size ; i++){

		if((courseArr[i]->getCourseName() == name) || (courseArr[i]->getCourseCode() == code)){

			return true;
		}
	}

	return false;
}

int SchoolManagementSystem::findSameStudentIndex(int id){

	int index;

	for(int i=0 ; i<student_size ; i++){

        if(studentArr[i]->getStudentId() == id){

            index = i; // same id index
        }
    }

    return index;
}

int SchoolManagementSystem::findSameCourseIndex(string name, string code){

	int index;

	for(int i=0 ; i<course_size ; i++){

		if((courseArr[i]->getCourseName() == name) || (courseArr[i]->getCourseCode() == code)){

			index = i;
		}
	}

	return index;
}

void SchoolManagementSystem::AddStudent(string name, int id){

	Student **newArr = new Student*[student_size+1];

    for(int i=0; i<student_size; i++){

        newArr[i] = studentArr[i];
    }

    delete[] studentArr;
    studentArr = newArr;

    studentArr[student_size] = new Student(name, id);

    ++student_size;
}

void SchoolManagementSystem::DeleteStudent(string name, int id){

	int pos = -1;
	Student **newArr;

    if(student_size > 1){

        newArr = new Student*[student_size-1];
    }

    else if(student_size == 1){

    	--student_size;
    	delete studentArr[0];
    	delete[] studentArr;
    	return;
    }

    else
        return;

    // check if the student is in the array
    for(int i=0; (i<student_size) && (pos==-1); i++){

        if((studentArr[i]->getStudentName() == name) && (studentArr[i]->getStudentId() == id)){

            pos = i;
        }
    }

    // if student not found ,exit
    if(pos == -1)
        return;


    for(int i=0,j=0; i<student_size; i++){

        //skip over the delete element
        if(i != pos){

            newArr[j] = studentArr[i];
            j++;
        }
    }

    --student_size;
    delete[] studentArr;
    studentArr = newArr;
}


void SchoolManagementSystem::AddCourse(string name, string code){

	Course **newArr = new Course*[student_size+1];

    for(int i=0; i<course_size; i++){

        newArr[i] = courseArr[i];
    }

    delete[] courseArr;
    courseArr = newArr;

    courseArr[course_size] = new Course(name, code);

    ++course_size;
}


void SchoolManagementSystem::DeleteCourse(string name, string code){

	int pos = -1;
	Course **newArr;

    if(course_size > 1){

        newArr = new Course*[course_size-1];
    }

    else if(course_size == 1){

    	--course_size;
    	delete courseArr[0];
    	delete[] courseArr;
    	return;
    }

    else
        return;

    // check if the course is in the array
    for(int i=0; (i<course_size) && (pos==-1); i++){

        if((courseArr[i]->getCourseName() == name) && (courseArr[i]->getCourseCode() == code)){

            pos = i;
        }
    }

    // if course not found ,exit
    if(pos == -1)
        return;


    for(int i=0,j=0; i<course_size; i++){

        //skip over the delete element
        if(i != pos){

            newArr[j] = courseArr[i];
            j++;
        }
    }

    --course_size;
    delete[] courseArr;
    courseArr = newArr;
}
/*
void SchoolManagementSystem::AddSelectedStudentToCourse(string student_name, int id, string course_name, string code){

	int studentIndex = findSameStudentIndex(id);
	int courseIndex = findSameCourseIndex(course_name, code);

	studentArr[studentIndex]->AddSelectedCourse(courseArr[courseIndex]);
	courseArr[courseIndex]->AddSelectedStudent(studentArr[studentIndex]);
} */
