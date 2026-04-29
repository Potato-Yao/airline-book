//
// Created by rusty-potato on 4/23/26.
//

#ifndef AIRLINE_BOOK_CSV_HANDLER_H
#define AIRLINE_BOOK_CSV_HANDLER_H

#include <string>
#include <utility>
#include <vector>
#include <algorithm>
#include <filesystem>

#include "../dao/db_action_cell.h"

enum InitMode {
    Load, // if title existing, then load. generate title otherwise
    Override, // no matter what, override existing content then generate title
};

class CSVHandler {
    std::string path;
    int row_count; // title row doesn't count, starts with 0
    int column_count; // starts with 0
    std::vector<std::string> titles;

    /// @param count column count of the csv. -1 for use member variable column_count, -2 for doesn't care
    void write_row(std::ostream &file, const std::vector<std::string> &values, int count = -1) const;

    static std::vector<std::string> split_line(const std::string &s);

    static std::string generate_line(const std::vector<std::string> &values, int column);

    void update_cell_inner(std::vector<std::tuple<int, int, std::string &> > &relations) const;

    int get_column_number(const std::string &title) const;

    int get_row_number(const std::string &key) const;

public:
    explicit CSVHandler(std::string path) : path(std::move(path)), row_count(0), column_count(0) {
    }

    bool is_empty() const;

    void init(const std::vector<std::string> &titles, InitMode mode);

    void insert(const std::vector<std::string> &values);

    void close();

    template<typename... Args>
    void insert(Args &&... args) {
        const std::vector<std::string> values{std::forward<Args>(args)...};
        insert(values);
    }

    /// @param start 0 for from the beginning since row number strats with 0
    /// @param end -1 for read to the end of the file
    std::vector<std::vector<std::string> > read_rows(int start, int end) const;

    std::vector<std::vector<std::string> > load() const;

    void remove_row(int index);

    std::vector<std::string> read_column(int index);

    void update_cell(const std::vector<DBActionCell> &cells) const;

    int get_row_count() const;

    int get_column_count() const;

    const std::vector<std::string> &get_titles() const;
};


#endif //AIRLINE_BOOK_CSV_HANDLER_H
