#include "Course.h"
#include <string>

string Course::getCName() const { return this->name; }

string Course::getCourseCode() const { return this->code; }

Course::Course() : name(""), code("") {}

Course::Course(string newName, string newCode) : name(newName), code(newCode) {}

Course::~Course() {}

void Course::AddSelectedStudent(Student &students) {

  Student **newArr = new Student *[size + 1];

  for (int i = 0; i < size; i++) {

    newArr[i] = studentNames[i];
  }

  delete[] studentNames;
  studentNames = newArr;

  studentNames[size] = &students;

  ++size;
}
/*
void Course::DeleteCourse(Course &courses){

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
