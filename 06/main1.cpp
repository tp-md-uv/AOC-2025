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

long int sum_vec(const vector<vector<int>> &nums, const int &j){
    long int sum_total = 0;
    for (int i = 0; i < nums.size();i++){
        sum_total += nums[i][j];
    }
    // cout << "Sum vec " << sum_total << endl;
    return sum_total;
}

long int mult_vec(const vector<vector<int>> &nums, const int &j){
    long int mult_total = 1;
    for (int i = 0; i < nums.size();i++){
        mult_total *= nums[i][j];
    }
    // cout << "Sum vec " << mult_total << endl;
    return mult_total;
}


long int do_homework(const vector<vector<int>> &numbers, const vector<string> operators){
    long int total = 0;
    string op;
    for (int j = 0; j < operators.size(); j++){
        op = operators[j];
        if (op =="*"){
            total += mult_vec(numbers, j);
        }
        else if (op=="+"){
            total += sum_vec(numbers, j);
        }
    }
    return total;
}

bool is_operator(const string &s){
    return ((s == "+") || (s == "*"));
}

int main() {
    auto start = chrono::high_resolution_clock::now();
    
    ifstream data("example.txt");
    string line;
    vector<vector<int>> numbers;
    vector<string> operators;
    while (getline(data, line)) {
        stringstream ss(line);
        vector<int> row;
        string temp;
        while (ss >> temp){
            if (is_operator(temp)){
                operators.push_back(temp);
                continue;
            }
            row.push_back(stoi(temp));
        }
        if (row.size() == 0){
            continue;
        }
        numbers.push_back(row);
    }
    // print_vec(operators);
    // cout << "PRINTING ROWS" << endl;
    // for (auto row : numbers){
    //     print_vec(row);
    // }
    long int total = do_homework(numbers, operators);
    cout << "TOTAL " << total << endl;
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Time: " << duration.count() << " ms" << endl;
    return 0;
}
