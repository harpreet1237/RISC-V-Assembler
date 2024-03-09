#include<bits/stdc++.h>
#include "global.hh"
using namespace std;

bool ifopened = false;

void print(vector<string>&tokens){
    if(!ifopened){
        freopen("output.mc", "w", stdout);
        ifopened = true;    
    }
    for(auto token:tokens) {
        cout << token << " ";
    }
    cout << endl;
    cout << format_code[tokens[0]] << endl;
    cout << format_code.size() << endl;
}


uint32_t decodeRFormat(vector<string>&tokens) {
    
}

uint32_t decodeIFormat(vector<string>&tokens) {
    
}

uint32_t decodeSFormat(vector<string>&tokens) {
    
}

uint32_t decodeSBFormat(vector<string>&tokens) {
    
}

uint32_t decodeUFormat(vector<string>&tokens) {
    
}

uint32_t decodeUJFormat(vector<string>&tokens) {
    
}

uint32_t process_Instruction(string& Inst, uint32_t& Instruction_Address) {
    // code to process the instruction goes here
    
    uint32_t machine_code;                          // to be sent to callee

    vector<string>tokens = process_token(Inst);
    print(tokens);

    string operation_token = tokens[0];         // first word specifying Instruction (add,sub,and,etc)

    if(format_code[operation_token] == "R"){
        machine_code = decodeRFormat(tokens);
    } else if (format_code[operation_token] == "I") {
        machine_code = decodeIFormat(tokens);
    } else if (format_code[operation_token] == "S") {
        machine_code = decodeSFormat(tokens);
    } else if(format_code[operation_token] == "SB") {
        machine_code = decodeSBFormat(tokens);
    } else if(format_code[operation_token] == "U") {
        machine_code = decodeUFormat(tokens);
    } else if (format_code[operation_token] == "UJ") {
        machine_code = decodeUJFormat(tokens);
    } else {
        machine_code = -1;
    }

    // Return Machine Code
    return machine_code;
}