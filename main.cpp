#include <iostream>

#include "cli/interface.h"

int main() {
    try {
        auto interface = Interface::get_interface();
        interface.run();
    } catch (const std::exception &e) {
        std::cerr << "\033[31m" << "Fatal error: " << e.what() << "\033[0m" << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "\033[31m" << "Fatal error: unknown exception" << "\033[0m" << std::endl;
        return 1;
    }

    return 0;
}
