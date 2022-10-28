#include "Student.h"
#include <string>

string Student::getStudentName() const{
    return this->name;
}

int Student::getStudentId() const{
    return this->id;
}

Student::Student() 
    :name(""), id(0) {
}

Student::Student(string newName, int newId)
    :name(newName), id(newId) {
}

Student::~Student(){ }

/*

void Student::AddSelectedCourse(Course &courses){

    Course **newArr = new Course*[size+1];

    for(int i=0; i<size; i++){

        newArr[i] = courseNames[i];
    }

    delete[] courseNames;
    courseNames = newArr;

    courseNames[size] = &courses; // the adrress of new student object

    ++size;
}
*/
/*
void Student::DeleteCourse(Course &courses){

    int pos = -1;

    if(size > 1){

        Course **newArr = new Course*[size-1];
    }

    else
        return;

    // check if the course is in the array
    for(int i=0; (i<size) && (pos==-1); i++){

        if(courseNames[i] == &courses){

            pos = i;
        }
    }

    // if course not found ,exit
    if(pos == -1)
        return;


    for(int i=0,j=0; i<size; i++){

        //skip over the delete element
        if(i != pos){

            newArr[j] = courseNames[i];
            j++;
        }
    }

    size--;
    delete[] courseNames;
    courseNames = newArr;

}  */

