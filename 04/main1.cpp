#include <iostream> 
#include <fstream> 
#include <vector>
#include <algorithm>
#include <cmath>
#include<unordered_map>
#include <sstream>
#include <cassert>
#include <string>
#include <tuple>
#include <chrono>
#include "../helpers.h"
using namespace std;

void print_grid(const vector<vector<char>> &grid){
    for (auto &row : grid){
        for (auto &c : row){
            cout << c;
        }
        cout << endl;
    }
}

bool is_valid(const vector<vector<char>> &grid, const int &i, const int &j){
    const int num_rows = grid.size();
    const int num_cols = grid[0].size();

    if ((i >= 0) && (i < (num_rows)) && (j >= 0) && (j < (num_cols))){
        return true;
    }
    return false; 
}

bool check_position(const vector<vector<char>> &grid, const int &i, const int &j){
    // chcek all the 8 positions
    vector<int> d = {-1, 0, 1};
    int count = 0;
    // cout << "CHECKING " << i << "," << j << endl;
    for (int di : d){
        for (int dj : d){
            if (di == 0 && dj == 0){ continue;} // do not check same pos
            if (!is_valid(grid, i + di, j + dj)) {continue;} // stop if invalid pos
            // cout << i + di << " " << j + dj << " is valid pos. Val=" << grid[i+di][j+dj] << endl;
            if (grid[i + di][j + dj] != '@'){continue;}
            count += 1; // so we count the number of rolls of paper
        }
    }
    // if (count < 4){
    //     cout << "Num rolls: " << count << "||" << i << "," << j << endl << endl;
    // }
    return (count < 4);
}

int num_removed(vector<vector<char>> &grid){
    vector<vector<char>> new_grid = grid;

    int num_removed = 0;
    for (int i = 0; i < grid.size(); i++){
        for (int j = 0; j < grid[i].size(); j++){
            if (grid[i][j] != '@'){continue;}
            if (!check_position(grid, i, j)){continue;}
            // cout << "VALID: (i, j): " << i << " , " << j << endl;
            num_removed += 1;
            new_grid[i][j] = '.';
            // cout << "THE OLD GRID IS " << endl;
            // print_grid(grid);
            // cout << "THE NEW GRID IS " << endl;
            // print_grid(new_grid);
        }
    }
    grid = new_grid; // switch 
    return num_removed;
}



int main() {
    auto start = chrono::high_resolution_clock::now();
    
    ifstream data("example.txt");
    vector<vector<char>> grid; 
    string line;
    int i = 0;
    while (getline(data, line)) {
        // Your code here
        vector<char> row(line.begin(), line.end());
        grid.push_back(row);
    }
    // cout << "the grid " << endl;
    // print_grid(grid);
    int rem = num_removed(grid);
    // cout << "init removed " << rem << endl;
    int total_removed = rem;
    while (rem > 0){
        rem = num_removed(grid);
        // cout << "removed " << rem << endl;
        total_removed += rem;
    }

    cout << total_removed << endl;
    // print_grid(grid);

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Time: " << duration.count() << " ms" << endl;
    return 0;
}
