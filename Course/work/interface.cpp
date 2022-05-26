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
#include <iomanip>

bool Interface::Start() {
  const std::string database_file = "Work";

  if (!current_database.OpenDatabase(database_file)) {
    std::cout << DATABASE_OPEN_ERROR;
    system("pause");
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
  return LOGIN_ERROR_MAIN + LOGIN_EXIST_ERROR + LOGIN_ENTER_ERROR
      + LOGIN_USERNAME_ERROR + LOGIN_PASSWORD_ERROR;
}

std::string EnterPassword() {
  std::string pass;
  char symbol;
  const char BACKSPACE_CODE = 8;
  const char ENTER_CODE = 13;
  const char ENCRYPTED_SYMBOL = '*';
  const std::string BACKSPACE_FOR_CONSOLE = "\b \b";
  while (true) {
    symbol = getch();
    if (symbol == ENTER_CODE) {
      break;
    } else if (symbol == BACKSPACE_CODE) {
      if (pass.size() > 0) {
        pass.pop_back();
        std::cout << BACKSPACE_FOR_CONSOLE;
      }
    } else if (symbol > 31) {
      std::cout << ENCRYPTED_SYMBOL;
      pass += symbol;
    } else {
      std::cout << BACKSPACE_FOR_CONSOLE;
    }
  }
  std::cout << std::endl;
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

bool Interface::StudentsSortMenu() {
  system("cls");
  std::cout << MENU_EXIT;
  std::cout << STUDENTS_SORT_NAME;
  std::cout << STUDENTS_SORT_AVERAGE;
  std::cout << STUDENTS_SORT_GROUP;
  std::cout << STUDENTS_SORT_PRIORITY;
  char user_answer = getch();
  if(user_answer == MENU_EXIT_ANSWER) {
    return true;
  } else if(user_answer == STUDENTS_SORT_NAME_ANSWER) {
    return current_database.SortStudents(current_database.SORT_BY_NAME, ReverseSortMenu());
  } else if(user_answer == STUDENTS_SORT_AVERAGE_ANSWER) {
    return current_database.SortStudents(current_database.SORT_BY_AVERAGE, ReverseSortMenu());
  } else if(user_answer == STUDENTS_SORT_GROUP_ANSWER) {
    return current_database.SortStudents(current_database.SORT_BY_GROUP, ReverseSortMenu());
  } else if(user_answer == STUDENTS_SORT_PRIORITY_ANSWER) {
    return current_database.SortStudents(current_database.SORT_BY_PRIORITY, ReverseSortMenu());
  }
  return false;
}

bool Interface::StudentChangeMenu() {
  system("cls");
  if (!current_database.PrintAllStudents(std::cout)) {
    system("pause");
    return false;
  }
  std::cout << MENU_EXIT;
  std::cout << POSITION_CHANGE_MENU;
  int user_answer;
  std::cin >> user_answer;
  if(user_answer == MENU_EXIT_ANSWER) {
    return false;
  }
  if (StudentsAddMenu()) {
    return current_database.DeleteStudent(user_answer - 1, current_user);
  }
  return false;
}

bool Interface::StudentsAddMenu() {
  system("cls");
  std::cout << MENU_EXIT;
  std::cout << ANOTHER_TO_CONTINUE;
  std::cout << '\n';
  char user_answer = getch();
  if (user_answer == MENU_EXIT_ANSWER) {
    return false;
  }
  std::string surname;
  std::string name;
  std::string second_name;
  std::string group;
  double average;
  bool activity;
  double wages_per_person;

  const int ENTER_SPACE = 16;
  std::cout << ENTER_ONLY_LETTERS;
  std::cout << std::setw(ENTER_SPACE) << STUDENTS_ADD_SURNAME;
  std::cin >> surname;
  std::cout << std::setw(ENTER_SPACE) << STUDENTS_ADD_NAME;
  std::cin >> name;
  std::cout << std::setw(ENTER_SPACE) << STUDENTS_ADD_SECOND_NAME;
  std::cin >> second_name;
  std::cout << ENTER_CORRECT_GROUP;
  std::cout << std::setw(ENTER_SPACE) << STUDENTS_ADD_GROUP;
  std::cin >> group;
  std::cout << ENTER_CORRECT_AVERAGE;
  std::cout << std::setw(ENTER_SPACE) << STUDENTS_ADD_AVERAGE;
  std::cin >> average;
  std::cout << ENTER_CORRECT_ACTIVITY;
  std::cout << std::setw(ENTER_SPACE) << STUDENTS_ADD_ACTIVITY;
  std::cin >> activity;
  std::cout << ENTER_CORRECT_WAGES;
  std::cout << std::setw(ENTER_SPACE) << STUDENTS_ADD_WAGES;
  std::cin >> wages_per_person;

  return current_database.AddStudent(surname,name,
                                     second_name,group,
                                     average,activity,
                                     wages_per_person,
                                     current_user);
}

bool Interface::StudentsDeleteMenu() {
  system("cls");
  if (!current_database.PrintAllStudents(std::cout)) {
    system("pause");
    return false;
  }
  std::cout << MENU_EXIT;
  std::cout << POSITION_DELETE_MENU;
  std::cout << '\n';
  int user_answer;
  std::cin >> user_answer;
  return current_database.DeleteStudent(user_answer - 1, current_user);
}

bool Interface::StudentsWagesMenu() {
  system("cls");
  std::cout << "Current minimal wages: " << current_database.GetMinimalWages()
            << '\n';
  std::cout << "Enter new value: ";
  double new_wages;
  std::cin >> new_wages;
  return current_database.UpdateMinimalWages(new_wages);
}

bool Interface::StudentsMenu() {
  system("cls");

  int STUDENTS_TABLE_SPACE = 16;
  bool is_current_admin = current_database.IsUserAdmin(current_user);
  while (true) {
    if (!current_database.PrintAllStudents(std::cout)) {
      system("pause");
      return false;
    }
    std::cout << '\n';
    std::cout << MENU_EXIT;
    std::cout << STUDENTS_MENU_SORT;
    std::cout << STUDENTS_MENU_WAGES;
    if(is_current_admin) {
      std::cout << STUDENTS_MENU_CHANGE;
      std::cout << STUDENTS_MENU_ADD;
      std::cout << STUDENTS_MENU_DELETE;
    }
    std::cout << '\n';
    char user_answer = getch();
    if (user_answer == STUDENTS_SORT_ANSWER) {
      StudentsSortMenu();
    } else if (user_answer == STUDENTS_WAGES_ANSWER) {
      StudentsWagesMenu();
    } else if (user_answer == MENU_EXIT_ANSWER) {
      break;
    } else if(is_current_admin) {
      if (user_answer == STUDENTS_CHANGE_ANSWER) {
        StudentChangeMenu();
      } else if (user_answer == STUDENTS_ADD_ANSWER) {
        StudentsAddMenu();
      } else if (user_answer == STUDENTS_DELETE_ANSWER) {
        StudentsDeleteMenu();
      }
    }

    system("cls");
  }

  system("pause");
  return true;
}

bool Interface::ReverseSortMenu() {
  system("cls");

  std::cout << REVERSE_SORT_MENU_REVERSE;
  std::cout << REVERSE_SORT_MENU_DIR;
  char user_answer = getche();
  system("cls");
  return user_answer == REVERSE_SORT_ANSWER;
};

bool Interface::UsersSortMenu() {
  system("cls");
  std::cout << MENU_EXIT;
  std::cout << USERS_SORT_USERNAME;
  std::cout << USERS_SORT_PRIORITY;
  char user_answer = getch();
  if (user_answer == USERS_SORT_USERNAME_ANSWER) {
    return current_database.SortUsers(current_database.SORT_BY_USERNAME,
                                      ReverseSortMenu());
  } else if (user_answer == USERS_SORT_PRIORITY_ANSWER) {
    return current_database.SortUsers(current_database.SORT_BY_PRIORITY,
                                      ReverseSortMenu());
  } else {
    return false;
  }
}

bool Interface::UsersDeleteMenu() {
  system("cls");
  if (!current_database.PrintAllUsers(std::cout,
                                      current_user,
                                      USERS_TABLE_SPACE)) {
    system("pause");
    return false;
  }
  std::cout << MENU_EXIT;
  std::cout << POSITION_DELETE_MENU;
  std::cout << '\n';
  int user_answer;
  std::cin >> user_answer;
  return current_database.DeleteUser(user_answer - 1, current_user);
}

bool Interface::UsersAddMenu() {
  system("cls");
  std::cout << MENU_EXIT;
  std::cout << ANOTHER_TO_CONTINUE;
  std::cout << '\n';
  char user_answer = getch();
  if (user_answer == MENU_EXIT_ANSWER) {
    return false;
  }
  std::string username;
  std::string password;
  std::cout << LOGIN_USERNAME;
  std::cin >> username;
  do {
    std::cout << LOGIN_PASSWORD;
    password = EnterPassword();
    std::cout << "Repeat " << LOGIN_PASSWORD;
  } while (password != EnterPassword());
  int status;
  std::cout << ENTER << current_database.GetAdminStatus(current_user)
            << " to set admin status\n";
  std::cout << ENTER << current_database.GetUserStatus(current_user)
            << " to set user status\n";
  std::cout << ENTER << current_database.GetErrorStatus(current_user)
            << " to set error status\n";
  std::cout << USERS_ADD_STATUS;
  std::cin >> status;
  return current_database.AddUser(username, password, status, current_user);
}

bool Interface::UsersChangeMenu() {
  system("cls");
  if (!current_database.PrintAllUsers(std::cout,
                                      current_user,
                                      USERS_TABLE_SPACE)) {
    system("pause");
    return false;
  }
  std::cout << MENU_EXIT;
  std::cout << POSITION_CHANGE_MENU;
  int user_answer;
  std::cin >> user_answer;
  if(user_answer == MENU_EXIT_ANSWER) {
    return false;
  }
  if (UsersAddMenu()) {
    return current_database.DeleteUser(user_answer - 1, current_user);
  }
  return false;
}

bool Interface::UsersMenu() {
  while (true) {
    system("cls");
    if (!current_database.PrintAllUsers(std::cout,
                                        current_user,
                                        USERS_TABLE_SPACE)) {
      system("pause");
      return false;
    }
    std::cout << std::endl;
    std::cout << MENU_EXIT;
    std::cout << USERS_MENU_SORT;
    std::cout << USERS_MENU_CHANGE;
    std::cout << USERS_MENU_ADD;
    std::cout << USERS_DELETE;
    std::cout << std::endl;
    char user_answer = getch();
    if (user_answer == USERS_SORT_ANSWER) {
      UsersSortMenu();
    } else if (user_answer == USERS_CHANGE_ANSWER) {
      UsersChangeMenu();
    } else if (user_answer == USERS_ADD_ANSWER) {
      UsersAddMenu();
    } else if (user_answer == USERS_DELETE_ANSWER) {
      UsersDeleteMenu();
    } else if (user_answer == MENU_EXIT_ANSWER) {
      break;
    } else {
      system("cls");
    }
  }
  system("pause");
  return true;
}

bool Interface::MainMenu() {
  char user_answer;
  while (true) {
    system("cls");
    std::cout << MENU_EXIT;
    std::cout << MAIN_MENU_STUDENTS;
    if (current_database.IsUserAdmin(current_user)) {
      std::cout << MAIN_MENU_USERS;
    }
    user_answer = getch();
    if (user_answer == MENU_EXIT_ANSWER) {
      system("cls");
      std::cout << MAIN_MENU_EXIT;
      std::cout << MAIN_MENU_LOGOUT;
      std::cout << MAIN_MENU_GO_BACK;
      user_answer = getch();
      if (user_answer == MENU_EXIT_ANSWER) {
        system("cls");
        return false;
      } else if (user_answer == MAIN_MENU_LOGOUT_ANSWER) {
        system("cls");
        return true;
      }
    } else if (user_answer == MAIN_MENU_STUDENTS_ANSWER) {
      StudentsMenu();
    } else if (user_answer == MAIN_MENU_USERS_ANSWER
        && current_database.IsUserAdmin(current_user)) {
      UsersMenu();
    }
  }
}
