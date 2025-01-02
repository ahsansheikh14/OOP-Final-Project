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
#define RESET "\033[0m"
#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"


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
    // Open file in append mode
    ifstream file("vehicle_details.txt");
    string line;
    bool exists = false;

    // Check if the vehicle already exists in the file
    while (getline(file, line)) {
        if (line.find(vehicleID) != string::npos) {
            exists = true;  // Vehicle ID already exists
            break;
        }
    }

    file.close();

    // Only write the details if the vehicle doesn't already exist
    if (!exists) {
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
    else {
        cout << "This vehicle is already registered." << endl;
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
// Function to remove vehicle details by ID
void removeVehicleDetails(string vehicleID) {
    ifstream file("vehicle_details.txt"); // Open file to read
    string line;
    vector<string> remainingData; // To store remaining vehicle data

    // Read all vehicle data from the file and store it, skipping the removed vehicle
    while (getline(file, line)) {
        if (line.find(vehicleID) == string::npos) {
            remainingData.push_back(line); // Add all lines except the vehicle to remove
        }
    }
    file.close();

    // Rewrite the file with the remaining vehicle data
    ofstream outFile("vehicle_details.txt", ios::out | ios::trunc); // Open file in write mode
    for (const string& line : remainingData) {
        outFile << line << endl; // Write each line back to the file
    }
    outFile.close(); // Close the file after writing
}

// Function to exit a vehicle and remove from list
void exitVehicleAndRemove() {
    string id;
    bool found = false;
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
vector<string> loadVehicleIDs() {
    vector<string> vehicleIDs;
    ifstream file("vehicle_details.txt");
    string line;

    if (file.is_open()) {
        while (getline(file, line)) {
            if (line.find("Vehicle ID: ") != string::npos) {
                // Extract the Vehicle ID from the line
                string id = line.substr(line.find(": ") + 2);
                vehicleIDs.push_back(id);
            }
        }
        file.close();
    }
    else {
        cout << RED << "Unable to open vehicle details file!" << endl;
    }
    return vehicleIDs;
}


void menu() {
    cout <<BLUE<< "\n==============================\n";
    cout << "Parking Management System Menu\n";
    cout << "==============================\n";
    cout << "1. Register Vehicle\n";
    cout << "2. Exit Vehicle and Calculate Fee\n";
    cout << "3. View Parking Report\n";
    cout << "4. Exit\n";
    cout <<RESET<<"Enter your choice: ";
}

int main() {
    ParkingSlot::initializeSlots(50,10);

    Vehicle* vehicles[50];  // Array to store pointers to vehicles
    int vehicleCount = 0;    // Counter for registered vehicles

    Car carInstance;
    Bike bikeInstance;

    int choice = 0;
    int seconds;
    string num, name, type, vname, id;
    bool isVip, found = false;
    do {
        menu();
        cin >> choice;
        
        switch (choice) {
        case 1:
            cout << YELLOW << "Enter Owner name: ";
            cin.ignore();
            getline(cin, name);

            cout << YELLOW << "Enter your vehicle number: ";
            getline(cin, num);

            cout << YELLOW << "Enter your vehicle company: ";
            getline(cin, vname);

            do {
                cout << YELLOW << "Enter your vehicle type (Car/Bike): ";
                getline(cin, type);

                if (type != "Car" && type != "Bike") {
                    cout << RED << "Invalid vehicle type! Please enter 'Car' or 'Bike'.\n";
                }
            } while (type != "Car" && type != "Bike");

            do {
                cout << YELLOW << "Is the owner a VIP? (1 for Yes, 0 for No): ";
                if (!(cin >> isVip)) {
                    cout << RED << "Invalid input! Please enter 1 for Yes or 0 for No.\n";
                    cin.clear();               // Clear the error flag
                    cin.ignore(INT_MAX, '\n'); // Discard invalid input
                }
                else if (isVip != 0 && isVip != 1) {
                    cout << RED << "Invalid input! Please enter 1 for Yes or 0 for No.\n";
                }
            } while (isVip != 0 && isVip != 1);

            id = generateUniqueID();

            if (type == "Car") {
                vehicles[vehicleCount] = new Car();
            }
            else if (type == "Bike") {
                vehicles[vehicleCount] = new Bike();
            }

            vehicles[vehicleCount]->setDetails(name, num, vname, id, isVip);
            saveVehicleDetails(name, num, vname, id);
            ParkingSlot::allocateSlot(isVip); // Allocate slot after registration
            vehicleCount++;                   // Increment the vehicle count
            cout << GREEN << "Assigned Vehicle ID: " << id << endl;
            break;
        case 2:
            // Display Registered Vehicles
            displayRegisteredVehicles();

            cout << YELLOW << "Enter Vehicle ID to Exit: ";
            cin >> id;

            

            // First check in memory for vehicle
            for (int i = 0; i < vehicleCount; i++) {
                if (vehicles[i]->getVehicleID() == id) {
                    auto exitTime = system_clock::now();
                    auto duration = duration_cast<std::chrono::seconds>(exitTime - vehicles[i]->getEntryTime()); // Calculate time in seconds
                    int seconds = duration.count();

                    cout << "Vehicle was parked for " << seconds << " seconds" << endl;

                    vehicles[i]->calculateFee(seconds); // Calculate parking fee
                    ParkingSlot::freeSlot(); // Free the parking slot
                    removeVehicleDetails(id); // Remove vehicle from file
                    found = true;
                    break;
                }
            }

            // If vehicle is not found in memory, check in the file
            if (!found) {
                // Load all IDs from the file
                vector<string> existingVehicleIDs = loadVehicleIDs();
                auto it = find(existingVehicleIDs.begin(), existingVehicleIDs.end(), id);

                if (it != existingVehicleIDs.end()) {
                    cout << GREEN << "Vehicle ID found in file. Removing vehicle..." << endl;
                    ParkingSlot::freeSlot(); // Free slot after vehicle exits
                    removeVehicleDetails(id); // Remove vehicle details from file
                }
                else {
                    cout << RED << "Invalid Vehicle ID!" << endl;
                }
            }
            break;

        case 3:
            viewParkingReport();
            break;

        case 4:
            cout <<GREEN<< "Thank you for using the Parking Management System. Goodbye!\n";
            break;

        default:
            cout <<RED<< "Invalid choice! Try again.\n";
        }
    } while (choice != 4);

    return 0;
}