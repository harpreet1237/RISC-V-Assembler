#include<bits/stdc++.h>
#include "global.h"
using namespace std;

bool ifopened = false;

void print(vector<string>&tokens){
    
    for(auto token:tokens) {
        cerr << token << " ";
    }
    cerr << endl;
}


uint32_t decodeRFormat(vector<string>&tokens) {
    string instruction_name = tokens[0];
    string rd = tokens[1];
    string rs1 = tokens[2];
    string rs2 = tokens[3];         // fetching registers and instruction_name 

    uint32_t machine_code = 0;
    machine_code += func7[instruction_name];    //      adding func7
    machine_code <<= 5;                          //      5 bit shifted left for rs2
    machine_code += registerfile[rs2];          //      adding rs2
    machine_code <<= 5;                          //      5 bit shifted left for rs1
    machine_code += registerfile[rs1];          //      adding rs1
    machine_code <<= 3;                          //      3 bit shifted left for func3
    machine_code += func3[instruction_name];    //      adding func3
    machine_code <<= 5;                          //      5 bit shifted left for rd
    machine_code += registerfile[rd];           //      adding rd
    machine_code <<= 7;                          //      7 bit shifted left for opcode 
    machine_code += opcode[instruction_name];   //      adding opcode

    return machine_code;
}

uint32_t decodeIFormat(vector<string>&tokens,uint32_t Instruction_Address) {
    string instruction_name = tokens[0];        //instruction name
    string rd = tokens[1];                      //fetching rd
    string rs1;
    int offset = 0;

    if(offset_ready[instruction_name] == "READY") {
        rs1 = tokens[2];                        //fetching rs1 
        string offset_string = tokens[3];
        offset = stringToInteger(offset_string);
        if(offset > OFFSET_LIMIT || offset < -(OFFSET_LIMIT+1)) {
            return -1;
        }
        offset = (offset & 0xFFF);              // offset set to 12 bit 2s-compliment

    } else if(offset_ready[instruction_name] == "FETCH") {
        pair<string,int> rs1_offset = getrs_offset(tokens[2]);    // offset(rs1) -> fetched to pair<rs1,offset>
        rs1 = rs1_offset.first;
        offset = rs1_offset.second;
        offset = (offset & 0xFFF);              //12 bit 2s-compliment 

    } else if(instruction_name == "jalr") {
        rs1 = tokens[2];
        if(isInteger(tokens[3])) {
            offset = stringToInteger(tokens[3]);
            offset = (offset & 0xFFF);
        } else {
            if(lbl_mp.count(tokens[3])) {       //replacing label with offset from current Instruction address
                offset = lbl_mp[tokens[3]] - Instruction_Address;
            } else {
                return -1;
            }
        }

    } else {
        return -1;
    }

    uint32_t machine_code = 0;
    machine_code += offset;                 //12 bit offset added
    machine_code <<= 5;                      //5 bit left shifted for register rs1
    machine_code += registerfile[rs1];      //register address stored
    machine_code <<= 3;                      //3 bit left shifted for func3
    machine_code += func3[instruction_name];//func3 added
    machine_code <<= 5;                      //5 bit left shifted for register
    machine_code += registerfile[rd];       //register added
    machine_code <<= 7;                      //7 bit left shifted for opcode
    machine_code += opcode[instruction_name];//opcode added

    return machine_code;
}

uint32_t decodeSFormat(vector<string>&tokens) {
    string instruction_name = tokens[0];
    string rs2 = tokens[1];
    string rs1;
    int offset = 0;

    pair<string,int> rs1_offset = getrs_offset(tokens[2]);
    rs1 = rs1_offset.first;
    offset = rs1_offset.second;

    if(offset > OFFSET_LIMIT || offset < -(OFFSET_LIMIT+1)) {
        return -1;
    }

    offset = (offset & 0xFFF);
    uint32_t imm1 = ((offset & 0xFE0) >> 5);
    uint32_t imm2 = (offset & 0x1F);

    uint32_t machine_code = 0;

    machine_code += imm1;
    machine_code <<= 5;
    machine_code += registerfile[rs2];
    machine_code <<= 5;
    machine_code += registerfile[rs1];
    machine_code <<= 3;
    machine_code += func3[instruction_name];
    machine_code <<= 5;
    machine_code += imm2;
    machine_code <<= 7;
    machine_code += opcode[instruction_name];

    return machine_code;

}

uint32_t decodeSBFormat(vector<string>&tokens,uint32_t Instruction_Address) {

    string instruction_name = tokens[0];
    string rs1 = tokens[1];
    string rs2 = tokens[2];
    int offset = 0;

    if(isInteger(tokens[3])) {
            offset = stringToInteger(tokens[3]);

            if(offset > OFFSET_LIMIT || offset < -(OFFSET_LIMIT+1)) {
                return -1;
            }

            offset = (offset & 0xFFF);
    } else {
        if(lbl_mp.count(tokens[3])) {       //replacing label with offset from current Instruction address
            offset = lbl_mp[tokens[3]] - Instruction_Address;

            if(offset > OFFSET_LIMIT || offset < -(OFFSET_LIMIT+1)) {
                return -1;
            }
        } else {
            return -1;
        }
    }

    
    uint32_t machine_code = 0;

    machine_code += ((offset & 0x1000) >> 12);       //12th bit 
    machine_code <<= 6;
    machine_code += ((offset & 0x7E0) >> 5);         //5-10th bit
    machine_code <<= 5;
    machine_code += registerfile[rs2];               //rs2 added
    machine_code <<= 5;
    machine_code += registerfile[rs1];               //rs1 added 
    machine_code <<= 3;
    machine_code += func3[instruction_name];         //func3 added
    machine_code <<= 4;
    machine_code += ((offset & 0x1E) >> 1);          //1-4thbit added
    machine_code <<= 1;
    machine_code += ((offset & 0x800) >> 11);        //11th bit
    machine_code <<= 7;
    machine_code += opcode[instruction_name];        //opcode

    return machine_code;
}

uint32_t decodeUFormat(vector<string>&tokens) {
    string instruction_name = tokens[0];
    string rd = tokens[1];
    int offset = stringToInteger(tokens[2]);

    if(offset > OFFSET_UPPER_LIMIT || offset < -(OFFSET_UPPER_LIMIT+1)) {
        return -1;
    }
    uint32_t machine_code = 0;

    machine_code += (offset & 0xFFFFF);         //20-bit added
    machine_code <<= 5;
    machine_code += registerfile[rd];           //rd added
    machine_code <<= 7;
    machine_code += opcode[instruction_name];   //opcode added

    return machine_code;

}

uint32_t decodeUJFormat(vector<string>&tokens) {

    string instruction_name = tokens[0];
    string rd = tokens[1];
    int offset = 0;

    if(lbl_mp.count(tokens[2])) {       //replacing label with offset from current Instruction address
        offset = lbl_mp[tokens[2]] - Instruction_Address;
        offset = (offset & 0xFFFFF);
    } else {
        if(isInteger(tokens[2])) {
            offset = stringToInteger(tokens[2]);
            if(offset > OFFSET_UPPER_LIMIT || offset < -(OFFSET_UPPER_LIMIT+1)) {
                return -1;
            }
            offset = (offset & 0xFFFFF);
            
        } else {
            return -1;
        }
    }


    uint32_t machine_code = 0;
    machine_code += ((offset & 0x80000) >> 19);         //taking 20th bit 
    machine_code <<= 10;
    machine_code += ((offset & 0x7FE) >> 1);            //taking 1-10th bit
    machine_code <<= 1;
    machine_code += ((offset & 0x800) >> 11);           //taking 11th bit
    machine_code <<= 8;
    machine_code += ((offset & 0xFF000) >> 12);         //taking 12-19th bit
    machine_code <<= 5;
    machine_code += registerfile[rd];                   //taking rd register
    machine_code <<= 7;
    machine_code += opcode[instruction_name];           //adding opcode

    return machine_code;

}

uint32_t process_Instruction(string& Inst, uint32_t& Instruction_Address) {
    // code to process the instruction goes here
    
    uint32_t machine_code;                          // to be sent to callee

    vector<string>tokens = process_token(Inst);
    string operation_token = tokens[0];         // first word specifying Instruction (add,sub,and,etc)

    if(format_code[operation_token] == "R"){
        machine_code = decodeRFormat(tokens);
    } else if (format_code[operation_token] == "I") {
        machine_code = decodeIFormat(tokens,Instruction_Address);
    } else if (format_code[operation_token] == "S") {
        machine_code = decodeSFormat(tokens);
    } else if(format_code[operation_token] == "SB") {
        machine_code = decodeSBFormat(tokens,Instruction_Address);
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
