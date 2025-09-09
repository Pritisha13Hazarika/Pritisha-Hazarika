#include <iostream>
using namespace std;

int main() {
    double base, height, area;

    // Input base and height of the triangle
    cout << "Enter the base of the triangle: ";
    cin >> base;
    cout << "Enter the height of the triangle: ";
    cin >> height;

    // Calculate the area
    area = 0.5 * base * height;

    // Display the result
    cout << "The area of the triangle is: " << area << endl;

    return 0;
}