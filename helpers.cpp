#include "helpers.h"
#include <tuple>



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

