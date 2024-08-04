#include <iostream>
#include <functional>
#include <map>
using namespace std;
// Define calculator operations
typedef function<double(double, double)> Operation;
// Addition operation
double add(double a, double b) {
    return a + b;
}
// Subtraction operation
double subtract(double a, double b) {
    return a - b;
}
// Multiplication operation
double multiply(double a, double b) {
    return a * b;
}
// Division operation
double divide(double a, double b) {  
    if (b == 0) {
        cout << "Error: Division by zero is not allowed." << endl;
        return 0;  // Return 0 or some error value
    }
    return a / b;
}
int main() {
    // Map to store operations
    map<char, Operation> operations;
    operations['+'] = add;
    operations['-'] = subtract;
    operations['*'] = multiply;
    operations['/'] = divide;
    char operation;
    double num1, num2;
    cout << "Enter operation (+, -, *, /): ";
    cin >> operation;
    // Check if the operation is valid
    if (operations.find(operation) == operations.end()) {
        cout << "Invalid operation!" << endl;
        return 1;
    }
    cout << "Enter two numbers: ";
    cin >> num1 >> num2;
    if (operation == '/' && num2 == 0) {
        cout << "Error: Division by zero is not allowed." << endl;
        return 1;
    }
    double result = operations[operation](num1, num2);
    cout << "Result: " << result << endl;
    return 0;
}
