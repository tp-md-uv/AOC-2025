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
    
    while (getline(data, line)) {
        // Your code here
    }
    
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Time: " << duration.count() << " ms" << endl;
    return 0;
}
