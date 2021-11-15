/**
      ---------------------------------------------------
      |         Made by Shamal Viktor (@j3uktop)        |
      |           2019-2021 - Lyceum BSU - IM           |
      |             2021 - BSUIR - FCP - PMS            |
      |          *** All rights reserved ***            |
      ---------------------------------------------------
                                            			**/
#include <iostream>
#include <map>
#include <iomanip>
#include <algorithm>

using std::cin;
using std::cout;
using std::string;
using std::map;
using std::setw;
using std::sort;

string ERROR_MESSAGE[] = {"Num have to be not negative ( >= 0)\n",
};
enum ERRORS { IS_NEGATIVE, };

struct FullName {
  string name = "Ivan";
  string surname = "Ivanov";
  string patronymic = "Ivanovich";
};

std::ostream& operator<<(std::ostream& to_out, const FullName& value) {
  to_out << value.surname << " " << value.name << " " << value.patronymic;
  return to_out;
}

struct BirthDate {
  int day;
  int month;
  int year;
};

std::ostream& operator<<(std::ostream& to_out, const BirthDate& birth) {
  if(birth.day < 10) {
    to_out << "0";
  }
  to_out << birth.day << ".";
  if(birth.month < 10) {
    to_out << "0";
  }
  to_out << birth.month << "." << birth.year;
  return to_out;
}

struct student {
  FullName full_name;
  BirthDate birth;
  int marks_count = 0;
  int* marks = nullptr;
};

void workWithStudents();

student* createStudentArray(int& size);

void deleteStudentsArray(student* p_student, int& size);

void printStudents(student* p_student, int& size);

string findMostPopularName(student* p_student, int count);

/////////////////////////////////////////////////////////////////////
  int main() {
    workWithStudents();
    system("pause");
    return 0;
  }
/////////////////////////////////////////////////////////////////////

bool studentsByName(student& first, student& second) {
    if(first.full_name.surname != second.full_name.surname) {
      return first.full_name.surname < second.full_name.surname;
    } else if(first.full_name.name != second.full_name.name) {
      return first.full_name.name < second.full_name.name;
    } else {
      return first.full_name.patronymic < second.full_name.patronymic;
    }
}

void workWithStudents() {
  int students_count;
  student* students_list = createStudentArray(students_count);
  cout << "Students:\n";
  printStudents(students_list, students_count);
  cout << "Most popular name: ";
  cout << findMostPopularName(students_list, students_count) << '\n';
  sort(students_list, students_list + students_count, studentsByName);
  cout << "Students after sort by name:\n";
  printStudents(students_list, students_count);
  deleteStudentsArray(students_list, students_count);
}

string findMostPopularName(student* p_student, int size) {
  map<string, int> names;
  for (int index = 0; index < size; ++index) {
    names[p_student[index].full_name.name]++;
  }
  string most_popular = names.begin()->first;
  int max_ = 0;
  // C++17
  // for (auto&[name, value]: names) {
  //   if (value > max_) {
  //     max_ = value;
  //     most_popular = name;
  //   }
  // }
  // C++11
  for(auto& value: names) {
    if(value.second > max_) {
      max_ = value.second;
      most_popular = value.first;
    }
  }
  return most_popular;
}

student* createStudentArray(int& size) {
  do {
    cout << ERROR_MESSAGE[IS_NEGATIVE];
    cout << "Enter students count = ";
    cin >> size;
  } while (size < 0);
  auto* new_students = new student[size];
  cout << "Enter students info:\n";
  for (int student_index = 0; student_index < size; ++student_index) {
    cout << "Enter full FullName (Surname Name Patronymic): ";
    cin >> new_students[student_index].full_name.surname
        >> new_students[student_index].full_name.name
        >> new_students[student_index].full_name.patronymic;
    cout << "Enter full date of birth (Day Month Year): ";
    cin >> new_students[student_index].birth.day
        >> new_students[student_index].birth.month
        >> new_students[student_index].birth.year;
    cout << "Enter count of student's marks: ";
    cin >> new_students[student_index].marks_count;
    new_students[student_index].marks =
        new int[new_students[student_index].marks_count];
    cout << "Enter " << new_students[student_index].marks_count
         << " marks of this student\n";
    for (int mark_index = 0;
         mark_index < new_students[student_index].marks_count; ++mark_index) {
      cin >> new_students[student_index].marks[mark_index];
    }
  }
  return new_students;
}

void printStudents(student* p_student, int& size) {
  cout << size << " students:\n";
  for (int index = 0; index < size; ++index) {
    cout << setw(30) << p_student[index].full_name << setw(12)
         << p_student[index].birth << "   Marks: ";
    for(int mark_index = 0 ; mark_index < p_student[index].marks_count; ++mark_index) {
      cout << p_student[index].marks[mark_index] << ' ';
    }
    cout << '\n';
  }
}

void deleteStudentsArray(student* p_student, int& size) {
  for (int index = 0; index < size; ++index) {
    delete[] p_student[index].marks;
  }
  delete[] p_student;
};
