#include <bits/stdc++.h>
using namespace std;
// GLOBAL DEFINITIONS
map<string, uint32_t> lbl_mp; // maps labels to the instruction address
uint32_t Instruction_Address = 0;


map<string,uint32_t>opcode;
map<string,string>format_code;
map<string, uint32_t> func3;
map<string, uint32_t> func7;
map<string, uint32_t> registerfile;
map<string,string>offset_ready;

vector<string> R_Format = {"add", "and", "or", "sll", "slt", "sra", "srl", "sub", "xor", "mul", "div", "rem"};
vector<string> I_Format = {"addi", "andi", "ori", "lb", "ld", "lh", "lw", "jalr","slli","srai"};
vector<string> S_Format = {"sb", "sw", "sd", "sh"};
vector<string> SB_Format = {"beq", "bne", "bge", "blt"};
vector<string> U_Format = {"auipc", "lui"};
vector<string> UJ_Format = {"jal"};

vector<string> I_offsetreadyFormat = {"orri","addi","andi","slli","srai","srli","ori","jalr"};
vector<string> I_offsetfetchFormat = {"lw","lh","ld","lb"};


void initialize_globals() {
    
    for(auto operation_code:R_Format) {
        format_code[operation_code] = "R";
    }

    for(auto operation_code:S_Format) {
        format_code[operation_code] = "S";
    }

    for(auto operation_code:I_Format) {
        format_code[operation_code] = "I";
    }

    for(auto operation_code:SB_Format) {
        format_code[operation_code] = "SB";
    }

    for(auto operation_code:U_Format) {
        format_code[operation_code] = "U";
    }

    for(auto operation_code:UJ_Format) {
        format_code[operation_code] = "UJ";
    }

    //initialize format_offset

    for(auto operation_code:I_offsetfetchFormat) {
        offset_ready[operation_code] = "FETCH";
    }

    for(auto operation_code:I_offsetreadyFormat) {
        offset_ready[operation_code] = "READY";
    }
    // initialize opcodes for respective formats
    //R-Format
    opcode["add"] = 51;  // 0110011
    opcode["and"] = 51;  // 0110011
    opcode["or"] = 51;   // 0110011
    opcode["sll"] = 51;  // 0110011
    opcode["slt"] = 51;  // 0110011
    opcode["sra"] = 51;  // 0110011
    opcode["srl"] = 51;  // 0110011
    opcode["sub"] = 51;  // 0110011
    opcode["xor"] = 51;  // 0110011
    opcode["mul"] = 51;  // 0110011
    opcode["div"] = 51;  // 0110011
    opcode["rem"] = 51;  // 0110011

    //I-Format
    opcode["addi"] = 19;  // 0010011
    opcode["andi"] = 19;  // 0010011
    opcode["ori"] = 19;   // 0010011
    opcode["lb"] = 3;     // 0000011
    opcode["ld"] = 3;     // 0000011
    opcode["lh"] = 3;     // 0000011
    opcode["lw"] = 3;     // 0000011
    opcode["jalr"] = 103; // 1100111

    //S-Format
    opcode["sb"] = 23;  // 0100011
    opcode["sw"] = 27;  // 0100011
    opcode["sd"] = 31;  // 0100011
    opcode["sh"] = 25;  // 0100011


    //SB-Format
    opcode["beq"] = 99;  // 1100011
    opcode["bne"] = 99;  // 1100011
    opcode["bge"] = 99;  // 1100011
    opcode["blt"] = 99;  // 1100011

    //U-Format
    opcode["auipc"] = 23; // 0010111
    opcode["lui"] = 55;   // 0110111

    //UJ-Format
    opcode["jal"] = 111; // 1101111

    // Func3 for R-Format Instructions
    func3["add"] = 0;
    func3["and"] = 7;
    func3["or"] = 6;
    func3["sll"] = 1;
    func3["slt"] = 2;
    func3["sra"] = 5;
    func3["srl"] = 5;
    func3["sub"] = 0;
    func3["xor"] = 4;
    func3["mul"] = 1;
    func3["div"] = 4;
    func3["rem"] = 6;

    // Func7 for R-Format Instructions
    func7["add"] = 0;
    func7["and"] = 0;
    func7["or"] = 0;
    func7["sll"] = 0;
    func7["slt"] = 0;
    func7["sra"] = 32;
    func7["srl"] = 0;
    func7["sub"] = 32;
    func7["xor"] = 0;
    func7["mul"] = 1;
    func7["div"] = 1;
    func7["rem"] = 1;

    //Func3 for I-Format Instructions
    func3["addi"] = 0;
    func3["andi"] = 7;
    func3["ori"] = 6;
    func3["lb"] = 0;
    func3["ld"] = 3;
    func3["lh"] = 1;
    func3["lw"] = 2;
    func3["jalr"] = 0;

    //Func3 for S-Format Instructions
    func3["sb"] = 0;
    func3["sw"] = 2;
    func3["sd"] = 3;
    func3["sh"] = 1;

    //Func3 for SB-Format Instructions
    func3["beq"] = 0;
    func3["bne"] = 1;
    func3["bge"] = 5;
    func3["blt"] = 4;

    //Initialization of Register file address

    for (int i = 0; i < 32; i++) {
        string reg = "x" + to_string(i);
        registerfile[reg] = i; // Assigning the address directly as of the index
    }

    //if t0,t1 used
    for (int i = 0; i < 7; i++) {
        string reg = "t" + to_string(i);
        if(i >=3) {
            registerfile[reg] = i + 25;
        } else {
            registerfile[reg] = i + 5; 
        }
    }

    // if a0 - a7 are used
    for (int i = 0; i < 8; i++) {
        string reg = "a" + to_string(i);
        registerfile[reg] = i + 10; // Example address for a0 to a7
    }

    // s0 to s11 registers
    registerfile["s0"] = 8;
    registerfile["s1"] = 9;
    for (int i = 2; i < 12; i++) {
        string reg = "s" + to_string(i);
        registerfile[reg] = i + 16; // Example address for s0 to s11
    }

    //special register names
    registerfile["ra"] = registerfile["x1"];
    registerfile["sp"] = registerfile["x2"];
    registerfile["gp"] = registerfile["x3"];
    registerfile["tp"] = registerfile["x4"];
    registerfile["zero"] = registerfile["x0"];

    
}




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

vector<string>process_token(string &Inst) {
  vector<string>tokens_comma = splitString(Inst,',');
  vector<string> tokens;
  vector<string> tokens1 = splitByWhitespace(tokens_comma[0]);
  tokens.push_back(tokens1[0]);
  tokens.push_back(tokens1[1]);
  tokens_comma.erase(tokens_comma.begin());

  for(auto token:tokens_comma) {
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

int countOccurrences(const string& str, char ch) { // Count the number of occurences of a char in a string
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

void trimStrings(vector<string>&tokens) {       //utility function to trim vector of strings 
    for(int i=0;i<tokens.size();i++) {
        tokens[i] = trim(tokens[i]);
    }
}

pair<string,int> getrs1(string offset_rs1) {
    string offset_string;
    string rs1;
    bool isoffset = true;
    for(auto x:offset_rs1) {
        if(x == '(' || x == ')') {
            isoffset = false;
        } else {
            if(isoffset) {
                offset_string += x;
            } else {
                rs1 += x;
            }
        }
    }

    int offset = stoi(offset_string);
    
    return {rs1,offset};
}

uint32_t process_Instruction(string& Inst, uint32_t& Instruction_Address);
void initialize_globals();