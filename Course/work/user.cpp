/**
      ---------------------------------------------------
      |         Made by Shamal Viktor (@j3uktop)        |
      |           2019-2021 - Lyceum BSU - IM           |
      |             2021 - BSUIR - FCP - PMS            |
      |          *** All rights reserved ***            |
      ---------------------------------------------------
                                            			**/
#include "user.h"

User::User() {
  password = 0;
  status = 0;
}

User::User(const std::string& new_username, const std::string& new_password, int new_status) {
  CreateUser(new_username, new_password, new_status);
}

bool User::CreateUser(const std::string& new_username, const std::string& new_password, int new_status) {
  if(!IsUsernameCorrect(new_username) || !IsPasswordCorrect(new_password)) {
    return false;
  }
  username = new_username;
  password = GetPassword(new_password);
  status = new_status;
  return true;
}

bool User::CreateUser(const std::string& new_username, const std::string& new_password) {
  if(!IsUsernameCorrect(new_username) || !IsPasswordCorrect(new_password)) {
    return false;
  }
  username = new_username;
  password = GetPassword(new_password);
  return true;
}

bool User::CreateUser(const std::string& new_username, long long new_password, int new_status) {
  if(!IsUsernameCorrect(new_username)) {
    return false;
  }
  username = new_username;
  password = new_password;
  status = new_status;
  return true;
}

User::User(const User& copy) {
  *this = copy;
}

void User::swap(User& second) {
  std::swap(password, second.password);
  std::swap(status, second.status);
  username.swap(second.username);
}

// Получение "сырого" числового хеша из строки
long long User::CreateHashPassword(const std::string& new_password) const {
  const long long hash_const_first = 958038479;
  const long long hash_const_second = 1013572099;
  long long hash_cur_val = hash_const_first;
  long long hash_first = 0;
  long long hash_second = 0;
  for (auto symbol: new_password) {
    hash_first += hash_cur_val * symbol;
    hash_cur_val *= hash_const_first;
  }
  for (auto symbol: new_password) {
    hash_second += hash_cur_val * symbol;
    hash_cur_val *= hash_const_second;
  }
  return hash_first * (11 * hash_const_second - 7 * hash_const_first) + hash_second * (13 * hash_const_first + 5 * hash_const_second);
}

// Дополнение пароля солью
std::string User::GetSaltPassword(std::string new_password) const {
  srand(CreateHashPassword(new_password));
  const int SALT_SIZE = 15;
  for(int cur_salt = 0; cur_salt < SALT_SIZE; ++cur_salt) {
    new_password += char(rand() % 106 + 21);
  }
  return new_password;
}

// Окончательное получение хеша
long long User::GetPassword(const std::string& new_password) const {
  return CreateHashPassword(GetSaltPassword(new_password));
}

long long User::GetPassword() const {
  return password;
}

bool User::CheckPassword(const std::string& new_password) const {
  return (password == GetPassword(new_password));
}

bool User::CheckPassword(long long new_password) const {
  return password == new_password;
}

bool User::CheckUsername(const std::string& new_username) const {
  return new_username == username;
}

bool User::CheckUser(const std::string& new_username, const std::string& new_password) const {
  return (CheckUsername(new_username) && CheckPassword(new_password));
}

bool User::ChangeUsername(const std::string& new_username) {
  username = new_username;
  return true;
}

bool User::ChangePassword(const std::string& new_password) {
  password = GetPassword(new_password);
  return true;
}

bool User::ChangePassword(long long new_password) {
  password = new_password;
  return true;
}

bool User::ChangeStatus(int new_status) {
  status = new_status;
  return true;
}

int User::GetStatus() const {
  return status;
}

std::string User::GetUsername() const {
  return username;
}

bool IsLetter(char symbol) {
  return ((symbol >= 'a' && symbol <= 'z') || (symbol >= 'A' && symbol <= 'Z'));
}

bool IsDigit(char symbol) {
  return (symbol >= '0' && symbol <= '9');
}

bool User::IsUsernameCorrect(const std::string& new_username) {
  if(new_username.size() > MAX_USERNAME_SIZE) {
    return false;
  }
  for(auto& symbol: new_username) {
    if(!IsLetter(symbol) && !IsDigit(symbol)) {
      return false;
    }
  }
  return true;
}

bool User::IsPasswordCorrect(const std::string& new_password) {
  return new_password.size() <= MAX_PASSWORD_SIZE;
}

User& User::operator=(const User& copy) {
  username = copy.username;
  password = copy.password;
  status = copy.status;
  return *this;
}

bool User::operator==(const User& second) {
  return (CheckPassword(second.GetPassword()) && CheckUsername(second.GetUsername()));
}