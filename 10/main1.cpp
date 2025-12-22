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


// couldnt be bothered on how to read out the data
// so I vibed this part.
struct LineData {
    vector<char> pattern;
    vector<vector<int>> tuples;
};

LineData parseLine(const string& line) {
    LineData result;
    
    // Parse the pattern [.##.]
    size_t bracketStart = line.find('[');
    size_t bracketEnd = line.find(']');
    if (bracketStart != string::npos && bracketEnd != string::npos) {
        for (size_t i = bracketStart + 1; i < bracketEnd; i++) {
            result.pattern.push_back(line[i]);
        }
    }
    
    // Parse tuples (3) (1,3) (2) etc.
    size_t pos = bracketEnd + 1;
    while (pos < line.length()) {
        size_t parenStart = line.find('(', pos);
        if (parenStart == string::npos) break;
        
        size_t parenEnd = line.find(')', parenStart);
        if (parenEnd == string::npos) break;
        
        // Check if we've reached the curly braces section
        if (line.find('{', pos) != string::npos && line.find('{', pos) < parenStart) {
            break;
        }
        
        // Extract the tuple content
        string tupleContent = line.substr(parenStart + 1, parenEnd - parenStart - 1);
        vector<int> tuple;
        
        stringstream ss(tupleContent);
        string num;
        while (getline(ss, num, ',')) {
            tuple.push_back(stoi(num));
        }
        
        result.tuples.push_back(tuple);
        pos = parenEnd + 1;
    }
    
    return result;
}

void backtrack_k(
    const vector<vector<int>>& v,
    int start,
    int k,
    vector<vector<int>>& current,
    vector<vector<vector<int>>>& result) {

    if (current.size() == k) {
        result.push_back(current);
        return;
    }

    for (int i = start; i < v.size(); ++i) {
        current.push_back(v[i]);
        backtrack_k(v, i + 1, k, current, result);
        current.pop_back();
    }
}


void perform_press(vector<char> &lights, const vector<int> &buttons){
    for (const int &idx : buttons){
        if (lights[idx] == '.') lights[idx] = '#';
        else if (lights[idx] == '#') lights[idx] = '.';
    }
}

bool check_pattern(const vector<char> pattern, const vector<char> lights){
    for (int i = 0; i < pattern.size(); i++){
        if (pattern[i] != lights[i]){
            return false;
        } 
    }
    return true;
}

bool check_combination(const vector<vector<int>> &combination, const vector<char> pattern){
    vector<char> lights(pattern.size(), '.'); // start off with all the lights turned off
    for (const vector<int> &button : combination){
        // cout << "Lights: " << endl;
        // print_vec(lights);
        perform_press(lights, button);
    }
    return check_pattern(pattern, lights);
}


int main() {
    auto start = chrono::high_resolution_clock::now();
    
    ifstream data("example.txt");
    string line;
    int total = 0;
    while (getline(data, line)) {
        LineData lineData = parseLine(line);
        
        // // Print the pattern
        // cout << "Pattern: ";
        // for (char c : lineData.pattern) {
        //     cout << c;
        // }
        // cout << endl;
        
        // // Print the tuples
        // cout << "Tuples: ";
        // for (const auto& tuple : lineData.tuples) {
        //     cout << "(";
        //     for (size_t i = 0; i < tuple.size(); i++) {
        //         cout << tuple[i];
        //         if (i < tuple.size() - 1) cout << ",";
        //     }
        //     cout << ") ";
        // }
        // go over other options

        bool done = false;
        // Generate all combinations of k buttons
        for (int k=1; k < lineData.tuples.size(); k++){
            if (done) break;
            vector<vector<vector<int>>> combinations;
            vector<vector<int>> current;
            backtrack_k(lineData.tuples, 0, k, current, combinations);
            // Then check if the combination actually works
            for (const auto& combination : combinations){
                if (check_combination(combination, lineData.pattern)){
                    done = true;
                    // cout << "Correct combination : " << endl;
                    // print_grid(combination);
                    // cout << "Total: " << combination.size() << endl;
                    total += combination.size();
                    break;
                }
            }
        }
        // cout << endl << endl;
    }
    cout << "Total: " << total << endl;
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Time: " << duration.count() << " ms" << endl;
    return 0;
}
