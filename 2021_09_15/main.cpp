/**
      ---------------------------------------------------
      |         Made by Shamal Viktor (@j3uktop)        |
      |           2019-2021 - Lyceum BSU - IM           |
      |             2021 - BSUIR - FCP - PMS            |
      |          *** All rights reserved ***            |
      ---------------------------------------------------
                                            			**/
#include <iostream>

using namespace std;
//-----------------------------------------------------------------------------------------

void f1(int &x) {
    cout << "x = ";
    cin >> x;
    if (x % 2 == 0) {
        cout << "x is even\n";
    } else {
        cout << "x is odd\n";
    }
}
//-----------------------------------------------------------------------------------------

void f2(int &x) {
    int sum = 0, k = 0;
    while (x) {
        sum += x % 10;
        x /= 10;
        k++;
    }
    cout << "Arithmetic mean = " << (1. * sum) / k << '\n';
}
//-----------------------------------------------------------------------------------------

void f3() {
    cout << "Month number: ";
    int month_num;
    cin >> month_num;
    switch (month_num) {
        case 1:
            cout << "January\n";
            cout << "Winter\n";
            break;
        case 2:
            cout << "February\n";
            cout << "Winter\n";
            break;
        case 3:
            cout << "March\n";
            cout << "Spring\n";
            break;
        case 4:
            cout << "April\n";
            cout << "Spring\n";
            break;
        case 5:
            cout << "May\n";
            cout << "Spring\n";
            break;
        case 6:
            cout << "June\n";
            cout << "Summer\n";
            break;
        case 7:
            cout << "July\n";
            cout << "Summer\n";
            break;
        case 8:
            cout << "August\n";
            cout << "Summer\n";
            break;
        case 9:
            cout << "September\n";
            cout << "Autumn\n";
            break;
        case 10:
            cout << "October\n";
            cout << "Autumn\n";
            break;
        case 11:
            cout << "November\n";
            cout << "Autumn\n";
            break;
        case 12:
            cout << "December\n";
            cout << "Winter\n";
            break;
        default:
            cout << "Error\n";
            break;
    }
}
//-----------------------------------------------------------------------------------------

void chinese() {
    cout << "Chinese year number: ";
    int chin_year_num, chin_year_color;
    cin >> chin_year_num;
    if (chin_year_num < 0) {
//        cout << chin_year_num % 10 << '\n';
        chin_year_color = (chin_year_num - 10 * (chin_year_num / 10 - 1)) % 10 / 2;
        chin_year_num += -12 * (chin_year_num / 12 - 1);
    } else {
        chin_year_color = (chin_year_num % 10) / 2;
    }
    chin_year_num %= 12;
    string chin_year[12] = {"Monkey", "Chicken", "Dog", "Pig", "Rat", "Cow", "Tiger", "Rabbit", "Dragon", "Snake",
                            "Horse", "Sheep"};
    string chin_year_colors[5] = {"White", "Black", "Green", "Red", "Yellow"};

    cout << "This year the animal is a " << chin_year[chin_year_num] << '\n';
    cout << "This year the color is a " << chin_year_colors[chin_year_color] << '\n';
}
//-----------------------------------------------------------------------------------------

int main() {
    int x;
//    f1(x);
//    f2(x);
//    f3();
    chinese();
    system("pause");
    return 0;
}
