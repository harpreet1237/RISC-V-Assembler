#include <bits/stdc++.h>
using namespace std;
// GLOBAL DEFINITIONS
map<string, uint32_t> lbl_mp; // maps labels to the instruction address
uint32_t Instruction_Address = 0;
uint32_t Data_Address = 65536;

// assembler directives (data_types in .data segment)
// (and corresponding size)
vector<pair<string, int>> assembler_directives = {{".byte", 1}, {".half", 2}, {".word", 4}, {".dword", 8}, {".asciz", 0}, {".asciiz", 0}};
vector<pair<uint32_t, string>> data_mc; // address --> data_string
vector<pair<uint32_t, uint32_t>> text_mc; // address --> machine code

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
    opcode["add"] = 0x33;  // 0110011
    opcode["and"] = 0x33;  // 0110011
    opcode["or"]  = 0x33;  // 0110011
    opcode["sll"] = 0x33;  // 0110011
    opcode["slt"] = 0x33;  // 0110011
    opcode["sra"] = 0x33;  // 0110011
    opcode["srl"] = 0x33;  // 0110011
    opcode["sub"] = 0x33;  // 0110011
    opcode["xor"] = 0x33;  // 0110011
    opcode["mul"] = 0x33;  // 0110011
    opcode["div"] = 0x33;  // 0110011
    opcode["rem"] = 0x33;  // 0110011

    //I-Format
    opcode["addi"]  = 0x13;  // 0010011
    opcode["andi"]  = 0x13;  // 0010011
    opcode["ori"]   = 0x13;  // 0010011
    opcode["lb"]    = 0x3;   // 0000011
    opcode["ld"]    = 0x3;   // 0000011
    opcode["lh"]    = 0x3;   // 0000011
    opcode["lw"]    = 0x3;   // 0000011
    opcode["jalr"]  = 0x67;  // 1100111
    opcode["slli"]  = 0x13;

    //S-Format
    opcode["sb"] = 0x23;  // 0100011
    opcode["sw"] = 0x23;  // 0100011
    opcode["sd"] = 0x23;  // 0100011
    opcode["sh"] = 0x23;  // 0100011


    //SB-Format
    opcode["beq"] = 0x63;  // 1100011
    opcode["bne"] = 0x63;  // 1100011
    opcode["bge"] = 0x63;  // 1100011
    opcode["blt"] = 0x63;  // 1100011

    //U-Format
    opcode["auipc"] = 0x17;   // 0010111
    opcode["lui"]   = 0x37;   // 0110111

    //UJ-Format
    opcode["jal"] = 0x6F; // 1101111

    // Func3 for R-Format Instructions
    func3["add"] = 0;
    func3["and"] = 7;
    func3["or"]  = 6;
    func3["sll"] = 1;
    func3["slt"] = 2;
    func3["sra"] = 5;
    func3["srl"] = 5;
    func3["sub"] = 0;
    func3["xor"] = 4;
    func3["mul"] = 0;
    func3["div"] = 4;
    func3["rem"] = 6;

    // Func7 for R-Format Instructions
    func7["add"] = 0;
    func7["and"] = 0;
    func7["or"]  = 0;
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
    func3["ori"]  = 6;
    func3["lb"]   = 0;
    func3["ld"]   = 3;
    func3["lh"]   = 1;
    func3["lw"]   = 2;
    func3["jalr"] = 0;
    func3["slli"] = 1;


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

// Function to convert string representation to int64_t
int64_t parseInteger(const string& str) {
    // Check if the string starts with "0x" or "0b"
    if (str.substr(0, 2) == "0x") {
        // Convert hexadecimal string to int64_t
        return stoll(str, nullptr, 16);
    } else if (str.substr(0, 2) == "0b") {
        // Convert binary string to int64_t
        return stoll(str.substr(2), nullptr, 2);
    } else {
        // Convert decimal string to int64_t
        return stoll(str);
    }
}

// Function to extract integers from a string and return a vector<int64_t>
vector<int64_t> extractIntegers(const string& input) {
    istringstream iss(input);
    vector<int64_t> integers;

    string token;
    while (getline(iss, token, ',')) {
        // Trim leading and trailing spaces
        auto start = token.find_first_not_of(" ");
        auto end = token.find_last_not_of(" ");
        if (start != string::npos && end != string::npos) {
            token = token.substr(start, end - start + 1);
        } else {
            // Skip empty strings
            continue;
        }

        // Convert string representation to int64_t and add to vector
        integers.push_back(parseInteger(token));
    }

    return integers;
}

// Function to convert int64_t to hexadecimal string of given length (in bytes) with "0x" prefix
string int64ToHex(int64_t value, int lengthBytes) {
    // Create a stringstream to format the hexadecimal representation
    stringstream ss;
    ss << hex << uppercase << setw(lengthBytes * 2) << setfill('0') << value;

    // Extract the formatted hexadecimal string
    string hexStr = ss.str();

    // Adjust the length of the hexadecimal string if needed
    if (hexStr.length() > lengthBytes * 2) {
        hexStr = hexStr.substr(hexStr.length() - lengthBytes * 2);
    }

    // Prepend "0x" to the hexadecimal string
    hexStr = "0x" + hexStr;

    return hexStr;
}

// Function to convert uint32_t to 4-byte hexadecimal string with "0x" prefix
string uint32ToHex(uint32_t value) {
    // Create a stringstream to format the hexadecimal representation
    stringstream ss;
    ss << hex << uppercase << setw(8) << setfill('0') << value;

    // Extract the formatted hexadecimal string
    string hexStr = ss.str();

    // Prepend "0x" to the hexadecimal string
    hexStr = "0x" + hexStr;

    return hexStr;
}

bool isInteger(const string& str) {
    // Check if the string is empty or starts with a '+' or '-'
    if (str.empty() || (str[0] != '+' && str[0] != '-' && !isdigit(str[0]) && str[0] != '0'))
        return false;

    // Check if it's a hexadecimal value (starts with "0x" or "0X")
    if (str.size() >= 3 && (str.substr(0, 2) == "0x" || str.substr(0, 2) == "0X")) {
        // Check each character of the string after "0x" or "0X"
        for (size_t i = 2; i < str.size(); ++i) {
            if (!isxdigit(str[i]))
                return false;
        }
        return true; // It's a hexadecimal value
    }

    // Check if it's a binary value (starts with "0b" or "0B")
    if (str.size() >= 3 && (str.substr(0, 2) == "0b" || str.substr(0, 2) == "0B")) {
        // Check each character of the string after "0b" or "0B"
        for (size_t i = 2; i < str.size(); ++i) {
            if (str[i] != '0' && str[i] != '1')
                return false;
        }
        return true; // It's a binary value
    }

    // Check if it's a decimal integer
    for (size_t i = 1; i < str.size(); ++i) {
        if (!isdigit(str[i]))
            return false;
    }

    return true; // It's a decimal integer
}

int stringToInteger(const std::string& str) {
    // Check for negative sign
    bool isNegative = (str.size() > 0 && str[0] == '-');
    std::string valueStr = (isNegative) ? str.substr(1) : str;

    std::stringstream ss(valueStr);

    // Check for hexadecimal or binary prefix
    if (valueStr.size() > 2 && valueStr[0] == '0') {
        if (valueStr[1] == 'x' || valueStr[1] == 'X') {
            uint32_t value;
            ss >> std::hex >> value;
            return (isNegative) ? -static_cast<int>(value) : static_cast<int>(value);
        } else if (valueStr[1] == 'b' || valueStr[1] == 'B') {
            uint32_t value;
            ss >> std::setw(valueStr.size() - 2) >> std::setbase(2) >> value;
            return (isNegative) ? -static_cast<int>(value) : static_cast<int>(value);
        }
    }

    // Otherwise, assume decimal
    int value;
    ss >> value;
    return (isNegative) ? -value : value;
}

pair<string,int> getrs_offset(string offset_rs1) {      //function to obtain register source and offset 
    string offset_string;
    string rs1;
    bool isoffset = true;
    for(auto x:offset_rs1) {
        if(x == '(' || x == ')') {                      //fetching from ()
            isoffset = false;
        } else {
            if(isoffset) {
                offset_string += x;
            } else {
                rs1 += x;
            }
        }
    }

    int offset = stringToInteger(offset_string);        //using utility function 
    
    return {rs1,offset};
}

uint32_t process_Instruction(string& Inst, uint32_t& Instruction_Address);
void initialize_globals();