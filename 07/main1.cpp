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


void print_grid(const vector<vector<char>> &grid){
    for (auto row : grid) print_vec(row);
}

bool is_down_empty(const vector<vector<char>> &grid, const int &i, const int &j){
    // check if the pos immediately down is available 
    return (grid[i+1][j] == '.');
}


bool is_down_split(const vector<vector<char>> &grid, const int &i, const int &j){
    // check if the pos immediately down is available 
    return (grid[i+1][j] == '^');
}



void beam_split(vector<vector<char>> &grid){
    int num_splits = 0;
    // go over row by row
    for (int i = 0; i < grid.size() - 1; i++){
        for (int j = 0; j < grid[i].size(); j++){
            if ((grid[i][j] != 'S') && (grid[i][j] != '|')) continue;
            if (is_down_empty(grid, i, j)){
                // we can place a beam down
                grid[i+1][j] = '|';
            }       
            if (is_down_split(grid, i, j)){
                num_splits += 1;
                if ((j - 1) >= 0){
                    // place a pipe on the left
                    grid[i + 1][j-1] = '|';
                }
                if ((j + 1) < grid[i].size()){
                    // place a pipe on the right
                    grid[i + 1][j+1] = '|';
                }
            }
        }
        // print_grid(grid);
        // cout << endl << "----------------------------------" << endl;
    }
    print_grid(grid);
    cout << "#Splits " << num_splits << endl;
}

int main() {
    auto start = chrono::high_resolution_clock::now();
    
    ifstream data("example.txt");
    string line;
    vector<vector<char>> grid;
    while (getline(data, line)) {
        vector<char> row;
        for (char c : line) row.push_back(c);
        grid.push_back(row);
    }
    beam_split(grid);

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Time: " << duration.count() << " ms" << endl;
    return 0;
}
