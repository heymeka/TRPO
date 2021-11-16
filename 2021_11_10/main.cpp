/**
      ---------------------------------------------------
      |         Made by Shamal Viktor (@j3uktop)        |
      |           2019-2021 - Lyceum BSU - IM           |
      |             2021 - BSUIR - FCP - PMS            |
      |          *** All rights reserved ***            |
      ---------------------------------------------------
                                            			**/
#include <iostream>
#include <map>
#include <iomanip>
#include <algorithm>
#include <ctime>

using std::cin;
using std::cout;
using std::string;
using std::map;
using std::setw;
using std::sort;

const string ERROR_MESSAGE[] = {"Num have to be not negative ( >= 0)\n",
                           "The date of birth most be before the current moment\n"};
enum ERRORS { IS_NEGATIVE, BIRTH_ERROR };

struct FullName {
  string name = "Ivan";
  string surname = "Ivanov";
  string patronymic = "Ivanovich";
};

std::ostream& operator<<(std::ostream& to_out, const FullName& value) {
  to_out << value.surname << " " << value.name << " " << value.patronymic;
  return to_out;
}

struct Date {
  int day = 0;
  int month = 0;
  int year = 0;
};

std::ostream& operator<<(std::ostream& to_out, const Date& out_date) {
  if (out_date.day < 10) {
    to_out << "0";
  }
  to_out << out_date.day << ".";
  if (out_date.month < 10) {
    to_out << "0";
  }
  to_out << out_date.month << "." << out_date.year;
  return to_out;
}

struct student {
  FullName full_name;
  Date birth;
  int marks_count = 0;
  int* marks = nullptr;
};

void workWithStudents();

student* createStudentArray(int& size);

void deleteStudentsArray(student* p_student, int& size);

void printStudents(student* p_student, int& size);

string findMostPopularName(student* p_student, int size);

double getAverageAge(student* p_student, int size);

void workWithBMP();

/////////////////////////////////////////////////////////////////////
int main() {
  // workWithStudents();
  workWithBMP();
  system("pause");
  return 0;
}
/////////////////////////////////////////////////////////////////////

int getImageWidth(FILE* IMAGE) {
#define WIDTH_OFFSET 0x0012
  int image_width;
  fseek(IMAGE, WIDTH_OFFSET, SEEK_SET);
  fread(&image_width, 4, 1, IMAGE);
#undef WIDTH_OFFSET
  return image_width;
}

int getImageHeight(FILE* IMAGE) {
#define HEIGHT_OFFSET 0x0016
  int image_height;
  fseek(IMAGE, HEIGHT_OFFSET, SEEK_SET);
  fread(&image_height, 4, 1, IMAGE);
#undef HEIGHT_OFFSET
  return image_height;
}

int getImageDataOffset(FILE* IMAGE) {
#define HEADER_OFFSET 0x000A
  fseek(IMAGE, HEADER_OFFSET, SEEK_SET);
  int image_data_offset;
  fread(&image_data_offset, 4, 1, IMAGE);
#undef HEADER_OFFSET
  return image_data_offset;
}

int getImageBitsPerPixel(FILE* IMAGE) {
#define BITS_PER_PIXEL_OFFSET 0x001C
  int bits_per_pixel;
  fseek(IMAGE, BITS_PER_PIXEL_OFFSET, SEEK_SET);
  fread(&bits_per_pixel, 2, 1, IMAGE);
#undef BITS_PER_PIXEL_OFFSET
  return bits_per_pixel;
}

void workWithBMP() {
  const string IMAGE_NAME = "sample.bmp";
  FILE* IMAGE = fopen(IMAGE_NAME.c_str(), "rb");
  int image_data_offset = getImageDataOffset(IMAGE);
  int image_width = getImageWidth(IMAGE);
  int image_height = getImageHeight(IMAGE);
  int bits_per_pixel = getImageBitsPerPixel(IMAGE);
  cout << "Data offset = " << image_data_offset << '\n';
  cout << "Width = " << image_width << '\n';
  cout << "Height = " << image_height << '\n';
  cout << "Bits per pixel = " << bits_per_pixel << '\n';
}

bool studentsByName(student& first, student& second) {
  if (first.full_name.surname != second.full_name.surname) {
    return first.full_name.surname < second.full_name.surname;
  } else if (first.full_name.name != second.full_name.name) {
    return first.full_name.name < second.full_name.name;
  } else {
    return first.full_name.patronymic < second.full_name.patronymic;
  }
}

void workWithStudents() {
  int students_count;
  student* students_list = createStudentArray(students_count);

  cout << "Students:\n";
  printStudents(students_list, students_count);

  cout << "Most popular name: ";
  cout << findMostPopularName(students_list, students_count) << '\n';

  sort(students_list, students_list + students_count, studentsByName);
  cout << "Students after sort by name:\n";
  printStudents(students_list, students_count);

  double average_age = getAverageAge(students_list, students_count);
  cout << "Average age = " << average_age << '\n';

  deleteStudentsArray(students_list, students_count);
}

Date getCurrentDate() {
  std::time_t current_time = std::time(nullptr);
  std::tm* now = std::localtime(&current_time);
  Date current_date;

  current_date.year = 1900 + now->tm_year;
  current_date.month = 1 + now->tm_mon;
  current_date.day = now->tm_mday;
  return current_date;
}

int getAge(Date birth, Date current) {
  if (current.year < birth.year || (current.year == birth.year
      && (current.month < birth.month
          || (current.month == birth.month && current.day < birth.day)))) {
    cout << ERROR_MESSAGE[BIRTH_ERROR];
    return INT_MIN;
  }
  int age = current.year - birth.year;
  if (birth.month > current.month
      || (birth.month == current.month && birth.day >= current.day)) {
    age--;
  }
  age = std::max(age, 0);
  return age;
}

double getAverageAge(student* p_student, int size) {
  int age_sum = 0;
  Date current_date = getCurrentDate();
  for (int index = 0; index < size; ++index) {
    age_sum += getAge(p_student[index].birth, current_date);
  }
  return 1. * age_sum / size;
}

string findMostPopularName(student* p_student, int size) {
  map<string, int> names;
  for (int index = 0; index < size; ++index) {
    names[p_student[index].full_name.name]++;
  }
  string most_popular = names.begin()->first;
  int max_ = 0;
  // C++17
  // for (auto&[name, value]: names) {
  //   if (value > max_) {
  //     max_ = value;
  //     most_popular = name;
  //   }
  // }
  // C++11
  for (auto& value: names) {
    if (value.second > max_) {
      max_ = value.second;
      most_popular = value.first;
    }
  }
  return most_popular;
}

student* createStudentArray(int& size) {
  do {
    cout << ERROR_MESSAGE[IS_NEGATIVE];
    cout << "Enter students count = ";
    cin >> size;
  } while (size < 0);
  auto* new_students = new student[size];

  cout << "Enter students info:\n";
  for (int student_index = 0; student_index < size; ++student_index) {
    cout << "Enter full FullName (Surname Name Patronymic): ";
    cin >> new_students[student_index].full_name.surname
        >> new_students[student_index].full_name.name
        >> new_students[student_index].full_name.patronymic;

    cout << "Enter full date of birth (Day Month Year): ";
    cin >> new_students[student_index].birth.day
        >> new_students[student_index].birth.month
        >> new_students[student_index].birth.year;

    cout << "Enter count of student's marks: ";
    cin >> new_students[student_index].marks_count;
    new_students[student_index].marks =
        new int[new_students[student_index].marks_count];

    cout << "Enter " << new_students[student_index].marks_count
         << " marks of this student\n";
    for (int mark_index = 0;
         mark_index < new_students[student_index].marks_count; ++mark_index) {
      cin >> new_students[student_index].marks[mark_index];
    }
  }
  return new_students;
}

void printStudents(student* p_student, int& size) {
  cout << size << " students:\n";
  for (int index = 0; index < size; ++index) {
    cout << setw(30) << p_student[index].full_name << setw(12)
         << p_student[index].birth;
    cout << "   Marks: ";
    for (int mark_index = 0; mark_index < p_student[index].marks_count;
         ++mark_index) {
      cout << p_student[index].marks[mark_index] << ' ';
    }
    cout << '\n';
  }
}

void deleteStudentsArray(student* p_student, int& size) {
  for (int index = 0; index < size; ++index) {
    delete[] p_student[index].marks;
  }
  delete[] p_student;
  size = 0;
}