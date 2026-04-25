//
// Created by rusty-potato on 4/23/26.
//

#ifndef AIRLINE_BOOK_FLIGHT_H
#define AIRLINE_BOOK_FLIGHT_H

#include <string>

class Flight {
    std::string id;
    std::string start_station;
    std::string terminal_station;
    std::string flight_time; // YYYY-MM-DD-HH-MM
    int ticket_number = 0;

public:
    Flight(std::string id, std::string start_station, std::string terminal_station, std::string flight_time,
           int ticket_number);

    const std::string &get_id() const;

    const std::string &get_start_station() const;

    const std::string &get_terminal_station() const;

    const std::string &get_flight_time() const;

    int get_ticket_number() const;

    void increase_ticket_number();

    void decrease_ticket_number();
};


#endif //AIRLINE_BOOK_FLIGHT_H
