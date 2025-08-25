#include <iostream>
#include <vector>
#include <string>
using namespace std;

// Abstract class for Users
class User {
protected:
    string name;
    string contact;

public:
    User(const string& name, const string& contact) : name(name), contact(contact) {}
    virtual ~User() {}
    virtual void displayDetails() const = 0;
    virtual string getDestination() const = 0;
};

// CarOwner class
class CarOwner : public User {
private:
    string carModel;
    string licensePlate;
    string destination;
    int seatCapacity;
    int occupiedSeats;
    double totalFare;
    bool matched;  // New flag to track if the car owner has been matched

public:
    CarOwner(const string& name, const string& contact, const string& carModel,
        const string& licensePlate, const string& destination, int seatCapacity, double totalFare)
        : User(name, contact), carModel(carModel), licensePlate(licensePlate), destination(destination),
        seatCapacity(seatCapacity), occupiedSeats(1), totalFare(totalFare), matched(false) {}

    bool isMatched() const {
        return matched;
    }

    void setMatched() {
        matched = true;
    }

    void displayDetails() const override {
        cout << "Car Owner    : " << name << endl;
        cout << "Contact      : " << contact << endl;
        cout << "Car          : " << carModel << endl;
        cout << "License Plate: " << licensePlate << endl;
        cout << "Destination  : " << destination << endl;
        cout << "Seats (Occupied/Total): " << occupiedSeats << "/" << seatCapacity << endl;
        cout << "Total Fare   : " << totalFare << " PKR" << endl;
        cout << "Matched      : " << (matched ? "Yes" : "No") << endl;
    }

    string getDestination() const override {
        return destination;
    }

    bool isFull() const {
        return occupiedSeats >= seatCapacity;
    }

    void addPassenger() {
        if (!isFull()) {
            occupiedSeats++;
        }
    }

    int getAvailableSeats() const {
        return seatCapacity - occupiedSeats;
    }

    double getTotalFare() const {
        return totalFare;
    }
};

// Passenger class
class Passenger : public User {
private:
    string destination;

public:
    Passenger(const string& name, const string& contact, const string& destination)
        : User(name, contact), destination(destination) {}

    void displayDetails() const override {
        cout << "Passenger   : " << name << endl;
        cout << "Contact     : " << contact << endl;
        cout << "Destination : " << destination << endl;
    }

    string getDestination() const override {
        return destination;
    }
};

// Trip class
class Trip {
private:
    vector<CarOwner> carOwners;
    vector<Passenger> passengers;
    vector<Passenger> waitingList;

public:
    void registerCarOwner(const CarOwner& owner) {
        carOwners.push_back(owner);
    }

    void registerPassenger(const Passenger& passenger) {
        passengers.push_back(passenger);
    }

    void calculateFare(const CarOwner& owner, int passengerCount) const {
        int totalParticipants = passengerCount + 1;  // Including the driver
        double farePerParticipant = owner.getTotalFare() / totalParticipants;

        cout << "Total Fare: " << owner.getTotalFare() << " PKR" << endl;
        cout << "Each Participant Pays: " << farePerParticipant << " PKR" << endl;
    }

    void matchTrip() {
        waitingList.clear();  // Clear the waiting list at the start

        for (CarOwner& owner : carOwners) {
            if (owner.isMatched()) {
                continue;  // Skip car owners who have already been matched
            }

            int passengerCount = 0;
            cout << "\nChecking matches for Car Owner traveling to: " << owner.getDestination() << endl;

            for (auto it = passengers.begin(); it != passengers.end();) {
                Passenger& passenger = *it;

                if (owner.isFull()) {
                    break;  // Stop if the car owner has no available seats
                }

                if (passenger.getDestination() == owner.getDestination()) {
                    // Match found
                    cout << "\nMatch found!\n";
                    cout << "Car Owner Details:\n";
                    owner.displayDetails();
                    cout << "\nPassenger Details:\n";
                    passenger.displayDetails();

                    owner.addPassenger();
                    passengerCount++;

                    // Remove the matched passenger from the list
                    it = passengers.erase(it);
                }
                else {
                    ++it;
                }
            }

            if (passengerCount > 0) {
                calculateFare(owner, passengerCount);
                owner.setMatched();  // Mark the car owner as matched
            }
            else {
                cout << "No matches found for this car owner.\n";
            }
        }

        // Add remaining passengers to the waiting list
        for (const Passenger& passenger : passengers) {
            waitingList.push_back(passenger);
        }

        if (!waitingList.empty()) {
            cout << "\nPassengers in Waiting List:\n";
            for (const auto& passenger : waitingList) {
                passenger.displayDetails();
            }
        }
    }

    void displayAllParticipants() const {
        cout << "\nRegistered Car Owners:\n";
        for (const auto& owner : carOwners) {
            owner.displayDetails();
        }

        cout << "\nRegistered Passengers:\n";
        for (const auto& passenger : passengers) {
            passenger.displayDetails();
        }
    }
};

// Main function
int main() {
    Trip tripSystem;
    char choice;
    cout << "#############################################################   ECOROUTE (CARPOOLING SYSTEM)   #############################################################" << endl;

    do {
        cout << "\n                    Main Menu\n";
        cout << "                1. Register Car Owner\n";
        cout << "                2. Register Passenger\n";
        cout << "                3. Match Trip\n";
        cout << "                4. Display Waiting Participants\n";
        cout << "                5. Incase of any Emergency press 5\n";
        cout << "                6. Exit\n";
        cout << endl;
        cout << "                  Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case '1': {
            cout << "                Car owner details: " << endl;
            string name, contact, carModel, licensePlate, destination;
            int seatCapacity;
            double totalFare;
            cout << "Enter Car Owner Name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter Contact Number: ";
            getline(cin, contact);
            cout << "Enter Car Model: ";
            getline(cin, carModel);
            cout << "Enter License Plate: ";
            getline(cin, licensePlate);
            cout << "Enter Destination (from Islamabad to): ";
            getline(cin, destination);
            cout << "Enter Seat Capacity (5 or 7): ";
            cin >> seatCapacity;
            cout << "Enter Total Fare (in PKR): ";
            cin >> totalFare;

            CarOwner owner(name, contact, carModel, licensePlate, destination, seatCapacity, totalFare);
            tripSystem.registerCarOwner(owner);
            cout << "Car owner added successfully!" << endl;
            break;
        }
        case '2': {
            cout << "                Passenger details: " << endl;
            string name, contact, destination;
            cout << "Enter Passenger Name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter Contact Number: ";
            getline(cin, contact);
            cout << "Enter Destination (from Islamabad to): ";
            getline(cin, destination);

            Passenger passenger(name, contact, destination);
            tripSystem.registerPassenger(passenger);
            cout << "Passenger added successfully!" << endl;
            break;
        }
        case '3': {
            tripSystem.matchTrip();
            break;
        }
        case '4': {
            tripSystem.displayAllParticipants();
            break;
        }
        case '5': {
            cout << "Police is arriving shortly!\n";
            break;
        }
        case '6': {
            cout << "Exiting EcoRoute. Goodbye!\n";
            break;
        }
        default: {
            cout << "Invalid choice. Please try again.\n";
        }
        }
    } while (choice != '6');

    return 0;
}