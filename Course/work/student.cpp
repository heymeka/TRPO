/**
      ---------------------------------------------------
      |         Made by Shamal Viktor (@j3uktop)        |
      |           2019-2021 - Lyceum BSU - IM           |
      |             2021 - BSUIR - FCP - PMS            |
      |          *** All rights reserved ***            |
      ---------------------------------------------------
                                            			**/

#include "student.h"

Student::Student() {
  average = 0;
  activity = false;
  wages_per_person = 0;
}

Student::Student(const Student& copy) {
  *this = copy;
}

void Student::swap(Student& second) {
  std::swap(second.average, average);
  std::swap(second.activity, activity);
  std::swap(second.wages_per_person, wages_per_person);
  surname.swap(second.surname);
  name.swap(second.name);
  second_name.swap(second.second_name);
  group.swap(second.group);
}

bool Student::CreateStudent(const std::string& student_surname,
                            const std::string& student_name,
                            const std::string& student_second_name,
                            const std::string& student_group,
                            double student_average,
                            bool student_activity,
                            double student_wages_per_person) {
  bool is_ok = true;
  is_ok &= UpdateSurname(student_surname);
  is_ok &= UpdateName(student_name);
  is_ok &= UpdateSecondName(student_second_name);
  is_ok &= UpdateGroup(student_group);
  is_ok &= UpdateAverage(student_average);
  activity = student_activity;
  is_ok &= UpdateWagesPerPerson(student_wages_per_person);
  return is_ok;
}

bool Student::IsLetter(char symbol) {
  return ((symbol >= 'a' && symbol <= 'z') || (symbol >= 'A' && symbol <= 'Z'));
}

bool Student::IsDigit(char symbol) {
  return (symbol >= '0' && symbol <= '9');
}

bool Student::UpdateSurname(const std::string& new_surname) {
  for (auto& symbol: new_surname) {
    if (!IsLetter(symbol)) {
      return false;
    }
  }
  surname = new_surname;
  return true;
}

bool Student::UpdateName(const std::string& new_name) {
  for (auto symbol: new_name) {
    if (!IsLetter(symbol)) {
      return false;
    }
  }
  name = new_name;
  return true;
}

bool Student::UpdateSecondName(const std::string& new_second_name) {
  for (auto symbol: new_second_name) {
    if (!IsLetter(symbol)) {
      return false;
    }
  }
  second_name = new_second_name;
  return true;
}

bool Student::UpdateGroup(const std::string& new_group) {
  if (new_group.size() != GROUP_DIGITS_COUNT) {
    return false;
  }
  for (auto symbol: new_group) {
    if (!IsDigit(symbol)) {
      return false;
    }
  }
  group = new_group;
  return true;
}

bool Student::UpdateAverage(double new_average) {
  if (new_average < 0 || new_average > 10) {
    return false;
  }
  average = new_average;
  return true;
}

bool Student::UpdateWagesPerPerson(double new_wages_per_person) {
  if (new_wages_per_person < 0) {
    return false;
  }
  wages_per_person = new_wages_per_person;
  return true;
}

bool Student::UpdateActivity() {
  activity = !activity;
  return true;
}

std::string Student::GetName() {
  return name;
}

std::string Student::GetSurname() {
  return surname;
}

std::string Student::GetSecondName() {
  return second_name;
}

std::string Student::GetGroup() {
  return group;
}

bool Student::GetActivity() {
  return activity;
}

double Student::GetAverage() {
  return average;
}

double Student::GetWagesPerPerson() {
  return wages_per_person;
}

std::string Student::GetFullName() {
  return surname + " " + name + " " + second_name;
}

Student& Student::operator=(const Student& copy) {
  surname = copy.surname;
  name = copy.name;
  second_name = copy.second_name;
  group = copy.group;
  activity = copy.activity;
  average = copy.average;
  wages_per_person = copy.wages_per_person;
  return *this;
}

bool Student::operator==(const Student& second) {
  return (surname == second.surname && name == second.name
      && second_name == second.second_name && group == second.group
      && activity == second.activity && average == second.average
      && wages_per_person == second.wages_per_person);
}