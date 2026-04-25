//
// Created by rusty-potato on 4/23/26.
//

#include "flight.h"

#include <stdexcept>
#include <utility>

Flight::Flight(std::string id, std::string start_station, std::string terminal_station, std::string flight_time,
               const int ticket_number)
    : id(std::move(id)), start_station(std::move(start_station)), terminal_station(std::move(terminal_station)),
      flight_time(std::move(flight_time)), ticket_number(ticket_number) {
}

const std::string &Flight::get_id() const {
    return id;
}

const std::string &Flight::get_start_station() const {
    return start_station;
}

const std::string &Flight::get_terminal_station() const {
    return terminal_station;
}

const std::string &Flight::get_flight_time() const {
    return flight_time;
}

int Flight::get_ticket_number() const {
    return ticket_number;
}

void Flight::increase_ticket_number() {
    this->ticket_number++;
}

void Flight::decrease_ticket_number() {
    this->ticket_number--;
}
