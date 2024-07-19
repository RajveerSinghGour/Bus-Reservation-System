#include <iostream>
#include <vector>
#include <string>
#include <algorithm> // For transform() and tolower()
#include <limits>    // For numeric_limits
#include <iomanip>   // For setw() and left/right alignment

using namespace std;

struct Bus
{
  string busNumber;
  string destination;
  string sourceCity;
  vector<bool> seats; // true if booked, false if available
  double ticketPrice;

  Bus(const string &num, const string &dest, const string &source, int totalSeats, double price)
      : busNumber(num), destination(dest), sourceCity(source), seats(totalSeats, false), ticketPrice(price) {}
};

class BusReservationSystem
{
private:
  vector<Bus> buses;

  // Convert a string to lowercase
  string toLower(const string &str) const
  {
    string lowerStr = str;
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
  }

  // Trim leading and trailing whitespace from a string
  string trim(const string &str) const
  {
    size_t start = str.find_first_not_of(" \t\n\r");
    size_t end = str.find_last_not_of(" \t\n\r");
    return (start == string::npos || end == string::npos) ? "" : str.substr(start, end - start + 1);
  }

  void printLine(char ch, int length) const
  {
    cout << string(length, ch) << endl;
  }

public:
  void addBus(const string &busNumber, const string &destination, const string &sourceCity, int totalSeats, double price)
  {
    buses.push_back(Bus(busNumber, destination, sourceCity, totalSeats, price));
  }

  void displayBuses()
  {
    cout << "Available Buses:" << endl;
    printLine('=', 105);
    cout << left << setw(10) << "Index" << setw(15) << "Bus Number" << setw(20) << "Source City"
         << setw(20) << "Destination" << setw(15) << "Total Seats" << setw(15) << "Booked Seats"
         << "Ticket Price" << endl;
    printLine('-', 105);

    for (size_t i = 0; i < buses.size(); ++i)
    {
      const Bus &bus = buses[i];
      cout << left << setw(10) << (i + 1) << setw(15) << bus.busNumber
           << setw(20) << bus.sourceCity << setw(20) << bus.destination
           << setw(15) << bus.seats.size() << setw(15) << countBookedSeats(bus)
           << "$" << fixed << setprecision(2) << bus.ticketPrice << endl;
    }
    printLine('=', 105);
  }

  void searchBuses(const string &sourceCity, const string &destination)
  {
    string sourceLower = toLower(trim(sourceCity));
    string destLower = toLower(trim(destination));

    cout << "Searching for buses from " << sourceCity << " to " << destination << ":" << endl;
    printLine('=', 105);
    cout << left << setw(10) << "Index" << setw(15) << "Bus Number" << setw(20) << "Source City"
         << setw(20) << "Destination" << setw(15) << "Total Seats" << setw(15) << "Booked Seats"
         << "Ticket Price" << endl;
    printLine('-', 105);

    bool found = false;
    for (size_t i = 0; i < buses.size(); ++i)
    {
      const Bus &bus = buses[i];
      string busSourceLower = toLower(trim(bus.sourceCity));
      string busDestLower = toLower(trim(bus.destination));

      if (busSourceLower == sourceLower && busDestLower == destLower)
      {
        cout << left << setw(10) << (i + 1) << setw(15) << bus.busNumber
             << setw(20) << bus.sourceCity << setw(20) << bus.destination
             << setw(15) << bus.seats.size() << setw(15) << countBookedSeats(bus)
             << "$" << fixed << setprecision(2) << bus.ticketPrice << endl;
        found = true;
      }
    }
    if (!found)
    {
      cout << "No buses found for the given source and destination." << endl;
    }
    printLine('=', 105);
  }

  void bookSeats()
  {
    displayBuses();

    int busIndex;
    cout << "Enter the bus index to book seats (starting from 1): ";
    cin >> busIndex;

    if (busIndex < 1 || busIndex > buses.size())
    {
      cout << "Invalid bus index!" << endl;
      return;
    }

    Bus &bus = buses[busIndex - 1];
    displaySeatStatus(bus);

    int numSeats;
    cout << "Enter the number of seats you want to book: ";
    cin >> numSeats;

    if (numSeats <= 0 || numSeats > bus.seats.size())
    {
      cout << "Invalid number of seats!" << endl;
      return;
    }

    vector<int> seatsToBook(numSeats);
    cout << "Enter the seat numbers (separated by spaces):" << endl;
    for (int i = 0; i < numSeats; ++i)
    {
      cin >> seatsToBook[i];
      if (seatsToBook[i] < 1 || seatsToBook[i] > bus.seats.size() || bus.seats[seatsToBook[i] - 1])
      {
        cout << "Seat number " << seatsToBook[i] << " is invalid or already booked!" << endl;
        --i; // Decrement counter to re-enter this seat number
      }
    }

    for (int seat : seatsToBook)
    {
      bus.seats[seat - 1] = true;
    }

    cout << "Booking successful! Total cost: $" << numSeats * bus.ticketPrice << endl;
  }

  void viewReservations()
  {
    displayBuses();
    int busIndex;
    cout << "Enter the bus index to view reservations (starting from 1): ";
    cin >> busIndex;

    if (busIndex < 1 || busIndex > buses.size())
    {
      cout << "Invalid bus index!" << endl;
      return;
    }

    const Bus &bus = buses[busIndex - 1];
    cout << "Bus Number: " << bus.busNumber
         << ", Destination: " << bus.destination
         << ", Source City: " << bus.sourceCity
         << ", Total Seats: " << bus.seats.size()
         << ", Booked Seats: " << countBookedSeats(bus)
         << ", Ticket Price: $" << fixed << setprecision(2) << bus.ticketPrice << endl;
    displaySeatStatus(bus);
  }

private:
  int countBookedSeats(const Bus &bus)
  {
    int count = 0;
    for (bool seat : bus.seats)
    {
      if (seat)
        ++count;
    }
    return count;
  }

  void displaySeatStatus(const Bus &bus)
  {
    cout << "Seat Status for Bus " << bus.busNumber << ":" << endl;
    printLine('-', 30);
    cout << left << setw(10) << "Seat No." << "Status" << endl;
    printLine('-', 30);

    for (size_t i = 0; i < bus.seats.size(); ++i)
    {
      cout << left << setw(10) << (i + 1) << (bus.seats[i] ? "Booked" : "Available") << endl;
    }
    printLine('-', 30);
  }
};

int main()
{
  BusReservationSystem system;

  // Adding some buses
  system.addBus("123A", "New York", "Boston", 50, 30.0);
  system.addBus("456B", "Los Angeles", "San Francisco", 40, 25.0);
  system.addBus("789C", "Chicago", "Detroit", 30, 20.0);
  system.addBus("012D", "New York", "Boston", 50, 28.0); // Another bus for the same route

  int choice;
  while (true)
  {
    cout << "\nBus Reservation System\n";
    cout << "1. View Buses\n";
    cout << "2. Search Buses\n";
    cout << "3. Book Seats\n";
    cout << "4. View Reservations\n";
    cout << "5. Exit\n";
    cout << "Enter your choice: ";
    cin >> choice;

    switch (choice)
    {
    case 1:
      system.displayBuses();
      break;
    case 2:
    {
      string sourceCity, destination;
      cout << "Enter source city: ";
      cin.ignore();
      getline(cin, sourceCity);
      cout << "Enter destination: ";
      getline(cin, destination);

      system.searchBuses(sourceCity, destination);
      break;
    }
    case 3:
      system.bookSeats();
      break;
    case 4:
      system.viewReservations();
      break;
    case 5:
      cout << "Exiting..." << endl;
      return 0;
    default:
      cout << "Invalid choice! Please try again." << endl;
    }
  }
}
