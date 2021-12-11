/**
      ---------------------------------------------------
      |         Made by Shamal Viktor (@j3uktop)        |
      |           2019-2021 - Lyceum BSU - IM           |
      |             2021 - BSUIR - FCP - PMS            |
      |          *** All rights reserved ***            |
      ---------------------------------------------------
                                            			**/
#include <iostream>
#include <fstream>
#include <algorithm>
#include <conio.h>
#include <time.h>

using namespace std;
typedef long long ll;

ll time_ = time(NULL);

void draw_rectangle(int height, int width) {
  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width; col++) {
      cout << "*";
    }
    cout << '\n';
  }
}

void drawRectangleOnConsole() {
  int height, width;
  cout << "Task 1:\n";
  cout << "Set rectangle (height, width > 0)\n";
  cout << "height = ";
  cin >> height;
  cout << "width = ";
  cin >> width;
  int S = height * width;
  int P = (height + width) * 2;
  cout << "S = " << S << '\n';
  cout << "P = " << P << '\n';
  draw_rectangle(height, width);
  cout << "\nTask 1 is completed\n\n";
}

ll get_factorial(int number) {
  ll fact = 1;
  for (int i = 2; i <= number; i++)
    fact *= i;
  return fact;
}

void printFibonacciNumbers() {
  int number;
  cout << "Task 2:\n";
  cout << "number = ";
  cout << "First number (>= 0) fibonacci numbers:\number";
  cin >> number;
  int first_ = 1, second_ = 0, buf;
  for (int i = 0; i < number; i++) {
    cout << first_ << ' ';
    buf = second_;
    second_ = first_;
    first_ += buf;
  }
  cout << '\n';
  ll fact = get_factorial(number);
  cout << "number! = " << fact << '\n';
  cout << "\nTask 2 is completed\n\n";
}

ll get_gcd(ll a, ll b) {
  //return (a > 0) ? get_gcd(b % a, a) : b;
  ll buf;
  while (a > 0) {
    buf = a;
    a = b % a;
    b = buf;
  }
  return b;
}

bool is_prime(ll number) {
  for (ll division = 2; division * division <= number; division++)
    if (number % division == 0)
      return false;
  return true;
}

void infoAboutNumbers() {
  cout << "Task 3:\n";
  cout << "Find GCD (first_number >= 0, second_number >= 0)\n";
  ll first_number, second_number;
  cout << "first_number = ";
  cin >> first_number;
  cout << "second_number = ";
  cin >> second_number;
  ll gcd = get_gcd(first_number, second_number);
  cout << "gcd(" << first_number << ", " << second_number << ") = " << gcd << '\n';
  if (is_prime(first_number)) {
    cout << "first_number is prime\n";
  } else {
    cout << "first_number not is prime\n";
  }

  if (is_prime(second_number)) {
    cout << "second_number is prime\n";
  } else {
    cout << "second_number not is prime\n";
  }
  cout << "\nTask 3 is completed\n\n";
}

void printBinary() {
  cout << "Task 4:\n";
  cout << "Get binary representation (n > 0)\n";
  cout << "n = ";
  int n;
  cin >> n;
  string s = "";
  while (n) {
    s += (n % 2) + '0';
    n /= 2;
  }
  reverse(s.begin(), s.end());
  cout << s << '\n';
  cout << "\nTask 4 is completed\n\n";
}

ll get_rand(ll mod) {
  time_ += (time_ << 20) + (time_ >> 20);
  return abs(time_) % mod;
}

void draw_graph(ll& cnt, ll& mod, int* values) {
  int medium_value = cnt / mod;
  int min_el = *min_element(values, values + mod);
  int max_el = *max_element(values, values + mod);

  int max_graph_size = 15;
  if (mod > 100) {
    max_graph_size *= 2;
  }
  double eq = 1. * max_graph_size
      / max(1, max(max_el - medium_value, medium_value - min_el));
  string out = "";
  for (int i = 0; i < mod; i++) {
    values[i] = (values[i] - medium_value) * eq;
  }
  for (int row = max_graph_size; row >= 0; row--) {
    for (int col = 0; col < mod; col++) {
      if (values[col] > row) {
        out += '*';
      } else {
        out += ' ';
      }
    }
    out += '\n';
  }
  for (int i = 0; i < mod; i++) {
    out += "-";
  }
  out += '\n';
  for (int j = 0; j + max_graph_size > 0; j--) {
    for (int i = 0; i < mod; i++) {
      if (values[i] < j) {
        out += '*';
      } else {
        out += ' ';
      }
    }
    out += '\n';
  }
  if (mod < 101) {
    cout << "Graph of deviation from average:\n";
    cout << out;
  } else {
    cout
        << "Graph of deviation from average is too large. Located it in graph.txt ? Y/N\n";
    char user_answ = getche();
    if (user_answ == 'Y' || user_answ == 'y') {
      ofstream fout("graph.txt");
      fout << out;
      fout.close();
      cout << "Graph located in graph.txt\n";
    }
  }
}

void generateRandomNumbers() {
  cout << "Get the mod (> 0) for random numbers\n";
  ll mod;
  cout << "mod = ";
  cin >> mod;
  mod = abs(mod);
  if (mod > 1e7) {
    cout << "mod is too large.\nmod = " << 10000000 << '\n';
    mod = 1e7;
  }
  if (mod == 0) {
    cout << "mid can't be 0.\nmod = 2\n";
    mod = 2;
  }
  cout << "How many numbers generate?\n";
  ll cnt;
  cout << "count = ";
  cin >> cnt;
  cnt = abs(cnt);
  if (cnt > 1e8) {
    cout << "So many numbers, will be generate " << 100000000 << " numbers\n";
    cnt = 1e8;
  }
  int* a = new int[mod];
  for (int i = 0; i < mod; i++) {
    a[i] = 0;
  }
  for (int i = 0; i < cnt; i++) {
    a[get_rand(mod)]++;
  }
  draw_graph(cnt, mod, a);
  cout << "\nTask 5 is completed\n";
}

int main() {
  drawRectangleOnConsole();
  printFibonacciNumbers();
  infoAboutNumbers();
  printBinary();
  generateRandomNumbers();
  system("pause");
  return 0;
}
