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


long int area(const long int &x1, const long int &y1, const long int &x2, const long int &y2){
    return (1 + abs(x1 - x2)) * (1 + abs(y1 - y2));

}



int main() {
    auto start = chrono::high_resolution_clock::now();
    
    ifstream data("example.txt");
    string line;
    char comma;
    long int x, y;
    vector<vector<long int>> points;
    while (getline(data, line)) {
        vector<long int> point;
        stringstream ss(line);
        ss >> y >> comma >> x;
        // cout << x << comma << y << endl;
        point.push_back(x);
        point.push_back(y);
        points.push_back(point);
    }
    

    long int max_area = 0;
    long int temp_area;
    for (int i = 0; i < points.size() -1; i++){
        for (int j = i + 1; j < points.size();j++){
            temp_area = area(points[i][0], points[i][1], points[j][0], points[j][1]);
            if (temp_area > max_area){
                max_area = temp_area;
                // cout << "Area "<< max_area << ". Max: " << points[i][0] << "," << points[i][1] << " & " <<  points[j][0] << "," << points[j][1] << endl;
            } 
        }
    }
    cout << "Total: " <<  max_area << endl;

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Time: " << duration.count() << " ms" << endl;
    return 0;
}
