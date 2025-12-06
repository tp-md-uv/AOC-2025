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


// if the max is not the final element, then return that



tuple<int, int> max_el_last_pos(vector<int> &nums,int bat_size, int to_add_size, int num_dig){
    // add some conditions on WHEN to select the largest element
    int max = 0;
    int max_pos = -100;
    int nums_size = nums.size();
    for (int i=0;i<nums_size;i++){
        // cout << "compare " << nums[i] << " > " << max << endl;
        // how many digits we have currently
        int num_dig_cur = bat_size + to_add_size;
        int addflush = nums_size - i;

        if (addflush < (num_dig - num_dig_cur)){
            // cout << "continue " << endl;
            continue;
        }
        if (nums[i] > max){
            // cout << "New high " << nums[i] << " Pos " << i << endl;
            max = nums[i];
            max_pos = i;
        }
    }
    // cout << "returning maxPos " << max_pos << endl;
    return make_tuple(max, max_pos);
}

long int combine_scores(vector<int> &bat_score, vector<int> &to_add){
    // create one big vec
    for (int x: to_add){
        bat_score.push_back(x);
    }
    // cout << "combined list ";
    // print_vec(bat_score);
    long int score = 0;
    int num_digits = bat_score.size();
    for (int k = 0; k < num_digits; k++){
        score += pow(10, num_digits - 1 - k) * bat_score[k];
    }
    // cout << "FINAL SCORE " << score << endl;
    return score;
}


long int max_battery_score(vector<int> &nums, int num_dig){
    vector<int> bat_score, to_add;
    bool done = false;
    while (bat_score.size() + to_add.size() < num_dig){
        // break if the number of digits left is exactly 12
        if (done){
            break;
        }

        // cout << "\n\nNums (size " << nums.size() << ") ";
        // print_vec(nums);
        // cout << "bat_score ";
        // print_vec(bat_score);
        // cout << "to add ";
        // print_vec(to_add);
        // keep extracting digits until we have enough digits
        tuple<int, int> tup_max_flag = max_el_last_pos(nums, bat_score.size(), to_add.size(), num_dig);
        int max_num = get<0>(tup_max_flag);
        int max_pos = get<1>(tup_max_flag);
        // cout << "Max num " << max_num << ". Max pos " << max_pos << endl;
        // check if the max num is at the end of the vec
        // cout << "equal " << max_pos << " == " << nums.size() - 1 << endl; 
        if (max_pos == (nums.size() - 1)){
            // cout << "Removing final element " << endl;
            // Add to the to_add and remove it
            // remove the last element
            nums.pop_back();
            to_add.insert(to_add.begin(), max_num);
            continue;
        }     
        // remove all el up to that pos
        for (int l = 0; l <= max_pos; l++){
            // check if there are only 12 left, then BREAK
            // cout << "bat size " << bat_score.size() << " add size " << to_add.size() << " nums size " << nums.size() << endl;
            if ((bat_score.size() + to_add.size() + nums.size()) == 12){
                // cout << "ADDING FLUSH" << endl;
                // cout << "nums ";
                // print_vec(nums);
                // then add all the remaining elements in nums to the bat_score
                done = true;
                for (int num : nums){
                    bat_score.push_back(num);
                }
            }
            if (done){
                break;
            }
            nums.erase(nums.begin());
        }
        if (done){
            continue;
        }
        bat_score.push_back(max_num);
    }

    // cout << "DONE WITH PARSING: " << endl;
    // cout << "FINAL bat_score ";
    // print_vec(bat_score);
    // cout << "FINAL to add ";
    // print_vec(to_add);
    return combine_scores(bat_score, to_add);
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
        // for (int k =0; k<nums.size();k++){
        //     cout << nums[k] << " ";
        // }
        // cout << endl;
        // logic here
        score += max_battery_score(nums, 12);
        // cout << endl;

    }
    cout << "Score: " << score << endl;
    
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Time: " << duration.count() << " ms" << endl;
    return 0;
}
