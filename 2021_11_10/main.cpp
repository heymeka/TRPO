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
                                "This is not a BitMap image!\n",
                                "Unable to convert to black and white image\n"};
enum ERRORS { IS_NEGATIVE, BIRTH_ERROR, NOT_BITMAP_IMAGE, INVALID_BIT_COUNT };

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
 private:
  // public:
  FILE* IMAGE;
  byte** pixels;
  int width;
  int height;
  int real_row_width;
  int bordered_row_width;
  int bytes_per_pixel;
  int bits_per_pixel;
  void setImageWidth();
  void setImageHeight();
  int getImageDataOffset();
  void setImageBytesPerPixel();
  void saveImage_saveHeaderInfo();

 public:
  MyImage();
  MyImage(int init_height, int init_width, int init_bytes_per_color);
  void openImage(const string& file_name);
  void saveImage(const string& file_name);
  int getWidth();
  int getHeight();
  int getBitsPerPixel();
  MyImage transformToBlackAndWhite(int compress_bits_per_pixel);
  MyImage& operator=(const MyImage& new_image);
  void printInfoAboutImage();
  ~MyImage();
};
MyImage::MyImage() {
  IMAGE = nullptr;
  pixels = nullptr;
  width = 0;
  height = 0;
  bytes_per_pixel = 0;
  real_row_width = 0;
  bordered_row_width = 0;
  bits_per_pixel = 0;
}
MyImage::MyImage(int init_height, int init_width, int init_bits_per_color) {
  bits_per_pixel = init_bits_per_color;
  bytes_per_pixel = (bits_per_pixel + 7) / 8;
  height = init_height;
  width = init_width;
  real_row_width = width * bytes_per_pixel;
  bordered_row_width = ((width + 3) / 4) * 4 * bytes_per_pixel;
  IMAGE = nullptr;
  pixels = new byte* [height];
  for (int row = 0; row < height; ++row) {
    pixels[row] = new byte[real_row_width];
  }
}
MyImage::~MyImage() {
  for (int index = 0; index < height; ++index) {
    delete[] pixels[index];
  }
  delete[] pixels;
}

void MyImage::openImage(const string& file_name) {
  IMAGE = fopen(file_name.c_str(), "rb");
  char check_b, check_m;
  fread(&check_b, 1, 1, IMAGE);
  fread(&check_m, 1, 1, IMAGE);
  if (check_b != 'B' || check_m != 'M') {
    fclose(IMAGE);
    IMAGE = nullptr;
    throw std::invalid_argument(ERROR_MESSAGE[NOT_BITMAP_IMAGE]);
  }
  int image_data_offset = getImageDataOffset();
  setImageBytesPerPixel();
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
  IMAGE = nullptr;
}
void MyImage::saveImage(const string& file_name) {
  IMAGE = fopen(file_name.c_str(), "wb");
  this->saveImage_saveHeaderInfo();
  const unsigned char ZERO = 0;
  if (bordered_row_width - real_row_width > 0) {
    for (int index = 0; index < height; ++index) {
      fwrite(pixels[height - index - 1], 1, real_row_width, IMAGE);
      fwrite(&ZERO, 1, bordered_row_width - real_row_width, IMAGE);
    }
  } else {
    for (int index = 0; index < height; ++index) {
      fwrite(pixels[height - index - 1], 1, real_row_width, IMAGE);
    }
  }
  fclose(IMAGE);
  IMAGE = nullptr;
}

MyImage MyImage::transformToBlackAndWhite(int compress_bits_per_pixel) {
  compress_bits_per_pixel = std::min(8, compress_bits_per_pixel);
  compress_bits_per_pixel = std::max(1, compress_bits_per_pixel);
  if (bits_per_pixel != 24) {
    throw std::invalid_argument(ERROR_MESSAGE[INVALID_BIT_COUNT]);
  }
  MyImage new_image(height, width, 8);

  const double CONST_R_IN_RGB = 0.2126;
  const double CONST_G_IN_RGB = 0.7152;
  const double CONST_B_IN_RGB = 0.0722;

  int color_count = (1 << compress_bits_per_pixel) - 1;
  int color;
  int segment_length = 255 / color_count;
  for (int row = 0; row < height; ++row) {
    for (int col = 0; col < width; ++col) {
      color = CONST_B_IN_RGB * (int) (pixels[row][col * bytes_per_pixel])
          + CONST_G_IN_RGB * (int) (pixels[row][col * bytes_per_pixel + 1])
          + CONST_R_IN_RGB * (int) (pixels[row][col * bytes_per_pixel + 2])
          - 0.1;
      color =
          ((color + color % segment_length) / segment_length) * segment_length;
      new_image.pixels[row][col] = (byte) color;
    }
  }
  return new_image;
}

MyImage& MyImage::operator=(const MyImage& new_image) {
  if (this == &new_image) {
    return *this;
  }
  if (pixels != nullptr) {
    for (int index = 0; index < height; ++index) {
      delete[] pixels[index];
    }
    delete[] pixels;
  }
  IMAGE = nullptr;
  height = new_image.height;
  width = new_image.width;
  bits_per_pixel = new_image.bits_per_pixel;
  bytes_per_pixel = new_image.bytes_per_pixel;
  real_row_width = new_image.real_row_width;
  bordered_row_width = new_image.bordered_row_width;
  pixels = new byte* [height];
  for (int row = 0; row < height; ++row) {
    pixels[row] = new byte[real_row_width];
    for (int col = 0; col < real_row_width; ++col) {
      pixels[row][col] = new_image.pixels[row][col];
    }
  }
  return *this;
}

void MyImage::saveImage_saveHeaderInfo() {
  if (IMAGE == nullptr) {
    return;
  }
  // const int HEADER_SIZE = 124;
  const int HEADER_SIZE = 40;
  int DATA_OFFSET = 14 + HEADER_SIZE;
  int COLOUR_COUNT = 0;
  if (bits_per_pixel < 9) {
    COLOUR_COUNT = 1 << (bits_per_pixel);
    DATA_OFFSET += COLOUR_COUNT * 4;
  }
  const short int PLANES_COUNT = 1;
  const int COMPRESSION = 0;
  fwrite("BM", 2, 1, IMAGE);
  int image_size = real_row_width * height;
  int file_size = image_size + DATA_OFFSET;
  fwrite(&file_size, 4, 1, IMAGE);
  const int UNUSED_MEMORY = 0;
  fwrite(&UNUSED_MEMORY, 4, 1, IMAGE); // Unused memory
  fwrite(&DATA_OFFSET, 4, 1, IMAGE);
  fwrite(&HEADER_SIZE, 4, 1, IMAGE);
  fwrite(&width, 4, 1, IMAGE);
  fwrite(&height, 4, 1, IMAGE);
  fwrite(&PLANES_COUNT, 2, 1, IMAGE);
  fwrite(&bits_per_pixel, 2, 1, IMAGE);
  fwrite(&COMPRESSION, 4, 1, IMAGE);
  fwrite(&image_size, 4, 1, IMAGE);
  fwrite(&UNUSED_MEMORY, 4, 1, IMAGE); // Pixels per meter in X
  fwrite(&UNUSED_MEMORY, 4, 1, IMAGE); // Pixels per meter in Y
  fwrite(&COLOUR_COUNT, 4, 1, IMAGE); // Colors used
  fwrite(&UNUSED_MEMORY, 4, 1, IMAGE); // Important colours
  if (bits_per_pixel < 9) {
    unsigned char write_index;
    unsigned char ZERO = 0;
    int add_value = 1 << (8 - bits_per_pixel);
    cout << "Add_value = " << add_value << '\n';
    for (int index = 0; index < COLOUR_COUNT; index += add_value) {
      write_index = index;
      fwrite(&write_index, 1, 1, IMAGE);
      fwrite(&write_index, 1, 1, IMAGE);
      fwrite(&write_index, 1, 1, IMAGE);
      fwrite(&ZERO, 1, 1, IMAGE);
    }
  }
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
void MyImage::setImageBytesPerPixel() {
#define BITS_PER_PIXEL_OFFSET 0x001C
  fseek(IMAGE, BITS_PER_PIXEL_OFFSET, SEEK_SET);
  fread(&bits_per_pixel, 2, 1, IMAGE);
  bytes_per_pixel = (bits_per_pixel + 7) / 8;
#undef BITS_PER_PIXEL_OFFSET
}
int MyImage::getWidth() { return width; }
int MyImage::getHeight() { return height; }
int MyImage::getBitsPerPixel() { return bits_per_pixel; }
void MyImage::printInfoAboutImage() {
  cout << "Height = " << height << '\n';
  cout << "Width = " << width << '\n';
  cout << "Bits per pixel = " << bits_per_pixel << '\n';
  cout << "Bytes per pixel = " << bytes_per_pixel << "\n\n";
}

void workWithBMP() {
  string IMAGE_NAME = "sample.bmp";
  MyImage test_img;
  test_img.openImage(IMAGE_NAME);
  test_img.saveImage("test.bmp");

  cout << "Enter bit count to convert to grayscale"
       << " (1 - black/white, 8 - full 256 grey colors): ";
  int color_bit_count;
  cin >> color_bit_count;
  MyImage greyscale_img = test_img.transformToBlackAndWhite(color_bit_count);
  greyscale_img.saveImage("test_black_and_white.bmp");

  MyImage
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
    throw std::invalid_argument(ERROR_MESSAGE[BIRTH_ERROR]);
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