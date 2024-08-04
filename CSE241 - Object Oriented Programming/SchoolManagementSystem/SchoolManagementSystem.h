#include <iostream>
#include <string>
#include "Student.h"
#include "Course.h"

using namespace std;

class SchoolManagementSystem{

public:
    SchoolManagementSystem();
    ~SchoolManagementSystem();

    int getStudentSize() const;
    int getCourseSize() const;

    void listStudent();
    void listCourse();

    bool checkStudent(int id);
    bool checkCourse(string name, string code);

    int findSameStudentIndex(int id);
    int findSameCourseIndex(string name, string code);

    void AddStudent(string name, int id);
    void AddCourse(string name, string code);
    void DeleteStudent(string name, int id);
    void DeleteCourse(string name, string code);

    //void AddSelectedStudentToCourse(string student_name, int id, string course_name, string code);
    //void dropStudentFromCourse(Student& student, Course& course);

private:
    Course **courseArr=nullptr;
    Student **studentArr=nullptr;
    int student_size=0;
    int course_size=0;

};