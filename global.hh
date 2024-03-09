#include <bits/stdc++.h>
using namespace std;
// GLOBAL DEFINITIONS
map<string, uint32_t> lbl_mp; // maps labels to the instruction address
uint32_t Instruction_Address = 0;


map<string,uint32_t>opcode;
map<string,string>format_code;


vector<string> R_Format = {"add", "and", "or", "sll", "slt", "sra", "srl", "sub", "xor", "mul", "div", "rem"};
vector<string> I_Format = {"addi", "andi", "ori", "lb", "ld", "lh", "lw", "jalr","slli","srai"};
vector<string> S_Format = {"sb", "sw", "sd", "sh"};
vector<string> SB_Format = {"beq", "bne", "bge", "blt"};
vector<string> U_Format = {"auipc", "lui"};
vector<string> UJ_Format = {"jal"};

string trim(const string& str) { // Function to trim the leading and trailing whitespaces in string
    size_t start = str.find_first_not_of(" \t");
    if (start == string::npos) {
        return ""; // Entire string is whitespace
    }

    size_t end = str.find_last_not_of(" \t");
    return str.substr(start, end - start + 1);
}

vector<string> splitString(const string& input, char delimiter) { // split a string using a delimiter
    vector<string> tokens;
    istringstream stream(input);
    string token;

    while (getline(stream, token, delimiter)) {
        tokens.push_back(trim(token));
    }

    return tokens;
}

vector<string> splitByWhitespace(const string& input) { // Split a string by whitespaces.
    vector<string> tokens;
    stringstream ss(input);
    string token;

    while (ss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

string concatenateWithWhitespace(const vector<string>& vec) {
    string result;

    // Iterate through each element in the vector
    for (const auto& str : vec) {
        // Append the element to the result string
        result += str;
        // Append a whitespace separator if it's not the last element
        if (&str != &vec.back()) {
            result += " ";
        }
    }
    return result;
}

int countOccurrences(const std::string& str, char ch) { // Count the number of occurences of a char in a string
    int count = 0;
    // Iterate through each character in the string
    for (char c : str) {
        // If the character matches the desired character, increment the count
        if (c == ch) {
            count++;
        }
    }
    return count;
}

uint32_t process_Instruction(string& Inst, uint32_t& Instruction_Address);
void initialize_globals();