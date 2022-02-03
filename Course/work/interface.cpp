/**
      ---------------------------------------------------
      |         Made by Shamal Viktor (@j3uktop)        |
      |           2019-2021 - Lyceum BSU - IM           |
      |             2021 - BSUIR - FCP - PMS            |
      |          *** All rights reserved ***            |
      ---------------------------------------------------
                                            			**/

#include "interface.h"
#include <conio.h>

bool Interface::Start() {
  const std::string database_file = "Work";

  if (!current_database.OpenDatabase(database_file)) {
    std::cout << "ERROR" << '\n';
    return false;
  }
  system("cls");
  while (true) {
    if (!AuthorizationMenu()) {
      system("cls");
      std::cout << LoginError() << '\n';
      continue;
    }
    if (!MainMenu()) {
      break;
    }
  }
  current_database.SaveDatabase(database_file);
  return true;
}

std::string Interface::LoginError() {
  return POSSIBLE_LOGIN_ERRORS + USER_EXIST_ERROR + WRONG_ENTER_ERROR + USERNAME_ERROR + PASSWORD_ERROR;
}

std::string EnterPassword() {
  std::string pass;
  char symbol;
  const char BACK_SPACE = 8;
  const char ENTER = 13;
  while (true) {
    symbol = getch();
    if (symbol == ENTER) {
      break;
    } else if (symbol == BACK_SPACE) {
      if (pass.size() > 0) {
        pass.pop_back();
        std::cout << "\b \b";
      }
    } else if (symbol > 31) {
      std::cout << "*";
      pass += symbol;
    } else {
      std::cout << "\b \b";
    }
  }
  std::cout << '\n';
  return pass;
}

bool Interface::AuthorizationMenu() {
  std::cout << LOGIN_USERNAME;
  std::string current_user_username;
  std::cin >> current_user_username;

  std::cout << LOGIN_PASSWORD;
  std::string current_user_password = EnterPassword();

  current_user.CreateUser(current_user_username, current_user_password);
  return current_database.SetStatusToUser(current_user);;
}

bool Interface::StudentsMenu() {
  system("cls");
  std::cout << "This is a students menu\n";
  system("pause");
  return true;
}

bool Interface::UsersMenu() {
  system("cls");
  std::cout << "This is a users menu\n";
  system("pause");
  return true;
}

bool Interface::SettingsMenu() {
  system("cls");
  std::cout << "This is a setting menu\n";
  system("pause");
  return true;
}

bool Interface::MainMenu() {
  char user_answer;
  while (true) {
    system("cls");
    std::cout << "Enter 0 to exit\n";
    std::cout << "Enter 1 to show students\n";
    std::cout << "Enter 2 to show settings\n";
    if (current_database.IsUserAdmin(current_user)) {
      std::cout << "Enter 3 to show users\n";
    }
    user_answer = getche();
    if (user_answer == '0') {
      system("cls");
      std::cout << "Enter 0 to close program\n";
      std::cout << "Enter 1 to logout\n";
      std::cout << "Enter another to back\n";
      user_answer = getche();
      if (user_answer == '0') {
        system("cls");
        return false;
      } else if (user_answer == '1') {
        system("cls");
        return true;
      }
    } else if (user_answer == '1') {
      StudentsMenu();
    } else if (user_answer == '2') {
      SettingsMenu();
    } else if (user_answer == '3' && current_database.IsUserAdmin(current_user)) {
      UsersMenu();
    }
  }
}
