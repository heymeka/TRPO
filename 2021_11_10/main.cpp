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
#include <random>

using std::cin;
using std::cout;
using std::cerr;
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
  void copyImage(MyImage& copy);
  void smoothingImage_FillBufArray(byte* arr,
                                   int row_size,
                                   int col_size,
                                   int cur_row,
                                   int cur_byte);
  void smoothingImage_AddColumnToBuf(byte* arr,
                                     int row_size,
                                     int col_size,
                                     int cur_row,
                                     int cur_col,
                                     int cur_byte);
  void smoothingImage_AddRowToBuf(byte* arr,
                                  int row_size,
                                  int col_size,
                                  int cur_row,
                                  int cur_col,
                                  int cur_byte);
  void smoothingImage_FillAngleValue(int cur_row, int cur_col, int radius, int pixel_byte);
  byte smoothingImage_GetMedianOfArray(byte* arr, int row_size, int col_size);
 public:
  MyImage();
  MyImage(int init_height, int init_width, int init_bytes_per_color);
  MyImage(const MyImage& obj);
  void openImage(const string& file_name);
  void saveImage(const string& file_name);
  int getWidth();
  int getHeight();
  int getBitsPerPixel();
  MyImage greyscaleImage(int compress_bits_per_pixel);
  MyImage addWhiteNoise(int noise_percentage);
  MyImage addWhiteNoise(double noise_probability);
  MyImage smoothingImage(int smoothing_radius);
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
MyImage::MyImage(const MyImage& obj) {
  width = obj.width;
  height = obj.height;
  real_row_width = obj.real_row_width;
  bordered_row_width = obj.bordered_row_width;
  bytes_per_pixel = obj.bytes_per_pixel;
  bits_per_pixel = obj.bits_per_pixel;
  IMAGE = obj.IMAGE;
  pixels = new byte* [obj.height];
  for (int index = 0; index < height; ++index) {
    pixels[index] = new byte[real_row_width];
    for (int j = 0; j < real_row_width; j++) {
      pixels[index][j] = obj.pixels[index][j];
    }
  }
}

MyImage::~MyImage() {
  if (pixels != nullptr) {
    for (int index = 0; index < height; ++index) {
      delete[] pixels[index];
    }
    delete[] pixels;
    pixels = nullptr;
  }
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

MyImage MyImage::smoothingImage(int smoothing_radius) {
  if (smoothing_radius > 10) {
    smoothing_radius = 10;
  }
  int row_size = 2 * smoothing_radius - 1;
  int matrix_size = row_size * row_size;
  if (smoothing_radius < 2 || width * height < matrix_size) {
    return *this;
  }
  MyImage result_image;

  this->copyImage(result_image);

  byte* buf_arr = new byte[matrix_size];

  int min_cur = smoothing_radius - 1;
  int max_height = height - min_cur;
  int max_width = width - min_cur;
  int angle_size = min_cur * min_cur;
  byte* buf_angle = new byte[angle_size];
  int angle_mid = angle_size / 2;
  for (int pixel_byte = 0; pixel_byte < bytes_per_pixel; ++pixel_byte) {
    // top left angle
    result_image.smoothingImage_FillAngleValue(0, 0, smoothing_radius, pixel_byte);
    // top right angle
    result_image.smoothingImage_FillAngleValue(0, max_width, smoothing_radius, pixel_byte);
    // bottom left angle
    result_image.smoothingImage_FillAngleValue(max_height, 0, smoothing_radius, pixel_byte);
    // bottom right angle
    result_image.smoothingImage_FillAngleValue(max_height, max_width, smoothing_radius, pixel_byte);

    // top side
    for (int cur_row = 0; cur_row < min_cur; ++cur_row) {
      int row_count = cur_row + 1;
      smoothingImage_FillBufArray(buf_arr,
                                  row_count,
                                  row_size,
                                  cur_row,
                                  pixel_byte);
      for (int cur_col = min_cur; cur_col < max_width; ++cur_col) {
        smoothingImage_AddColumnToBuf(buf_arr,
                                      row_count,
                                      row_size,
                                      cur_row,
                                      cur_col,
                                      pixel_byte);
        result_image.pixels[cur_row][cur_col * bytes_per_pixel + pixel_byte] =
            smoothingImage_GetMedianOfArray(buf_arr, row_count, row_size);
      }
    }
    // bottom side
    for (int cur_row = max_height; cur_row < height; ++cur_row) {
      int row_count = height - cur_row;
      smoothingImage_FillBufArray(buf_arr,
                                  row_count,
                                  row_size,
                                  cur_row,
                                  pixel_byte);
      for (int cur_col = min_cur; cur_col < max_width; ++cur_col) {
        smoothingImage_AddColumnToBuf(buf_arr,
                                      row_count,
                                      row_size,
                                      cur_row,
                                      cur_col,
                                      pixel_byte);
        result_image.pixels[cur_row][cur_col * bytes_per_pixel + pixel_byte] =
            smoothingImage_GetMedianOfArray(buf_arr, row_count, row_size);
      }
    }
    // left side
    for (int cur_col = 0; cur_col < min_cur; ++cur_col) {
      int col_count = cur_col + 1;
      smoothingImage_FillBufArray(buf_arr, row_size, col_count, min_cur, pixel_byte);
      for (int cur_row = min_cur; cur_row < max_height; ++cur_row) {
        smoothingImage_AddRowToBuf(buf_arr, row_size, col_count, cur_row, cur_col, pixel_byte);
        result_image.pixels[cur_row][cur_col * bytes_per_pixel + pixel_byte] =
            smoothingImage_GetMedianOfArray(buf_arr, row_size, col_count);
      }
    }
    // right side
    for (int cur_col = max_width; cur_col < width; ++cur_col) {
      int col_count = width - cur_col;
      smoothingImage_FillBufArray(buf_arr, row_size, col_count, min_cur, pixel_byte);
      for (int cur_row = min_cur; cur_row < max_height; ++cur_row) {
        smoothingImage_AddRowToBuf(buf_arr, row_size, col_count, cur_row, cur_col, pixel_byte);
        result_image.pixels[cur_row][cur_col * bytes_per_pixel + pixel_byte] =
            smoothingImage_GetMedianOfArray(buf_arr, row_size, col_count);
      }
    }
    // Center
    for (int cur_row = min_cur; cur_row < max_height; ++cur_row) {
      int cur_row_size = std::min(smoothing_radius + cur_row, row_size);
      smoothingImage_FillBufArray(buf_arr,
                                  cur_row_size,
                                  row_size,
                                  cur_row - min_cur,
                                  pixel_byte);
      for (int cur_col = min_cur; cur_col < max_width; ++cur_col) {
        smoothingImage_AddColumnToBuf(buf_arr,
                                      cur_row_size,
                                      row_size,
                                      cur_row - min_cur,
                                      cur_col,
                                      pixel_byte);
        result_image.pixels[cur_row][cur_col * bytes_per_pixel + pixel_byte] =
            smoothingImage_GetMedianOfArray(buf_arr, cur_row_size, row_size);
      }

    }
  }
  delete[] buf_arr;
  return result_image;
}

MyImage MyImage::greyscaleImage(int compress_bits_per_pixel) {
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
      color = int(CONST_B_IN_RGB * (int) (pixels[row][col * bytes_per_pixel])
                      + CONST_G_IN_RGB
                          * (int) (pixels[row][col * bytes_per_pixel + 1])
                      + CONST_R_IN_RGB
                          * (int) (pixels[row][col * bytes_per_pixel + 2])
                      - 0.1);
      color =
          ((color + color % segment_length) / segment_length) * segment_length;
      new_image.pixels[row][col] = (byte) color;
    }
  }
  return new_image;
}

MyImage MyImage::addWhiteNoise(double noise_probability) {
  return this->addWhiteNoise((int) (noise_probability * 100));
}

MyImage MyImage::addWhiteNoise(int noise_percentage) {
  noise_percentage = std::max(0, std::min(100, noise_percentage));
  MyImage result_image;
  this->copyImage(result_image);
  std::mt19937 rand_seed(time(nullptr));
  std::uniform_int_distribution<int> generate_rand(0, 99);

  for (int row = 0; row < height; ++row) {
    for (int col = 0; col < width; ++col) {
      if (generate_rand(rand_seed) < noise_percentage) {
        for (int cur_pixel = 0; cur_pixel < bytes_per_pixel; ++cur_pixel) {
          result_image.pixels[row][col * bytes_per_pixel + cur_pixel] =
              (byte) 255;
        }
      }
    }
  }
  return result_image;
}

void MyImage::copyImage(MyImage& copy) {
  copy.height = height;
  copy.width = width;
  copy.bits_per_pixel = bits_per_pixel;
  copy.bytes_per_pixel = bytes_per_pixel;
  copy.real_row_width = real_row_width;
  copy.bordered_row_width = bordered_row_width;
  copy.IMAGE = nullptr;

  copy.pixels = new byte* [height];
  for (int row = 0; row < height; ++row) {
    copy.pixels[row] = new byte[real_row_width];
    for (int col = 0; col < real_row_width; ++col) {
      copy.pixels[row][col] = pixels[row][col];
    }
  }
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

void MyImage::smoothingImage_FillBufArray(byte* arr,
                                          int row_size,
                                          int col_size,
                                          int cur_row,
                                          int cur_byte) {
  for (int row = 0; row < row_size; ++row) {
    for (int col = 0; col < col_size; ++col) {
      arr[row * col_size + col] =
          pixels[cur_row + row][col * bytes_per_pixel + cur_byte];
    }
  }
}
void MyImage::smoothingImage_AddColumnToBuf(byte* arr,
                                            int row_size,
                                            int col_size,
                                            int cur_row,
                                            int cur_col,
                                            int cur_byte) {
  int buf_col = cur_col % col_size;
  cur_col = cur_col * bytes_per_pixel + cur_byte;
  for (int row = 0; row < row_size; ++row) {
    arr[row * col_size + buf_col] = pixels[row + cur_row][cur_col];
  }
}

void MyImage::smoothingImage_AddRowToBuf(byte* arr,
                                         int row_size,
                                         int col_size,
                                         int cur_row,
                                         int cur_col,
                                         int cur_byte) {
  int buf_row = cur_row % row_size;
  cur_col = cur_col * bytes_per_pixel + cur_byte;
  for (int col = 0; col < col_size; ++col) {
    arr[buf_row * col_size + col] = pixels[cur_row][cur_col + col * bytes_per_pixel];
  }

}

void MyImage::smoothingImage_FillAngleValue(int cur_row, int cur_col, int radius, int pixel_byte) {
  radius--;
  int buf_size = radius * radius;
  byte* buf = new byte[buf_size];
  for (int row = 0; row < radius; ++row) {
    for (int col = 0; col < radius; ++col) {
      buf[row * radius + col] = pixels[cur_row + row][(cur_col + col) * bytes_per_pixel + pixel_byte];
    }
  }
  sort(buf, buf + buf_size);
  int mid = buf_size / 2;
  for (int row = 0; row < radius; ++row) {
    for (int col = 0; col < radius; ++col) {
      pixels[cur_row + row][(cur_col + col) * bytes_per_pixel + pixel_byte] = buf[mid];
    }
  }
  delete[] buf;
}
byte MyImage::smoothingImage_GetMedianOfArray(byte* arr,
                                              int row_size,
                                              int col_size) {
  int full_size = row_size * col_size;
  byte* buf_arr = new byte[full_size];
  for (int index = 0; index < full_size; ++index) {
    buf_arr[index] = arr[index];
  }
  sort(buf_arr, buf_arr + full_size);
  byte median = buf_arr[full_size / 2];
  delete[] buf_arr;
  return median;
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
  std::mt19937 rand_seed(time(nullptr));
  std::uniform_real_distribution<double> generate_rand(0.10, 0.30);

  const string IMAGE_NAME = "sample.bmp";
  MyImage test_img;
  test_img.openImage(IMAGE_NAME);
  cout << "Image open from " << IMAGE_NAME << '\n';

  const string original_file = "test.bmp";
  // test_img.saveImage(original_file);
  cout << "Original image save in " << original_file << '\n';

  int color_bit_count;
  // cout << "Enter bit count to convert to grayscale"
  //      << " (1 - black/white, 8 - full 256 grey colors): ";
  // cin >> color_bit_count;
  color_bit_count = 5;

  const string greyscale_file = "greyscale.bmp";
  MyImage greyscale_img = test_img.greyscaleImage(color_bit_count);
  // greyscale_img.saveImage(greyscale_file);
  cout << "Greyscale image save in " << greyscale_file << '\n';

  double noise_percentage = generate_rand(rand_seed);
  const string noise_file = "noise.bmp";
  MyImage noise_image;
  noise_image = test_img.addWhiteNoise(noise_percentage);
  noise_image.saveImage(noise_file);
  cout << "Image with " << (int) (noise_percentage * 100)
       << "% of white noise save in " << noise_file << '\n';

  const string smoothing_file = "smoothing.bmp";
  int smoothing_radius = 2;
  cout << "Smoothing image...\n";
  MyImage smoothing_image = noise_image.smoothingImage(smoothing_radius);
  cout << "Successful smoothing\n";
  smoothing_image.saveImage(smoothing_file);
  cout << "Smoothing image save in " << smoothing_file << '\n';
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