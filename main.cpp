#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdint>
#include "functions.h"
#include "Client.h"
#include "Order.h"
using namespace std;

struct Identificator{
public:
    int cliId;
    int32_t clientAddress;
};


Identificator getClientIndex(int position){

    Identificator index;
    FILE* IndexDB = fopen("clientIndex.bin", "rb");
    fseek(IndexDB, position * sizeof(Identificator), SEEK_SET);
    fread(&index, sizeof(Identificator), 1, IndexDB);
    fclose(IndexDB);
    return index;
}

Client getClient(int position) {

    Client client;
    Identificator index = getClientIndex(position);
    FILE *clientDB = fopen("clients.bin", "rb");
    fseek(clientDB, index.clientAddress, SEEK_SET);
    fread(&client, sizeof(Client), 1, clientDB);
    fclose(clientDB);
    return client;
}

Order getOrder(int position){

    Order order;
    FILE* ordersDB = fopen("orders.bin", "rb");
    fseek(ordersDB, position * sizeof(Order), SEEK_SET);
    fread(&order, sizeof(Order), 1, ordersDB);
    fclose(ordersDB);
    return order;
}


void writeClient(Client& client, int position){

    Identificator index = getClientIndex(position);
    FILE* clientsDB = fopen("clients.bin", "rb+");
    fseek(clientsDB, index.clientAddress, SEEK_SET);
    fwrite(&client, sizeof(Client), 1, clientsDB);
    fclose(clientsDB);
}

void writeOrder(Order& order, int position){

    FILE* ordersDB = fopen("orders.bin", "rb+");
    fseek(ordersDB, position * sizeof(Order), SEEK_SET);
    fwrite(&order, sizeof(Order), 1, ordersDB);
    fclose(ordersDB);
}

void writeIndex(Identificator index, int position){

    FILE* clientIndex = fopen("clientIndex.bin", "rb+");
    fseek(clientIndex, position * sizeof(Identificator), SEEK_SET);
    fwrite(&index, sizeof(Identificator), 1, clientIndex);
    fclose(clientIndex);
}

int getAmountOfClients(){

    FILE* cliDb = fopen("clients.bin", "ab+");
    fseek(cliDb, 0, SEEK_END);
    int count = ftell(cliDb)/sizeof(Client);
    fclose(cliDb);
    return count;
}

int getAmountOfOrders(){

    FILE* ordersDB = fopen("orders.bin", "ab+");
    fseek(ordersDB, 0, SEEK_END);
    int count = ftell(ordersDB) / sizeof(Order);
    fclose(ordersDB);
    return count;
}

void addClient(Client& client) {

    Identificator index;
    FILE* IndexDB = fopen("clientIndex.bin", "ab+");
    fseek(IndexDB, 0, SEEK_END);
    int count = ftell(IndexDB)/sizeof(Identificator) + 1;

    index.cliId = count;
    index.clientAddress = (count - 1) * sizeof(Client);

    fseek(IndexDB, 0, SEEK_SET);
    fwrite(&index, sizeof(Identificator), 1, IndexDB);
    fclose(IndexDB);

    FILE* cliDb = fopen("clients.bin", "ab+");
    fwrite(&client, sizeof(Client), 1, cliDb);
    fclose(cliDb);
}

void addOrder(Order& order, int clientId){

    Client client = getClient(clientId - 1);
    client.ordersCounter++;
    writeClient(client, clientId - 1);

    FILE* ordersDB = fopen("orders.bin", "ab+");
    fseek(ordersDB, 0, SEEK_END);
    order.orderID = ftell(ordersDB) / sizeof(Order) + 1;
    fwrite(&order, sizeof(Order), 1, ordersDB);
    fclose(ordersDB);
}

int idClientValidator(int id){

    int all = getAmountOfClients();
    if(id > all || id < 1){
        cout << "Wrong id!" << endl;
        cout << endl;
        return -1;
    }

    Identificator index = getClientIndex(id - 1);
    if(index.cliId == -1){
        cout << "This client is already deleted!" << endl;
        cout << endl;
        return -1;
    }
    return 0;
}

int idOrderValidator(int id){

    int all = getAmountOfOrders();
    if(id > all || id < 1){
        cout << "Wrong id!" << endl;
        cout << endl;
        return -1;
    }

    Order reservation = getOrder(id - 1);
    if(reservation.orderID == -1){
        cout << "This order is already deleted!" << endl;
        cout << endl;
        return -1;
    }
    return 0;
}

void deleteClient(int id){

    int result = idClientValidator(id);
    if(result == -1) return;

    Identificator index = getClientIndex(id - 1);
    Client client = getClient(id - 1);
    Order order;
    int amount = getAmountOfOrders();

    if(client.ordersCounter > 0){
        for(int i = 0; i < amount; i++){
            order = getOrder(i);
            if(index.cliId == order.clientId){
                order.orderID = -1;
                writeOrder(order, i);
            }
        }
    }

    index.cliId = -1;
    writeIndex(index, id - 1);
    cout << "Removed client with all orders" <<endl;
    cout << endl;
}

void deleteOrder(int clientId, int orderID){

    Client client = getClient(clientId - 1);
    client.ordersCounter--;
    writeClient(client, clientId - 1);

    Order order = getOrder(orderID - 1);
    order.orderID = -1;
    writeOrder(order, orderID - 1);
}


int main() {

    Client client;
    Order order;
    Identificator index;
    int choice  = 0;
    int id, clientId, days, amount;
    string name, address, number, dateOfB, dateOfArr, description;

    getOptions();
    while(true){

        while(!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Wrong input, try again" << endl;
        }

        switch (choice) {
            case 1:

                getClientInfo(name, address, number, dateOfB);
                strcpy(client.name, name.c_str());
                strcpy(client.emailAddress, name.c_str());
                strcpy(client.phoneNumber, name.c_str());
                strcpy(client.dateOfBirth, name.c_str());
                client.ordersCounter = 0;
                addClient(client);
                break;

            case 2:
                cout << "Enter id of a client that you want to remove" << endl;
                cin >> id;
                deleteClient(id);
                break;

            case 3:
                cout << "Enter id of a client that you want to edit" << endl;
                cin >> id;
                if (idClientValidator(id) == 0) {
                    getClientInfo(name, address, number, dateOfB);
                    client = getClient(id - 1);
                    strcpy(client.name, name.c_str());
                    strcpy(client.emailAddress, address.c_str());
                    strcpy(client.phoneNumber, number.c_str());
                    strcpy(client.dateOfBirth, dateOfB.c_str());
                    writeClient(client, id - 1);
                }
                break;

            case 4:
                cout << "Enter id of a client who wants to make a order" << endl;
                cin >> id;
                if (idClientValidator(id) == 0) {

                    getOrderInfo(dateOfArr, days, description);
                    order.days = days;
                    order.clientId = id;
                    strcpy(order.dateOfArrival, dateOfArr.c_str());
                    strcpy(order.description, description.c_str());
                    addOrder(order, id);
                }
                break;

            case 5:
                cout << "Enter client id" << endl;
                cin >> clientId;
                if (idClientValidator(clientId) == 0) {
                    client = getClient(clientId - 1);
                    if (client.ordersCounter > 0) {

                        cout << "Enter order id" << endl;
                        cin >> id;
                        if (idOrderValidator(id) == 0) deleteOrder(clientId, id);

                    } else  cout << "This client has no orders" << endl;
                }
                break;

            case 6:
                cout << "Enter client id" << endl;
                cin >> clientId;
                if (idClientValidator(clientId) == 0) {
                    client = getClient(clientId - 1);
                    if (client.ordersCounter > 0) {

                        cout << "Enter order id" << endl;
                        cin >> id;
                        if (idOrderValidator(id) == 0) {

                            order = getOrder(id - 1);
                            getOrderInfo(dateOfArr, days, description);
                            order.days = days;
                            strcpy(order.dateOfArrival, dateOfArr.c_str());
                            strcpy(order.description, description.c_str());
                            writeOrder(order, id - 1);
                            cout << "order updated" << endl;
                        }
                    } else cout << "This client has no orders" << endl;
                    cout << endl;
                }
                break;

            case 7:
                amount = getAmountOfClients();
                cout << "The list of all clients:" << endl;
                for (int i = 0; i < amount; i++) {
                    client = getClient(i);
                    index = getClientIndex(i);
                    if (index.cliId != -1) {
                        cout << "client id: " << index.cliId << endl;
                        cout << "client name: " << client.name << endl;
                        cout << "amount of orders: " << client.ordersCounter << endl;
                        cout << endl;
                    }
                }

                break;

            case 8:
                amount = getAmountOfOrders();
                cout << "All orders are:" << endl;
                for (int i = 0; i < amount; i++) {
                    order = getOrder(i);
                    if (order.orderID != -1) {
                        cout << "order id: " << order.orderID << endl;
                        cout << "description: " << order.description << endl;
                        cout << endl;
                    }
                }
                break;

            case 9:
                cout << "Enter id of a client" << endl;
                cin >> id;
                amount = getAmountOfOrders();
                if (idClientValidator(id) == 0) {
                    client = getClient(id - 1);
                    if (client.ordersCounter > 0) {
                        cout << "List of orders for this client:" << endl;
                        for (int i = 0; i < amount; i++) {
                            order = getOrder(i);
                            if (id == order.clientId){
                                cout << "order id: " << order.orderID << endl;
                                cout << "description: " << order.description << endl;
                            }
                        }
                    } else cout << "This client has no orders" << endl;
                    cout << endl;
                }
                break;

            case 10:
                return 0;

            default:
                cout << "Wrong input!" << endl;
                break;
        }
        cout << "What do you want to do next?" << endl;
    }
}
