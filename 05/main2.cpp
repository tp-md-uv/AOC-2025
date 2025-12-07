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

vector<vector<long int>> merge(vector<vector<long int>> ranges){
    vector<vector<long int>> merged_ranges;
    bool merged = false;
    bool cont = false;
    int i = 0; // index for ranges
    long int low, high, low_j, high_j;
    vector<bool> seen(ranges.size(), false);
    vector<bool> to_remove(ranges.size(), false);
    while (i < ranges.size()){
        merged = false;
        cont = false;
        low = ranges[i][0];
        high = ranges[i][1];
        // cout << "\n\nChecking for " << low << " & " << high << endl;
        if (to_remove[i]){
            i += 1;
            continue;
        }
        if (seen[i]){
            // then we have already merged that range
            // cout << "continue" << endl;
            cont = true;
            i += 1;
            continue;
        }         
        for (int j = i + 1; j < ranges.size(); j++){
            // see if we can merge anything
            low_j = ranges[j][0];
            high_j = ranges[j][1];
            // cout << "Against " << low_j << " & " << high_j << endl;
            // option 1
            if ((low <= low_j) && (high <= high_j) && (high >= low_j)){
                // cout << "option 1" << endl;
                vector<long int> merged_range;
                merged_range.push_back(low);
                merged_range.push_back(high_j);
                merged_ranges.push_back(merged_range);

                // update seen
                // cout << "Updating index " << j << endl;
                seen[j] = true;
                merged = true;
                break;
            }

            // option 2
            if ((low_j <= low) && (low <= high_j) && (high_j <= high)){
                // cout << "option 2" << endl;
                vector<long int> merged_range;
                merged_range.push_back(low_j);
                merged_range.push_back(high);
                merged_ranges.push_back(merged_range);

                // update seen
                seen[j] = true;
                merged = true;
                break;            
            }
            
            // check if we are CONTAINED in a range
            if ((low <= low_j) && (high >= high_j)){
                to_remove[j] = true;
                // cout << "HIT" << endl;
                break;
            }
        }
        if (!merged && !cont){
            // then just add the range to the new_merged vec
            // cout << "Didn't merge. Adding" << endl;
            merged_ranges.push_back(ranges[i]);
        }

        // cout << "New merged " << endl;
        // print_vec_vec(merged_ranges);
        i ++;
    }
    // cout << "FINAL merged " << endl;
    // print_vec_vec(merged_ranges);
    return merged_ranges;
}



int main() {
    auto start = chrono::high_resolution_clock::now();
    
    ifstream data("example.txt");
    string line;
    vector<vector<long int>> ranges;
    long int low, high;
    char dash;   
    while (getline(data, line)) {
        if (line == ""){
            // cout << "We break" << endl;
            break;
        }
        // must be an easier way to read out this data
        vector<long int> range;
        // populate range
        stringstream ss(line);
        ss >> low >> dash >> high;
        range.push_back(low);
        range.push_back(high);
        ranges.push_back(range);
    }
    sort(ranges.begin(), ranges.end()); // Sorts vector of vectors based on first element    cout << "ORIGINAL RANGES " <<endl; 
    // print_grid(ranges);
    // now do set intersection
    int old_size = ranges.size();
    vector<vector<long int>> new_range = merge(ranges);
    while (new_range.size() < old_size){
        old_size = new_range.size();
         new_range = merge(new_range);
    }
    // cout << "FINAL FINAL RANGES " <<endl; 
    // print_grid(new_range);  
    long int total = 0;
    for (int k = 0; k < new_range.size(); k++){
        total += new_range[k][1] - new_range[k][0] + 1;
    }
    cout << "Total:  " << total << endl;

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Time: " << duration.count() << " ms" << endl;
    return 0;
}

