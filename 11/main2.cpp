#include <iostream> 
#include <fstream> 
#include <vector>
#include <algorithm>
#include <cmath>
#include <unordered_map>
#include<map>
#include <sstream>
#include <tuple>
#include <cassert>
#include <string>
#include <chrono>
#include "../helpers.h"
using namespace std;

long int compute_paths(const unordered_map<string, vector<string>> &device_map, const string &key, bool fft, bool dac, map<tuple<string, bool, bool>, long int> &cache){
    // Check and update if we visit fft or dac
    if (key == "fft") fft = true;
    if (key == "dac") dac = true;

    // First, check the cache
    tuple<string, bool, bool> cache_key = make_tuple(key, fft, dac);
    if (cache.find(cache_key) != cache.end()){
        return cache[cache_key];
    }
    
    // base case, if we are at the 'out' device.
    if (key == "out"){
        if (fft && dac) return 1;
        else return 0;
    }

    long int total = 0;

    if (device_map.find(key) != device_map.end()){
        for (const string &device : device_map.at(key)){
            // if (device == "you") continue; // not sure if this will give the correct answer.
            total += compute_paths(device_map,  device, fft, dac, cache);
        }
    }
    // why here?
    cache[cache_key] = total;

    return total;
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

    // We need some sort of caching
    // We do not care where we came from, we want to count the number of paths from a position, from the 
    // moment we are at a device, and we have visited both 'dac' and 'fft'.
    map<tuple<string, bool, bool>, long int> cache;

    bool fft = false;
    bool dac = false;
    long int total = compute_paths(device_map, "svr", fft, dac, cache);
    cout << "Total: " << total << endl;
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Time: " << duration.count() << " ms" << endl;
    return 0;
}
