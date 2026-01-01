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

// z3 solver
#include"z3++.h"

using namespace z3;

// couldnt be bothered on how to read out the data
// so I vibed this part.
struct LineData {
    vector<vector<int>> buttons;
    vector<int> joltages;
};

LineData parseLine(const string& line) {
    LineData result;
    
    // Find the end of the pattern section
    size_t bracketEnd = line.find(']');
    
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
        
        result.buttons.push_back(tuple);
        pos = parenEnd + 1;
    }
    
    // Parse curly braces {32,23,39,9}
    size_t curlyStart = line.find('{');
    size_t curlyEnd = line.find('}');
    if (curlyStart != string::npos && curlyEnd != string::npos) {
        string curlyContent = line.substr(curlyStart + 1, curlyEnd - curlyStart - 1);
        stringstream ss(curlyContent);
        string num;
        while (getline(ss, num, ',')) {
            result.joltages.push_back(stoi(num));
        }
    }
    
    return result;
}

int solve(const LineData &linedata){
    context c;
    // read out the information and solve.
    // Make variables for the button presses.

    // Look at the 'expr_vector_example' on https://github.com/Z3Prover/z3/blob/master/examples/c%2B%2B/example.cpp
    expr_vector buttonVars(c);
    for (size_t i = 0; i < linedata.buttons.size(); i++) {
        buttonVars.push_back(c.int_const(("b" + to_string(i)).c_str()));
    }
    
    // Create optimizer (for minimization)
    optimize opt(c);
    
    // Add non-negativity constraints (buttons can't be pressed negative times)
    for (size_t i = 0; i < buttonVars.size(); i++) {
        opt.add(buttonVars[i] >= 0);
    }

    for (size_t j = 0; j < linedata.joltages.size(); j++){
        expr equation = c.int_val(0);  // Start with 0
        // check which buttons influence joltage j
        for (size_t k = 0; k < linedata.buttons.size(); k++){
            const vector<int>& button = linedata.buttons[k];
            // check if the index of the joltage is in the button
            if (find(button.begin(), button.end(), j) != button.end()) {
                equation = equation + buttonVars[k];
            }
        }
        // Then set it equal to a joltage value
        opt.add(equation == linedata.joltages[j]);
    }

    // Minimize total button presses
    expr totalPresses = c.int_val(0);
    for (size_t i = 0; i < buttonVars.size(); i++) {
        totalPresses = totalPresses + buttonVars[i];
    }
    opt.minimize(totalPresses);

    // Solve and return result
    if (opt.check() == sat) {
        model m = opt.get_model();
        int result = m.eval(totalPresses).get_numeral_int();
        
        // Print the solution
        cout << "Solution found - Total presses: " << result << endl;
        cout << "Button values: ";
        for (size_t i = 0; i < buttonVars.size(); i++) {
            cout << "b" << i << "=" << m.eval(buttonVars[i]) << " ";
        }
        cout << endl;
        
        return result;
    } else {
        cout << "No solution found" << endl;
        return -1;
    }
} 


int main() {
    auto start = chrono::high_resolution_clock::now();
    
    ifstream data("example.txt");
    string line;
    
    int totalResult = 0;
    while (getline(data, line)) {
        LineData lineData = parseLine(line);
        totalResult += solve(lineData);
    }
    
    cout << "Total: " << totalResult << endl;
    
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Time: " << duration.count() << " ms" << endl;
    return 0;
}
