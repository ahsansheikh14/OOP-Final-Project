#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include<chrono>
using namespace std;
using namespace std::chrono;


class Vehicle {
protected:
    string ownerName, vehicleNumber, vehicleName, vehicleID;
    bool isVip;
    system_clock::time_point entryTime;


public:
    Vehicle() :isVip(false) {};
    virtual void calculateFee(int hours) = 0;

    void setDetails(string name, string num, string vname, string id,bool vipStatus=false) {
        ownerName = name;
        vehicleNumber = num;
        vehicleName = vname;
        vehicleID = id;
        isVip = vipStatus;
        entryTime = system_clock::now();
    }

    string getVehicleID() {
        return vehicleID;
    }
    bool getVipStatus() {
        return isVip;
    }

    void displayDetails() {
        cout << "Owner Name: " << ownerName << endl;
        cout << "Vehicle Number: " << vehicleNumber << endl;
        cout << "Vehicle Name: " << vehicleName << endl;
        cout << "Vehicle ID: " << vehicleID << endl;
        cout << "VIP Status: " << (isVip ? "Yes" : "No") << endl;
    }
    system_clock::time_point getEntryTime() {
        return entryTime;
    }
};

class Car : public Vehicle {
    void calculateFee(int seconds) override {
        int fee = seconds * 10;
        if (isVip) {
            fee -= 5;  
            cout << "VIP Discount Applied!" << endl;
        }
        fee = max(fee, 0);
        cout << "Parking fee for car: Rs " << fee << endl;
    }

};

class Bike : public Vehicle {
    void calculateFee(int seconds) override {
        int fee = seconds* 5;
        if (isVip) {
            fee -= 2;  
            cout << "VIP Discount Applied!" << endl;
        }
        fee = max(fee, 0);
        cout << "Parking fee for bike: Rs " << fee << endl;
    }

};

class ParkingSlot {
protected:
    static int totalSlots;
    static int availableSlots;
    static int vipSlots;


public:
    static void initializeSlots(int total,int vip=0) {
        totalSlots = total;
        availableSlots = total;
        vipSlots = vip;
    }


    static void allocateSlot(bool isVIP) {
        if (isVIP && vipSlots > 0) {
            vipSlots--;
            cout << "VIP Slot allocated!" << endl;
        }
        else if (availableSlots > 0) {
            availableSlots--;
            cout << "Regular slot allocated!" << endl;
        }
        else {
            cout << "No slots available!" << endl;
        }
        cout << "Available slots: " << availableSlots << endl;
    }

    static void freeSlot() {
        if (availableSlots < totalSlots) {
            availableSlots++;
            cout << "Slot freed!" << endl;
            cout << "Available slots: " << availableSlots << endl;
        }
        else {
            cout << "No slots to free!" << endl;
        }
    }
};

int ParkingSlot::totalSlots = 0;
int ParkingSlot::availableSlots = 0;
int ParkingSlot::vipSlots = 0;

string generateUniqueID() {
    srand(time(0));
    stringstream ss;
    ss << rand() % 10000;
    return ss.str();
}

void saveVehicleDetails(const string& ownerName, const string& vehicleNumber, const string& vehicleName, const string& vehicleID) {
    ofstream file("vehicle_details.txt", ios::app);
    if (file.is_open()) {
        file << "Owner Name: " << ownerName << endl;
        file << "Vehicle Number: " << vehicleNumber << endl;
        file << "Vehicle Company: " << vehicleName << endl;
        file << "Vehicle ID: " << vehicleID << endl;
        file << "------------------------" << endl;
        file.close();
        cout << "Vehicle details saved successfully!" << endl;
    }
    else {
        cout << "Unable to save vehicle details." << endl;
    }
}
void removeVehicleDetails(const string& id) {
    ifstream file("vehicle_details.txt");
    ofstream tempFile("temp_vehicle_details.txt");

    string line;
    bool vehicleFound = false;

    if (file.is_open() && tempFile.is_open()) {
        while (getline(file, line)) {
            // Check if the current line contains the specific Vehicle ID
            if (line.find("Vehicle ID: " + id) != string::npos) {
                vehicleFound = true;
                // Skip the next lines related to this vehicle record
                for (int i = 0; i < 3 && getline(file, line); i++);
            }
            else {
                tempFile << line << endl;
            }
        }
        file.close();
        tempFile.close();

        if (vehicleFound) {
            // Remove the original file and rename the temp file
            remove("vehicle_details.txt");
            rename("temp_vehicle_details.txt", "vehicle_details.txt");
            cout << "Vehicle details removed successfully!" << endl;
        }
        else {
            cout << "Vehicle not found in the file!" << endl;
        }
    }
    else {
        cout << "Unable to open file for removal!" << endl;
    }
}

void viewParkingReport() {
    ifstream file("vehicle_details.txt");
    string line;

    if (file.is_open()) {
        cout << "Parking Report:" << endl;
        cout << "----------------------------------\n";
        while (getline(file, line)) {
            cout << line << endl;
        }
        cout << "----------------------------------\n";
        file.close();
    }
    else {
        cout << "Unable to open parking report file." << endl;
    }
}

void displayRegisteredVehicles() {
    ifstream file("vehicle_details.txt");
    string line;
    vector<string> vehicleRecords;
    bool vehicleFound = false;

    while (getline(file, line)) {
        vehicleRecords.push_back(line);
    }
    file.close();

    if (vehicleRecords.empty()) {
        cout << "No vehicles are currently registered." << endl;
        return;
    }

    cout << "Registered Vehicles:" << endl;
    for (size_t i = 0; i < vehicleRecords.size(); i++) {
        cout << vehicleRecords[i] << endl;
    }
}

void menu() {
    cout << "\n==============================\n";
    cout << "Parking Management System Menu\n";
    cout << "==============================\n";
    cout << "1. Register Vehicle\n";
    cout << "2. Exit Vehicle and Calculate Fee\n";
    cout << "3. View Parking Report\n";
    cout << "4. Exit\n";
    cout << "Enter your choice: ";
}

int main() {
    ParkingSlot::initializeSlots(50,10);

    Vehicle* vehicles[50];  // Array to store pointers to vehicles
    int vehicleCount = 0;    // Counter for registered vehicles

    Car carInstance;
    Bike bikeInstance;

    int choice, seconds;
    string num, name, type, vname, id;
    bool isVip, found = false;
    do {
        menu();
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Enter Owner name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter your vehicle number: ";
            getline(cin, num);
            cout << "Enter your vehicle company: ";
            getline(cin, vname);
            cout << "Enter your vehicle type (Car/Bike): ";
            getline(cin, type);
            cout << "Is the owner a VIP? (1 for Yes, 0 for No): ";
            cin >> isVip;

            id = generateUniqueID();

            if (type == "Car") {
                vehicles[vehicleCount] = new Car();
            }
            else if (type == "Bike") {
                vehicles[vehicleCount] = new Bike();
            }
            else {
                cout << "Invalid vehicle type! Registration failed. Please try again." << endl;
                break;
            }

            vehicles[vehicleCount]->setDetails(name, num, vname, id,isVip);
            saveVehicleDetails(name, num, vname, id);
            ParkingSlot::allocateSlot(isVip);  // Allocate slot after registration
            vehicleCount++;  // Increment the vehicle count
            cout << "Assigned Vehicle ID: " << id << endl;
            break;

        case 2:
            displayRegisteredVehicles();  // Show registered vehicles from file

            cout << "Enter Vehicle ID to Exit: ";
            cin >> id;


            for (int i = 0; i < vehicleCount; i++) {
                if (vehicles[i]->getVehicleID() == id) {
                    auto exitTime = system_clock::now();
                    auto duration = duration_cast<std::chrono::seconds>(exitTime - vehicles[i]->getEntryTime());  // Calculate the time difference in hours
                    int seconds = duration.count();

                    cout << "Vehicle was parked for " << seconds<< " hours." << endl;


                    vehicles[i]->calculateFee(seconds);
                    ParkingSlot::freeSlot();  // Free slot after vehicle exits
                    removeVehicleDetails(id);  // Remove vehicle details from file
                    found = true;
                    break;
                }
            }

            if (!found) {
                cout << "Invalid Vehicle ID!" << endl;
            }
            break;

        case 3:
            viewParkingReport();
            break;

        case 4:
            cout << "Thank you for using the Parking Management System. Goodbye!\n";
            break;

        default:
            cout << "Invalid choice! Try again.\n";
        }
    } while (choice != 4);

    return 0;
}