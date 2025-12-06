#include "helpers.h"
#include <tuple>


void print_vec_vec(const std::vector<std::vector<long int>>& nums) {
    std::cout << "[";
    for (size_t i = 0; i < nums.size(); i++) {
        for (int j = 0; j < nums[i].size(); j++){
            std::cout << nums[i][j] << ",";
        }
        std::cout << " "; 
    }
    std::cout << "]" << std::endl;
}

void print_tup_vec(const std::vector<std::tuple<int, int>>& nums){
    std::cout << "[";
    int x, y;
    for (size_t i = 0; i < nums.size(); i++) {
        x = std::get<0>(nums[i]);
        y = std::get<1>(nums[i]);
        std::cout << x << "," << y << " ";
    }
    std::cout << "]" << std::endl;
}

