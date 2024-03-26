# CS204 - Computer Architecture - Mini-Project - RISC-V Assembler 

### Instructions :
1. Please follow the RISC-V instruction guidelines to write Assembly code for input file for this assembler.
2. Do not use space in place of comma (may give incorrect results)
3. To run the file:
 - Compile the C++ program using the command `g++ main.cpp -o main`.
 - Run the executive program using the command `./main <input.asm> <output.mc>` and see the results in output.mc file.
 - Error will be visible in terminal with immediate termination of program with exit code 1 and an error message.

### Instructions (Trace Reader Utility) :
cd into the project directory and use the following instructions for trace_reader functionality.
1. Use the following command to compile the C++ program: `g++ trace_reader.cpp -o trace_reader`
2. Run the executable program using the command `./trace_reader <input_file> <output_directory> <Branch_Predictor_Code>`
3. `<input_file>` contains the input trace in the appropriate format.
4. The results (including Branch Prediction Accuracy and Branch Target Buffer State) are stored in `<output_directory>`.
5. `<Branch_Predictor_Code>` indicates which Branch Prediction Algorithm to use
    - `1` : Always Taken
    - `2` : Always Not Taken
    - `3` : 1-bit Branch Predictor
    - `4` : 2-bit Branch Predictor

Sample Input: `./trace_reader RISCV_Traces_Lab/Fac_test_Lab Output 3`

#### Contributors
- [@harpreet1237](https://github.com/harpreet1237)
- [@aman-saini-10](https://github.com/aman-saini-10)
- [@BhavjotSingh44](https://github.com/BhavjotSingh44)        

