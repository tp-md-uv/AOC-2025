#include <iostream> 
#include <fstream> 
#include <vector>
#include <algorithm>
#include <cmath>
#include <map>
#include <sstream>
#include <set>
#include <unordered_set>

#include <tuple>
#include <cassert>
#include <string>
#include <chrono>
#include "../helpers.h"
using namespace std;

void print_bdry_points(const set<pair<int, int>> &bdry_points){
    for (auto point : bdry_points) {
        cout << "(" << point.first << ", " << point.second << ")" << endl;
    }
}

long long int get_area(const long int &x1, const long int &y1, const long int &x2, const long int &y2){
    return (1 + abs(x1 - x2)) * (1 + abs(y1 - y2));
}

void add_boundary_points(set<pair<int, int>> &boundaries, set<pair<int, int>> &extra_layer, const vector<int> &p1, const vector<int> &p2){
    // Add boundary points between p1 and p2
    // p[0] = x (column), p[1] = y (row)
    int temp;
    if (p1[0] == p2[0]){
        // Same x (column), loop over y (rows)
        // move down
        temp = p1[1];
        if (p1[1] < p2[1]){
            while (temp <= p2[1]){
                boundaries.insert({p1[0], temp});
                extra_layer.insert({p1[0] + 1, temp});
                temp++;
            }

        }else{
            // move up
            while (temp >= p2[1]){
                boundaries.insert({p1[0], temp});
                extra_layer.insert({p1[0] - 1, temp});
                temp--;
            }        
        }

    } else if (p1[1] == p2[1]){ 
        // cout << "Move horizontally" << endl;
        // Same y (row), loop over x (columns)
        // move right
        temp = p1[0];
        if (p1[0] < p2[0]){
            // cout << "Move right " << endl;
            while (temp <= p2[0]){
                boundaries.insert({temp, p1[1]});
                extra_layer.insert({temp, p1[1] - 1});
                temp++;
            }
        }else{
            // move left
            // cout << "Move left" << endl;
            while (temp >= p2[0]){
                boundaries.insert({temp, p1[1]});
                extra_layer.insert({temp, p1[1] + 1});
                temp--;
            }        
        }
    }else throw("Values not on horizontal/vertical line.");
}


vector<vector<int>> get_corners(const vector<int> &p1, const vector<int> &p2){
    // Check if on a straight line
    if (p1[0] == p2[0] || p1[1] == p2[1]){
        // on a line
        return {p1, p2};
    }

    // first point has the same x value as the third point
    // 
    return {p1, p2, {p1[0], p2[1]}, {p2[0], p1[1]}};
}

bool check_valid(const set<pair<int, int>> &extra_layer, const vector<int> &p1, const vector<int> &p2){
    // cout << "Checking validity between: ";
    // print_vec(p1);
    // cout << "and: ";
    // print_vec(p2);
    // Check if the points cross cross the extra layer
    // from p1 --> {p1[0], p2[1]}
    int min_y = min(p1[1], p2[1]);
    int max_y = max(p1[1], p2[1]);

    // cout << "Move 1" << endl;
    while (min_y <= max_y){
        // connect p1 to {p1[0], p2[1]}
        // move vertically
        if (extra_layer.find({p1[0], min_y}) != extra_layer.end()) return false;
        min_y++;
    }
    // from p1 --> {p2[0], p1[1]}
    int min_x = min(p1[0], p2[0]);
    int max_x = max(p1[0], p2[0]);
    // cout << "Move 2" << endl;
    while (min_x <= max_x){
        // connect p1 to {p2[0], p1[1]}
        // move horizontally
        // cout << min_x << ", " << p1[0] << endl;
        if (extra_layer.find({min_x, p1[1]}) != extra_layer.end()) return false;
        min_x++;
    }

    // from p2 --> {p2[0], p1[1]}
    min_y = min(p1[1], p2[1]);
    max_y = max(p1[1], p2[1]);
    // cout << "Move 3" << endl;    
    while (min_y <= max_y){
        // connect p2 to {p2[0], p1[1]} 
        // move vertically
        if (extra_layer.find({p2[0], min_y}) != extra_layer.end()){
            return false;
        } 
        min_y++;
    }
    // from p2 --> {p2[0], p1[1]}
    // Now the connections for p2
    min_x = min(p1[0], p2[0]);
    max_x = max(p1[0], p2[0]);

    // cout << "Move 4" << endl;
    while (min_x <= max_x){
        // connect p2 to 
        // move horizontally {p1[0], p2[1]}
        if (extra_layer.find({min_x, p2[1]}) != extra_layer.end()) return false;
        min_x++;
    }
    // cout << "[VALID]" << endl;
    return true;
}


// We make the boundary such that we are always on the left
// if the rotation turns out to be ccw, we just parse in reverse order or something
// for now, assume left

int main() {
    auto start = chrono::high_resolution_clock::now();
    
    ifstream data("example.txt");
    string line;
    char comma;
    int x, y;
    vector<vector<int>> points;
    vector<int> prev_point(2, 0); // init a vec, size 2 of zeroes
    vector<int> point(2, 0);
    vector<int> first_point(2, 0);
    set<pair<int, int>> boundaries;
    set<pair<int, int>> extra_layer;
    while (getline(data, line)) {
        
        stringstream ss(line);
        ss >> x >> comma >> y;  // Input is (x, y) = (column, row)
        // cout << x << comma << y << endl;
        point[0] = x;  // Store x (column) in first position
        point[1] = y;  // Store y (row) in second position
            
        points.push_back(point);


        // cout << "Point ";
        // print_vec(point);
        // cout << "PrevPoint ";
        // print_vec(prev_point);

        if (prev_point[0] + prev_point[1] != 0){
            // add the boundaries between point and prev_point.
            // cout << "\n\nComputing bdry points between "<< endl;
            // print_vec(prev_point);
            // cout << "And" << endl;
            // print_vec(point);

            add_boundary_points(boundaries, extra_layer, prev_point, point);

            // cout << "Updated boundary points" << endl;
            // print_bdry_points(boundaries);

            // cout << "Updated extra_layer" << endl;
            // print_bdry_points(extra_layer);

            // Update the previous point
            prev_point[0] = point[0];
            prev_point[1] = point[1];
        }else {
            prev_point[0] = point[0];
            prev_point[1] = point[1];
            first_point[0] = point[0];
            first_point[1] = point[1];
        }
    }

    // // Connect the first and the last point
    add_boundary_points(boundaries, extra_layer, point, first_point);


    // cout << "FINAL boundary points" << endl;
    // print_bdry_points(boundaries);
    // cout << "FINAL layer " << endl;
    // print_bdry_points(extra_layer);

    // Remove all the elements of the additional layer that are also in the normal boundary
    for (const auto &p : boundaries) {
        extra_layer.erase(p);
    }
    long long int area;
    long long int max_area = 0;
    // Go over all pairs of points, check if the potential area is larger than 
    // the max area. Then check if we cross the extra_layer by connecting the corners.
    vector<int> p1, p2;

    for (int i = 0; i < points.size() - 1; i++){
        for (int j = i + 1; j < points.size(); j++){
            const vector<int>& p1 = points[i];
            const vector<int>& p2 = points[j];
            area = get_area(p1[0], p1[1], p2[0], p2[1]);
            if (area <= max_area) continue;
            vector<vector<int>> corners = get_corners(p1, p2);
            if (corners.size() == 2) continue; // Do not check if the points are on a horizontal or vertical line.
            if (!check_valid(extra_layer, p1, p2)) continue;
            cout << "Valid : " << endl;
            cout << "(i, j) " << i << " " << j << endl; 
            cout << "p1: ";
            print_vec(p1);
            cout << "p2: ";
            print_vec(p2);
            cout << "Area: " << area << endl;
            cout << endl << endl;
            if (area > max_area) max_area = area;
        } 
    }


    cout << "Total: " <<  max_area << endl;

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Time: " << duration.count() << " ms" << endl;
    return 0;
}
