#include <iostream>
#include <algorithm>
using std::string;
using std::cin;
using std::cout;

string getStringFromValue(int value, int numbers_count = (1 << 27));
string getStringFromValue(double value);
int countOfArrayOccurrences(string& array, char value);
int countOfArrayOccurrences(int* array, int array_size, int value);
int countOfArrayOccurrences(long long* array, int array_size, long long value);
int countOfArrayOccurrences(char* array, int array_size, char value);
int countOfArrayOccurrences(double* array, int array_size, double value);

void testTaskValues();
void testTaskArray();

int main() {
  testTaskValues();
  system("pause");
  return 0;
}

void testTaskValues() {
  cout << "Task 1 + Task 2 test (value to string)\n\n";

  int int_value;
  cout << "Test int value = ";
  cin >> int_value;
  cout << "String value = " << getStringFromValue(int_value) << '\n';

  int digits_count;
  cout << "Test int value = ";
  cin >> int_value;
  cout << "Digits to output = ";
  cin >> digits_count;
  cout << "String value = " << getStringFromValue(int_value, digits_count) << '\n';


  double double_value;
  cout << "Test double value = ";
  cin >> double_value;
  cout << "String value = " << getStringFromValue(double_value) << '\n';
  cout << "Task 1 + Task 2 is completed\n\n";
}

void testTaskArray() {
  //
}

string getStringFromValue(int value, int numbers_count) {
  string raw_value;
  if (value == 0) {
    raw_value = "0";
    return raw_value;
  }
  bool is_negative = (value < 0);
  if (value < 0) {
    value *= -1;
  }
  while (value != 0 && numbers_count > 0) {
    raw_value += '0' + value % 10;
    value /= 10;
    numbers_count--;
  }

  if (is_negative) {
    raw_value += "-";
  }
  reverse(raw_value.begin(), raw_value.end());
  return raw_value;
}

string getStringFromValue(double value) {
  string raw_value;

  if (value == 0) {
    raw_value = "0";
    return raw_value;
  }

  bool is_negative = false;
  if (value < 0) {
    value *= -1;
    is_negative = true;
  }
  const double MIN_VALUE = 1;
  const double MAX_VALUE = 10;
  int last_zero_count = 0;
  int first_zero_count = 0;
  while (value < MIN_VALUE) {
    value *= 10;
    first_zero_count++;
  }
  while (value >= MAX_VALUE) {
    value /= 10;
    last_zero_count++;
  }

  int digit;
  int digits_count = 17;
  while (digits_count != 0) {
    digit = value;
    raw_value += digit + '0';
    value = (value - digit) * 10;
    digits_count--;
  }
  string first_zeroes;
  while (first_zero_count != 0) {
    first_zeroes += "0";
    first_zero_count--;
  }
  string last_zeroes;
  int dot_index = 1 + last_zero_count;
  while (last_zero_count != 0) {
    last_zeroes += "0";
    last_zero_count--;
  }
  raw_value = first_zeroes + raw_value + last_zeroes;
  raw_value.insert(dot_index, ".");
  if (is_negative) {
    raw_value.insert(0, "-");
  }
  return raw_value;
}

int countOfArrayOccurrences(string& array, char value) {
  int occurrences = 0;
  for (auto item: array) {
    if (item == occurrences) {
      occurrences++;
    }
  }
  return occurrences;
}

int countOfArrayOccurrences(int* array, int array_size, int value) {
  int occurrences = 0;
  for(int item_index = 0; item_index < array_size; item_index++) {
    if (array[item_index] == value) {
      occurrences++;
    }
  }
  return occurrences;
}

int countOfArrayOccurrences(long long* array, int array_size, long long value) {
  int occurrences = 0;
  for(int item_index = 0; item_index < array_size; item_index++) {
    if (array[item_index] == value) {
      occurrences++;
    }
  }
  return occurrences;
}

int countOfArrayOccurrences(char* array, int array_size, char value) {
  int occurrences = 0;
  for(int item_index = 0; item_index < array_size; item_index++) {
    if (array[item_index] == value) {
      occurrences++;
    }
  }
  return occurrences;
}

int countOfArrayOccurrences(double* array, int array_size, double value) {
  int occurrences = 0;
  for(int item_index = 0; item_index < array_size; item_index++) {
    if (array[item_index] == value) {
      occurrences++;
    }
  }
  return occurrences;
}