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

// Saw this really cool idea that you can check if a point is inside a shape if there is an 
// odd number of times the ray (starting from any boundary) crossed the boundary
// Should work (although not optimally), but from the moment I realized I parsed the elements
// as (i, j) instead of (x, y) debugging has been a mess. Would like to return to this later though

// custom hash function
struct VectorHash {
    size_t operator()(const vector<int>& v) const {
        return hash<int>()(v[0]) ^ (hash<int>()(v[1]) << 1);
    }
};

void print_bdry_points(const unordered_set<vector<int>, VectorHash> &bdry_points){
    for (auto point : bdry_points) print_vec(point);
}

long int area(const long int &x1, const long int &y1, const long int &x2, const long int &y2){
    return (1 + abs(x1 - x2)) * (1 + abs(y1 - y2));
}

void add_boundary_points(unordered_set<vector<int>, VectorHash> &boundaries, const vector<int> &p1, const vector<int> &p2){
    // Add boundary points between p1 and p2
    // p[0] = x (column), p[1] = y (row)

    if (p1[0] == p2[0]){
        // Same x (column), loop over y (rows)
        int max_y = max(p1[1], p2[1]);
        int min_y = min(p1[1], p2[1]);
        while (min_y <= max_y){
            boundaries.insert({p1[0], min_y});
            min_y++;
        }

    } else if (p1[1] == p2[1]){ 
        // Same y (row), loop over x (columns)
        int max_x = max(p1[0], p2[0]);
        int min_x = min(p1[0], p2[0]);
        while (min_x <= max_x){
            boundaries.insert({min_x, p1[1]});
            min_x++;
        }

    }else throw("Values not on horizontal/vertical line.");
}

bool is_point_valid(const unordered_set<vector<int>, VectorHash> &boundaries, const vector<int> &p, const int &min_x, const int &min_y){
    // Simple horizontal ray casting from left edge
    int crosses = 0;
    bool come_from_boundary = false;
    
    // Cast ray from left edge to point, count vertical boundary crossings
    for (int x = min_x; x < p[0]; x++){
        bool on_boundary = boundaries.find({x, p[1]}) != boundaries.end();
        
        if (on_boundary && !come_from_boundary){
            // Entering boundary - check if it's a vertical segment
            bool above = boundaries.find({x, p[1] - 1}) != boundaries.end();
            bool below = boundaries.find({x, p[1] + 1}) != boundaries.end();
            if (above || below) crosses++;
        }
        come_from_boundary = on_boundary;
    }
    
    return crosses % 2 == 1;
}

bool is_rectangle_valid(const unordered_set<vector<int>, VectorHash> &boundaries, 
                        const vector<int> &p1, const vector<int> &p2, 
                        const int &min_x, const int &min_y,
                        unordered_map<vector<int>, bool, VectorHash> &valid_points){
    // If all 4 corners are valid, entire rectangle is valid
    int x1 = min(p1[0], p2[0]);
    int x2 = max(p1[0], p2[0]);
    int y1 = min(p1[1], p2[1]);
    int y2 = max(p1[1], p2[1]);
    
    vector<vector<int>> corners = {{x1, y1}, {x1, y2}, {x2, y1}, {x2, y2}};
    for (const auto &corner : corners){
        if (valid_points.find(corner) != valid_points.end()){
            if (!valid_points[corner]) return false;
        } else {
            bool valid = boundaries.find(corner) != boundaries.end() || 
                        is_point_valid(boundaries, corner, min_x, min_y);
            valid_points[corner] = valid;
            if (!valid) return false;
        }
    }
    
    return true;
}

vector<vector<int>> get_corners(const vector<int> &p1, const vector<int> &p2){
    // Check if on a straight line
    if (p1[0] == p2[0] || p1[1] == p2[1]){
        // on a line
        return {p1, p2};
    }
    return {p1, p2, {p1[0], p2[1]}, {p2[0], p1[1]}};
}



int main() {
    auto start = chrono::high_resolution_clock::now();
    
    ifstream data("example.txt");
    string line;
    char comma;
    int x, y;
    long int min_x = 10000000;
    long int max_x = 0;
    long int min_y = 10000000;
    long int max_y = 0;

    // make some sort of 'valid grid', perhaps too big? --> yes, checked the max y diff and max x diff ~95000 = 1-^6
    // With all the points 'in between' and then merge them in some way
    // In the end, we do not need to process all pairs of points anymore.
    // Some points are invalid now. We can check if they are valid if all 4 courners are INSIDE
    // the green/red tiles. So perhaps making a grid, with is_red_green wouldn't be such a bad idea
    // do this in a set, incrementally


    // Do 1 loop over all the points, create a set of boundary points, perhaps this can already be done when the points are read out
    // Then, when we want to compute the area between 2 points, compute the 4 boundary points. 
    // For each point, check if it is valid. Then if the count is odd, we are good. (ray tracing tactic)
    vector<vector<int>> points;
    vector<int> prev_point(2, 0); // init a vec, size 2 of zeroes
    vector<int> point(2, 0);
    vector<int> first_point(2, 0);
    unordered_set<vector<int>, VectorHash> boundaries; // need to change the tuples to use a set.
    while (getline(data, line)) {
        
        stringstream ss(line);
        ss >> x >> comma >> y;  // Input is (x, y) = (column, row)
        // cout << x << comma << y << endl;
        point[0] = x;  // Store x (column) in first position
        point[1] = y;  // Store y (row) in second position
        points.push_back(point);

        // Store boundary points
        if (x < min_x) min_x = x;
        if (y < min_y) min_y = y;
        if (x > max_x) max_x = x;
        if (y > max_y) max_y = y;

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

            add_boundary_points(boundaries, point, prev_point);

            // cout << "Updated boundary points" << endl;
            // print_bdry_points(boundaries);
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

    // Connect the first and the last point
     add_boundary_points(boundaries, first_point, point);
    // cout << "FINAL boundary points" << endl;
    // print_bdry_points(boundaries);
    // cout << "Min " << min_i << " " << min_j << "\nMax " << max_i << " " << max_j << endl;

    long int max_area = 0;
    long int temp_area;

    unordered_map<vector<int>, bool, VectorHash> valid_points;
    for (const vector<int> &p : boundaries) valid_points[p] = true;
    int points_size = points.size();
    for (int idx_i = 0; idx_i < points_size -1; idx_i++){
        // print_vec(points[idx_i]);
        for (int idx_j = idx_i + 1; idx_j < points_size; idx_j++){
            // Skip if the points are the same
            if (points[idx_i][0] == points[idx_j][0] && points[idx_i][1] == points[idx_j][1]) continue;
            temp_area = area(points[idx_i][0], points[idx_i][1], points[idx_j][0], points[idx_j][1]);
            // Skip if the potential area is smaller anyway
            if (temp_area <= max_area) continue;
            
            // Check if entire rectangle is valid (all edges inside or on boundary)
            if (!is_rectangle_valid(boundaries, points[idx_i], points[idx_j], min_x, min_y, valid_points)) continue;
            
            if (temp_area > max_area){
                max_area = temp_area;
                cout << "Area "<< max_area << ". Points: " << points[idx_i][0] << "," << points[idx_i][1] << " & " <<  points[idx_j][0] << "," << points[idx_j][1] << endl;
            } 
        } // end loop over points index j
        // break;
    }// end loop over points index i
    cout << "Total: " <<  max_area << endl;

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Time: " << duration.count() << " ms" << endl;
    return 0;
}
