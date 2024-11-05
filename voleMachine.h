#ifndef OOPTASK4_VOLEMACHINE_H
#define OOPTASK4_VOLEMACHINE_H

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <fstream>
#include <cmath>

using namespace std;

class Register {
private:
    vector<string> registers;
public:
    Register();
    string getCell(int address);
    void setCell(int address, string cellContent);
    void clear();
};

class Memory {
private:
    vector<string> memory;
public:
    Memory();
    string getCell(int address);
    void setCell(int address, string cellContent);
    void clear();
};

class ALU {
public:
    int hexaToDecimal(string hex);
    string decimalToHex(int decimal);
    string decimalToBin(int decimal);
    int binToDecimal(string binary);
    int convert2ComplementToDec(string binary);
    string convertDecTo2Complement(int decimal);
    double convertBinToFloat(string binary);
    string convertFloatToBin(double floatNum);
    string loadZero(string &s1, string &s2);
    void OR(int idxReg1, int idxReg2, int idxReg3, Register& reg);
    void AND(int idxReg1, int idxReg2, int idxReg3, Register& reg);
    void XOR(int idxReg1, int idxReg2, int idxReg3, Register& reg);
};

class CU {
public:
    void load(int idxReg, int MemAddr, Register& reg, Memory& mem);
    void load(int idxReg, string val, Register& reg);
    void store(int idxReg, int idxMem, Register& reg, Memory& mem);
    void move(int idxReg1, int idxReg2, Register& reg);
    void jump(int idxReg, int MemAddr, Register& reg, int& PC);
    void Rotate(int idxReg, int step, ALU& alu, Register& reg);
    void halt();
};

class CPU {
private:
    ALU alu;
    CU cu;
    Register registers;
    Memory mem;
    string instructionRegister;

public:
    int programCounter;

    CPU();
    bool Fetch(Memory& memory);
    int DecodeAndExecute( Memory memory,const string& instruction);
    Register& getRegister();
    ALU& getALU();
};

class Machine {
private:
    CPU processor;
    Memory memory;
    int counter = 0;
public:
    void loadProgramFile(string& filename);
    void outputState();
    CPU& getProcessor();
    Memory& getMemory();

    void run();

};




#endif //OOPTASK4_VOLEMACHINE_H
