//
// Created by rusty-potato on 4/26/26.
//

#include "interface.h"

void Interface::run_inner() {
    display_message("Welcome to the airline booking system!");
    display_message("Type help for help, type exit for exiting");

    std::vector<std::string> lines;
    std::string input;
    std::cout << "$ " << std::flush;
    while (state != Exit) {
        if (symbol_mode == None) {
            symbol_mode = Dollar;
            std::cout << "$ " << std::flush;
        }

        getline(std::cin, input);
        execute(split_command(input));

        if (symbol_mode == Dollar) {
            std::cout << "$ " << std::flush;
        } else if (symbol_mode == Arrow) {
            std::cout << "> " << std::flush;
        }
    }
}

void Interface::execute_list(const std::vector<std::string> &input) {
    if (input.size() == 1) {
        display_flights(database_manager.get_flights());
    } else {
        // command option argument
        if (input.size() != 3) {
            display_error("list command requires one option and one argument!");
        } else {
            if (input[1] == "id") {
                display_flights(database_manager.query_flight_by_id(input[2]));
            } else if (input[1] == "time") {
                display_flights(database_manager.query_flight_by_time(input[2]));
            } else if (input[1] == "terminal") {
                display_flights(database_manager.query_flight_by_terminal(input[2]));
            } else {
                display_error("unknown option for list!");
            }
        }
    }
}

void Interface::execute_book(const std::vector<std::string> &input) {
    if (input.size() != 2) {
        display_error("book command requires id as argument!");
    } else {
        book_tasks.push_back(input[1]);
    }

    symbol_mode = Arrow;
}

void Interface::execute_refund(const std::vector<std::string> &input) {
    if (input.size() != 2) {
        display_error("refund command requires id as argument!");
    } else {
        refund_tasks.push_back(input[1]);
    }

    symbol_mode = Arrow;
}

void Interface::execute_commit() {
    database_manager.book_ticket_from(book_tasks);
    database_manager.refund_ticket_from(refund_tasks);

    book_tasks.clear();
    refund_tasks.clear();

    symbol_mode = Dollar;
}

void Interface::execute_register(const std::vector<std::string> &input) {
    // command arguments[five]
    if (input.size() != 6) {
        display_error("register command requires five arguments!");
    } else {
        try {
            auto manager = FlightManager::register_flight(std::vector<std::string>{
                input.begin() + 1, input.end()
            });
            database_manager.register_flight(manager);
        } catch (const std::exception &e) {
            display_error(e.what());
        }
    }
}

void Interface::execute_help() {
    const auto runtime_path = std::filesystem::path(".") / "grammar.txt";
    std::ifstream grammar_file(runtime_path);

    if (!grammar_file.is_open()) {
        const auto source_path = std::filesystem::path(__FILE__).parent_path() / "grammar.txt";
        grammar_file.open(source_path);

        if (!grammar_file.is_open()) {
            display_error("Cannot open grammar file: " + runtime_path.string());
            return;
        }
    }

    std::cout << grammar_file.rdbuf();
}

void Interface::execute_exit() {
    database_manager.close();
    state = Exit;
}

void Interface::execute(const std::vector<std::string> &input) {
    if (input.empty()) {
        return;
    }

    try {
        if (input[0] == LIST) {
            execute_list(input);
        } else if (input[0] == BOOK) {
            execute_book(input);
        } else if (input[0] == REFUND) {
            execute_refund(input);
        } else if (input[0] == COMMIT) {
            execute_commit();
        } else if (input[0] == REGISTER) {
            execute_register(input);
        } else if (input[0] == HELP) {
            execute_help();
        } else if (input[0] == EXIT) {
            execute_exit();
        } else {
            display_error("Unknown command!");
        }
    } catch (const std::exception &e) {
        display_error(e.what());
    }
}

void Interface::display_error(const std::string &error) {
    symbol_mode = None;
    // do not use std error, output order issus may occur
    std::cout << "\033[31m" << "Error: " << error << "\033[0m" << std::endl;
}

void Interface::display_flights(const std::vector<const FlightManager *> &input) {
    int sizes[]{0, 0, 0, 0, 0};

    for (const auto *flight: input) {
        sizes[0] = std::max(sizes[0], static_cast<int>(flight->get_flight_id().size()));
        sizes[1] = std::max(sizes[1], static_cast<int>(flight->get_flight_start_station().size()));
        sizes[2] = std::max(sizes[2], static_cast<int>(flight->get_flight_terminal_station().size()));
        sizes[3] = std::max(sizes[3], static_cast<int>(flight->get_flight_time().size()));
        sizes[4] = std::max(sizes[4], static_cast<int>(std::to_string(flight->get_flight_ticket_number()).size()));
    }

    int max_length = 0;
    for (const int size: sizes) {
        max_length += size;
        max_length += 3; // two space, one |
    }

    const auto bar = std::string(max_length, '-');
    std::cout << bar << '\n';
    for (const auto *flight: input) {
        std::string par[] = {
            flight->get_flight_id(),
            flight->get_flight_start_station(),
            flight->get_flight_terminal_station(),
            flight->get_flight_time(),
            std::to_string(flight->get_flight_ticket_number()),
        };
        display_single_line(par, sizes, max_length);
        std::cout << bar << '\n';
    }
}

void Interface::display_single_line(std::string input[5], int sizes[5], int max_length) {
    std::cout << '|';
    for (int i = 0; i < 5; ++i) {
        std::cout << ' ' << input[i] << std::string(sizes[i] - input[i].size() + 1, ' ') << '|';
    }
    std::cout << '\n';
}

template<typename T>
void Interface::display_message(const T &message) {
    std::cout << message << '\n';
}

void Interface::display_commit() {
    std::cout << std::endl;
}

std::vector<std::string> Interface::split_command(const std::string &input) {
    std::vector<std::string> result;
    std::stringstream ss(input);
    std::string word;

    while (ss >> word) {
        result.push_back(word);
    }

    return result;
}

Interface &Interface::get_interface() {
    static Interface instance;
    return instance;
}

void Interface::run() {
    run_inner();
}
