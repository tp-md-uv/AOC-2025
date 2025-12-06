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
#include <chrono>
#include "../helpers.h"
using namespace std;

void rem_trailing_zeroes(vector<int> &nums){
    while (nums.back() == 0) nums.pop_back();
}

long int do_homework_vertically(vector<string> numbers, string op){
    long int total = 0;
    if (op == "*") total = 1;
    vector<vector<int>> digits;
    for (string num : numbers){
        vector<int> digit_num;
        for (char c : num){
            if (c == ' ') digit_num.push_back(0);
            else digit_num.push_back(c - '0');
        }
        digits.push_back(digit_num);
    }
    cout << endl << "DIGITS " << endl;
    for (auto x : digits){
      print_vec(x);
    } 

    // now parse vertically
    int digits_size = digits.size();
    for (int j = 0; j < digits[0].size(); j++){
        vector<int> col_res;
        for (int i = 0; i < digits_size; i++){
            col_res.push_back(digits[i][j]);
        }
        rem_trailing_zeroes(col_res);
        // unpack the number 
        long int col_res_sum = 0;
        int col_res_size = col_res.size();
        for (int k = 0; k < col_res_size; k++){
            col_res_sum += pow(10, col_res_size - 1 - k) * col_res[k];;
        }
        cout << "Col res sum (" << op << ") " << col_res_sum << endl;
        if (col_res_sum == 0) continue;
        if (op == "+") total += col_res_sum;
        else if (op == "*") total *= col_res_sum;
    }
    cout << "Subtotal " << total << endl;
    return total;
}

int main() {
    auto start = chrono::high_resolution_clock::now();
    
    ifstream data("example.txt");
    string line;
    vector<vector<string>> numbers;
    vector<string> operators;

    vector<string> data_lines;
    while (getline(data, line)) {
        cout << "line " << line << endl;
        if (line.find('*') != std::string::npos || line.find('+') != std::string::npos){
            stringstream ss(line);
            string op;
            while (ss >> op) operators.push_back(op); // this doesn't go well yet, but for later
            continue;
        }
        data_lines.push_back(line);
    }
    cout << "Data lines ";
    print_vec(data_lines);
    cout << "Operator lines ";
    print_vec(operators);
    // We'll look for cols where ALL elements are spaces
    vector<bool> is_space(data_lines[0].size(), true);
    for (int j = 0; j < data_lines[0].size(); j++){
        bool found = false;
        for (int i = 0; i < data_lines.size(); i++){
            if (data_lines[i][j] != ' '){
                is_space[j] = false;
                found = true;
                break;
            };
        }
    }
    for (int k = 0; k < is_space.size(); k++){
        if (is_space[k]) cout << "VERTICAL SPACE " << k << " " << is_space.size() << endl;
    }
    int left_bdry = 0;
    int right_bdry;
    int op_idx = 0;
    long int total = 0;
    for (int k = 0; k < is_space.size(); k++){
        vector<string> l;
        if (is_space[k]){
            right_bdry = k;
            for (string &s : data_lines){
                // then we need to parse the substring
                l.push_back(s.substr(left_bdry, right_bdry - left_bdry));                
            }
            left_bdry = right_bdry + 1; // start AFTER the space again        
        }
        if (k == (is_space.size() - 1)){
            right_bdry = is_space.size();
            for (string &s : data_lines){
                // then we need to parse the substring
                l.push_back(s.substr(left_bdry, is_space.size() - left_bdry));                
            }
        }

        if (l.size() == 0) continue;
        total += do_homework_vertically(l, operators[op_idx]);
        op_idx += 1;
    }
    cout << "TOTAL " << total << endl;
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Time: " << duration.count() << " ms" << endl;
    return 0;
}
