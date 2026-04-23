#include <iostream>
#include "database/csv_handler.h"

void init();
void run_app();

int main() {
    init();
    CSVHandler csv_handler("data.csv");
    // if (!csv_handler.is_initialized()) {
        auto titles = std::vector<std::string>{"name", "age", "email"};
        csv_handler.init(titles);
    // }
    csv_handler.insert("Alice");
    csv_handler.insert("Bob", "121", "ddd");
    csv_handler.insert("Clay", "332", "fff");
    auto l = csv_handler.read_row(1);
    for (const auto &s : l) {
        std::cout << s << std::endl;
    }
    // csv_handler.remove_row(1);
    std::string name("Bob jr.");
    csv_handler.update_cell(1, 0, name);
    l = csv_handler.read_row(1);
    for (const auto &s : l) {
        std::cout << s << std::endl;
    }

    // run_app();

    return 0;
}

void init() {
}

void run_app() {
    while (true) {

    }
}
