#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

const int SLEEPER_SEATS = 6;
const int RAC_SEATS = 1;
const int WAITING_LIST_SEATS = 1;

struct Passenger {
    string name;
    int age;
    char gender;
    string berthPreference;
    int seatNumber;
};

struct Ticket {
    int ticketId;
    Passenger passenger;
};

class ReservationSystem {
    vector<Ticket> tickets;
    int ticketID = 1;
    queue<int> lowerBerthSeats, middleBerthSeats, upperBerthSeats, racSeats, waitingListSeats;

public:
    ReservationSystem() {
        for (int i = 1; i <= SLEEPER_SEATS; i += 3) {
            lowerBerthSeats.push(i);
            middleBerthSeats.push(i + 1);
            upperBerthSeats.push(i + 2);
        }
        for (int i = 1; i <= RAC_SEATS; ++i) racSeats.push(i);
        for (int i = 1; i <= WAITING_LIST_SEATS; ++i) waitingListSeats.push(i);
    }

    void bookTicket();
    void cancelTicket(int ticketID);
    void printAvailability();
    void printBookedTickets();
};

void ReservationSystem::cancelTicket(int ticketID) {
    auto it = find_if(tickets.begin(), tickets.end(), [ticketID](const Ticket& t) { return t.ticketId == ticketID; });
    if (it != tickets.end()) {
        string berth = it->passenger.berthPreference;
        int seatNumber = it->passenger.seatNumber;
        tickets.erase(it);
        cout << "Ticket cancelled successfully!" << endl;

        if (berth == "RAC") {
            racSeats.push(seatNumber);
            auto racIt = find_if(tickets.begin(), tickets.end(), [](const Ticket& t) { return t.passenger.berthPreference == "RAC"; });
            if (racIt != tickets.end()) {
                racSeats.push(racIt->passenger.seatNumber);
                racIt->passenger.berthPreference = berth;
                racIt->passenger.seatNumber = seatNumber;
                auto wlIt = find_if(tickets.begin(), tickets.end(), [](const Ticket& t) { return t.passenger.berthPreference == "WL"; });
                if (wlIt != tickets.end()) {
                    waitingListSeats.push(wlIt->passenger.seatNumber);
                    wlIt->passenger.seatNumber = racSeats.front();
                    racSeats.pop();
                    wlIt->passenger.berthPreference = "RAC";
                }
                return;
            }
        }

        if (berth == "LB") lowerBerthSeats.push(seatNumber);
        else if (berth == "MB") middleBerthSeats.push(seatNumber);
        else if (berth == "UB") upperBerthSeats.push(seatNumber);
    } else {
        cout << "Ticket ID not found" << endl;
    }
}

void ReservationSystem::bookTicket() {
    if (tickets.size() == SLEEPER_SEATS + RAC_SEATS + WAITING_LIST_SEATS) {
        cout << "No tickets available" << endl;
        return;
    }

    Passenger passenger;
    cout << "Enter passenger details:\nName: ";
    cin.ignore();
    getline(cin, passenger.name);
    cout << "Age: ";
    cin >> passenger.age;
    cout << "Gender (M/F): ";
    cin >> passenger.gender;
    cout << "Berth Preference (LB/MB/UB): ";
    cin >> passenger.berthPreference;

    if (passenger.age < 5) {
        cout << "No need for ticket reservation for age below 5" << endl;
        return;
    }

    if (passenger.age > 60) {
        passenger.berthPreference = "LB";
        if (!lowerBerthSeats.empty()) {
            passenger.seatNumber = lowerBerthSeats.front();
            lowerBerthSeats.pop();
        } else {
            cout << "Lower berth for senior citizen not available!" << endl;
            return;
        }
        cout << "Your berth preference changed to LB" << endl;
    }

    auto allocateSeat = [this, &passenger]() {
        if (passenger.berthPreference == "LB" && !lowerBerthSeats.empty()) passenger.seatNumber = lowerBerthSeats.front(), lowerBerthSeats.pop();
        else if (passenger.berthPreference == "MB" && !middleBerthSeats.empty()) passenger.seatNumber = middleBerthSeats.front(), middleBerthSeats.pop();
        else if (passenger.berthPreference == "UB" && !upperBerthSeats.empty()) passenger.seatNumber = upperBerthSeats.front(), upperBerthSeats.pop();
        else if (!lowerBerthSeats.empty()) { passenger.seatNumber = lowerBerthSeats.front(); passenger.berthPreference = "LB"; lowerBerthSeats.pop(); }
        else if (!middleBerthSeats.empty()) { passenger.seatNumber = middleBerthSeats.front(); passenger.berthPreference = "MB"; middleBerthSeats.pop(); }
        else if (!upperBerthSeats.empty()) { passenger.seatNumber = upperBerthSeats.front(); passenger.berthPreference = "UB"; upperBerthSeats.pop(); }
    };

    if (!lowerBerthSeats.empty() || !middleBerthSeats.empty() || !upperBerthSeats.empty()) {
        allocateSeat();
        tickets.push_back({ticketID++, passenger});
        cout << "Ticket booked successfully, Your berth is confirmed. Ticket ID: " << ticketID - 1 << endl;
    } else if (!racSeats.empty()) {
        passenger.seatNumber = racSeats.front();
        passenger.berthPreference = "RAC";
        racSeats.pop();
        tickets.push_back({ticketID++, passenger});
        cout << "Ticket booked successfully, Your ticket is in RAC. Ticket ID: " << ticketID - 1 << endl;
    } else if (!waitingListSeats.empty()) {
        passenger.seatNumber = waitingListSeats.front();
        passenger.berthPreference = "WL";
        waitingListSeats.pop();
        tickets.push_back({ticketID++, passenger});
        cout << "Ticket booked successfully, Your ticket is in Waiting List. Ticket ID: " << ticketID - 1 << endl;
    }
}

void ReservationSystem::printBookedTickets() {
    for (const auto& t : tickets) {
        cout << "Ticket ID:" << t.ticketId << ", Name:" << t.passenger.name << ", Age:" << t.passenger.age 
             << ", Gender:" << t.passenger.gender << ", Berth:" << t.passenger.seatNumber << t.passenger.berthPreference 
             << ", Status:" << (t.passenger.berthPreference == "RAC" ? "RAC" : (t.passenger.berthPreference == "WL" ? "WL" : "Confirmed")) 
             << endl;
    }
}

void ReservationSystem::printAvailability() {
    cout << "Available Sleeper Tickets: Lower Berth:" << lowerBerthSeats.size() << ", Middle Berth:" << middleBerthSeats.size() 
         << ", Upper Berth:" << upperBerthSeats.size() << endl;
    cout << "Available RAC Tickets:" << racSeats.size() << endl;
    cout << "Available Waiting List Tickets:" << waitingListSeats.size() << endl;
}

int main() {
    ReservationSystem system;
    int choice;
    do {
        cout << "\nRailway Reservation System Menu:\n";
        cout << "1. Book Ticket\n2. Cancel Ticket\n3. Print Booked Tickets\n4. Print Available Tickets\n0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice) {
        case 1: system.bookTicket(); break;
        case 2: {
            int ID;
            cout << "Enter Ticket ID: ";
            cin >> ID;
            system.cancelTicket(ID);
            break;
        }
        case 3: system.printBookedTickets(); break;
        case 4: system.printAvailability(); break;
        case 0: cout << "Exiting program. Thank you!"; break;
        default: cout << "Invalid choice, please try again." << endl; break;
        }
    } while (choice != 0);
    return 0;
}