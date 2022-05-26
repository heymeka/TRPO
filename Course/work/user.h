/**
      ---------------------------------------------------
      |         Made by Shamal Viktor (@j3uktop)        |
      |           2019-2021 - Lyceum BSU - IM           |
      |             2021 - BSUIR - FCP - PMS            |
      |          *** All rights reserved ***            |
      ---------------------------------------------------
                                            			**/
#pragma once

#include <iostream>

class User {
 private:
  // public:
  std::string username;
  long long password;
  int status;
  long long CreateHashPassword(const std::string& new_password) const;
  long long GetPassword(const std::string& new_password) const;
  std::string GetSaltPassword(std::string new_password) const;
  bool CheckPassword(const std::string& new_password) const;
  bool CheckPassword(long long new_password) const;
  bool IsUsernameCorrect(const std::string& new_username);
  bool IsPasswordCorrect(const std::string& new_password);
 public:
  const int MAX_USERNAME_SIZE = 32;
  const int MAX_PASSWORD_SIZE = 32;
  User();
  User(const User& copy);
  User(const std::string& new_username, const std::string& new_password, int new_status);
  bool CreateUser(const std::string& new_username, const std::string& new_password, int new_status);
  bool CreateUser(const std::string& new_username, const std::string& new_password);
  bool CreateUser(const std::string& new_username, long long new_password, int new_status);
  void swap(User& second);
  bool CheckUser(const std::string& new_username, const std::string& new_password) const;
  bool CheckUsername(const std::string& new_username) const;
  bool ChangeUsername(const std::string& new_username);
  bool ChangePassword(const std::string& new_password);
  bool ChangePassword(long long new_password);
  bool ChangeStatus(int new_status);
  int GetStatus() const;
  std::string GetUsername() const;
  long long GetPassword() const;
  User& operator=(const User& copy);
  bool operator==(const User& second);
};

