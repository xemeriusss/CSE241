#ifndef _COURSE_H
#define _COURSE_H

#include <iostream>
#include <string>
using namespace std;

class Student;
class Course{
public:
    
    string getCourseName() const;
    string getCourseCode() const;

    Course();
    Course(string newName, string newCode);
    ~Course();
    
    void AddSelectedStudent(Student &students);
    //void DeleteStudent(Student &students);

private:
    string code;
    string name;
    int size=0;
    Student **studentNames = nullptr;
};

#endif 