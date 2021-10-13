/**
      ---------------------------------------------------
      |         Made by Shamal Viktor (@j3uktop)        |
      |           2019-2021 - Lyceum BSU - IM           |
      |             2021 - BSUIR - FCP - PMS            |
      |          *** All rights reserved ***            |
      ---------------------------------------------------
                                            			**/
#include <iostream>
#include <ctime>
using std::cout;
using std::cin;

struct matrix {
  int** values;
  int row = 0;
  int col = 0;
};

matrix* initMatrix();

void printMatrix(matrix* matr);

double getSum(int* array, int& array_size);

matrix* multiplyMatrix(matrix* matr1, matrix* matr2);

void deleteDuplicateItems(int* array, int& array_size);

double getMedium(int* array, int& array_size);

void eraseMatrix(matrix* matr);

int main() {
  srand(time(NULL));
  matrix* matr1;
  matr1 = initMatrix();
  printMatrix(matr1);

  eraseMatrix(matr1);
  system("pause");
  return 0;
}

matrix* initMatrix() {
  matrix* matr = new matrix;
  int row;
  int col;
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
  cout << "Generate\nfrom ";
  cin >> interval_start;
  cout << "to ";
  cin >> interval_finish;

  while (interval_start >= interval_finish) {
    cout << "Interval input error\n";
    cout << "Generate\nfrom ";
    cin >> interval_start;
    cout << "to ";
    cin >> interval_finish;

  }
  mod = (interval_finish - interval_start + 1);

  matr->row = row;
  matr->col = col;
  matr->values = new int* [row];
  for (int i = 0; i < row; i++) {
    matr->values[i] = new int[col];
  }

  for (int r = 0; r < row; r++) {
    for (int c = 0; c < col; c++) {
      matr->values[r][c] = rand() % mod + interval_start;
    }
  }
  return matr;
}

void eraseMatrix(matrix* matr) {
  for (int i = 0; i < matr->row; i++) {
    delete[] matr->values[i];
  }
  delete[] matr->values;
}


void printMatrix(matrix* matr) {
  if(matr -> row == 0 || matr -> col == 0) {
    cout << "Error. Invalid matrix to print\n";
    return;
  }
  cout << "Matrix:\n";
  for(int r = 0; r < matr -> row; r++) {
    for(int c = 0; c < matr -> col; c++) {
      cout << matr -> values[r][c] << ' ';
    }
    cout << '\n';
  }
}