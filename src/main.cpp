#include "kenken_solver.cpp"

int main() {
    cout << "KenKen Solver - C++ Implementation\n";
    cout << "===================================\n\n";
    
    char choice;
    cout << "Choose mode:\n";
    cout << "1. Interactive input\n";
    cout << "2. Read from file\n";
    cout << "Enter choice (1, 2): ";
    cin >> choice;
    
    if (choice == '1') {
        // Interactive mode
        char cp_choice;
        cout << "Enable Constraint Propagation? (y/n): ";
        cin >> cp_choice;
        bool use_cp = (cp_choice == 'y' || cp_choice == 'Y');
        
        KenKenSolver solver(0, use_cp); // Size will be set in readPuzzle()
        
        if (solver.readPuzzle()) {
            cout << "\nSolving puzzle...\n";
            auto start = high_resolution_clock::now();
            bool solved = solver.solve();
            auto end = high_resolution_clock::now();
            auto dur = duration_cast<duration<double, milli>>(end - start);
            
            if (solved) {
                solver.printGrid();
                solver.printTime("Solving time", dur);
                solver.printStats();
            } else {
                cout << "No solution found!\n";
                solver.printTime("Search time", dur);
                solver.printStats();
            }
        }
    } else if (choice == '2') {
        // File input mode
        string filename;
        cout << "Enter filename (e.g., example_input.txt): ";
        cin >> filename;
        
        char cp_choice;
        cout << "Enable Constraint Propagation? (y/n): ";
        cin >> cp_choice;
        bool use_cp = (cp_choice == 'y' || cp_choice == 'Y');
        
        KenKenSolver solver(0, use_cp); // Size will be set in readPuzzleFromFile()
        
        if (solver.readPuzzleFromFile(filename)) {
            cout << "\nSolving puzzle...\n";
            auto start = high_resolution_clock::now();
            bool solved = solver.solve();
            auto end = high_resolution_clock::now();
            auto dur = duration_cast<duration<double, milli>>(end - start);
            
            if (solved) {
                solver.printGrid();
                solver.printTime("Solving time", dur);
                solver.printStats();
            } else {
                cout << "No solution found!\n";
                solver.printTime("Search time", dur);
                solver.printStats();
            }
        }
    } else {
        printf("Please implement test examples in the code.");
    }
    
    return 0;
}
