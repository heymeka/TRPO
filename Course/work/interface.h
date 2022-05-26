/**
      ---------------------------------------------------
      |         Made by Shamal Viktor (@j3uktop)        |
      |           2019-2021 - Lyceum BSU - IM           |
      |             2021 - BSUIR - FCP - PMS            |
      |          *** All rights reserved ***            |
      ---------------------------------------------------
                                            			**/

#pragma once
#include "database.h"

class Interface {
 private:
  Database current_database;
  User current_user;
  //////// Main
  const char MENU_EXIT_ANSWER = '0';
  const std::string ENTER = "Enter ";
  const std::string MENU_EXIT = ENTER + MENU_EXIT_ANSWER + " to exit menu\n";
  const std::string ANOTHER_TO_CONTINUE = "Enter another to continue\n";
  const std::string POSITION_DELETE_MENU = "Enter position to delete\n";
  const std::string POSITION_CHANGE_MENU = "Enter position to change\n";

  const std::string ENTER_ONLY_LETTERS = "(Enter only letters)\n";
  const std::string ENTER_CORRECT_GROUP = "(Enter 6 digits, example: 123456)\n";
  const std::string ENTER_CORRECT_AVERAGE = "(Enter 0..10)\n";
  const std::string ENTER_CORRECT_ACTIVITY = "(Enter 0 or 1)\n";
  const std::string ENTER_CORRECT_WAGES = "(Enter > 0)\n";
  //////// Main menu
 private:
  const char MAIN_MENU_STUDENTS_ANSWER = '1';
  const char MAIN_MENU_USERS_ANSWER = '2';
  const char MAIN_MENU_LOGOUT_ANSWER = '1';
  const std::string MAIN_MENU_STUDENTS = ENTER + MAIN_MENU_STUDENTS_ANSWER + " to show students\n";
  const std::string MAIN_MENU_USERS = ENTER + MAIN_MENU_USERS_ANSWER + " to show users\n";
  const std::string MAIN_MENU_EXIT = ENTER + MENU_EXIT_ANSWER + " to close program\n";
  const std::string MAIN_MENU_LOGOUT = ENTER + MAIN_MENU_LOGOUT_ANSWER + " to logout\n";
  const std::string MAIN_MENU_GO_BACK = "Enter another to back\n";
  //////// Login menu
 private:
  const std::string LOGIN_USERNAME = "username: ";
  const std::string LOGIN_PASSWORD = "password: ";

  const std::string
      LOGIN_ERROR_MAIN = "Check one of this possible errors:\n";
  const std::string LOGIN_EXIST_ERROR = "The user does not exist\n";
  const std::string LOGIN_ENTER_ERROR = "Incorrect username or password\n";
  const std::string LOGIN_USERNAME_ERROR =
      "The username must be no more than 32 characters and consist ONLY of numbers or letters\n";
  const std::string
      LOGIN_PASSWORD_ERROR = "The password must be no more than 32 characters\n";
  //////// Database menu
 private:
  const std::string DATABASE_OPEN_ERROR = "Database file error\n";
  //////// Users menu
 private:
  const int USERS_TABLE_SPACE = 16;
  const char USERS_SORT_ANSWER = '1';
  const char USERS_CHANGE_ANSWER = '2';
  const char USERS_ADD_ANSWER = '3';
  const char USERS_DELETE_ANSWER = '4';
  const std::string USERS_MENU_SORT = ENTER + USERS_SORT_ANSWER + " to sort users\n";
  const std::string USERS_MENU_CHANGE = ENTER + USERS_CHANGE_ANSWER + " to change user info\n";
  const std::string USERS_MENU_ADD = ENTER  + USERS_ADD_ANSWER + " to add user\n";
  const std::string USERS_DELETE = ENTER + USERS_DELETE_ANSWER + " to delete user\n";

  const char USERS_SORT_USERNAME_ANSWER = '1';
  const char USERS_SORT_PRIORITY_ANSWER = '2';
  const std::string USERS_SORT_USERNAME = ENTER + USERS_SORT_USERNAME_ANSWER + " to sort users by username\n";
  const std::string USERS_SORT_PRIORITY = ENTER + USERS_SORT_PRIORITY_ANSWER + " to sort users by priority\n";

  const std::string USERS_ADD_STATUS = "Status: ";

  //////// Sort
  const char REVERSE_SORT_ANSWER = '1';
  const std::string REVERSE_SORT_MENU_REVERSE = ENTER + REVERSE_SORT_ANSWER + " to reverse sort\n";
  const std::string REVERSE_SORT_MENU_DIR = "Enter another symbol to direct sort\n";

  //////// Students menu
  const char STUDENTS_SORT_ANSWER = '1';
  const char STUDENTS_WAGES_ANSWER = '2';
  ///
  const char STUDENTS_CHANGE_ANSWER = '4';
  const char STUDENTS_ADD_ANSWER = '5';
  const char STUDENTS_DELETE_ANSWER = '6';

  const std::string STUDENTS_MENU_SORT = ENTER + STUDENTS_SORT_ANSWER + " to sort students\n";
  const std::string STUDENTS_MENU_WAGES = ENTER + STUDENTS_WAGES_ANSWER + " to change minimal wages per person\n";
  ///
  const std::string STUDENTS_MENU_CHANGE = ENTER + STUDENTS_CHANGE_ANSWER + " to change student info\n";
  const std::string STUDENTS_MENU_ADD = ENTER + STUDENTS_ADD_ANSWER + " to add student\n";
  const std::string STUDENTS_MENU_DELETE = ENTER + STUDENTS_DELETE_ANSWER + " to delete student\n";

  const std::string STUDENTS_ADD_SURNAME = "surname: ";
  const std::string STUDENTS_ADD_NAME = "name: ";
  const std::string STUDENTS_ADD_SECOND_NAME = "second name: ";
  const std::string STUDENTS_ADD_GROUP = "group: ";
  const std::string STUDENTS_ADD_AVERAGE = "average: ";
  const std::string STUDENTS_ADD_ACTIVITY = "activity: ";
  const std::string STUDENTS_ADD_WAGES = "wages per person: ";

  const char STUDENTS_SORT_NAME_ANSWER = '1';
  const char STUDENTS_SORT_AVERAGE_ANSWER = '2';
  const char STUDENTS_SORT_GROUP_ANSWER = '3';
  const char STUDENTS_SORT_PRIORITY_ANSWER = '4';
  const std::string STUDENTS_SORT_NAME = ENTER + STUDENTS_SORT_NAME_ANSWER + " to sort students by name\n";
  const std::string STUDENTS_SORT_AVERAGE = ENTER + STUDENTS_SORT_AVERAGE_ANSWER + " to sort students by average\n";
  const std::string STUDENTS_SORT_GROUP = ENTER + STUDENTS_SORT_GROUP_ANSWER + " to sort students by group\n";
  const std::string STUDENTS_SORT_PRIORITY = ENTER + STUDENTS_SORT_PRIORITY_ANSWER + " to sort students by priority in dormitory (wages < 2min, average, activity)\n";
 private:
  ////////
  std::string LoginError();
  ////////
 private:
  bool StudentsSortMenu();
  bool StudentChangeMenu();
  bool StudentsAddMenu();
  bool StudentsDeleteMenu();
  bool StudentsWagesMenu();

  bool AuthorizationMenu();
  bool MainMenu();
  bool StudentsMenu();
  bool UsersMenu();
  bool ReverseSortMenu();

  bool UsersSortMenu();
  bool UsersAddMenu();
  bool UsersChangeMenu();
  bool UsersDeleteMenu();
 public:
  bool Start();
};
