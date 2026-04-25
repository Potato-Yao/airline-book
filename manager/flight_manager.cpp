//
// Created by rusty-potato on 4/24/26.
//

#include "flight_manager.h"

#include <stdexcept>

FlightManager FlightManager::register_flight(std::string id, std::string start_station, std::string terminal_station,
                                             std::string flight_time, const int ticket_number) {
    const Flight flight(std::move(id), std::move(start_station), std::move(terminal_station), std::move(flight_time),
                        ticket_number);

    return FlightManager(flight);
}

FlightManager FlightManager::register_flight(const std::vector<std::string> &flight) {
    if (flight.size() != 5) {
        throw std::invalid_argument("Flight should have 5 fields");
    }

    return register_flight(flight[0], flight[1], flight[2], flight[3], std::stoi(flight[4]));
}

bool FlightManager::has_free_ticket() const {
    return this->flight.get_ticket_number() > 0;
}

void FlightManager::take_ticket() {
    if (!has_free_ticket()) {
        throw std::runtime_error("No free ticket");
    }
    this->flight.decrease_ticket_number();
}

void FlightManager::refund_ticket() {
    this->flight.increase_ticket_number();
}

std::vector<std::string> FlightManager::display_flight() const {
    return std::vector{
        this->flight.get_id(),
        this->flight.get_start_station(),
        this->flight.get_terminal_station(),
        this->flight.get_flight_time(),
        std::to_string(this->flight.get_ticket_number())
    };
}

std::string FlightManager::get_flight_id() const {
    return this->flight.get_id();
}

std::string FlightManager::get_flight_start_station() const {
    return this->flight.get_start_station();
}

std::string FlightManager::get_flight_terminal_station() const {
    return this->flight.get_terminal_station();
}

std::string FlightManager::get_flight_time() const {
    return this->flight.get_flight_time();
}

int FlightManager::get_flight_ticket_number() const {
    return this->flight.get_ticket_number();
}
