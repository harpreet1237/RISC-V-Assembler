#include <bits/stdc++.h>
#include <filesystem>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <string>

#include "opcode_parser.cpp"
using namespace std;
namespace fs = std::filesystem;

filesystem::path cwd = filesystem::current_path();

int main(int argc, char *argv[]) { 

    if (argc != 4) { // Check if the correct number of arguments is provided
        cerr << "Usage: " << argv[0] << " <Input file> <Output_Path> <Branch Predictor>" << endl;
        cerr << "Provide the following integers as input for Branch Predictor: " << endl;
        cerr << "1: Always Taken" << endl;
        cerr << "2: Always Not Taken" << endl;
        cerr << "3: 1 Bit Branch Predictor" << endl;
        cerr << "4: 2 Bit Branch Predictor" << endl;

        cerr << endl << "Sample Input: \"" << argv[0] << " test output_path 1\" (analyzes test file with branch predictor \"Always taken\" and produces output file)" << endl;
        return 1;
    }

    string input_file_path = argv[1];
    // Check if the file exists
    if (fs::exists(input_file_path) && fs::is_regular_file(input_file_path)) {
    } else {
        std::cout << "File '" << input_file_path << "' does not exist." << endl;
        return 0;
    }

    // check if the output directory exists
    // Check if the directory exists
    string output_file_path = argv[2];
    if (fs::exists(output_file_path) && fs::is_directory(output_file_path)) {
    } else {
        cout << "Directory '" << output_file_path << "' does not exist." << endl;
        return 0;
    }

    if(output_file_path[output_file_path.size() - 1] == '/') {
        output_file_path = output_file_path.substr(0, output_file_path.size() - 1);
    }
    // Extract the filename from the input file path
    filesystem::path file_path(input_file_path);
    string filename = file_path.filename().string();
    string output_file = output_file_path + "/" + filename + "_Analysis_BP" + argv[3];



    initialize_globals();
    freopen(argv[1], "r", stdin);
    freopen(output_file.c_str(), "w", stdout);
    string line;

    int branch_predictor = argv[3][0] - '0'; 
    // Previous Instruction Status
    uint32_t prev = 0;  // Address of previous Instruction
    bool prev_ = false; // whether previous instruction was a branch instruction
    bool pred_ = false; // branch pred of prev branch instr.
    
    // Accuracy calc Utilities
    long long total = 0, correct = 0;
    int idx = 0;
    while(getline(cin, line)) {
        if(line.empty()) { // Skip the line, if empty
            continue;
        } if(idx >= 1e6) { // Evaluate for 1-million Instructions
            break;
        }

        vector<string> tmp2 = splitByWhitespace(line);
        // CHECKS FOR STANDARD FORMAT
        if(tmp2.size() < 5) continue; // Not a standard instruction
        if(tmp2[0] != "core" or tmp2[2] == ">>>>") {
            continue; // Check_2
        }

        auto tmp = get_info(line);
        bool is_taken = false;
        if(prev_) { // Previous Instruction was a Branch Instruction
            bool actual = (tmp.first == BTB[prev].target_address) ? true : false;
            total += 1;
            if(actual == pred_) {
                correct += 1;
            }
            if(actual) { // branch is taken
                BTB[prev].state1 = 1;
                BTB[prev].state2 = (BTB[prev].state2 == 3) ? 3 : BTB[prev].state2 + 1;
            } else { // branch is not taken
                BTB[prev].state1 = 0;
                BTB[prev].state2 = (BTB[prev].state2 == 0) ? 0 : BTB[prev].state2 - 1;            
            }
        }
        if(tmp.second) {
            // is a branch instruction
            if(BTB.find(tmp.first) == BTB.end()) {
                // Entry not present in BTB
                BTB_Entry tmp1;
                tmp1.target_address = tmp.first + stoi(tmp2[tmp2.size() - 1]);
                tmp1.state1 = 0; // Not taken state
                tmp1.state2 = 1; // Weakly Not taken state
                BTB[tmp.first] = tmp1; // Inserting the entry to BTB
            }
            is_taken = predict(branch_predictor, tmp.first);
        }
        // Updating the prev status
        prev  = tmp.first;
        prev_ = tmp.second;
        pred_ = is_taken;
        idx++; // Incrementing the index
    }

// Generating the OUTPUT file
    cout << "Selected Trace: " << cwd.string() << "/" << argv[1] << endl;
    cout << "Selected Branch Predictor: ";
    switch (branch_predictor) {
    case 1:
        cout << "Always Taken" << endl;
        break;
    case 2:
        cout << "Always Not Taken" << endl;
        break;
    case 3:
        cout << "1-bit Branch Predictor" << endl;
        break;
    case 4:
        cout << "2-bit Branch Predictor" << endl;
        break;
    } cout << endl;
    cout << "Total Branch:            " << total << endl;
    cout << "Correctly Predicted:     " << correct << endl;
    cout << "Accuracy:                " << ((float)correct/(float)total) * 100 << " %" << endl;
    cout << endl << "BRANCH TARGET BUFFER" << endl; // Printing the branch target buffer

    int INDEX = 0;
    for (auto x:BTB) {
        auto temp = INDEX;
        int digits = 0;
        while(temp > 0) {
            digits++; temp/=10;
        } digits = (digits == 0) ? 1 : digits;
        string space = "";
        for (int i = 0; i<(8 - digits)/2; i++) {
            space += ' ';
        }
        cout << "================================" << endl;
        cout << "|        |Instr.    |" << uint32ToHex(x.first) << "|"  << endl;
        if(!(digits%2)) {
            cout << "|" << space << INDEX;
            cout << space <<"|Target    |" << uint32ToHex(x.second.target_address) << "|" << endl;
        } else {
            cout << "|" << space << INDEX;
            cout << space <<" |Target    |" << uint32ToHex(x.second.target_address) << "|" << endl;
        }
        if(branch_predictor == 3) {
            cout << "|        |State     |    " << x.second.state1 << "     |" << endl;
        } else if(branch_predictor == 4) {
            cout << "|        |State     |    " << x.second.state2 << "     |" << endl;

        }
        INDEX++;
    } cout << "================================" << endl;
    return 0;
}