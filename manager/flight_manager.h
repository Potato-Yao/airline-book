//
// Created by rusty-potato on 4/24/26.
//

#ifndef AIRLINE_BOOK_FLIGHT_HANDLER_H
#define AIRLINE_BOOK_FLIGHT_HANDLER_H

#include <vector>

#include "../dao/flight.h"

class FlightManager {
    Flight flight;

    explicit FlightManager(Flight flight) : flight(std::move(flight)) {
    }

public:
    static FlightManager register_flight(std::string id, std::string start_station, std::string terminal_station,
                                         std::string flight_time, int ticket_number);

    static FlightManager register_flight(const std::vector<std::string> &flight);

    bool has_free_ticket() const;

    void take_ticket();

    void refund_ticket();

    std::vector<std::string> display_flight() const;

    std::string get_flight_id() const;

    std::string get_flight_start_station() const;

    std::string get_flight_terminal_station() const;

    std::string get_flight_time() const;

    int get_flight_ticket_number() const;
};


#endif //AIRLINE_BOOK_FLIGHT_HANDLER_H
