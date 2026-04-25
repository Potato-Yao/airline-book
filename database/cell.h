//
// Created by rusty-potato on 4/25/26.
//

#ifndef AIRLINE_BOOK_CELL_H
#define AIRLINE_BOOK_CELL_H
#include <string>


struct Cell {
    std::string key; // for the row
    std::string column; // title of the column
    std::string value;

    Cell(std::string key, std::string column, std::string value) : key(std::move(key)), column(std::move(column)),
                                                                   value(std::move(value)) {
    }
};


#endif //AIRLINE_BOOK_CELL_H
