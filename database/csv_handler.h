//
// Created by rusty-potato on 4/23/26.
//

#ifndef AIRLINE_BOOK_CSV_HANDLER_H
#define AIRLINE_BOOK_CSV_HANDLER_H

#include <string>
#include <utility>
#include <vector>
#include <fstream>
#include <climits>
#include <algorithm>
#include <filesystem>
#include <iostream>

class CSVHandler {
    std::string path;
    int row_count; // title row doesn't count
    int column_count;

    /// @param count column count of the csv. -1 for use member variable column_count, -2 for doesn't care
    void write_row(std::ostream &file, const std::vector<std::string> &values, int count = -1) const;
    static std::vector<std::string> split_line(const std::string &s);
    static std::string generate_line(const std::vector<std::string> &values, int column);

public:
    explicit CSVHandler(std::string path) : path(std::move(path)), row_count(0), column_count(0) {
    }

    bool is_initialized() const;

    void init(const std::vector<std::string>& titles);

    void insert(const std::vector<std::string>& values);

    template <typename... Args>
    void insert(Args&&... args) {
        const std::vector<std::string> values{std::forward<Args>(args)...};
        insert(values);
    }

    std::vector<std::string> read_row(int index) const;

    void remove_row(int index);

    std::vector<std::string> read_column(int index);

    void update_cell(int row, int column, std::string &value) const;

    int get_row_count() const;

    int get_column_count() const;
};


#endif //AIRLINE_BOOK_CSV_HANDLER_H
