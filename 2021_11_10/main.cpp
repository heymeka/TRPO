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
using std::byte;

const string ERROR_MESSAGE[] = {"Num have to be not negative ( >= 0)\n",
                                "The date of birth most be before the current moment\n",
                                "This is not a BitMap image!\n"};
enum ERRORS { IS_NEGATIVE, BIRTH_ERROR, NOT_BITMAP_IMAGE };

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
  // system("pause");
  return 0;
}
/////////////////////////////////////////////////////////////////////

class MyImage {
  // private:
 public:
  FILE* IMAGE;
  byte** pixels;
  int width;
  int height;
  int real_row_width;
  int bordered_row_width;
  int bytes_per_pixel;
  void setImageWidth();
  void setImageHeight();
  int getImageDataOffset();
  void setImageBitsPerPixel();

 public:
  void openImage(string file_name);
  void saveImage(string file_name);
  int getWidth() const { return width; }
  int getHeight() const { return height; }
  int getBytesPerPixel() const { return bytes_per_pixel; }
  ~MyImage();
};

MyImage::~MyImage() {
    for (int index = 0; index < height; ++index) {
      delete[] pixels[index];
    }
    delete[] pixels;
}

void MyImage::openImage(string file_name) {
  IMAGE = fopen(file_name.c_str(), "rb");
  char check_b, check_m;
  fread(&check_b, 1, 1, IMAGE);
  fread(&check_m, 1, 1, IMAGE);
  if(check_b != 'B' || check_m != 'M') {
    cout << ERROR_MESSAGE[NOT_BITMAP_IMAGE];
    fclose(IMAGE);
    return;
  }
  int image_data_offset = getImageDataOffset();
  setImageBitsPerPixel();
  setImageHeight();
  setImageWidth();

  real_row_width = width * bytes_per_pixel;
  bordered_row_width = ((width + 3) / 4) * 4 * bytes_per_pixel;
  pixels = new byte* [height];

  for (int index = 0; index < height; ++index) {
    pixels[index] = new byte[real_row_width];
  }

  for (int index = 0; index < height; index++) {
    fseek(IMAGE, image_data_offset + (index * bordered_row_width), SEEK_SET);
    fread(pixels[height - index - 1], 1, real_row_width, IMAGE);
  }
  fclose(IMAGE);
}

void MyImage::setImageWidth() {
#define WIDTH_OFFSET 0x0012
  fseek(IMAGE, WIDTH_OFFSET, SEEK_SET);
  fread(&width, 4, 1, IMAGE);
#undef WIDTH_OFFSET
}
void MyImage::setImageHeight() {
#define HEIGHT_OFFSET 0x0016
  fseek(IMAGE, HEIGHT_OFFSET, SEEK_SET);
  fread(&height, 4, 1, IMAGE);
#undef HEIGHT_OFFSET
}
int MyImage::getImageDataOffset() {
#define HEADER_OFFSET 0x000A
  fseek(IMAGE, HEADER_OFFSET, SEEK_SET);
  int image_data_offset;
  fread(&image_data_offset, 4, 1, IMAGE);
#undef HEADER_OFFSET
  return image_data_offset;
}
void MyImage::setImageBitsPerPixel() {
#define BITS_PER_PIXEL_OFFSET 0x001C
  short int bits;
  fseek(IMAGE, BITS_PER_PIXEL_OFFSET, SEEK_SET);
  fread(&bits, 2, 1, IMAGE);
  bytes_per_pixel = bits / 8;
#undef BITS_PER_PIXEL_OFFSET
}

void workWithBMP() {
  string IMAGE_NAME = "sample.bmp";
  MyImage test_image;
  test_image.openImage(IMAGE_NAME);
  cout << "Width = " << test_image.getWidth() << '\n';
  cout << "Height = " << test_image.getHeight() << '\n';
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