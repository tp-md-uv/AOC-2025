#include <iostream> 
#include <fstream> 
#include <vector>
#include <algorithm>
#include <cmath>
#include <sstream>
#include <chrono>

using namespace std;

bool is_correct(long int num){
    // actually build the str from substrings
    // cout << "\nParsing " << num << endl;
    string str_num = to_string(num);
    int size = str_num.size();

    for (int j = 0; j < size / 2; j++){
        // Only need to go up to half the size
        string sub_str_part = str_num.substr(0, j + 1);
        string sub_str = "";
        // cout << "Substr " << sub_str_part << endl;
        while (sub_str.size() < size){
            sub_str += sub_str_part;
        }
        // cout << "Final substr " << sub_str << endl;
        if (sub_str.size() > size) {continue;}
        if (sub_str == str_num){
            // If we are the same, it is invalid, so we return false.
            // cout << num << " is invalid." << endl;
            return false;
        }
    }
    return true;
}

long int check_range(long int low, long int high) {
    long int total = 0;
    // cout << "low " << low << " high " << high << endl;
    for (long int i = low; i < high + 1; i++){
        bool check = is_correct(i);
        if (!check){
            // if invalid, we sum
            total += i;
        }
    }
    return total;
}

int main() {
    auto start = chrono::high_resolution_clock::now();
    ifstream data("example.txt");
    string line, range;
    long int total = 0;
    while (getline(data, line)){
        stringstream ss(line);
        while (getline(ss, range, ',')) {
            stringstream ss2(range);
            long int low, high;
            char dash;
            ss2 >> low >> dash >> high;
            total += check_range(low, high);
        }
    }
    cout << "Total: " << total << endl;
    
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Time: " << duration.count() << " ms" << endl;
    return 0;
}