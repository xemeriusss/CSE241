#include "SchoolManagementSystem.h"
#include <iostream>
#include <string>

using namespace std;

void StudentSelection(SchoolManagementSystem *school);
void Select_StudentSelection(SchoolManagementSystem *school);

void CourseSelection(SchoolManagementSystem *school);
void Select_CourseSelection(SchoolManagementSystem *school);

int main() {

  int sel0;

  // making a pointer object
  SchoolManagementSystem *school = new SchoolManagementSystem();

  do {

    cout << "Main Menu" << endl;
    cout << "0 exit" << endl;
    cout << "1 student" << endl;
    cout << "2 course" << endl;
    cout << "3 list_all_students" << endl;
    cout << "4 list_all_courses" << endl;

    cin >> sel0;

    if (sel0 == 1) { // student
      StudentSelection(school);
    }

    else if (sel0 == 2) { // course

      CourseSelection(school);
    }

    else if (sel0 == 3) { // list all students

      school->listStudent();
    }

    else if (sel0 == 4) { // list all courses

      school->listCourse();
    }

  } while (sel0 != 0);

  return 0;
}

void StudentSelection(SchoolManagementSystem *school) {

  int sel;

  cout << "0 up" << endl;
  cout << "1 add_student" << endl;
  cout << "2 select_student" << endl;

  cin >> sel;

  if (sel == 0) { // up

    return;
  }

  else if (sel == 1) { // add student

    string name = "";
    int id = 0;
    string temp;
    char ch;

    cin >> ch; // for enter

    getline(cin, temp);

    temp = ch + temp; // for concatenate first letter

    for (int i = 0; i < temp.size(); ++i) {

      if (temp[i] == ' ') {

        name = name + ' '; // concatenate name
      }

      else {

        if (temp[i] < '0' || temp[i] > '9') {

          name = name + temp[i]; // concatenate id
        }

        else {

          for (; i < temp.size(); ++i) {

            id = id * 10;
            id = id + (int)temp[i] - 48; // string to int
          }
        }
      }
    }

    if (school->checkStudent(id)) { // if there is same id, return

      return;
    }

    school->AddStudent(name, id);
  }

  else if (sel == 2) { // select student

    Select_StudentSelection(school);
  }
}

void Select_StudentSelection(SchoolManagementSystem *school) {

  int sel;
  char ch;

  string temp;
  string name = "";
  int id = 0;

  cout << "0 up" << endl;
  cout << "1 delete_student" << endl;
  cout << "3 add_selected_student_to_a_course" << endl;
  cout << "4 drop_selected_student_from_a_course" << endl;

  cin >> ch;

  getline(cin, temp);

  temp = ch + temp;

  for (int i = 0; i < temp.size(); ++i) {

    if (temp[i] == ' ') {

      name = name + ' '; // concatenate name
    }

    else {

      if (temp[i] < '0' || temp[i] > '9') {

        name = name + temp[i];
      }

      else {

        for (; i < temp.size(); ++i) {

          id = id * 10;
          id = id + (int)temp[i] - 48; // string to int
        }
      }
    }
  }

  cin >> sel;

  if (sel == 0) { // up
    return;
  }

  else if (sel == 1) { // delete

    school->DeleteStudent(name, id);
    school->listStudent();
  }

  /*
  else if(sel == 3){ // add_selected_student_to_a_course

          int sel;

          cout << "0 up" << endl;
          school->listCourse();

          cin >> sel;

          if(sel == 0){ // up
                  return;
          }


          else{

                  school->AddSelectedStudentToCourse()
          }

  } */

  /*
  else if(sel == 4){ // drop_selected_student_from_a_course


  } */
}

void CourseSelection(SchoolManagementSystem *school) {

  int sel;

  cout << "0 up" << endl;
  cout << "1 add_course" << endl;
  cout << "2 select_course" << endl;

  cin >> sel;

  if (sel == 0) { // up

    return;
  }

  else if (sel == 1) { // add course

    string code = "";  // CSE241
    string name = "";  // Object Oriented Programming
    string name2 = ""; // final version of name
    string temp;
    char ch;
    int flag = 0;

    cin >> ch;

    getline(cin, temp);

    temp = ch + temp;

    for (int i = 0; i < temp.size(); ++i) {

      if (temp[i] == ' ') {

        if (flag == 0) { // for code

          code = name;
          flag++;
        }

        else {

          name2 = name2 + ' '; // concatenate name
        }
      }

      else {

        if (flag != 0) {

          name2 = name2 + temp[i];
        }

        name = name + temp[i];
      }
    }

    if (school->checkCourse(name2,
                            code)) { // if there is same name or code, return

      return;
    }

    school->AddCourse(name2, code);
  }

  else if (sel == 2) { // select_course

    Select_CourseSelection(school);
  }
}

void Select_CourseSelection(SchoolManagementSystem *school) {

  int sel;

  cout << "0 up" << endl;
  cout << "1 delete_course" << endl;
  cout << "2 list_students_registered_to_the_selected_course" << endl;

  string code = ""; // CSE241
  string name = ""; // Object Oriented Programming
  string name2 = "";
  string temp;
  int flag = 0;
  char ch;

  cin >> ch;

  getline(cin, temp);

  temp = ch + temp;

  for (int i = 0; i < temp.size(); ++i) {

    if (temp[i] == ' ') {

      if (flag == 0) { // for code

        code = name;
        flag++;
      }

      else {

        name2 = name2 + ' '; // concatenate name
      }
    }

    else {

      if (flag != 0) {

        name2 = name2 + temp[i];
      }

      name = name + temp[i];
    }
  }

  cin >> sel;

  if (sel == 0) { // up

    return;
  }

  else if (sel == 1) { // delete course

    school->DeleteCourse(name2, code);
    school->listCourse();
  }
  /*
  else if(sel == 2){


  } */
}