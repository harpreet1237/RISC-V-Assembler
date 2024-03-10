#include <bits/stdc++.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <string>
#include "opcode_parser.cpp"

using namespace std;


int main(int argc, char *argv[]) { 

    if (argc != 3) { // Check if the correct number of arguments is provided
        cerr << "Usage: " << argv[0] << " <Input filename> <Output filename>" << endl;
        return 1;
    }
    
    initialize_globals();

    freopen(argv[1], "r", stdin);
    freopen(argv[2], "w", stdout);
    bool TEXT_SEG = true, DATA_SEG = false;
    string line;

    // FIRST ITERATION (Get the Label addresses)

    while(getline(cin, line)) {
        // Find the position of '#' character in the line
        size_t pos = line.find('#');
        // If '#' character found, remove everything after it
        if (pos != string::npos) {
            line.erase(pos);
        }

        line = trim(line); // trim the leading and trailing whitespaces in the line

        if(line.empty()) {
            continue; // continue if the line is empty
        } else { 

            vector<string> tokens = splitByWhitespace(line);

            if(tokens[0] == ".data") { // DATA SEGEMENT
                DATA_SEG = true; TEXT_SEG = false;
                continue;

            } else if (tokens[0] == ".text") { // TEXT SEGMENT
                TEXT_SEG = true; DATA_SEG = false;
                if(tokens.size() == 1) {
                    continue; // No instruction in the given line
                }
                tokens.erase(tokens.begin());

            }
            if(TEXT_SEG) {
                if(line.find(":") == string::npos) { // Label is not present in the current line
                    Instruction_Address += 4;
                    continue;
                }

                line = concatenateWithWhitespace(tokens);
                tokens = splitString(line, ':');
                int num_labels = countOccurrences(line, ':');
                for (int i = 0; i < num_labels; i++) {
                    lbl_mp[tokens[i]] = Instruction_Address; // updating the map
                }

                if(tokens.size() == num_labels + 1) { 
                    Instruction_Address += 4;
                }
            }
        }  
    } 
    
    // RESETTING for SECOND ITERATION
    Instruction_Address = 0;
    TEXT_SEG = true, DATA_SEG = false;
    // Clear end-of-file flag and seek back to the beginning of the stream
    cin.clear();
    cin.seekg(0, ios::beg);
    // SECOND ITERATION
    while (getline(cin, line)) { // Read each line of the file using std::cin

        // Find the position of '#' character in the line
        size_t pos = line.find('#');
        // If '#' character found, remove everything after it
        if (pos != string::npos) {
            line = line.substr(0, pos);
        }

        line = trim(line); // trim the leading and trailing whitespaces in the line

        if(line.empty()) {
            continue; // continue if the line is empty
        } else { 

            vector<string> tokens = splitByWhitespace(line);

            if(tokens[0] == ".data") { // DATA SEGEMENT
                DATA_SEG = true; TEXT_SEG = false;
                continue;

            } else if (tokens[0] == ".text") { // TEXT SEGMENT
                TEXT_SEG = true; DATA_SEG = false;
                if(tokens.size() == 1) {
                    continue; // No instruction in the given line
                }
                tokens.erase(tokens.begin());

            }

            if(TEXT_SEG) {
                uint32_t machine_code;
                
                line = concatenateWithWhitespace(tokens);
                tokens = splitString(line, ':');
                int num_labels = countOccurrences(line, ':');

                if(line.find(":") == string::npos) { // Label is not present in the current line

                    // Getting the machine code of the instruction
                    machine_code = process_Instruction(line, Instruction_Address);
                    if(machine_code == -1) {
                        cerr << "Error in assembly Code" << endl;
                        exit(1);
                    }
                    cout << Instruction_Address << " " << machine_code << endl;
                    Instruction_Address += 4;
                    continue;
                }

                if(tokens.size() == num_labels + 1) { 
                    // Getting the machine code of the instruction

                    machine_code = process_Instruction(tokens[tokens.size() - 1], Instruction_Address);
                    if(machine_code == -1) {
                        cerr << "Error in assembly Code" << endl;
                        exit(1);
                    }
                    text_mc.push_back({Instruction_Address, machine_code});
                    cout << Instruction_Address << " " << machine_code << endl;
                    Instruction_Address += 4;
                }
            }

            if(DATA_SEG) {
                int data_size = -1; // getting the size of the data_type
                for (auto token: tokens) {
                    if (token[0] == '.') {
                        for (auto a_d: assembler_directives) {
                            if(a_d.first == token) {
                                data_size = a_d.second;
                                break;
                            }
                        }
                    }
                }

                if(data_size == -1) {
                    cout << "assembler directive to indicate data type not included in data segment" << endl;
                    exit(1);
                }

                bool array = false;
                size_t pos = line.find('('); // array
                if (pos != string::npos) {
                    array = true; 
                    string vals = line.substr (pos + 1);
                    vals.erase(vals.size() - 1); // erasing the last ')' character

                    vector<int64_t> array = extractIntegers(vals);
                    for(auto element:array) {
                        data_mc.push_back({Data_Address, int64ToHex(element, data_size)});
                        Data_Address += data_size; // Increment by "Data_Size" bytes.
                    }
                    continue;
                }
                pos = line.find('"'); // string (.asciz)
                if (pos != string::npos) {
                    array = true; 
                    string asciz_str = line.substr (pos + 1);
                    asciz_str.erase(asciz_str.size() - 1); // erasing the last '"' character

                    // including an '\0' character in end
                    asciz_str.push_back('\0');

                    for(char c: asciz_str) {
                        data_mc.push_back({Data_Address, int64ToHex(static_cast<int64_t>(c), 1)});
                        Data_Address += 1; // increment by 1 byte.
                    }
                    continue;
                }

                // Handling Logic for Single Value element
                int64_t element = parseInteger(tokens[tokens.size() - 1]);
                data_mc.push_back({Data_Address, int64ToHex(element, data_size)});
                Data_Address += data_size;
            }
        }
    }

    // Writing the ouput.mc file
    for (auto entry: data_mc) { // DATA SEGMENT
        auto address = uint32ToHex(entry.first);
        cout << address << " " << entry.second << endl;
    }
    cout << endl;
    for (auto entry: text_mc) { // TEXT SEGMENT
        auto address = uint32ToHex(entry.first);
        auto machine_code = uint32ToHex(entry.second);
        cout << address << " " << machine_code << endl;
    }
    return 0;
}