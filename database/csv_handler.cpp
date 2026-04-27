//
// Created by rusty-potato on 4/23/26.
//

#include "csv_handler.h"

#include <fstream>
#include <iostream>
#include <variant>

std::vector<std::string> CSVHandler::split_line(const std::string &s) {
    std::vector<std::string> result;
    std::string current;
    for (const char c: s) {
        if (c == ',') {
            result.push_back(current);
            current.clear();
        } else {
            if (c == '\r') {
                continue;
            }
            current += c;
        }
    }
    result.push_back(current);

    return result;
}

std::string CSVHandler::generate_line(const std::vector<std::string> &values, const int column) {
    std::string line;
    for (int i = 0; i < std::min(column, static_cast<int>(values.size())); ++i) {
        if (i > 0) {
            line.append(",");
        }
        line.append(values[i]);
    }

    if (const int sub = column - static_cast<int>(values.size()); sub > 0) {
        for (int i = 0; i < sub; ++i) {
            line.append(",");
        }
    }

    line.append("\n");

    return line;
}

bool CSVHandler::is_empty() const {
    std::ifstream file(path);
    return file.peek() == std::ifstream::traits_type::eof();
}

void CSVHandler::write_row(std::ostream &file, const std::vector<std::string> &values, const int count) const {
    int column;
    if (count == -1) {
        column = column_count;
    } else if (count == -2) {
        column = static_cast<int>(values.size());
    } else {
        column = count;
    }

    const auto line = generate_line(values, column);
    file << line;
}

void CSVHandler::init(const std::vector<std::string> &titles, InitMode mode) {
    if (mode == Override || (mode == Load && is_empty())) {
        std::ofstream file(path, std::ofstream::trunc);

        write_row(file, titles, -2);

        column_count = static_cast<int>(titles.size());
        this->titles = std::vector(titles);
    } else {
        std::ifstream file(path);
        std::string line;
        getline(file, line);
        this->titles = split_line(line);
        column_count = static_cast<int>(this->titles.size());

        int row_counter = 0;
        while (getline(file, line)) {
            ++row_counter;
        }
        row_count = row_counter;
    }
}

void CSVHandler::insert(const std::vector<std::string> &values) {
    std::ofstream file(path, std::ofstream::app);

    write_row(file, values);
    ++row_count;
}

void CSVHandler::close() {
}

std::vector<std::string> CSVHandler::read_row(int index) const {
    index += 1; // skip the title line
    if (index > row_count) {
        throw std::out_of_range("Cannot read row from index out of range");
    }

    std::ifstream file(path);
    std::string line;
    std::vector<std::string> result;

    int counter = 0;
    while (getline(file, line)) {
        if (counter == index) {
            result = split_line(line);
            break;
        }
        ++counter;
    }

    return result;
}

std::vector<std::vector<std::string> > CSVHandler::read_rows(int start, int end) const {
    if (start > row_count || end > row_count) {
        throw std::out_of_range("Cannot read row from index out of range");
    }
    if (end != -1 && start > end) {
        throw std::out_of_range("End should be greater than start");
    }
    start += 1; // skip the title line
    end += 1; // for same reason

    std::vector<std::vector<std::string> > result{};
    std::ifstream file(path);
    std::string line;

    for (int counter = 0; getline(file, line); ++counter) {
        if (end != 0 && counter > end) {
            // end = -1 for reading to the end. but this already increased by one before
            break;
        }
        if (counter < start) {
            continue;
        }

        auto curr = split_line(line);
        result.push_back(curr);
    }

    return result;
}

std::vector<std::vector<std::string> > CSVHandler::load() const {
    return read_rows(0, -1);
}

void CSVHandler::remove_row(int index) {
    index += 1;
    if (index > row_count) {
        return;
    }

    std::ifstream file(path);
    std::ofstream temp(path + ".tmp", std::ofstream::trunc);
    int counter = 0;
    std::string line;

    while (getline(file, line)) {
        if (counter != index) {
            temp << line << '\n';
        }
        ++counter;
    }

    file.close();
    temp.close();

    std::filesystem::remove(path);
    std::filesystem::rename(path + ".tmp", path);

    --row_count;
}

void CSVHandler::update_cell_inner(std::vector<std::tuple<int, int, std::string &> > &relations) const {
    std::sort(relations.begin(), relations.end(), [](const auto &a, const auto &b) {
        return std::get<0>(a) <= std::get<0>(b);
    });

    std::ifstream file(path);
    std::ofstream temp(path + ".tmp", std::ofstream::trunc);
    int counter = 0;
    std::string line;

    for (auto relation: relations) {
        auto row = std::get<0>(relation);
        auto column = std::get<1>(relation);
        if (row > row_count || column > column_count) {
            throw std::out_of_range("Cannot update row index out of range");
        }

        while (getline(file, line)) {
            if (counter != row) {
                temp << line << '\n';
                ++counter;
            } else {
                auto l = split_line(line);
                l[column] = std::get<2>(relation);

                temp << generate_line(l, static_cast<int>(l.size()));
                ++counter;
                break;
            }
        }
    }

    // deal with left lines
    while (getline(file, line)) {
        temp << line << '\n';
        ++counter;
    }

    file.close();
    temp.close();

    std::filesystem::remove(path);
    std::filesystem::rename(path + ".tmp", path);
}

int CSVHandler::get_column_number(const std::string &title) const {
    for (int i = 0; i < column_count; ++i) {
        if (titles[i] == title) {
            return i;
        }
    }

    throw std::out_of_range("Cannot find column with title " + title);
}

int CSVHandler::get_row_number(const std::string &key) const {
    std::ifstream file(path);

    int counter = 0;
    std::string line;

    while (getline(file, line)) {
        if (auto v = split_line(line); v[0] == key) {
            return counter;
        }
        ++counter;
    }

    throw std::out_of_range("Cannot find row with key " + key);
}

void CSVHandler::update_cell(const std::vector<DBActionCell *> &cells) const {
    if (cells.empty()) {
        return;
    }

    std::vector<std::tuple<int, int, std::string &> > relations;

    for (const auto cell: cells) {
        auto row = get_row_number(cell->key);
        auto column = get_column_number(cell->column);

        relations.emplace_back(row, column, std::ref(cell->value));
    }

    update_cell_inner(relations);
}

const int CSVHandler::get_column_count() const {
    return column_count;
}

const int CSVHandler::get_row_count() const {
    return row_count;
}

const std::vector<std::string> &CSVHandler::get_titles() const {
    return titles;
}
