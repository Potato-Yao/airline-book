//
// Created by rusty-potato on 4/26/26.
//

#ifndef AIRLINE_BOOK_INTERFACE_H
#define AIRLINE_BOOK_INTERFACE_H
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

#include "state.h"
#include "../manager/database_manager.h"


class Interface {
    State state = Initial;
    DatabaseManager &database_manager;
    std::vector<std::string> book_tasks;
    std::vector<std::string> refund_tasks;

    Interface() : database_manager(DatabaseManager::get_manager()) {
    }

    void run_inner();

    void execute_list(const std::vector<std::string> &input) const;

    void execute_book(const std::vector<std::string> &input);

    void execute_refund(const std::vector<std::string> &input);

    void execute_commit();

    void execute_register(const std::vector<std::string> &input) const;

    void execute_exit();

    static void execute_help();

    void execute(const std::vector<std::string> &input);

    static void display_error(const std::string &error);

    static void display_flights(const std::vector<const FlightManager *> &input);

    static void display_single_line(std::string input[5], int sizes[5], int max_length);

    template<typename T>
    static void display_message(const T &message);

    static void display_commit();

    static std::vector<std::string> split_command(const std::string &input);

public:
    static Interface &get_interface();

    void run();
};


#endif //AIRLINE_BOOK_INTERFACE_H
