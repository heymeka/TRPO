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
#include <iomanip>

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

// Зашифрованная запись студентов в файл
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
  // students_size = 10;
  if (students_size < 0 || students_size > 100000) {
    fclose(fin);
    return false;
  }
  if(students_size == 0) {
    fclose(fin);
    return true;
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
  if(users_size == 0) {
    fclose(fin);
    return true;
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

// Чтение данных из файлов
bool Database::OpenDatabase(const std::string& filename) {
  if (!(DecryptDatabaseStudents(filename) &&
      DecryptDatabaseUsers(filename))) {
    return false;
  }
  if (users.size() == 0) {
    users.emplace_back("admin", "admin", ADMIN_STATUS);
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

// Добавление учётной записи
bool Database::AddUser(const std::string& new_username,
                       const std::string& new_password,
                       int new_status,
                       User& current_user) {
  if (current_user.GetStatus() != ADMIN_STATUS) {
    return false;
  } else {
    User new_user;
    if (!new_user.CreateUser(new_username, new_password, new_status) || !IsUniqueUsername(new_username)) {
      return false;
    } else {
      users.push_back(new_user);
      return true;
    }
  }
}

bool Database::DeleteStudent(int number, User& current_user) {
  if (current_user.GetStatus() == ADMIN_STATUS && number < students.size() && number >= 0) {
    students.erase(students.begin() + number);
    return true;
  } else {
    return false;
  }
}

bool Database::DeleteUser(int number, User& current_user) {
  if (current_user.GetStatus() != ADMIN_STATUS || users.size() < 2) {
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

int Database::FindUserAndGetStatus(const std::string& username,
                                   const std::string& password) {
  return FindUserAndGetStatus(User(username, password, ERROR_STATUS));
}

bool Database::IsUserAdmin(const User& curr_user) const {
  return curr_user.GetStatus() == ADMIN_STATUS;
}

bool Database::SetStatusToUser(User& current) {
  int status = FindUserAndGetStatus(current);
  current.ChangeStatus(status);
  return status != ERROR_STATUS;
}

std::string GetStudentsTableLine(int SPACE) {
  std::string line = "";
  const std::string TABLE = "-+-";
  // Number
  line = "----" + TABLE;
  // Name
  for (int ind = 0; ind < SPACE; ++ind) {
    line += "-";
  }
  // Group
  // Average
  for (int col = 0; col < 3; ++col) {
    for (int ind = 0; ind < SPACE; ++ind) {
      line += '-';
    }
    line += TABLE;
  }
  // Activity
  for (int ind = 0; ind < std::string("ACTIVITY").size(); ++ind) {
    line += '-';
  }
  line += TABLE;
  // Wages
  for (int ind = 0; ind < SPACE; ++ind) {
    line += '-';
  }
  line += TABLE;
  line.pop_back();
  line += '\n';
  return line;
}

bool Database::PrintStudentsByName(std::ostream& out, const std::string& name) const {
  const int NUMBER_SPACE = 4;
  const int SPACE = 16;
  const std::string TABLE = " | ";
  const std::string ACTIVITY = "Activity";
  const std::string LINE = GetStudentsTableLine(SPACE);
  PrintHeaderStudentsTable(out, SPACE, TABLE);
  int cnt = 0;
  for (auto& current: students) {
    if(current.GetFullName().find(name) != std::string::npos)
      out << std::setw(NUMBER_SPACE) << ++cnt << TABLE
          << std::setw(2 * SPACE) << current.GetFullName() << TABLE
          << std::setw(SPACE) << current.GetGroup() << TABLE
          << std::setw(SPACE) << current.GetAverage() << TABLE
          << std::setw(ACTIVITY.size()) << current.GetActivity() << TABLE
          << std::setw(SPACE) << current.GetWagesPerPerson() << TABLE << '\n'
          << LINE;
  }
  return (cnt != 0);
}

bool Database::PrintStudentsByGroup(std::ostream& out, const std::string& group) const {
  const int NUMBER_SPACE = 4;
  const int SPACE = 16;
  const std::string TABLE = " | ";
  const std::string ACTIVITY = "Activity";
  const std::string LINE = GetStudentsTableLine(SPACE);
  PrintHeaderStudentsTable(out, SPACE, TABLE);
  int cnt = 0;
  for (auto& current: students) {
    if(current.GetGroup().find(group) != std::string::npos)
      out << std::setw(NUMBER_SPACE) << ++cnt << TABLE
        << std::setw(2 * SPACE) << current.GetFullName() << TABLE
        << std::setw(SPACE) << current.GetGroup() << TABLE
        << std::setw(SPACE) << current.GetAverage() << TABLE
        << std::setw(ACTIVITY.size()) << current.GetActivity() << TABLE
        << std::setw(SPACE) << current.GetWagesPerPerson() << TABLE << '\n'
        << LINE;
  }
  return (cnt != 0);
}

bool Database::PrintHeaderStudentsTable(std::ostream& out, const int SPACE, const std::string& TABLE) const {
  const int NUMBER_SPACE = 4;
  const std::string NUMBER = "#";
  const std::string FULLNAME = "Full name";
  const std::string GROUP = "Group";
  const std::string AVERAGE = "Average";
  const std::string ACTIVITY = "Activity";
  const std::string WAGES = "Wages per person";
  const std::string MINIMAL_WAGES_INFO = "\nMinimal wages: ";
  const std::string LINE = GetStudentsTableLine(SPACE);
  out << std::setw(NUMBER_SPACE) << NUMBER << TABLE
      << std::setw(2 * SPACE) << FULLNAME << TABLE
      << std::setw(SPACE) << GROUP << TABLE
      << std::setw(SPACE) << AVERAGE << TABLE
      << std::setw(ACTIVITY.size()) << ACTIVITY << TABLE
      << std::setw(SPACE) << WAGES << TABLE << '\n' << LINE;
  return true;
}

bool Database::PrintAllStudents(std::ostream& out) {
  const int NUMBER_SPACE = 4;
  const int SPACE = 16;
  const std::string TABLE = " | ";
  const std::string ACTIVITY = "Activity";
  const std::string MINIMAL_WAGES_INFO = "\nMinimal wages: ";
  const std::string LINE = GetStudentsTableLine(SPACE);
  PrintHeaderStudentsTable(out, SPACE, TABLE);
  int cnt = 0;
  for (auto& current: students) {
    out << std::setw(NUMBER_SPACE) << ++cnt << TABLE
        << std::setw(2 * SPACE) << current.GetFullName() << TABLE
        << std::setw(SPACE) << current.GetGroup() << TABLE
        << std::setw(SPACE) << current.GetAverage() << TABLE
        << std::setw(ACTIVITY.size()) << current.GetActivity() << TABLE
        << std::setw(SPACE) << current.GetWagesPerPerson() << TABLE << '\n'
        << LINE;
  }
  std::cout << MINIMAL_WAGES_INFO << MINIMAL_WAGES << "\n";
  return true;
}

bool Database::PrintAllUsers(std::ostream& out,
                             User& current_user,
                             const int SPACE) {
  const int NUMBER_SIZE = 4;
  const std::string TABLE = " | ";
  const std::string USER_NUMBER = "#";
  const std::string USER_USERNAME = "Username";
  const std::string USER_STATUS = "Status";
  if (current_user.GetStatus() == ADMIN_STATUS) {
    out << std::setw(NUMBER_SIZE) << USER_NUMBER << TABLE
        << std::setw(SPACE) << USER_USERNAME << TABLE
        << std::setw(SPACE) << USER_STATUS << TABLE << '\n';
    int cnt = 1;
    for (auto& current: users) {
      out << std::setw(NUMBER_SIZE) << cnt++ << TABLE
          << std::setw(SPACE) << current.GetUsername() << TABLE
          << std::setw(SPACE) << current.GetStatus() << TABLE << '\n';
    }
  }
  return true;
}

int Database::GetAdminStatus(const User &current_user) const {
  if(IsUserAdmin(current_user)) {
    return ADMIN_STATUS;
  } else {
    return ERROR_STATUS;
  }
}

int Database::GetUserStatus(const User& current_user) const {
  if(IsUserAdmin(current_user)) {
    return DEFAULT_USER_STATUS;
  } else {
    return ERROR_STATUS;
  }
}

int Database::GetErrorStatus(const User& current_user) const {
  return ERROR_STATUS;
}

bool Database::PrintUserByIndex(const User& current_user, int index) const {
  if(!IsUserAdmin(current_user) || index >= users.size() || index < 0) {
    return false;
  }
  std::cout << users[index].GetUsername() << ' ' << users[index].GetStatus() << '\n';
  return true;
}

bool Database::PrintStudentByIndex(const User& current_user, int index) const {
  if(!IsUserAdmin(current_user) || index >= students.size() || index < 0) {
    return false;
  }
  const Student &cur = students[index];
  std::cout << cur.GetFullName() << ' ' << cur.GetGroup() << ' ' << cur.GetAverage() << ' ' << cur.GetActivity() << ' ' << cur.GetWagesPerPerson() << '\n';
  return true;
}

bool Database::IsUniqueUsername(const std::string& username) const {
  for (auto& cur: users) {
    if(cur.GetUsername() == username) {
      return false;
    }
  }
  return true;
}

Database::~Database() {
  SaveDatabase("Work");
}