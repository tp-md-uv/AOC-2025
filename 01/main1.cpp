#include <iostream> 
#include <fstream> 
#include <vector>
#include <algorithm>
#include <cmath>
#include <chrono>

using namespace std;

int main() {
    auto start = chrono::high_resolution_clock::now();
    ifstream infile;
    ifstream data("example.txt");
    vector<char> rotation;
    vector<int> direction;
    char c;
    int x;
    while (data >> c >> x){
        rotation.push_back(c);
        direction.push_back(x);
    }
    int count_zeroes = 0;
    int dig = 50;
    for (int j = 0; j < direction.size(); j++){
        char rot = rotation[j];
        int num = direction[j];
        if (rot == 'L'){
            dig -= num;
            // cout << dig << endl;
        }
        else {
            dig += num;
            // cout << dig << endl;        
        }
        dig %= 100;
        if (dig == 0){
            count_zeroes++;
        } 

    }
    cout << "Total: " << count_zeroes << endl;
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Time: " << duration.count() << " ms" << endl;    return 0;
}