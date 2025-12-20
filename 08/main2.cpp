#include <iostream> 
#include <fstream> 
#include <vector>
#include <algorithm>
#include <cmath>
#include <map>
#include <sstream>
#include <tuple>
#include <cassert>
#include <string>
#include <chrono>
#include <set>

#include "../helpers.h"
using namespace std;

void print_point(const tuple<int, int, int> &p) {
    cout << "(" << get<0>(p) << "," << get<1>(p) << "," << get<2>(p) << ")";
}

void print_component(const set<tuple<int, int, int>> &comp){
    for (auto p : comp){
        print_point(p);
        cout << ", ";
    }
    cout << "  ("<< comp.size() << ")";
}

void print_grid(const vector<tuple<int, int, int>> &grid){
    cout << "[" << endl;
    for (auto x : grid){
        cout << "[" << get<0>(x) << "," << get<2>(x) << "," <<  get<2>(x) << "," << "]" << endl; 
    }
    cout << "]" << endl;
}

void remove_el(vector<tuple<int, int, int>> &vec, tuple<int, int, int> to_rem){
    for (int i = 0; i < vec.size(); i++){
        if (vec[i] != to_rem) continue;
        vec.erase(vec.begin() + i);
        break;
    }
}

long double euclid_dist(tuple<int, int, int> p1, tuple<int, int, int> p2){
    long double dist = sqrt(
        pow(get<0>(p1) - get<0>(p2), 2) + 
        pow(get<1>(p1) - get<1>(p2), 2) + 
        pow(get<2>(p1) - get<2>(p2), 2)
    );

    // cout << "Calculating distance between ";
    // print_point(p1);
    // cout << " and ";
    // print_point(p2);
    // cout << endl;

    return dist;
}

void print_edges(const map<tuple<int, int, int>, tuple<int, int, int>>& E) {
    for (const auto& pair : E) {
        print_point(pair.first);
        cout << " --> ";
        print_point(pair.second);
        cout << endl;
    }
}



bool in_different_components(const vector<set<tuple<int, int, int>>> &components, const tuple<int, int, int> &p1, const tuple<int, int, int> &p2, int &idx_1, int &idx_2){
    // for each component, check if p1 and p2 are in there. Return the idx
    for (int i=0; i < components.size(); i++){
        if (components[i].find(p1) != components[i].end()) idx_1 = i;
        if (components[i].find(p2) != components[i].end()) idx_2 = i;
    }
    if (idx_1 == -1 || idx_2 == -1) return false; // If either of the points is not in a component, return false
    return (idx_1 != idx_2);
}


int main() {
    auto start = chrono::high_resolution_clock::now();
    ifstream data("example.txt");
    string line, coordinate;
    map<tuple<int, int, int>, tuple<int, int, int>> E;
    vector<tuple<int, int, int>> V;
    vector<tuple<int, int, int>> V_c;
    int x, y, z;
    tuple<int, int, int> p, p1, p2;
    while (getline(data, line)){
        stringstream ss(line);
        char comma;
        ss >> x >> comma >> y >> comma >> z;  
        // cout << "Reading " << x << " " << y << " " << z << endl;
        p = make_tuple(x, y, z); // create coordinate
        V.push_back(p); // add to V
        V_c.push_back(p); // copy
    }
    // print_grid(coordinates);
    
    // apparently maps already sort by key, didn't know
    map<long double, vector<tuple<int, int, int>>> dist_map;
    long double dist;
    // Compute the distances betwee all the pair of points
    for (int i = 0; i < V.size() - 1; i++){
        p1 = V[i];
        for (int j = i + 1; j < V.size(); j++){
            p2 = V[j];
            dist = euclid_dist(p1, p2);
            dist_map[dist] = {p1, p2}; 
        }
    // cout << "Calculating distance between ";
    // print_point(p1);
    // cout << " and ";
    // print_point(p2);
    // cout << endl;        }
    }

    // for (auto x : dist_map){
    //     cout << x.first << " ";
    //     print_point(x.second[0]);
    //     cout << " ";
    //     print_point(x.second[1]); 
    //     cout << endl;
    // }
    // compute the distances between all points.
    // int i = 0;
    // for (auto x : dist_map){
    //     cout << "Distance: " << x.first << endl;
    //     cout << x.first << " ";
    //     print_point(x.second[0]);
    //     cout << " ";
    //     print_point(x.second[1]); 
    //     cout << endl;
    //     E[x.second[0]] = x.second[1];
    //     i ++;
    //     if (i > 10) break;
    // }   

    // cout << "Number elements in dist map " << dist_map.size() << endl;
    // cout << endl << endl << "Constructing the components" << endl;
    // cout << "Printing the edges " << endl;
    // print_edges(E);
    vector<set<tuple<int, int, int>>> components;
    set<tuple<int, int, int>> new_component;
    bool done;
    int num_connections = 1;
    bool found;
    int idx_1, idx_2;

    for (auto dist_points : dist_map){
        // now just break if there is only 1 component and that size is the number of points
        if (components.size() == 1 && components[0].size() == V.size()){
            // cout << "CLOSED CIRCUIT. componenets size " << V.size() << endl;
            // cout << "p1(x) " << (long int)get<0>(p1) * (long int)get<0>(p2);
            cout << "Total: " << (long int)get<0>(p1) * (long int)get<0>(p2) << endl;
            break;
        } 
        idx_1 = -1;
        idx_2 = -1;
        p1 = dist_points.second[0];
        p2 = dist_points.second[1]; 


        // cout << endl << "Number connections "<< num_connections << ". Distance " << dist_points.first << " ";
        // print_point(p1);
        // cout << " ";
        // print_point(p2);
        // cout << endl;
        
        // Check all the existing components
        // and see if p1 OR p2 is in there. If so, add the points
        // to the components
        if (components.size() == 0){
            // cout << "initialzing components " << endl;
            components.push_back({p1, p2}); // Init the components vec
            num_connections++;
            continue;
        } 

        found = false;

        // cout << "------COMPONENTS------" << endl;
        // for (auto component : components){
        //     print_component(component);
        //     cout << endl;
        // }
        // cout << "-----------------------" << endl;

        if (in_different_components(components, p1, p2, idx_1, idx_2)){
            // merge

            // Add the second set into the first one, then remove the second set
            // cout << "Merging " <<  components[idx_1].size() << " " << components[idx_2].size() <<    endl;
            // cout << endl;

            components[idx_1].insert(components[idx_2].begin(), components[idx_2].end());
            components.erase(components.begin() + idx_2);
            num_connections++;
            continue;
        }
        for (auto &component : components){
            // cout << "Checking component: ";
            // print_component(component);
            // cout << endl;
            // if both p1 AND p2 are in the component, continue
            if (component.find(p1) != component.end() && component.find(p2) != component.end()){
                // cout << "p1 and p2 in the same component" << endl;
                found = true;
                num_connections++;
                break;
            } 

            // check if p1 or p2 in the components
            if (component.find(p1) != component.end() || component.find(p2) != component.end()){
                // cout << "Adding element to component" << endl;
                component.insert(p1);
                component.insert(p2);
                num_connections++;
                found = true;
                // cout << "UPDATED COMPONENT: " << endl;
                // print_component(component);
                // cout << endl;
                break;
            }
        } // end loop over components
        // If we start with a new component, create it here
        if (!found){
            // cout << "p1 and p2 not in any component" << endl;
            new_component = {p1, p2};
            num_connections++;
            components.push_back(new_component); // add the new component
        }
    } // end loop over distance map

    
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Time: " << duration.count() << " ms" << endl;
    return 0;
}