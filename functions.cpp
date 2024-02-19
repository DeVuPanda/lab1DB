#include <iostream>
#include "functions.h"
using namespace std;

void getClientInfo(string& name, string& address, string& number, string& dateOfB){
    cout << "Enter name" << endl;
    cin >> name;
    cout << "Enter emailAddress" << endl;
    cin >> address;
    cout << "Enter phone number" << endl;
    cin >> number;
    cout << "Enter date of birth" << endl;
    cin >> dateOfB;
}

void getOrderInfo(string& dateOfArr, int& days, string& description){
    cout << "Enter date of arrival" << endl;
    cin >> dateOfArr;
    cout << "Enter amount of days" << endl;
    cin >> days;
    cout << "Enter description" << endl;
    cin >> description;
}

void getOptions(){

    cout << "What do you want to do next?" << endl;
    cout << "1 - Add client" << endl;
    cout << "2 - Remove client" << endl;
    cout << "3 - Edit client" << endl;
    cout << "4 - Add order" << endl;
    cout << "5 - Remove order" << endl;
    cout << "6 - Edit order" << endl;
    cout << "7 - Get list of all clients" << endl;
    cout << "8 - Get list of all orders" << endl;
    cout << "9 - Get list of all orders for specific client" << endl;
    cout << "10 - Exit" << endl;
}
