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

struct Shape {
    vector<string> pattern;
    vector<vector<string>> rotations; // All 4 rotations
};

struct Constraint {
    int width, height;
    vector<int> shapeCounts; // How many of each shape needed
};

// Rotate a shape 90 degrees clockwise
vector<string> rotateShape(const vector<string>& shape) {
    int rows = shape.size();
    int cols = shape[0].size();
    vector<string> rotated(cols, string(rows, '.'));
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            rotated[j][rows - 1 - i] = shape[i][j];
        }
    }
    return rotated;
}

// Generate all 4 rotations of a shape
vector<vector<string>> generateRotations(const vector<string>& shape) {
    vector<vector<string>> rotations;
    vector<string> current = shape;
    
    for (int i = 0; i < 4; i++) {
        rotations.push_back(current);
        current = rotateShape(current);
    }
    return rotations;
}

// Parse input data
pair<vector<Shape>, vector<Constraint>> parseData(const string& filename) {
    vector<Shape> shapes;
    vector<Constraint> constraints;
    
    ifstream file(filename);
    string line;
    
    // cout << "DEBUG: Starting to parse shapes..." << endl;
    
    // Parse shapes
    while (getline(file, line)) {
        // cout << "DEBUG: Read line: '" << line << "'" << endl;
        
        // Skip empty lines
        if (line.empty()) {
            // cout << "DEBUG: Skipping empty line" << endl;
            continue;
        }
        
        // Check if this is a constraint line (has 'x' and ':')
        if (line.find('x') != string::npos && line.find(':') != string::npos) {
            // cout << "DEBUG: Found constraint line, stopping shape parsing: " << line << endl;
            // Put this line back for constraint parsing - we need to reprocess it
            // For now, we'll handle the first constraint separately
            break;
        }
        
        if (line.find(':') != string::npos) {
            // cout << "DEBUG: Found shape header: " << line << endl;
            // Found shape header like "0:"
            Shape shape;
            // Read next 3 lines as pattern
            for (int i = 0; i < 3; i++) {
                string patternLine;
                getline(file, patternLine);
                // cout << "DEBUG: Pattern line " << i << ": '" << patternLine << "'" << endl;
                shape.pattern.push_back(patternLine);
            }
            shape.rotations = generateRotations(shape.pattern);
            shapes.push_back(shape);
            // cout << "DEBUG: Added shape " << (shapes.size() - 1) << endl;
        }
    }
    
    // cout << "DEBUG: Finished parsing shapes, found " << shapes.size() << endl;
    // cout << "DEBUG: Starting to parse constraints..." << endl;
    
    // Parse constraints - process the line we already read if it's a constraint
    if (line.find('x') != string::npos && line.find(':') != string::npos) {
        // cout << "DEBUG: Processing first constraint line: '" << line << "'" << endl;
        Constraint constraint;
        
        // Parse "12x5: 1 0 1 0 3 2"
        size_t colonPos = line.find(':');
        string sizeStr = line.substr(0, colonPos);
        string countsStr = line.substr(colonPos + 1);
        
        // Parse size "12x5"
        size_t xPos = sizeStr.find('x');
        constraint.width = stoi(sizeStr.substr(0, xPos));
        constraint.height = stoi(sizeStr.substr(xPos + 1));
        
        // Parse counts "1 0 1 0 3 2"
        stringstream ss(countsStr);
        int count;
        while (ss >> count) {
            constraint.shapeCounts.push_back(count);
        }
        
        constraints.push_back(constraint);
        // cout << "DEBUG: Added constraint " << (constraints.size() - 1) << endl;
    }
    
    // Parse remaining constraints
    while (getline(file, line)) {
        // cout << "DEBUG: Read constraint line: '" << line << "'" << endl;
        if (line.find('x') != string::npos && line.find(':') != string::npos) {
            Constraint constraint;
            
            // Parse "12x5: 1 0 1 0 3 2"
            size_t colonPos = line.find(':');
            string sizeStr = line.substr(0, colonPos);
            string countsStr = line.substr(colonPos + 1);
            
            // Parse size "12x5"
            size_t xPos = sizeStr.find('x');
            constraint.width = stoi(sizeStr.substr(0, xPos));
            constraint.height = stoi(sizeStr.substr(xPos + 1));
            
            // Parse counts "1 0 1 0 3 2"
            stringstream ss(countsStr);
            int count;
            while (ss >> count) {
                constraint.shapeCounts.push_back(count);
            }
            
            constraints.push_back(constraint);
        }
    }
    
    return make_pair(shapes, constraints);
}

// Check if shape can be placed at position without overlap
bool isValid(const vector<vector<int>>& grid, const vector<string>& shape, int row, int col) {
    int shapeRows = shape.size();
    int shapeCols = shape[0].size();
    int gridRows = grid.size();
    int gridCols = grid[0].size();
    
    // Check bounds
    if (row < 0 || col < 0 || row + shapeRows > gridRows || col + shapeCols > gridCols) {
        return false;
    }
    
    // Check overlap
    for (int i = 0; i < shapeRows; i++) {
        for (int j = 0; j < shapeCols; j++) {
            if (shape[i][j] == '#' && grid[row + i][col + j] != 0) {
                return false; // Overlap detected
            }
        }
    }
    
    return true;
}

// Check if there's at least one 3x3 area of empty cells in the grid
bool hasEmpty3x3Grid(const vector<vector<int>>& grid) {
    int gridRows = grid.size();
    int gridCols = grid[0].size();
    
    // Check all possible 3x3 positions
    for (int r = 0; r <= gridRows - 3; r++) {
        for (int c = 0; c <= gridCols - 3; c++) {
            // Check if this 3x3 area is completely empty
            bool isEmpty = true;
            for (int i = 0; i < 3 && isEmpty; i++) {
                for (int j = 0; j < 3 && isEmpty; j++) {
                    if (grid[r + i][c + j] != 0) {
                        isEmpty = false;
                    }
                }
            }
            if (isEmpty) {
                return true; // Found at least one empty 3x3 grid
            }
        }
    }
    
    return false; // No 3x3 empty area exists
}

// Place shape on grid with given counter
void placeShape(vector<vector<int>>& grid, const vector<string>& shape, int row, int col, int shapeId) {
    int shapeRows = shape.size();
    int shapeCols = shape[0].size();
    
    for (int i = 0; i < shapeRows; i++) {
        for (int j = 0; j < shapeCols; j++) {
            if (shape[i][j] == '#') {
                grid[row + i][col + j] = shapeId;
            }
        }
    }
}

// Remove shape from grid
void removeShape(vector<vector<int>>& grid, const vector<string>& shape, int row, int col) {
    int shapeRows = shape.size();
    int shapeCols = shape[0].size();
    
    for (int i = 0; i < shapeRows; i++) {
        for (int j = 0; j < shapeCols; j++) {
            if (shape[i][j] == '#') {
                grid[row + i][col + j] = 0;
            }
        }
    }
}

// Debug function to print grid
void printGrid(const vector<vector<int>>& grid) {
    for (const auto& row : grid) {
        for (int cell : row) {
            cout << (cell == 0 ? '.' : to_string(cell)[0]) << " ";
        }
        cout << endl;
    }
    cout << "---" << endl;
}

// Calculate area of a shape (number of '#' cells)
int getShapeArea(const vector<string>& shape) {
    int area = 0;
    for (const string& row : shape) {
        for (char c : row) {
            if (c == '#') area++;
        }
    }
    return area;
}

// Heuristic: check if total shape area fits in grid
bool canFitInGrid(const vector<Shape>& shapes, const Constraint& c) {
    int gridArea = c.width * c.height;
    int totalShapeArea = 0;
    
    for (int i = 0; i < c.shapeCounts.size() && i < shapes.size(); i++) {
        int shapeArea = getShapeArea(shapes[i].pattern);
        totalShapeArea += shapeArea * c.shapeCounts[i];
    }
    
    if (totalShapeArea > gridArea) {
        // cout << "Grid area: " << gridArea << ", Shape area needed: " << totalShapeArea << endl;
        return false;
    }
    
    return true;
}

int solve(const vector<Shape>& shapes, Constraint& c, vector<vector<int>>& grid, int& counter, int& placementId, vector<vector<int>>& solutionGrid) {
    // Give up if we've tried too many iterations (counter overflow or too large)
    const int MAX_ITERATIONS = 1000000000; // 1 billion
    if (counter < 0 || counter > MAX_ITERATIONS) {
        return 0;
    }
    
    // Heuristic: early exit if impossible
    if (!canFitInGrid(shapes, c)) {
        // cout << "Heuristic: Cannot fit shapes in grid" << endl;
        return 0;
    }
    
    // Check if all shapes are placed
    int num_shapes_to_place = 0;
    for (int i = 0; i < c.shapeCounts.size(); i++) {
        num_shapes_to_place += c.shapeCounts[i];
    }
    if (num_shapes_to_place == 0) {
        solutionGrid = grid; // Save the solution
        return 1; // Found a solution - stop searching
    }
    
    int totalSolutions = 0;
    
    // Find first available shape type
    for (int shapeIdx = 0; shapeIdx < c.shapeCounts.size(); shapeIdx++) {
        if (c.shapeCounts[shapeIdx] == 0) continue; // Skip if none left
        if (shapeIdx >= shapes.size()) continue; // Bounds check - skip if shape doesn't exist
        
        c.shapeCounts[shapeIdx]--; // Use one piece
        
        // Try all rotations of this shape
        for (int rotIdx = 0; rotIdx < shapes[shapeIdx].rotations.size(); rotIdx++) {
            const auto& rotation = shapes[shapeIdx].rotations[rotIdx];
            
            // Try all positions on grid
            for (int row = 0; row < c.height; row++) {
                for (int col = 0; col < c.width; col++) {
                    counter++;
                    
                    if (!isValid(grid, rotation, row, col)) continue; // Skip invalid positions
                    
                    // Valid placement - place shape and recurse
                    placeShape(grid, rotation, row, col, placementId);
                    placementId++;
                    
                    // Check if we still have shapes to place
                    int remaining_shapes = 0;
                    for (int i = 0; i < c.shapeCounts.size(); i++) {
                        remaining_shapes += c.shapeCounts[i];
                    }
                    
                    // If we still have shapes to place but no 3x3 empty area exists, backtrack immediately
                    if (remaining_shapes > 0 && !hasEmpty3x3Grid(grid)) {
                        // Invalid state - can't place remaining shapes
                        placementId--;
                        removeShape(grid, rotation, row, col);
                        continue;
                    }
                    
                    // printGrid(grid);
                    
                    int result = solve(shapes, c, grid, counter, placementId, solutionGrid);
                    if (result > 0) {
                        // Found a solution - stop immediately
                        placementId--;
                        removeShape(grid, rotation, row, col);
                        c.shapeCounts[shapeIdx]++;
                        return 1;
                    }
                    
                    // cout << "    Backtracking from (" << row << "," << col << ")" << endl;
                    // Backtrack - remove shape
                    placementId--;
                    removeShape(grid, rotation, row, col);
                }
            }
        }
        
        c.shapeCounts[shapeIdx]++; // Restore count for backtracking
    }
    
    return totalSolutions;
}

int main() {
    auto start = chrono::high_resolution_clock::now();
    
    auto [shapes, constraints] = parseData("example.txt");
    
    vector<bool> hasSolution(constraints.size(), false);
    
    // Test solving all constraints
    for (size_t constraintIdx = 0; constraintIdx < constraints.size(); constraintIdx++) {
        Constraint constraint = constraints[constraintIdx]; // Make a copy
        
        // Create grid and solve
        vector<vector<int>> grid(constraint.height, vector<int>(constraint.width, 0));
        vector<vector<int>> solutionGrid; // Will store the solution if found
        int counter = 1;
        int placementId = 1;
        
        int numSolutions = solve(shapes, constraint, grid, counter, placementId, solutionGrid);
        hasSolution[constraintIdx] = (numSolutions > 0);
        
        // if (numSolutions > 0) {
        //     cout << "Constraint " << constraintIdx << " - Solution found:" << endl;
        //     for (const auto& row : solutionGrid) {
        //         for (int cell : row) {
        //             cout << (cell == 0 ? '.' : to_string(cell)[0]) << " ";
        //         }
        //         cout << endl;
        //     }
        //     cout << endl;
        // }
    }
    
    // Print summary
    int solutionCount = 0;
    for (bool solved : hasSolution) {
        if (solved) solutionCount++;
    }
    auto end = chrono::high_resolution_clock::now();
    cout << "Total: " << solutionCount << endl;
    auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
    cout << "Time: " << duration.count() << " ms" << endl;
    return 0;
}
