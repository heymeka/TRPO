/**
      ---------------------------------------------------
      |         Made by Shamal Viktor (@j3uktop)        |
      |           2019-2021 - Lyceum BSU - IM           |
      |             2021 - BSUIR - FCP - PMS            |
      |          *** All rights reserved ***            |
      ---------------------------------------------------
                                            			**/

#ifndef INTERFACE_H
#define INTERFACE_H
#include "database.h"

class Interface {
 private:
  Database current_database;
  User current_user;
  const std::string LOGIN_USERNAME = "username: ";
  const std::string LOGIN_PASSWORD = "password: ";
  const std::string POSSIBLE_LOGIN_ERRORS = "Check one of this possible errors:\n";
  const std::string USER_EXIST_ERROR = "The user does not exist\n";
  const std::string WRONG_ENTER_ERROR = "Incorrect username or password\n";
  const std::string USERNAME_ERROR = "The username must be no more than 32 characters and consist ONLY of numbers or letters\n";
  const std::string PASSWORD_ERROR = "The password must be no more than 32 characters\n";
  std::string LoginError();
 public:
  bool Start();
  bool AuthorizationMenu();
  bool StudentsMenu();
  bool UsersMenu();
  bool MainMenu();
  bool SettingsMenu();
};

#endif //INTERFACE_H
