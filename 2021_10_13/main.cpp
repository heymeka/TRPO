/**
      ---------------------------------------------------
      |         Made by Shamal Viktor (@j3uktop)        |
      |           2019-2021 - Lyceum BSU - IM           |
      |             2021 - BSUIR - FCP - PMS            |
      |          *** All rights reserved ***            |
      ---------------------------------------------------
                                            			**/
#include <iostream>
#include <iomanip>
#include <conio.h>
#include <ctime>
using std::cout;
using std::cin;
using std::setw;
const int SPACE = 5;

class matrix {
 private:
  int** values;
  int row;
  int col;
 public:
  matrix() {
    row = 0;
    col = 0;
  }
  void init(int _row, int _col);
  void init();
  void print();
  void erase();
  matrix operator*(matrix& matr2);
};

double getSum(int* array, int& array_size);

void deleteDuplicateItems(int* array, int& array_size);

double getMedium(int* array, int& array_size);

void taskPrint();

void taskSum();

void taskMultiply();

void taskDelete();

void taskMatrixMedian();

template<typename T>
T max(T first, T second) {
  if (first < second)
    first = second;
  return first;
}

int main() {
  srand(time(NULL));
  // taskPrint();
  // taskSum();
  // taskMultiply();
  // taskDelete();
  // taskMatrixMedian();
  system("pause");
  return 0;
}

void taskPrint() {
  cout << "Task 1: Print N*K Matrix\n\n";
  matrix matr1;
  matr1.init();
  cout << "Matrix:\n";
  matr1.print();
  matr1.erase();
  cout << "\n\n";
}

void taskSum() {
  cout
      << "Task2: Get the sum and average value of the elements in the array\n\n";
  cout << "Enter element count\n";
  int size;
  cout << "size = ";
  cin >> size;
  while (size < 1) {
    cout << "Input error. Size must be > 0\n";
    cout << "size = ";
    cin >> size;
  }
  double* value = new double[size];
  cout << "Generate random elements? [Y/N]: ";
  char user_answer = getche();
  cout << "\n";
  if (user_answer == 'Y' || user_answer == 'y') {
    double interval_start;
    double interval_finish;
    cout << "Enter the number generation interval\n";
    cout << "Min value = ";
    cin >> interval_start;
    cout << "Max value = ";
    cin >> interval_finish;

    int mod = abs(interval_start - interval_finish) + 1;
    mod = max(2, mod);
    cout << "Interval: " << interval_start << ".."
         << int(interval_start) + mod - 1 << '\n';

    for (int ind = 0; ind < size; ind++) {
      ind[value] = rand() % mod + interval_start;
    }
    cout << "Array:\n";
    for (int ind = 0; ind < size; ind++) {
      cout << ind[value] << ' ';
    }
    cout << '\n';
  } else {
    cout << "Enter " << size << " elements\n";
    for (int ind = 0; ind < size; ind++) {
      cin >> ind[value];
    }
  }
  int sum = 0;
  for (int ind = 0; ind < size; ind++) {
    sum += ind[value];
  }
  cout << "Sum = " << sum << '\n';
  double average = double(sum) / size;
  cout << "Average = " << average << '\n';

  cout << "\n\n";
}

void taskMultiply() {
  cout << "Task3: Matrix multiply\n\n";
  matrix matr1;
  matrix matr2;
  cout << "First Matrix\n\n";
  matr1.init();
  cout << "\n";
  matr1.print();

  cout << "Second Matrix\n\n";
  matr2.init();
  cout << "\n";
  matr2.print();

  cout << "\nResult:\n";
  matrix matr3 = matr1 * matr2;
  matr3.print();

  matr1.erase();
  matr2.erase();
  matr3.erase();
  cout << "\n\n";
}

void taskDelete() {

}

void taskMatrixMedian() {

}

void matrix::init() {
  cout << "Enter matrix size:\n";
  cout << "row count = ";
  cin >> row;
  cout << "col count = ";
  cin >> col;
  while (row < 1 || col < 1) {
    cout << "Input error. Row and col count must be >= 1\n";
    cout << "row count = ";
    cin >> row;
    cout << "col count = ";
    cin >> col;
  }
  int mod;
  int interval_start;
  int interval_finish;
  cout << "Enter the number generation interval\n";
  cout << "Min value = ";
  cin >> interval_start;
  cout << "Max value = ";
  cin >> interval_finish;

  while (interval_start >= interval_finish) {
    cout << "Interval input error\n";
    cout << "Min value = ";
    cin >> interval_start;
    cout << "Max value = ";
    cin >> interval_finish;
  }
  mod = (interval_finish - interval_start + 1);

  values = new int* [row];
  for (int i = 0; i < row; i++) {
    values[i] = new int[col];
  }

  for (int r = 0; r < row; r++) {
    for (int c = 0; c < col; c++) {
      values[r][c] = rand() % mod + interval_start;
    }
  }
}

void matrix::init(int _row, int _col) {
  if (_row < 1) {
    _row = 1;
  }
  if (_col < 1) {
    _col = 1;
  }
  row = _row;
  col = _col;
  values = new int* [row];
  for (int i = 0; i < row; i++) {
    values[i] = new int[col];
  }
}

void matrix::erase() {
  for (int i = 0; i < row; i++) {
    delete[] values[i];
  }
  delete[] values;
}

void matrix::print() {
  if (row < 1 || col < 1) {
    cout << "Error. Invalid matrix to print\n";
    return;
  }
  for (int r = 0; r < row; r++) {
    for (int c = 0; c < col; c++) {
      cout << setw(SPACE) << values[r][c] << ' ';
    }
    cout << '\n';
  }
}

matrix matrix::operator*(matrix& matr2) {
  matrix result;
  if (row < 1 || col < 1 || matr2.row < 1 || matr2.col < 1
      || col != matr2.row) {
    cout << "Error. Invalid matrix to multiply\n";
    result.init(1, 1);
    result.values[0][0] = -(1 << 25);
    return result;
  }
  result.init(row, matr2.col);
  for (int r = 0; r < row; r++) {
    for (int c = 0; c < matr2.col; c++) {
      int buf_result = 0;
      for (int check = 0; check < col; check++) {
        buf_result += values[r][check] * matr2.values[check][c];
      }
      result.values[r][c] = buf_result;
    }
  }
  return result;
}