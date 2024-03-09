#include<bits/stdc++.h>
#include "global.hh"
using namespace std;


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

void trimStrings(vector<string>&tokens) {       //utility function to trim vector of strings 
    for(int i=0;i<tokens.size();i++) {
        tokens[i] = trim(tokens[i]);
    }
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