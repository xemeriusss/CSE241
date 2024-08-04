#ifndef _STUDENT_H
#define _STUDENT_H

#include <iostream>
#include <string>
using namespace std;

class Course;
class Student{
public:
    
    string getStudentName() const;
    int getStudentId() const;

    Student();
    Student(string newName, int newId);
    ~Student();

    //void AddSelectedCourse(Course &courses);
    //void DeleteCourse(Course &courses);

private:
    int id;
    string name;
    int size=0;
    Course **courseNames = nullptr;
};

#endif 
