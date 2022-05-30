/**
      ---------------------------------------------------
      |         Made by Shamal Viktor (@j3uktop)        |
      |           2019-2021 - Lyceum BSU - IM           |
      |             2021 - BSUIR - FCP - PMS            |
      |          *** All rights reserved ***            |
      ---------------------------------------------------
                                            			**/
#pragma once
#include "student.h"
#include "user.h"
#include <vector>

class Database {
 private:
  // public:
  const long long ENCRYPT_CONST_STUDENTS = 6171494297548927679ll;
  const long long ENCRYPT_CONST_USERS = 2343948959754927671ll;
  const int ADMIN_STATUS = 100;
  const int DEFAULT_USER_STATUS = 1;
  const int ERROR_STATUS = -1;
  double MINIMAL_WAGES = 457;
  std::vector<Student> students;
  std::vector<User> users;
  bool EncryptDatabaseStudents(const std::string& filename);
  bool EncryptDatabaseUsers(const std::string& filename);
  bool DecryptDatabaseStudents(const std::string& filename);
  bool DecryptDatabaseUsers(const std::string& filename);
  int FindUserAndGetStatus(const User& curr);
  int FindUserAndGetStatus(const std::string& username,
                           const std::string& password);
  bool PrintHeaderStudentsTable(std::ostream& out, const int SPACE, const std::string& TABLE) const;
  bool IsUniqueUsername(const std::string& username) const;
 public:
  const int SORT_BY_NAME = 1;
  const int SORT_BY_PRIORITY = 2;
  const int SORT_BY_AVERAGE = 3;
  const int SORT_BY_GROUP = 4;
  const int SORT_BY_USERNAME = 5;

  Database();
  Database(Database& second);
  Database(const std::string& filename);
  ~Database();
  bool OpenDatabase(const std::string& filename);
  bool SaveDatabase(const std::string& filename);
  bool AddStudent(const std::string& student_surname,
                  const std::string& student_name,
                  const std::string& student_second_name,
                  const std::string& student_group,
                  double student_average,
                  bool student_activity,
                  double student_wages_per_person,
                  User& current_user);
  bool AddUser(const std::string& new_username,
               const std::string& new_password,
               int new_status,
               User& current_user);
  bool DeleteStudent(int number, User& current_user);
  bool DeleteUser(int number, User& current_user);
  bool UpdateMinimalWages(double new_minimal_wages);
  double GetMinimalWages();
  bool SortStudents(int type, bool is_reverse);
  bool SortUsers(int type, bool is_reverse);
  bool IsUserAdmin(const User& curr_user) const;
  bool SetStatusToUser(User& current);
  bool PrintAllStudents(std::ostream& out);
  bool PrintAllUsers(std::ostream& out,
                     User& current_user,
                     const int SPACE);
  bool PrintUserByIndex(const User& current_user, int index) const;
  bool PrintStudentByIndex(const User& current_user, int index) const;
  bool PrintStudentsByName(std::ostream& out, const std::string& name) const;
  bool PrintStudentsByGroup(std::ostream& out, const std::string& group) const;
  int GetAdminStatus(const User &current_user) const;
  int GetUserStatus(const User &current_user) const;
  int GetErrorStatus(const User &current_user) const;
};