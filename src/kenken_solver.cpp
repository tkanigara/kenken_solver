#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <algorithm>
#include <cmath>
#include <sstream>
#include <fstream>
#include <iomanip>

using namespace std;
using namespace chrono;

struct Cage {
    vector<pair<int, int>> cells;
    char operation;
    int target;
    
    Cage(vector<pair<int, int>> c, char op, int t) : cells(c), operation(op), target(t) {}
};

class KenKenSolver {
private:
    int size;
    vector<vector<int>> grid;
    vector<Cage> cages;
    int backtrack_count;
    int constraint_checks;
    bool use_constraint_propagation;
    
public:
    KenKenSolver(int n, bool use_cp = true) : size(n), backtrack_count(0), constraint_checks(0), use_constraint_propagation(use_cp) {
        grid.assign(size, vector<int>(size, 0));
    }
    
    void setConstraintPropagation(bool enabled) {
        use_constraint_propagation = enabled;
    }
    
    void addCage(vector<pair<int, int>> cells, char operation, int target) {
        cages.push_back(Cage(cells, operation, target));
    }
    
    bool isValidRow(int row, int num) {
        for (int col = 0; col < size; col++) {
            if (grid[row][col] == num) return false;
        }
        return true;
    }
    
    bool isValidCol(int col, int num) {
        for (int row = 0; row < size; row++) {
            if (grid[row][col] == num) return false;
        }
        return true;
    }
      bool checkCageConstraint(const Cage& cage) {
        constraint_checks++;
        
        // Check if all cells in cage are filled
        vector<int> values;
        bool allFilled = true;
        for (auto cell : cage.cells) {
            if (grid[cell.first][cell.second] == 0) {
                allFilled = false;
            } else {
                values.push_back(grid[cell.first][cell.second]);
            }
        }
        
        // If not all cells are filled, we can't check yet - return true (valid so far)
        if (!allFilled) return true;
        
        // All cells filled, check constraint
        switch (cage.operation) {
            case '=':
                return values[0] == cage.target;
            case '+': {
                int sum = 0;
                for (int v : values) sum += v;
                return sum == cage.target;
            }
            case '-': {
                if (values.size() != 2) return false;
                return abs(values[0] - values[1]) == cage.target;
            }
            case '*': {
                int product = 1;
                for (int v : values) product *= v;
                return product == cage.target;
            }
            case '/': {
                if (values.size() != 2) return false;
                return (values[0] != 0 && values[1] != 0) && 
                       ((values[0] % values[1] == 0 && values[0] / values[1] == cage.target) || 
                        (values[1] % values[0] == 0 && values[1] / values[0] == cage.target));
            }
        }
        return false;
    }
      bool isValid(int row, int col, int num) {
        // Check row and column constraints
        if (!isValidRow(row, num) || !isValidCol(col, num)) return false;
        
        // If constraint propagation is disabled, just return true for cage constraints
        if (!use_constraint_propagation) return true;
        
        // Temporarily place number
        grid[row][col] = num;
        
        // Check all cages that contain this cell
        bool valid = true;
        for (const auto& cage : cages) {
            for (auto cell : cage.cells) {
                if (cell.first == row && cell.second == col) {
                    if (!checkCageConstraint(cage)) {
                        valid = false;
                        break;
                    }
                }
            }
            if (!valid) break;
        }
        
        // Remove temporary placement
        grid[row][col] = 0;
        return valid;
    }
    
    pair<int, int> findNextEmpty() {
        // Simple strategy: find first empty cell
        for (int row = 0; row < size; row++) {
            for (int col = 0; col < size; col++) {
                if (grid[row][col] == 0) {
                    return {row, col};
                }
            }
        }
        return {-1, -1}; // No empty cell found
    }
      bool solve() {
        auto pos = findNextEmpty();
        if (pos.first == -1) {
            // All cells filled - if CP disabled, check all cage constraints now
            if (!use_constraint_propagation) {
                for (const auto& cage : cages) {
                    if (!checkCageConstraint(cage)) {
                        return false;
                    }
                }
            }
            return true; // All cells filled and constraints satisfied
        }
        
        int row = pos.first, col = pos.second;
        
        for (int num = 1; num <= size; num++) {
            if (isValid(row, col, num)) {
                grid[row][col] = num;
                
                if (solve()) return true;
                
                // Backtrack
                grid[row][col] = 0;
                backtrack_count++;
            }
        }
        
        return false;
    }
    
    void printGrid() {
        cout << "\nSolution:\n";
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                cout << grid[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }    void printStats() {
        cout << "Statistics:\n";
        cout << "Backtracks: " << backtrack_count << endl;
        cout << "Constraint checks: " << constraint_checks << endl;
        cout << "Constraint Propagation: " << (use_constraint_propagation ? "Enabled" : "Disabled") << endl;
    }
    
    // Helper function to print time with high precision
    void printTime(const string& label, const chrono::duration<double, milli>& duration) {
        cout << label << ": " << fixed << setprecision(6) << duration.count() << " ms\n";
    }
    
    // Parse cell position from string like "11" -> (1,1) or "23" -> (2,3)
    pair<int, int> parseCell(const string& cellStr) {
        if (cellStr.length() != 2) {
            cout << "Error: Cell format should be two digits (e.g., '11' for row 1, col 1)\n";
            return {-1, -1};
        }
        int row = cellStr[0] - '1';  // Convert '1' to 0, '2' to 1, etc.
        int col = cellStr[1] - '1';
        return {row, col};
    }
    
    // Read puzzle from input
    bool readPuzzle() {
        cout << "Enter puzzle size: ";
        cin >> size;
        
        if (size < 3 || size > 9) {
            cout << "Error: Size must be between 3 and 9\n";
            return false;
        }
        
        grid.assign(size, vector<int>(size, 0));
        cages.clear();
        
        cout << "Enter constraints (format: target operation cell1 cell2 ...):\n";
        cout << "Example: 6 + 11 12 13 (means cells (1,1), (1,2), (1,3) sum to 6)\n";
        cout << "Operations: + - * / =\n";
        cout << "Enter empty line to finish:\n";
        
        cin.ignore(); // Clear input buffer
        string line;
        
        while (getline(cin, line) && !line.empty()) {
            istringstream iss(line);
            int target;
            char operation;
            string cellStr;
            
            if (!(iss >> target >> operation)) {
                cout << "Error parsing line: " << line << endl;
                continue;
            }
            
            vector<pair<int, int>> cells;
            while (iss >> cellStr) {
                auto cell = parseCell(cellStr);
                if (cell.first == -1) {
                    cout << "Error parsing cell: " << cellStr << endl;
                    continue;
                }
                if (cell.first >= size || cell.second >= size) {
                    cout << "Error: Cell " << cellStr << " is out of bounds for " << size << "x" << size << " puzzle\n";
                    continue;
                }
                cells.push_back(cell);
            }
            
            if (cells.empty()) {
                cout << "Error: No valid cells found in line: " << line << endl;
                continue;
            }
            
            addCage(cells, operation, target);
            cout << "Added constraint: " << target << " " << operation << " with " << cells.size() << " cells\n";
        }
          cout << "Total constraints added: " << cages.size() << endl;
        return true;
    }
    
    // Read puzzle from file
    bool readPuzzleFromFile(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Error: Cannot open file " << filename << endl;
            return false;
        }
        
        // Read size
        if (!(file >> size)) {
            cout << "Error: Cannot read puzzle size from file\n";
            return false;
        }
        
        if (size < 3 || size > 9) {
            cout << "Error: Size must be between 3 and 9\n";
            return false;
        }
        
        grid.assign(size, vector<int>(size, 0));
        cages.clear();
        
        cout << "Reading " << size << "x" << size << " puzzle from " << filename << endl;
        
        string line;
        getline(file, line); // Clear the rest of size line
        
        while (getline(file, line) && !line.empty()) {
            istringstream iss(line);
            int target;
            char operation;
            string cellStr;
            
            if (!(iss >> target >> operation)) {
                cout << "Error parsing line: " << line << endl;
                continue;
            }
            
            vector<pair<int, int>> cells;
            while (iss >> cellStr) {
                auto cell = parseCell(cellStr);
                if (cell.first == -1) {
                    cout << "Error parsing cell: " << cellStr << endl;
                    continue;
                }
                if (cell.first >= size || cell.second >= size) {
                    cout << "Error: Cell " << cellStr << " is out of bounds for " << size << "x" << size << " puzzle\n";
                    continue;
                }
                cells.push_back(cell);
            }
            
            if (cells.empty()) {
                cout << "Error: No valid cells found in line: " << line << endl;
                continue;
            }
            
            addCage(cells, operation, target);
            cout << "Added constraint: " << target << " " << operation << " with " << cells.size() << " cells\n";
        }
        
        file.close();
        cout << "Total constraints added: " << cages.size() << endl;
        return true;
    }
};