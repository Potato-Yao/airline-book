//
// Created by rusty-potato on 4/23/26.
//

#include "csv_handler.h"

std::vector<std::string> CSVHandler::split_line(const std::string &s) {
    std::vector<std::string> result;
    std::string current;
    for (const char c: s) {
        if (c == ',') {
            result.push_back(current);
            current.clear();
        } else {
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

bool CSVHandler::is_initialized() const {
    std::ifstream file(path);
    return file.peek() != std::ifstream::traits_type::eof();
}

void CSVHandler::write_row(std::ostream &file, const std::vector<std::string> &values, const int count) const {
    int column;
    if (count == -1) {
        column = column_count;
    } else if (count == -2) {
        column = INT_MAX;
    } else {
        column = count;
    }

    for (int i = 0; i < std::min(column, static_cast<int>(values.size())); ++i) {
        if (i > 0) {
            file << ',';
        }
        file << values[i];
    }

    if (const int sub = column - static_cast<int>(values.size()); count != -2 && sub > 0) {
        for (int i = 0; i < sub; ++i) {
            file << ',';
        }
    }

    file << '\n';
}

void CSVHandler::init(const std::vector<std::string> &titles) {
    std::ofstream file(path, std::ofstream::trunc);

    write_row(file, titles, -2);

    column_count = static_cast<int>(titles.size());
}

void CSVHandler::insert(const std::vector<std::string> &values) {
    std::ofstream file(path, std::ofstream::app);

    write_row(file, values);
    ++row_count;
}

std::vector<std::string> CSVHandler::read_row(int index) const {
    index += 1;
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

void CSVHandler::update_cell(int row, int column, std::string &value) const {
    row += 1;
    if (row > row_count || column > column_count) {
        throw std::out_of_range("Cannot update row index out of range");
    }

    std::ifstream file(path);
    std::ofstream temp(path + ".tmp", std::ofstream::trunc);
    int counter = 0;
    std::string line;

    while (getline(file, line)) {
        if (counter != row) {
            temp << line << '\n';
        } else {
            auto l = split_line(line);
            l[column] = value;

            temp << generate_line(l, static_cast<int>(l.size()));
        }
        ++counter;
    }

    file.close();
    temp.close();

    std::filesystem::remove(path);
    std::filesystem::rename(path + ".tmp", path);
}

int CSVHandler::get_column_count() const {
    return column_count;
}

int CSVHandler::get_row_count() const {
    return row_count;
}
