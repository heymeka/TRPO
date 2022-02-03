/**
      ---------------------------------------------------
      |         Made by Shamal Viktor (@j3uktop)        |
      |           2019-2021 - Lyceum BSU - IM           |
      |             2021 - BSUIR - FCP - PMS            |
      |          *** All rights reserved ***            |
      ---------------------------------------------------
                                            			**/

#include "database.h"
#include <algorithm>
#include <fstream>

const std::string RESERVE_DATABASE = "Reserve";
const std::string STUDENTS_DATABASE = "Students";
const std::string USERS_DATABASE = "Users";

std::string GetReserveDatabase(const std::string& filename) {
  std::string res = RESERVE_DATABASE + filename;
  return res;
}

std::string GetStudentsDatabase(const std::string& filename) {
  std::string stud = filename + STUDENTS_DATABASE;
  return stud;
}

std::string GetUsersDatabase(const std::string& filename) {
  std::string usr = filename + USERS_DATABASE;
  return usr;
}

std::string EncryptString(std::string str) {
  for (auto& symbol: str) {
    symbol ^= rand() & 255;
  }
  return str;
}

int EncryptShortInt(int value) {
  return ((value & 255) ^ (rand() & 255)) |
      ((value & (255 << 8)) ^ ((rand() & 255)) << 8);
}

long long EncryptInt(long long value) {
  return ((value & 255) ^ (rand() & 255)) |
      ((value & (255ll << 8)) ^ ((rand() & 255ll)) << 8) |
      ((value & (255ll << 16)) ^ ((rand() & 255ll)) << 16) |
      ((value & (255ll << 24)) ^ ((rand() & 255ll)) << 24);
}

long long EncryptLL(long long value) {
  long long first = value >> 32;
  long long second = value & ((1ll << 32) - 1);
  return ((EncryptInt(first) << 32) | EncryptInt(second));
}

long long EncryptDouble(double value) {
  long long copy;
  memcpy(&copy, &value, sizeof(double));
  return EncryptLL(copy);
}

double DecryptDouble(long long value) {
  double copy;
  value = EncryptLL(value);
  memcpy(&copy, &value, sizeof(double));
  return copy;
}

bool EncryptBool(bool value) {
  return (value ^ (rand() & 255));
}

void ReadString(char* str, FILE*& file) {
  short int size;
  fread(&size, 2, 1, file);
  size = (short int) EncryptShortInt(size);
  fread(str, 1, size, file);
  str[size] = '\0';
}

void WriteString(const std::string& str, FILE*& file) {
  short int size = (short int) EncryptShortInt(int(str.size()));
  std::string encrypt = EncryptString(str);
  fwrite(&size, 2, 1, file);
  for (auto symbol: encrypt) {
    fwrite(&symbol, 1, 1, file);
  }
}

bool Database::EncryptDatabaseStudents(const std::string& filename) {
  FILE* fin = fopen(GetStudentsDatabase(filename).c_str(), "wb");
  if (fin == nullptr) {
    return false;
  }
  int students_size = students.size();
  long long average_buf;
  long long wages_per_person_buf;
  bool activity_buf;
  /// Encrypt start
  srand(ENCRYPT_CONST_STUDENTS);
  students_size = EncryptInt(students_size);

  fwrite(&students_size, 4, 1, fin);

  for (auto& cur_student: students) {
    activity_buf = EncryptBool(cur_student.GetActivity());
    fwrite(&activity_buf, 1, 1, fin);

    wages_per_person_buf = EncryptDouble(cur_student.GetWagesPerPerson());
    fwrite(&wages_per_person_buf, 8, 1, fin);

    average_buf = EncryptDouble(cur_student.GetAverage());
    fwrite(&average_buf, 8, 1, fin);

    WriteString(cur_student.GetGroup(), fin);
    WriteString(cur_student.GetSecondName(), fin);
    WriteString(cur_student.GetName(), fin);
    WriteString(cur_student.GetSurname(), fin);

  }
  fclose(fin);

  return true;
}

bool Database::DecryptDatabaseStudents(const std::string& filename) {
  const std::string
      reserve_filename = GetReserveDatabase(GetStudentsDatabase(filename));
  students.clear();

  FILE* fin;
  // fwrite(&value, bits_count, count, file);
  // fread(&value, bits_count, count, file);
  fin = fopen(GetStudentsDatabase(filename).c_str(), "rb");
  if (fin == nullptr) {
    fin = fopen(reserve_filename.c_str(), "rb");
    if (fin == nullptr) {
      return false;
    }
  }
  int students_size;
  /// buffers
  const int MAX_BUF_STRING_SIZE = 100;
  char name_buf[MAX_BUF_STRING_SIZE];
  char surname_buf[MAX_BUF_STRING_SIZE];
  char second_name_buf[MAX_BUF_STRING_SIZE];
  char group_buf[MAX_BUF_STRING_SIZE];
  long long average_buf;
  long long wages_per_person_buf;
  bool activity_buf;
  /// end of buffers
  /// Decrypt start
  srand(ENCRYPT_CONST_STUDENTS);

  fread(&students_size, 4, 1, fin);
  students_size = EncryptInt(students_size);
  if (students_size < 0 || students_size > 100000) {
    fclose(fin);
    return false;
  }
  students.resize(students_size);
  for (auto& cur_student: students) {
    fread(&activity_buf, 1, 1, fin);
    activity_buf = EncryptBool(activity_buf);

    fread(&wages_per_person_buf, 8, 1, fin);
    double wages_per_person = DecryptDouble(wages_per_person_buf);

    fread(&average_buf, 8, 1, fin);
    double average = DecryptDouble(average_buf);

    ReadString(group_buf, fin);
    std::string group = EncryptString(group_buf);

    ReadString(second_name_buf, fin);
    std::string second_name = EncryptString(second_name_buf);

    ReadString(name_buf, fin);
    std::string name = EncryptString(name_buf);

    ReadString(surname_buf, fin);
    std::string surname = EncryptString(surname_buf);

    if (!cur_student.CreateStudent(surname,
                                   name,
                                   second_name,
                                   group,
                                   average,
                                   activity_buf,
                                   wages_per_person)) {
      students.clear();
      fclose(fin);
      return false;
    }
  }
  fclose(fin);
  return true;
}

bool Database::EncryptDatabaseUsers(const std::string& filename) {
  FILE* fin = fopen(GetUsersDatabase(filename).c_str(), "wb");
  if (fin == nullptr) {
    return false;
  }
  int users_size = users.size();
  long long password_buf;
  int status_buf;
  /// Encrypt start
  srand(ENCRYPT_CONST_USERS);
  users_size = EncryptInt(users_size);

  fwrite(&users_size, 4, 1, fin);

  for (auto& cur_user: users) {
    status_buf = EncryptInt(cur_user.GetStatus());
    fwrite(&status_buf, 4, 1, fin);

    password_buf = EncryptLL(cur_user.GetPassword());
    fwrite(&password_buf, 8, 1, fin);

    WriteString(cur_user.GetUsername(), fin);
  }
  fclose(fin);
  return true;
}

bool Database::DecryptDatabaseUsers(const std::string& filename) {
  const std::string
      reserve_filename = GetReserveDatabase(GetUsersDatabase(filename));
  users.clear();
  FILE* fin = fopen(GetUsersDatabase(filename).c_str(), "rb");
  if (fin == nullptr) {
    fin = fopen(reserve_filename.c_str(), "rb");
    if (fin == nullptr) {
      return false;
    }
  }
  const int MAX_STR_SIZE = 100;
  char username_buf[MAX_STR_SIZE];
  int users_size;
  long long password_buf;
  int status_buf;
  /// Encrypt start
  srand(ENCRYPT_CONST_USERS);
  fread(&users_size, 4, 1, fin);
  users_size = EncryptInt(users_size);
  if (users_size < 0 || users_size > 100000) {
    fclose(fin);
    return false;
  }
  users.resize(users_size);

  for (auto& cur_user: users) {
    fread(&status_buf, 4, 1, fin);
    status_buf = EncryptInt(status_buf);
    fread(&password_buf, 8, 1, fin);
    password_buf = EncryptLL(password_buf);
    ReadString(username_buf, fin);
    if (!cur_user.CreateUser(EncryptString(username_buf),
                             password_buf,
                             status_buf)) {
      users.clear();
      fclose(fin);
      return false;
    }
  }
  fclose(fin);
  return true;
}

Database::Database() {
  users.emplace_back("admin", "admin", ADMIN_STATUS);
}

Database::Database(const std::string& filename) {
  OpenDatabase(filename);
}

Database::Database(Database& second) {
  MINIMAL_WAGES = second.MINIMAL_WAGES;
  users = second.users;
  students = second.students;
}

bool Database::OpenDatabase(const std::string& filename) {
  if (!(DecryptDatabaseStudents(filename) &&
      DecryptDatabaseUsers(filename))) {
    users.emplace_back("admin", "admin", ADMIN_STATUS);
    return false;
  }
  return true;
}

bool Database::SaveDatabase(const std::string& filename) {
  return EncryptDatabaseStudents(filename) &&
      EncryptDatabaseStudents(GetReserveDatabase(filename)) &&
      EncryptDatabaseUsers(filename) &&
      EncryptDatabaseUsers(GetReserveDatabase(filename));
}

bool Database::AddStudent(const std::string& student_surname,
                          const std::string& student_name,
                          const std::string& student_second_name,
                          const std::string& student_group,
                          double student_average,
                          bool student_activity,
                          double student_wages_per_person,
                          User& current_user) {
  if (current_user.GetStatus() != ADMIN_STATUS) {
    return false;
  } else {
    Student new_student;
    if (!new_student.CreateStudent(student_surname,
                                   student_name,
                                   student_second_name,
                                   student_group,
                                   student_average,
                                   student_activity,
                                   student_wages_per_person)) {
      return false;
    } else {
      students.push_back(new_student);
    }
  }
  return true;
}

bool Database::AddUser(const std::string& new_username,
                       const std::string& new_password,
                       int new_status,
                       User& current_user) {
  if (current_user.GetStatus() != ADMIN_STATUS) {
    return false;
  } else {
    User new_user;
    if (!new_user.CreateUser(new_username, new_password, new_status)) {
      return false;
    } else {
      users.push_back(new_user);
      return true;
    }
  }
}

bool Database::DeleteStudent(int number, User& current_user) {
  if (current_user.GetStatus() != ADMIN_STATUS) {
    return false;
  } else if (number < students.size()) {
    students.erase(students.begin() + number);
    return true;
  } else {
    return false;
  }
}

bool Database::DeleteUser(int number, User& current_user) {
  if (current_user.GetStatus() != ADMIN_STATUS) {
    return false;
  } else if (number < users.size()) {
    users.erase(users.begin() + number);
    return true;
  } else {
    return false;
  }
}

bool SortComparatorStudentsByName(Student& first, Student& second) {
  if (first.GetSurname() != second.GetSurname()) {
    return first.GetSurname() < second.GetSurname();
  }
  if (first.GetName() != second.GetName()) {
    return first.GetName() < second.GetName();
  }
  if (first.GetSecondName() != second.GetSecondName()) {
    return first.GetSecondName() < second.GetSecondName();
  }
  if (first.GetAverage() != second.GetAverage()) {
    return first.GetAverage() < second.GetAverage();
  }
  return !first.GetActivity();
}

bool SortComparatorStudentsByGroup(Student& first, Student& second) {
  if (first.GetGroup() != second.GetGroup()) {
    return first.GetGroup() < second.GetGroup();
  }
  if (first.GetSurname() != second.GetSurname()) {
    return first.GetSurname() < second.GetSurname();
  }
  if (first.GetName() != second.GetName()) {
    return first.GetName() < second.GetName();
  }
  if (first.GetSecondName() != second.GetSecondName()) {
    return first.GetSecondName() < second.GetSecondName();
  }
  if (first.GetAverage() != second.GetAverage()) {
    return first.GetAverage() < second.GetAverage();
  }
  return !first.GetActivity();
}

bool SortComparatorStudentsByAverage(Student& first, Student& second) {
  if (first.GetAverage() != second.GetAverage()) {
    return first.GetAverage() > second.GetAverage();
  }
  if (first.GetActivity() != second.GetActivity()) {
    return first.GetActivity();
  }
  if (first.GetSurname() != second.GetSurname()) {
    return first.GetSurname() < second.GetSurname();
  }
  if (first.GetName() != second.GetName()) {
    return first.GetName() < second.GetName();
  }
  return first.GetSecondName() < second.GetSecondName();
}

bool SortComparatorUsersByUsername(User& first, User& second) {
  return first.GetUsername() < second.GetUsername();
}

bool SortComparatorUsersByPriority(User& first, User& second) {
  return first.GetStatus() > second.GetStatus();
}

bool Database::SortStudents(int type, bool is_reverse) {
  if (type == SORT_BY_NAME) {
    std::sort(students.begin(), students.end(), SortComparatorStudentsByName);
  } else if (type == SORT_BY_PRIORITY) {
    const double cur_minimal_wages = MINIMAL_WAGES;
    std::sort(students.begin(),
              students.end(),
              [=](Student& first, Student& second) {
                if (first.GetWagesPerPerson()
                    < 2 * cur_minimal_wages) { return false; }
                return SortComparatorStudentsByAverage(first, second);
              });
  } else if (type == SORT_BY_AVERAGE) {
    std::sort(students.begin(),
              students.end(),
              SortComparatorStudentsByAverage);
  } else if (type == SORT_BY_GROUP) {
    std::sort(students.begin(), students.end(), SortComparatorStudentsByGroup);
  } else {
    return false;
  }
  if (is_reverse) {
    std::reverse(students.begin(), students.end());
  }
  return true;
}

bool Database::SortUsers(int type, bool is_reverse) {
  if (type == SORT_BY_USERNAME) {
    std::sort(users.begin(), users.end(), SortComparatorUsersByUsername);
  } else if (type == SORT_BY_PRIORITY) {
    std::sort(users.begin(), users.end(), SortComparatorUsersByPriority);
  } else {
    return false;
  }
  if (is_reverse) {
    std::reverse(users.begin(), users.end());
  }
  return true;
}

bool Database::UpdateMinimalWages(double new_minimal_wages) {
  if (new_minimal_wages <= 0) {
    return false;
  }
  MINIMAL_WAGES = new_minimal_wages;
  return true;
}

double Database::GetMinimalWages() {
  return MINIMAL_WAGES;
}

int Database::FindUserAndGetStatus(const User& curr) {
  for (auto& possible_user: users) {
    if (possible_user == curr) {
      return possible_user.GetStatus();
    }
  }
  return ERROR_STATUS;
}

int Database::FindUserAndGetStatus(const std::string& username, const std::string& password) {
  return FindUserAndGetStatus(User(username, password, ERROR_STATUS));
}

bool Database::IsUserAdmin(const User& curr_user) {
  return curr_user.GetStatus() == ADMIN_STATUS;
}

bool Database::SetStatusToUser(User& current) {
  int status = FindUserAndGetStatus(current);
  current.ChangeStatus(status);
  return status != ERROR_STATUS;
}

Database::~Database() {
  SaveDatabase("Work");
}

////////////////
/*
template<class T>
void Database::MergeVectors(std::vector<T>& first, std::vector<T>& second, std::vector<T>& result, bool Comparator(T&, T&)) {
  int left = 0;
  int right = 0;
  int first_size = first.size();
  int second_size = second.size();
  while(left < first_size && right < second_size) {
    if(Comparator(first[left], second[right])) {
      result[left + right] = first[left];
      left++;
    } else {
      result[left + right] = second[right];
      right++;
    }
  }
  while(left < first_size) {
    result[left + right] = first[left];
    left++;
  }
  while(right < second_size) {
    result[left + right] = second[right];
    right++;
  }
}
template<class T>
void Database::SortVector(std::vector<T>& sort_vector, bool Comparator(T&, T&)) {
  int size = sort_vector.size();
  if(sort_vector.size() < 32) {
    for(int first = 1; first < size; first++)
    {
      for(int second = first; second > 0 && Comparator(sort_vector[second - 1], sort_vector[second]); second--)
        sort_vector[second - 1].swap(sort_vector[second]);
    }
  } else {
    std::vector<T> first_half(sort_vector.begin(), sort_vector.begin() + size / 2);
    std::vector<T> second_half(sort_vector.begin() + size / 2, sort_vector.end());
    SortVector(first_half, Comparator);
    SortVector(second_half, Comparator);
    MergeVectors(first_half, second_half, sort_vector, Comparator);
  }
}
*/