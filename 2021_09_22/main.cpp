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

void draw_rectangle(int a, int b) {
    for (int i = 0; i < a; i++) {
        for (int j = 0; j < b; j++) {
            cout << "*";
        }
        cout << '\n';
    }
}

void task1() {
    int a, b;
    cout << "Task 1:\n";
    cout << "Set rectangle (a, b > 0)\n";
    cout << "a = ";
    cin >> a;
    cout << "b = ";
    cin >> b;
    int S = a * b;
    int P = (a + b) * 2;
    cout << "S = " << S << '\n';
    cout << "P = " << P << '\n';
    draw_rectangle(a, b);
    cout << "\nTask 1 is completed\n\n";
}

ll get_factorial(int n) {
    ll fact = 1;
    for (int i = 2; i <= n; i++)
        fact *= i;
    return fact;
}

void task2() {
    int n;
    cout << "Task 2:\n";
    cout << "n = ";
    cout << "First n (>= 0) fibonacci numbers:\n";
    cin >> n;
    int first_ = 1, second_ = 0, buf;
    for (int i = 0; i < n; i++) {
        cout << first_ << ' ';
        buf = second_;
        second_ = first_;
        first_ += buf;
    }
    cout << '\n';
    ll fact = get_factorial(n);
    cout << "n! = " << fact << '\n';
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

bool is_prime(ll n) {
    for (ll i = 2; i * i <= n; i++)
        if (n % i == 0)
            return false;
    return true;
}

void task3() {
    cout << "Task 3:\n";
    cout << "Find GCD (a >= 0, b >= 0)\n";
    ll a, b;
    cout << "a = ";
    cin >> a;
    cout << "b = ";
    cin >> b;
    ll gcd = get_gcd(a, b);
    cout << "gcd(" << a << ", " << b << ") = " << gcd << '\n';
    if (is_prime(a)) {
        cout << "a is prime\n";
    } else {
        cout << "a not is prime\n";
    }

    if (is_prime(b)) {
        cout << "b is prime\n";
    } else {
        cout << "b not is prime\n";
    }
    cout << "\nTask 3 is completed\n\n";
}

void task4() {
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

void draw_graph(ll &cnt, ll &mod, int *values) {
    int medium_value = cnt / mod;
    int min_el = *min_element(values, values + mod);
    int max_el = *max_element(values, values + mod);

    int max_graph_size = 15;
    if(mod > 100) {
        max_graph_size *= 2;
    }
    double eq = 1. * max_graph_size / max(1, max(max_el - medium_value, medium_value - min_el));
    string out = "";
    for (int i = 0; i < mod; i++) {
        values[i] = (values[i] - medium_value) * eq;
    }
    for (int j = max_graph_size; j >= 0; j--) {
        for (int i = 0; i < mod; i++) {
            if (values[i] > j) {
                out += '*';
            } else {
                out += ' ';
            }
        }
        out +='\n';
    }
    for (int i = 0; i < mod; i++) {
        out +=  "-";
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
    if(mod < 101) {
        cout << "Graph of deviation from average:\n";
        cout << out;
    } else {
        cout << "Graph of deviation from average is too large. Located it in graph.txt ? Y/N\n";
        char user_answ = getche();
        if(user_answ == 'Y' || user_answ == 'y') {
            ofstream fout("graph.txt");
            fout << out;
            fout.close();
            cout << "Graph located in graph.txt\n";
        }
    }
}

void task5() {
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
    int *a = new int[mod];
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
//    task1();
//    task2();
//    task3();
//    task4();
    task5();
    system("pause");
    return 0;
}
