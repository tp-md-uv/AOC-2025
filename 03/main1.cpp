#include <iostream> 
#include <fstream> 
#include <vector>
#include <algorithm>
#include <cmath>
#include<unordered_map>
#include <sstream>
#include <tuple>
#include <cassert>
#include <string>
#include <tuple>
#include <chrono>
#include "../helpers.h"
using namespace std;

tuple<int, int> max_el_last_pos(const vector<int> &nums){
    // add some conditions on WHEN to select the largest element
    int max = 0;
    int max_pos;
    int nums_size = nums.size();
    for (int i=0;i<nums_size;i++){
        if (nums[i] > max){
            // cout << "New high " << nums[i] << " Pos " << i << endl;
            max = nums[i];
            max_pos = i;
        }
    }
    // cout << "returning maxPos " << max_pos << endl;
    return make_tuple(max, max_pos);
}

long int compute_joltage(const vector<int> &batteries){
    long int joltage = 0;
    int num_digits = batteries.size();
    for (int k = 0; k < num_digits; k++){
        joltage += pow(10, num_digits - 1 - k) * batteries[k];
    }
    // print_vec(batteries);
    // cout << "Battery joltage " << joltage << endl;
    return joltage;
}

long int max_battery_score(vector<int> &nums){
    vector<int> batteries;
    // keep extracting digits until we have enough digits
    tuple<int, int> tup_max_flag = max_el_last_pos(nums);
    int max_num = get<0>(tup_max_flag);
    int max_pos = get<1>(tup_max_flag);
    // if the element is in the final position. 
    // Look for the largest el in the 'first part', then just append the max_num
    int nums_size = nums.size();
    int max_el = 0;
    if (max_pos == (nums_size - 1)){
        for (int i=0; i < nums_size - 1; i++){
            if (nums[i] > max_el) max_el = nums[i];
        }
        batteries.push_back(max_el);
        batteries.push_back(max_num);
        // cout << "Removing final element " << endl;
        // Add to the to_add and remove it
        // remove the last element
    } else{
        batteries.push_back(max_num);
        for (int i=max_pos + 1; i < nums_size; i++){
            if (nums[i] > max_el) max_el = nums[i];
        }
        batteries.push_back(max_el);
        // Prepend this, and look from the max_pos onwards
    }     

    return compute_joltage(batteries);
}

int main() {
    auto start = chrono::high_resolution_clock::now();
    
    ifstream data("example.txt");
    string line;
    long int score = 0;
    while (getline(data, line)){
        vector<int> nums;
        // cout << "line " << line << endl;
        for (int i = 0; i < line.size(); i++){
            nums.push_back(line[i] - '0');
        }
        score += max_battery_score(nums);
    }
    cout << "Total: " << score << endl;
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Time: " << duration.count() << " ms" << endl;
    return 0;
}
