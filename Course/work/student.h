/**
      ---------------------------------------------------
      |         Made by Shamal Viktor (@j3uktop)        |
      |           2019-2021 - Lyceum BSU - IM           |
      |             2021 - BSUIR - FCP - PMS            |
      |          *** All rights reserved ***            |
      ---------------------------------------------------
                                            			**/

#ifndef STUDENT_H
#define STUDENT_H

#include <iostream>

class Student {
 private:
  const int GROUP_DIGITS_COUNT = 6;
  std::string surname;
  std::string name;
  std::string second_name;
  std::string group;
  double average;
  bool activity;
  double wages_per_person;
  bool IsLetter(char symbol);
  bool IsDigit(char symbol);
 public:
  Student();
  Student(const Student& copy);
  void swap(Student& second);
  bool CreateStudent(const std::string& student_surname,
                     const std::string& student_name,
                     const std::string& student_second_name,
                     const std::string& student_group,
                     double student_average,
                     bool student_activity,
                     double student_wages_per_person);
  std::string GetName();
  std::string GetSurname();
  std::string GetSecondName();
  std::string GetFullName();
  std::string GetGroup();
  bool GetActivity();
  double GetAverage();
  double GetWagesPerPerson();
  bool UpdateSurname(const std::string& new_surname);
  bool UpdateName(const std::string& new_name);
  bool UpdateSecondName(const std::string& new_second_name);
  bool UpdateGroup(const std::string& new_group);
  bool UpdateAverage(double new_average);
  bool UpdateWagesPerPerson(double new_wages_per_person);
  bool UpdateActivity();
  Student& operator=(const Student& copy);
  bool operator==(const Student& second);
};

#endif //STUDENT_H
