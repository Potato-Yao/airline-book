//
// Created by rusty-potato on 4/25/26.
//

#ifndef AIRLINE_BOOK_DATABASE_MANAGER_H
#define AIRLINE_BOOK_DATABASE_MANAGER_H

#include "../database/csv_handler.h"
#include "flight_manager.h"


class DatabaseManager {
    constexpr static auto database_path = "__airline_data__.csv";
    CSVHandler csv_handler;
    std::vector<FlightManager> flight_managers;

    DatabaseManager();

    FlightManager &query_flight_by_id_inner(const std::string &id);
public:
    static DatabaseManager &get_manager();

    void register_flight(FlightManager &manager);

    const std::vector<FlightManager> &get_flights();

    void book_ticket_from(const std::vector<std::string> &ids);

    void refund_ticket_from(const std::vector<std::string> &ids);

    const FlightManager &query_flight_by_id(const std::string &id);

    std::vector<const FlightManager *> query_flight_by_time(const std::string &time) const;

    std::vector<const FlightManager *> query_flight_by_terminal(const std::string &terminal) const;
};


#endif //AIRLINE_BOOK_DATABASE_MANAGER_H
