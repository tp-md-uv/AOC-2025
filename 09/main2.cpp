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



// custom hash function
struct VectorHash {
    size_t operator()(const vector<int>& v) const {
        return hash<int>()(v[0]) ^ (hash<int>()(v[1]) << 1);
    }
};


void print_bdry_points(const unordered_set<vector<int>, VectorHash> &bdry_points){
    for (auto point : bdry_points){
        print_vec(point);
    }
}

long int area(const long int &x1, const long int &y1, const long int &x2, const long int &y2){
    return (1 + abs(x1 - x2)) * (1 + abs(y1 - y2));

}

void add_boundary_points(unordered_set<vector<int>, VectorHash> &boundaries, const vector<int> &p1, const vector<int> &p2){
    // Add boundary points between p1 and p2

    if (p1[0] == p2[0]){
        int max_j = max(p1[1], p2[1]);
        int min_j = min(p1[1], p2[1]);
        while (min_j <= max_j){
            boundaries.insert({p1[0], min_j});
            min_j++;
        }

    } else if (p1[1] == p2[1]){ 
        int max_i = max(p1[0], p2[0]);
        int min_i = min(p1[0], p2[0]);
        while (min_i <= max_i){
            boundaries.insert({min_i, p1[1]});
            min_i++;
        }

    }else throw("Values not on horizontal/vertical line.");
}


vector<int> starting_point_ray(const vector<int> &p, const int &min_i, const int max_i, const int &min_j, const int max_j){
    // p[0] = i, p[1] = j
    // Distance to edges in i-direction (vertical)
    int dist_to_top_edge = p[0] - min_i;
    int dist_to_bottom_edge = max_i - p[0];

    // Distance to edges in j-direction (horizontal)
    int dist_to_left_edge = p[1] - min_j;
    int dist_to_right_edge = max_j - p[1];

    vector<int> ray(2, 0);
    int offset = 3; // be safe with the offset
    
    // Find closest edge and set ray accordingly
    if ((dist_to_top_edge <= dist_to_bottom_edge) && (dist_to_top_edge <= dist_to_left_edge) && (dist_to_top_edge <= dist_to_right_edge)){
        // Closest to top edge (min_i), cast ray from above
        ray = {min_i - offset, p[1]}; 
    }
    else if ((dist_to_bottom_edge <= dist_to_top_edge) && (dist_to_bottom_edge <= dist_to_left_edge) && (dist_to_bottom_edge <= dist_to_right_edge)){
        // Closest to bottom edge (max_i), cast ray from below
        ray = {max_i + offset, p[1]};    
    }
    else if ((dist_to_left_edge <= dist_to_top_edge) && (dist_to_left_edge <= dist_to_bottom_edge) && (dist_to_left_edge <= dist_to_right_edge)){
        // Closest to left edge (min_j), cast ray from left
        ray = {p[0], min_j - offset};
    }
    else if ((dist_to_right_edge <= dist_to_top_edge) && (dist_to_right_edge <= dist_to_bottom_edge) && (dist_to_right_edge <= dist_to_left_edge)){
        // Closest to right edge (max_j), cast ray from right
        ray = {p[0], max_j + offset};
    }
    return ray;
}




bool is_point_valid(const unordered_set<vector<int>, VectorHash> &boundaries, const vector<int> &p, const int &min_i, const int max_i, const int &min_j, const int max_j){
    // check if the point is valid based on the sets of outside edges. 
    // Always start looping from the closest edge, (min, max), and then start in
    // a straight line until we have hit the point and count how many times we crossed 
    // the boundary, if this is odd, we're inside, else outside.

    // For now just always move the 'top , the first index that is.
    // TODO: make a speedup for to see to what corner we are closest to.
    auto t_start = chrono::high_resolution_clock::now();

    int count_bdry_crosses = 0;

    int diff_min_i = abs(p[0] - min_i);
    int diff_max_i = abs(p[0] - max_i);

    int diff_min_j = abs(p[1] - min_j);
    int diff_max_j = abs(p[1] - max_j);

    // Check which one is the smallest
    // Start from the correct position

    vector<int> ray = starting_point_ray(p, min_i, max_i, min_j, max_j);
    // cout << "Checking ";
    // print_vec(p);
    // cout << "Bounds: i[" << min_i << "," << max_i << "], j[" << min_j << "," << max_j << "], ray: (" << ray[0] << "," << ray[1] << ")" << endl;
    // Beam from the edge until we reach the point.
    // Then count how many times we hit the boundary. 
    // If this is even, we're NOT in the polygon.

    // cout << "Valid check for ";
    // print_vec(p);
    // cout << "Ray starting at: ";
    // print_vec(ray);
    // cout << "CHECK: " << (ray[0] != p[0] && ray[1] != p[1]) << endl;
    vector<int> prev_ray(2, 0);
    bool come_from_boundary = false;
    bool on_boundary = false;
    while (ray[0] != p[0] || ray[1] != p[1]){
        // Check if we are in the polygon
        
        on_boundary = boundaries.find(ray) != boundaries.end();
        // cout << "Ray on boundary: " << (on_boundary) << ". Prev ray NOT on boundary: " << (come_from_boundary) << endl;
        if (on_boundary && !come_from_boundary){
            // We only want to add once if we move along the boundary, so the prev point should NOT be in the boundary.
            count_bdry_crosses++;
        } 
        come_from_boundary = on_boundary;

        // cout << "(#cross " << count_bdry_crosses<<  ") New ray: ";
        // print_vec(ray);
        prev_ray[0] = ray[0];
        prev_ray[1] = ray[1];
        // Check where to move to
        if (ray[0] < p[0]) ray[0]++;
        if (ray[0] > p[0]) ray[0]--;
        if (ray[1] < p[1]) ray[1]++;
        if (ray[1] > p[1]) ray[1]--;
    }



    // cout << "# Bdry crosses " << (count_bdry_crosses) << ". p = (" << p[0] << "," << p[1] << "). Valid = " << (count_bdry_crosses % 2) << endl; 


    auto t_end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(t_end - t_start);
    // cout << "is_point_valid took: " << duration.count() << " µs" << endl;

    return (count_bdry_crosses % 2);
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
    int i, j;
    long int min_i = 10000000;
    long int max_i = 0;
    long int min_j = 10000000;
    long int max_j = 0;

    // make some sort of 'valid grid', perhaps too big? --> yes, checked the max y diff and max x diff ~95000 = 1-^6
    // With all the points 'in between' and then merge them in some way
    // In the end, we do not need to process all pairs of points anymore.
    // Some points are invalid now. We can check if they are valid if all 4 courners are INSIDE
    // the green/red tiles. So perhaps making a grid, with is_red_green wouldn't be such a bad idea
    // do this in a set, incrementally


    // Do 1 loop over all the points, create a set of boundary points, perhaps this can already be done when the points are read out
    // Then, when we want to compute the area between 2 points, compute the 4 boundary points. 
    // For each point, check if it is valid. Then if the count is odd, we are good.
    vector<vector<int>> points;
    vector<int> prev_point(2, 0);
    vector<int> point(2, 0);
    vector<int> first_point(2, 0);
    unordered_set<vector<int>, VectorHash> boundaries; // need to change the tuples to use a set.
    while (getline(data, line)) {
        
        stringstream ss(line);
        ss >> j >> comma >> i;  // Input is (j, i), so read j first
        // cout << x << comma << y << endl;
        point[0] = i;  // Store i in first position
        point[1] = j;  // Store j in second position
        points.push_back(point);

        // Store boundary points
        if (i < min_i) min_i = i;
        if (j < min_j) min_j = j;
        if (i > max_i) max_i = i;
        if (j > max_j) max_j = j;

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
    bool valid;

    unordered_map<vector<int>, bool, VectorHash> valid_points;
    for (const vector<int> &p : boundaries) valid_points[p] = true;
    vector<vector<int>> corners;
    int points_size = points.size();
    for (int i = 0; i < points_size -1; i++){
        temp_area = area(points[i][0], points[i][1], points[j][0], points[j][1]);
        if (temp_area < max_area) continue;
        cout << "\nChecking from idx_i: " << i << "/" << points_size << ". Size valid points "<< valid_points.size() << endl;
        // print_vec(points[i]);
        for (int j = i + 1; j < points_size; j++){
            if (points[i][0] == points[j][0] && points[i][1] == points[j][1]) continue;
            // cout << "\n\nComputing corners between "<< endl;
            // print_vec(points[i]);
            // cout << "And" << endl;
            // print_vec(points[j]);     
            // cout << "CORNERS: " << endl;
            corners = get_corners(points[i], points[j]);
            // print_grid(corners);
            valid = true;
            for (auto p : corners){
                // A speedup. See if we already calculated this point
                if (valid_points.find(p) != valid_points.end()){
                    // cout << "Used valid map. valid = " << valid_points[p] << ". Point: ";
                    // print_vec(p); 
                    // cout << endl;
                    valid = valid_points[p];
                    if (valid) continue;
                    break;
                }
                if (!is_point_valid(boundaries, p, min_i, max_i, min_j, max_j)){
                    valid = false;
                    valid_points[p] = false;
                    // cout << "INVALID POINT ";
                    // print_vec(p);
                    break;
                }else{
                    valid_points[p] = true;
                }
            }
            if (!valid) continue;

            temp_area = area(points[i][0], points[i][1], points[j][0], points[j][1]);
            if (temp_area > max_area){
                max_area = temp_area;
                cout << "Area "<< max_area << ". Points: " << points[i][0] << "," << points[i][1] << " & " <<  points[j][0] << "," << points[j][1] << endl;
                // cout << "Points: " << endl;
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
