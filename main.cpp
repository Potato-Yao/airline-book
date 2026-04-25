#include <iostream>
#include "database/csv_handler.h"
#include "manager/database_manager.h"

void init();

void run_app();

int main() {
    init();
    // CSVHandler csv_handler("data.csv");
    // // if (!csv_handler.is_initialized()) {
    // auto titles = std::vector<std::string>{"name", "age", "email"};
    // csv_handler.init(titles);
    // // }
    // csv_handler.insert("Alice");
    // csv_handler.insert("Bob", "121", "ddd");
    // csv_handler.insert("Clay", "332", "fff");
    // csv_handler.insert("David", "23", "ggg");
    // auto l = csv_handler.read_row(1);
    // for (const auto &s: l) {
    //     std::cout << s << std::endl;
    // }
    // // csv_handler.remove_row(1);
    // std::string name("Bob jr.");
    // csv_handler.update_cell(std::vector{new Cell("Bob", "name", name)});
    // l = csv_handler.read_row(1);
    // for (const auto &s: l) {
    //     std::cout << s << std::endl;
    // }
    //
    // auto test = csv_handler.read_rows(0, -1);
    // for (const auto &s: test) {
    //     for (const auto &t: s) {
    //         std::cout << t << std::endl;
    //     }
    // }
    // test = csv_handler.read_rows(0, 2);
    // for (const auto &s: test) {
    //     for (const auto &t: s) {
    //         std::cout << t << std::endl;
    //     }
    // }
    // run_app();
    auto manager = DatabaseManager::get_manager();
    auto flight0 = FlightManager::register_flight("123", "Beijing", "Shanghai", "2024-06-01-12-00", 100);
    auto flight1 = FlightManager::register_flight("456", "Shanghai", "Guangzhou", "2024-06-02-12-00", 200);
    auto flight2 = FlightManager::register_flight("789", "Guangzhou", "Shenzhen", "2024-06-02-12-00", 300);
    auto flight3 = FlightManager::register_flight("012", "Guangzhou", "Shenzhen", "2024-06-04-12-00", 400);
    manager.register_flight(flight0);
    manager.register_flight(flight1);
    manager.register_flight(flight2);
    manager.register_flight(flight3);
    for (auto e: manager.get_flights()) {
        std::cout << e.get_flight_id() << " " << e.get_flight_start_station() << " " << e.get_flight_terminal_station()
                << " " << e.get_flight_time() << " " << e.get_flight_ticket_number() << std::endl;
    }
    manager.book_ticket_from(std::vector<std::string>{"123", "456"});
    for (auto e: manager.get_flights()) {
        std::cout << e.get_flight_id() << " " << e.get_flight_start_station() << " " << e.get_flight_terminal_station()
                << " " << e.get_flight_time() << " " << e.get_flight_ticket_number() << std::endl;
    }
    manager.refund_ticket_from(std::vector<std::string>{"123", "456"});
    for (auto e: manager.get_flights()) {
        std::cout << e.get_flight_id() << " " << e.get_flight_start_station() << " " << e.get_flight_terminal_station()
                << " " << e.get_flight_time() << " " << e.get_flight_ticket_number() << std::endl;
    }
    for (auto e: manager.query_flight_by_terminal("Shenzhen")) {
        std::cout << e->get_flight_id() << " " << e->get_flight_start_station() << " " << e->get_flight_terminal_station()
                << " " << e->get_flight_time() << " " << e->get_flight_ticket_number() << std::endl;
    }

    return 0;
}

void init() {
}

void run_app() {
    while (true) {
    }
}
