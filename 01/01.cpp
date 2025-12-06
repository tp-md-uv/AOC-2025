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
    vector<int> a;
    char c;
    int x;
    while (data >> c >> x){
        rotation.push_back(c);
        a.push_back(x);
    }
    int total = 0;
    int dig = 50;
    int to_add;
    int num_rot;
    bool was_zero = false;
    for (int j = 0; j < a.size(); j++){
        char rot = rotation[j];
        int num = a[j];
        num_rot = abs(num / 100);
        total += num_rot;
        num %= 100;
        if (rot == 'L'){
            dig -= num;
            cout << dig << endl;
        }
        else {
            dig += num;
            cout << dig << endl;        
        }
        if (dig > 100){
            total += 1;
            dig -= 100;            
        }
        if (dig < 0){
            if (!was_zero){
                total += 1;
            }
            dig += 100;
        }
        dig %= 100;
        if (dig == 0){
            total += 1;
        }
        was_zero = (dig == 0); // don't add if we come from zero
    }
    cout << "TOTAL" << endl;
    cout << total << endl;

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Time: " << duration.count() << " ms" << endl;
    return 0;

}