//
// Created by rusty-potato on 4/25/26.
//

#include "./database_manager.h"

#include <stdexcept>
#include <utility>

DatabaseManager::DatabaseManager() : csv_handler(database_path) {
    csv_handler.init(std::vector<std::string>{
                         "id", "start_station", "terminal_station", "flight_time", "ticket_number"
                     }, Load);
    const auto flights_raw = csv_handler.load();
    flight_managers.reserve(flights_raw.size());

    for (auto &e: flights_raw) {
        flight_managers.push_back(FlightManager::register_flight(e));
    }
}

DatabaseManager &DatabaseManager::get_manager() {
    static DatabaseManager manager;
    return manager;
}

void DatabaseManager::close() {
    csv_handler.close();
}

void DatabaseManager::register_flight(FlightManager &manager) {
    for (const auto &existing: flight_managers) {
        if (existing.get_flight_id() == manager.get_flight_id()) {
            throw std::invalid_argument("Flight with id " + manager.get_flight_id() + " already exists");
        }
    }

    csv_handler.insert(manager.display_flight());
    flight_managers.push_back(std::move(manager));
}

std::vector<const FlightManager *> DatabaseManager::get_flights() const {
    std::vector<const FlightManager *> result;
    result.reserve(flight_managers.size());
    for (const auto &flight: flight_managers) {
        result.push_back(&flight);
    }
    return result;
}

FlightManager &DatabaseManager::query_flight_by_id_inner(const std::string &id) {
    for (auto &e: flight_managers) {
        if (e.get_flight_id() == id) {
            return e;
        }
    }

    throw std::out_of_range("Cannot find flight with id " + id);
}

void DatabaseManager::book_ticket_from(const std::vector<std::string> &ids) {
    std::vector<DBActionCell> action_cells;

    for (const auto &id: ids) {
        auto &flight = query_flight_by_id_inner(id);
        if (!flight.has_free_ticket()) {
            throw std::out_of_range("Flight has no free ticket with id " + id);
        }

        flight.take_ticket();
        action_cells.emplace_back(id, "ticket_number",
                                  std::to_string(flight.get_flight_ticket_number()));
    }
    csv_handler.update_cell(action_cells);
}

void DatabaseManager::refund_ticket_from(const std::vector<std::string> &ids) {
    std::vector<DBActionCell> action_cells;

    for (const auto &id: ids) {
        auto &flight = query_flight_by_id_inner(id);
        flight.refund_ticket();
        action_cells.emplace_back(id, "ticket_number",
                                  std::to_string(flight.get_flight_ticket_number()));
    }

    csv_handler.update_cell(action_cells);
}

std::vector<const FlightManager *> DatabaseManager::query_flight_by_id(const std::string &id) {
    return {&query_flight_by_id_inner(id)};
}

std::vector<const FlightManager *> DatabaseManager::query_flight_by_time(const std::string &time) const {
    std::vector<const FlightManager *> result;
    for (const auto &e: flight_managers) {
        if (e.get_flight_time() == time) {
            result.push_back(&e);
        }
    }

    if (result.empty()) {
        throw std::out_of_range("Cannot find flight with time " + time);
    }

    return result;
}

std::vector<const FlightManager *> DatabaseManager::query_flight_by_terminal(const std::string &terminal) const {
    std::vector<const FlightManager *> result;
    for (const auto &e: flight_managers) {
        if (e.get_flight_terminal_station() == terminal) {
            result.push_back(&e);
        }
    }

    if (result.empty()) {
        throw std::out_of_range("Cannot find flight with terminal " + terminal);
    }

    return result;
}
