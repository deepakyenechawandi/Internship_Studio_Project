#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <algorithm>
#include <unordered_map>
#include <iomanip> // Include <iomanip> for std::get_time

using namespace std;

// Structure to represent a room booking
struct Booking {
    string host;
    time_t start_time;
    time_t end_time;
    int chairs;
    int room_number;

    // Constructor
    Booking(string _host, time_t _start_time, time_t _end_time, int _chairs, int _room_number)
        : host(_host), start_time(_start_time), end_time(_end_time), chairs(_chairs), room_number(_room_number) {}
};

// Class to manage room bookings and availability
class RoomBookingSystem {
private:
    vector<Booking> bookings;
    unordered_map<int, vector<pair<time_t, time_t>>> room_schedule;

public:
    // Function to book a room
    void bookRoom(string host, time_t start_time, time_t end_time, int chairs, int room_number) {
        bookings.emplace_back(host, start_time, end_time, chairs, room_number);
        room_schedule[room_number].push_back({start_time, end_time});
    }

    // Function to update a booked room
    void updateBookedRoom(int index, string host, time_t start_time, time_t end_time, int chairs, int room_number) {
        // Update booking details
        bookings[index].host = host;
        bookings[index].start_time = start_time;
        bookings[index].end_time = end_time;
        bookings[index].chairs = chairs;
        bookings[index].room_number = room_number;

        // Update room schedule
        room_schedule[room_number].push_back({start_time, end_time});
    }

    // Function to check room availability for a given time range
    bool isRoomAvailable(int room_number, time_t start_time, time_t end_time) {
        auto& schedule = room_schedule[room_number];
        for (const auto& slot : schedule) {
            if (!(end_time <= slot.first || start_time >= slot.second)) {
                return false; // Room is not available
            }
        }
        return true; // Room is available
    }

    // Function to get the number of available rooms
    int getNumberOfAvailableRooms() {
        return 25 - room_schedule.size(); // Assuming room numbers are between 1 and 25
    }

    // Function to get the number of booked rooms
    int getNumberOfBookedRooms() {
        return bookings.size();
    }

    // Function to check room status
    void checkRoomStatus() {
        cout << "Room Status:\n";
        for (const auto& booking : bookings) {
            char startBuffer[80];
            char endBuffer[80];
            strftime(startBuffer, 80, "%Y-%m-%d %H:%M", localtime(&booking.start_time));
            strftime(endBuffer, 80, "%Y-%m-%d %H:%M", localtime(&booking.end_time));

            cout << "Room " << booking.room_number << " booked by " << booking.host
                 << " from " << startBuffer << " to " << endBuffer
                 << " with " << booking.chairs << " chairs.\n";
        }
    }
};

int main() {
    RoomBookingSystem roomSystem;
    int choice;

    do {
        cout << "\nRoom Allotment System\n";
        cout << "1. Book a room\n";
        cout << "2. Update booked room\n";
        cout << "3. Check room status\n";
        cout << "4. Check number of available rooms\n";
        cout << "5. Check number of booked rooms\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string host;
                tm start_tm, end_tm;
                int chairs, room_number;

                cout << "Enter host name: ";
                cin >> host;
                cout << "Enter start time (YYYY-MM-DD HH:MM): ";
                cin >> get_time(&start_tm, "%Y-%m-%d %H:%M");
                cout << "Enter end time (YYYY-MM-DD HH:MM): ";
                cin >> get_time(&end_tm, "%Y-%m-%d %H:%M");
                cout << "Enter number of chairs needed: ";
                cin >> chairs;
                cout << "Enter room number (1-25): ";
                cin >> room_number;

                // Convert tm to time_t
                time_t start_time = mktime(&start_tm);
                time_t end_time = mktime(&end_tm);

                if (roomSystem.isRoomAvailable(room_number, start_time, end_time)) {
                    roomSystem.bookRoom(host, start_time, end_time, chairs, room_number);
                    cout << "Room booked successfully!\n";
                } else {
                    cout << "Room is not available for the given time range.\n";
                }
                break;
            }
            case 2: {
                int index;
                string host;
                tm start_tm, end_tm;
                int chairs, room_number;

                cout << "Enter index of the booking to update: ";
                cin >> index;
                cout << "Enter new host name: ";
                cin >> host;
                cout << "Enter new start time (YYYY-MM-DD HH:MM): ";
                cin >> get_time(&start_tm, "%Y-%m-%d %H:%M");
                cout << "Enter new end time (YYYY-MM-DD HH:MM): ";
                cin >> get_time(&end_tm, "%Y-%m-%d %H:%M");
                cout << "Enter new number of chairs needed: ";
                cin >> chairs;
                cout << "Enter new room number (1-25): ";
                cin >> room_number;

                // Convert tm to time_t
                time_t start_time = mktime(&start_tm);
                time_t end_time = mktime(&end_tm);

                if (roomSystem.isRoomAvailable(room_number, start_time, end_time)) {
                    roomSystem.updateBookedRoom(index, host, start_time, end_time, chairs, room_number);
                    cout << "Room updated successfully!\n";
                } else {
                    cout << "Room is not available for the given time range.\n";
                }
                break;
            }
            case 3:
                roomSystem.checkRoomStatus();
                break;
            case 4:
                cout << "Number of available rooms: " << roomSystem.getNumberOfAvailableRooms() << endl;
                break;
            case 5:
                cout << "Number of booked rooms: " << roomSystem.getNumberOfBookedRooms() << endl;
                break;
            case 6:
                cout << "Exiting program.\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 6);

    return 0;
}
