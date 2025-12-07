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

int main() {
    auto start = chrono::high_resolution_clock::now();
    
    ifstream data("example.txt");
    string line;
    vector<tuple<long int, long int>> ranges;
    bool done = false;
    long int num_fresh = 0;
    long int low, high;
    char dash;   
    while (getline(data, line)) {
        if (!done){
            // populate range
            stringstream ss(line);

            ss >> low >> dash >> high;
            ranges.push_back(make_tuple(low, high));
        }
        if (done){
            // Check the nums
            // cout << "tup" << endl;
            // print_tup_vec(ranges);
            long long ingr_id = stol(line);
            // cout << "ID " << ingr_id << endl;
            for (int j = 0; j < ranges.size(); j++){
                low = get<0>(ranges[j]);
                high = get<1>(ranges[j]);
                if ((ingr_id >= low) && (ingr_id <= high)){
                    num_fresh += 1;
                    break;
                }
            }
            continue;
        }
        done = (line == "");
    }
    cout << "Total:  " << num_fresh << endl;
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Time: " << duration.count() << " ms" << endl;
    return 0;
}

