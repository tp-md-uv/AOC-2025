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


long int compute_paths(const unordered_map<string, vector<string>> device_map, long int &num_paths, const string &key){
    // pass on 'you'
    if (key == "out") {
        num_paths++;
    }

    // Check if the key is in there or not
    if (device_map.find(key) != device_map.end()){
        for (const string &device : device_map.at(key)){
            if (device == "you") continue; // not sure if this will give the correct answer.
            compute_paths(device_map, num_paths, device);
        }
    }

    // in the end, return num_paths
    return num_paths;

}

int main() {
    auto start = chrono::high_resolution_clock::now();
    
    ifstream data("example.txt");
    string line;
    
    unordered_map<string, vector<string>> device_map;

    while (getline(data, line)) {
        size_t colonPos = line.find(':');
        string key = line.substr(0, colonPos);
        
        string values = line.substr(colonPos + 2);  // Skip ": "
        stringstream ss(values);
        string value;
        
        vector<string> neighbors;
        while (ss >> value) {
            neighbors.push_back(value);
        }
        
        device_map[key] = neighbors;
    }
    
    // // Get neighbors of 'you'
    // for (const auto& neighbor : device_map["you"]) {
    //     cout << neighbor << " ";  // prints: ddd eee fff
    // }
    long int num_paths = 0;
    long int total = compute_paths(device_map, num_paths, "you");
    cout << "Total: " << total << endl;
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Time: " << duration.count() << " ms" << endl;
    return 0;
}
