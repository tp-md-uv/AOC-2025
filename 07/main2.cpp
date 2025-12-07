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


bool is_int(const string &s){
    for (char c : s){
        if (!isdigit(c)) return false;
    }
    return true;
}

void print_grid(const vector<vector<string>> &grid){
    for (auto row : grid) print_vec(row);
}

bool is_down_empty(const vector<vector<string>> &grid, const int &i, const int &j){
    // check if the pos immediately down is available 
    return ((grid[i+1][j] == ".") || (is_int(grid[i + 1][j])));
}

bool is_down_split(const vector<vector<string>> &grid, const int &i, const int &j){
    // check if the pos immediately down is available 
    return (grid[i+1][j] == "^");
}

string update_path_count(const string &str_int, string to_add){
    if (to_add == ".") to_add = "0";
    return to_string(stol(str_int) + stol(to_add)); 
}

void count_timelines(const vector<vector<string>> &grid){
    long int num_paths = 0;
    for (int j = 0; j < grid[grid.size() - 1].size(); j++){
        if (!is_int(grid[grid.size()-1][j])) continue;
        num_paths += stol(grid[grid.size()-1][j]);
    }
    cout << "Total number of paths " << num_paths << endl;
}

void beam_split(vector<vector<string>> &grid){
    int num_splits = 0;
    string count_path;
    // go over row by row
    for (int i = 0; i < grid.size() - 1; i++){
        for (int j = 0; j < grid[i].size(); j++){
            if ((grid[i][j] != "S") && (!is_int(grid[i][j]))) continue;
            if (is_down_empty(grid, i, j)){
                // we can place a beam down
                if ((grid[i][j]) == "S") grid[i+1][j] = "1";
                else{
                    count_path = update_path_count(grid[i][j], grid[i + 1][j]);
                    grid[i+1][j] = count_path; 
                }                 
            }       
            // Count as we move down (I guess we could also count 'upwards'? Very similar to PE 18 & 67)
            if (is_down_split(grid, i, j)){
                // From visual inspection, we can always safely place left and right.
                // place a pipe on the left
                count_path = update_path_count(grid[i][j], grid[i + 1][j - 1]);
                grid[i + 1][j - 1] = count_path;

                // place a pipe on the right
                count_path = update_path_count(grid[i][j], grid[i + 1][j + 1]);
                grid[i + 1][j + 1] = count_path;
            }
        }
    }
    cout << endl;
    print_grid(grid);
}

int main() {
    auto start = chrono::high_resolution_clock::now();
    
    ifstream data("example.txt");
    string line;
    vector<vector<string>> grid;
    while (getline(data, line)) {
        vector<string> row;
        for (char c : line) row.push_back(string(1, c));
        grid.push_back(row);
    }
    print_grid(grid);
    beam_split(grid);
    count_timelines(grid);
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Time: " << duration.count() << " ms" << endl;
    return 0;
}


// apparently stol is a thing, use that instead of stoi -.-