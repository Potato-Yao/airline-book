//
// Created by rusty-potato on 4/26/26.
//

#ifndef AIRLINE_BOOK_STATE_H
#define AIRLINE_BOOK_STATE_H

constexpr auto COMMIT = "commit";
constexpr auto LIST = "list";
constexpr auto BOOK = "book";
constexpr auto REFUND = "refund";
constexpr auto REGISTER = "register";
constexpr auto HELP = "help";
constexpr auto EXIT = "exit";

enum State {
    Initial,
    Running,
    Exit,
};

#endif //AIRLINE_BOOK_STATE_H
