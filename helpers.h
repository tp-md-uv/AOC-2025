#ifndef HELPERS_H
#define HELPERS_H

#include <vector>
#include <iostream>
#include <tuple>




template <typename T>
void print_vec(const std::vector<T>& vec) {
    std::cout << "[";
    for (const auto& element : vec) {
        std::cout << element << ",";
    }
    std::cout << "]" << std::endl;
}

template <typename T>
void print_grid(const std::vector<std::vector<T>>& grid) {
    for (const auto& row : grid) {
        print_vec(row);
    }
}


void print_vec_vec(const std::vector<std::vector<long int>>& nums);
void print_tup_vec(const std::vector<std::tuple<int, int>>& nums);

#endif
