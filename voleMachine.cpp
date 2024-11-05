#include "voleMachine.h"

// Register class methods
Register::Register() : registers(16, "00") {}

string Register::getCell(int address) {
    return registers[address];
}

void Register::setCell(int address, string cellContent) {
    if (cellContent.size() < 2)
        cellContent = '0' + cellContent;
    registers[address] = cellContent;
}

void Register::clear() {
    fill(registers.begin(), registers.end(), "00");
}

// Memory class methods
Memory::Memory() : memory(256, "00") {}

string Memory::getCell(int address) {
    return memory[address];
}

void Memory::setCell(int address, string cellContent) {
    if (cellContent.size() < 2)
        cellContent = '0' + cellContent;
    memory[address] = cellContent;
}

void Memory::clear() {
    fill(memory.begin(), memory.end(), "00");
}

// ALU class methods
int ALU::hexaToDecimal(string hex) {
    int res = 0;
    int base = 1;
    for (int i = hex.size() - 1; i >= 0; i--) {
        char hexDigit = hex[i];
        if (hexDigit >= '0' && hexDigit <= '9') {
            res += (hexDigit - '0') * base;
        } else if (hexDigit >= 'A' && hexDigit <= 'F') {
            res += (hexDigit - 'A' + 10) * base;
        } else if (hexDigit >= 'a' && hexDigit <= 'f') {
            res += (hexDigit - 'a' + 10) * base;
        }
        base *= 16;
    }
    return res;
}

string ALU::decimalToHex(int decimal) {
    if (decimal == 0) return "0";
    string hex;
    const char hexDigits[] = "0123456789ABCDEF";
    while (decimal > 0) {
        int remainder = decimal % 16;
        hex = hexDigits[remainder] + hex;
        decimal /= 16;
    }
    return hex;
}

string ALU::decimalToBin(int decimal) {
    if (decimal == 0)
        return "0";
    string binary;
    while (decimal > 0) {
        if(decimal%2 == 0) {
            binary += '0';
        }
        else {
            binary += '1';
            decimal--;

        }
        decimal /= 2;
    }
    reverse(binary.begin(), binary.end());

    return binary;
}

int ALU::binToDecimal(string binary) {
    int decimal = 0;
    reverse(binary.begin(), binary.end());
    for(int i = 0; i < binary.size(); i++) {
        decimal += (binary[i] - '0') * pow(2,i);
    }
    return decimal;
}

int ALU::convert2ComplementToDec(string binary){
    while (binary.size() < 8){
        binary = '0' + binary;
    }
    if (binary[0] == '1'){
        binary = decimalToBin(binToDecimal(binary) - 1);
        for (int i = 0; i < 8; ++i) {
            if (binary[i] == '1')
                binary[i] = '0';
            else
                binary[i] = '1';
        }
        return -1*binToDecimal(binary);
    }
    return binToDecimal(binary);
}

string ALU::convertDecTo2Complement(int decimal){
    string binary;
    if (decimal < 0){
        binary = decimalToBin(-1*(decimal+1));
        while (binary.size() < 8){
            binary = '0' + binary;
        }
        for (int i = 0; i < 8; ++i) {
            if (binary[i] == '1')
                binary[i] = '0';
            else
                binary[i] = '1';
        }
    }
    else {
        binary = decimalToBin(decimal);
    }
    return binary;
}

double ALU::convertBinToFloat(string binary){
    // Explicit floating-point conversion
    // (-1)^sign * 0.mantissa * 2^(exponent - 4)

    while (binary.size() < 8){
        binary = '0' + binary;
    }
    int sign = (binary[0] == '1') ? -1 : 1;
    string exponent = binary.substr(1, 3);
    string mantissa = binary.substr(4, 4);
    double mantissaFloat = 0;
    int counter = -1;
    for (int i = 0; i < 4; ++i) {
        mantissaFloat += pow(2, counter) * (mantissa[i] - '0');
        counter--;
    }
    int exponentInt = binToDecimal(exponent) - 4;
    double ans = sign * pow(2, exponentInt) * mantissaFloat;
    return ans;
}

string ALU::convertFloatToBin(double floatNum) {
    if (floatNum == 0) return "00000000";
    char sign = (floatNum < 0) ? '1' : '0';
    floatNum *= (floatNum < 0) ? -1 : 1;

    int integerPart = static_cast<int>(floatNum);
    double fractionPart = floatNum - integerPart;

    string binaryInteger = decimalToBin(integerPart);

    string binaryFraction;
    int precision = 4; // Set precision for fractional part
    while (fractionPart > 0 && precision-- > 0) {
        fractionPart *= 2;
        int fractionalBit = static_cast<int>(fractionPart);
        binaryFraction += to_string(fractionalBit);
        fractionPart -= fractionalBit;
    }

    string binaryResult = binaryInteger;
    if (!binaryFraction.empty()) {
        binaryResult += "." + binaryFraction;
    }
    int exponent = binaryInteger.size() + 4;
    string exponentBin = decimalToBin(exponent);
    string mantissa;
    for (char i : binaryResult) {
        if (i == '.')
            continue;
        mantissa += i;
        if (mantissa.size() == 4)
            break;
    }
    while (mantissa.size() < 4){
        mantissa = '0' + mantissa;
    }
    string result = sign + exponentBin + mantissa;
    return result;
}

string ALU::loadZero(string &s1, string &s2) {
    reverse(s2.begin(), s2.end());
    while(s2<s1) {
        s2.push_back('0');
    }
    return  s2;
}

void ALU::OR(int idxReg1, int idxReg2, int idxReg3, Register& reg) {
    int n1 = hexaToDecimal(reg.getCell(idxReg1));
    int n2 = hexaToDecimal(reg.getCell(idxReg2));

    string num1 = decimalToBin(n1);
    string num2 = decimalToBin(n2);
    string result;

    if (num1.size() > num2.size()) {
        num2 = loadZero(num1, num2);
    } else if (num2.size() > num1.size()) {
        num1 = loadZero(num2, num1);
    }

    for (int i = 0; i < num1.size(); i++) {
        if (num1[i] == '0' || num2[i] == '0') {
            result += '0';
        } else {
            result += '1';
        }
    }

    int decimalResult = binToDecimal(result);
    reg.setCell(idxReg3, decimalToHex(decimalResult));
}

void ALU::AND(int idxReg1, int idxReg2, int idxReg3, Register& reg) {
    int n1=hexaToDecimal(reg.getCell(idxReg1));
    int n2=hexaToDecimal(reg.getCell(idxReg2));

    string num1 = decimalToBin(n1);
    string num2 = decimalToBin(n2);
    string result;

    if (num1.size() > num2.size()) {
        num2 = loadZero(num1, num2);
    } else if (num2.size() > num1.size()) {
        num1 = loadZero(num2, num1);
    }

    for (int i = 0; i < num1.size(); i++) {
        if (num1[i] == '1' || num2[i] == '1') {
            result += '1';
        } else {
            result += '0';
        }
    }

    int decimalResult = binToDecimal(result);
    reg.setCell(idxReg3, decimalToHex(decimalResult));
}

void ALU::XOR(int idxReg1, int idxReg2, int idxReg3, Register& reg) {
    int n1=hexaToDecimal(reg.getCell(idxReg1));
    int n2=hexaToDecimal(reg.getCell(idxReg2));

    string num1 = decimalToBin(n1);
    string num2 = decimalToBin(n2);
    string result;

    if (num1.size() > num2.size()) {
        num2 = loadZero(num1, num2);
    } else if (num2.size() > num1.size()) {
        num1 = loadZero(num2, num1);
    }

    for (int i = 0; i < num1.size(); i++) {
        if (num1[i] == num2[i] ) {
            result += '0';
        } else {
            result += '1';
        }
    }

    int decimalResult = binToDecimal(result);
    reg.setCell(idxReg3, decimalToHex(decimalResult));
}


// CPU class methods
CPU::CPU() : programCounter(10) {}

bool CPU::Fetch(Memory& memory) {
    instructionRegister = memory.getCell(programCounter++);
    instructionRegister +=memory.getCell(programCounter++);
    cout<<"instruction "<<  instructionRegister<<" I " << programCounter<<endl;
    if (instructionRegister == "C000") {
        return true;
    }

}


int CPU::DecodeAndExecute(Memory memory,const string& instruction) {
    char opCode = instruction[0];
    int idxReg1, idxReg2, idxReg3, idxMem;

    if (opCode == '1') {
        string Reg1(1, instruction[1]);
        idxReg1 = alu.hexaToDecimal(Reg1) - '0';
        string hexMemAdd = instruction.substr(2, 2);
        idxMem = alu.hexaToDecimal(hexMemAdd);
        cu.load(idxReg1, idxMem, registers, mem);
    }
    else if (opCode == '2') {
        string Reg1(1, instruction[1]);
        idxReg1 = alu.hexaToDecimal(Reg1) - '0';
        string val = instruction.substr(2, 2);
        cu.load(idxReg1, val, registers);
    }
    else if (opCode == '3') {
        string Reg1(1, instruction[1]);
        idxReg1 = alu.hexaToDecimal(Reg1) - '0';
        if (instruction[2] == '0' && instruction[3] == '0') {
            string x = registers.getCell(idxReg1);
            mem.setCell(0, x);
            cout << x << endl;
        } else {
            string hexMemAdd = instruction.substr(2, 2);
            idxMem = alu.hexaToDecimal(hexMemAdd);
            cu.store(idxReg1, idxMem, registers, mem);
        }
    }
    else if (opCode == '4') {
        if (instruction[1] == '0') {
            string Reg1(1, instruction[2]);
            string Reg2(1, instruction[3]);
            idxReg1 = alu.hexaToDecimal(Reg1) - '0';
            idxReg2 = alu.hexaToDecimal(Reg2) - '0';
            cu.move(idxReg1, idxReg2, registers);
        }
    }
    else if (opCode == '5') {
        string Reg1(1, instruction[1]);
        string Reg2(1, instruction[2]);
        string Reg3(1, instruction[3]);
        idxReg1 = alu.hexaToDecimal(Reg1) - '0';
        idxReg2 = alu.hexaToDecimal(Reg2) - '0';
        idxReg3 = alu.hexaToDecimal(Reg3) - '0';

        string binary1 = alu.decimalToBin(alu.hexaToDecimal(registers.getCell(idxReg2)));
        string binary2 = alu.decimalToBin(alu.hexaToDecimal(registers.getCell(idxReg3)));

        int decimal1 = alu.convert2ComplementToDec(binary1);
        int decimal2 = alu.convert2ComplementToDec(binary2);

        string result = alu.convertDecTo2Complement(decimal1+decimal2);

        while (result.size() < 8){
            result = '0' + result;
        }
        result = alu.decimalToHex(alu.binToDecimal(result));

        registers.setCell(idxReg1, result);
    }
    else if (opCode == '6') {
        string Reg1(1, instruction[1]);
        string Reg2(1, instruction[2]);
        string Reg3(1, instruction[3]);
        idxReg1 = alu.hexaToDecimal(Reg1) - '0';
        idxReg2 = alu.hexaToDecimal(Reg2) - '0';
        idxReg3 = alu.hexaToDecimal(Reg3) - '0';

        string binary1 = alu.decimalToBin(alu.hexaToDecimal(registers.getCell(idxReg2)));
        string binary2 = alu.decimalToBin(alu.hexaToDecimal(registers.getCell(idxReg3)));

        double decimal1 = alu.convertBinToFloat(binary1);
        double decimal2 = alu.convertBinToFloat(binary2);

        string result = alu.decimalToBin(alu.binToDecimal(alu.convertFloatToBin(decimal1 + decimal2)));

        registers.setCell(idxReg1, alu.decimalToHex(alu.binToDecimal(result)));

    }
    else if (opCode == '7') {
        string Reg1(1, instruction[2]);
        string Reg2(1, instruction[3]);
        string Reg3(1, instruction[1]);
        idxReg1 = alu.hexaToDecimal(Reg1) - '0';
        idxReg2 = alu.hexaToDecimal(Reg2) - '0';
        idxReg3 = alu.hexaToDecimal(Reg3) - '0';

        alu.OR(idxReg1, idxReg2, idxReg3,registers);

    }
    else if (opCode == '8') {
        string Reg1(1, instruction[2]);
        string Reg2(1, instruction[3]);
        string Reg3(1, instruction[1]);
        idxReg1 = alu.hexaToDecimal(Reg1) - '0';
        idxReg2 = alu.hexaToDecimal(Reg2) - '0';
        idxReg3 = alu.hexaToDecimal(Reg3) - '0';

        alu.AND(idxReg1, idxReg2, idxReg3,registers);
    }
    else if (opCode == '9') {
        string Reg1(1, instruction[2]);
        string Reg2(1, instruction[3]);
        string Reg3(1, instruction[1]);
        idxReg1 = alu.hexaToDecimal(Reg1) - '0';
        idxReg2 = alu.hexaToDecimal(Reg2) - '0';
        idxReg3 = alu.hexaToDecimal(Reg3) - '0';

        alu.XOR(idxReg1, idxReg2, idxReg3,registers);
    }
    else if (opCode == 'A') {
        string Reg2(1, instruction[3]);
        string Reg3(1, instruction[1]);
        idxReg2 = alu.hexaToDecimal(Reg2) - '0';
        idxReg3 = alu.hexaToDecimal(Reg3) - '0';

        cu.Rotate(idxReg3, idxReg2 ,alu,registers);
    }
    else if (opCode == 'B') {
        string Reg1(1, instruction[1]);
        idxReg1 = alu.hexaToDecimal(Reg1) - '0';
        string hexMemAdd = instruction.substr(2, 2);
        idxMem = alu.hexaToDecimal(hexMemAdd);
        cu.jump(idxReg1, idxMem, registers, programCounter);
    }
    else if (opCode == 'C') {
        if (instruction[1] == '0' && instruction[2] == '0' && instruction[3] == '0') {
            cu.halt();
        }
    }
    else if (opCode == 'D'){
        string Reg1(1, instruction[1]);
        idxReg1 = alu.hexaToDecimal(Reg1) - '0';
        string hexMemAdd = instruction.substr(2, 2);
        idxMem = alu.hexaToDecimal(hexMemAdd);

        string binary1 = alu.decimalToBin(alu.hexaToDecimal(registers.getCell(idxReg1)));
        string binary2 = alu.decimalToBin(alu.hexaToDecimal(registers.getCell(0)));
        int num1 = alu.convert2ComplementToDec(binary1);
        int num2 = alu.convert2ComplementToDec(binary2);

        if (num1 > num2){
            cu.jump(idxReg1, idxMem, registers, programCounter);
        }
    }

    return programCounter;
}


// CU class methods
void CU::load(int idxReg, int MemAddr, Register& reg, Memory& mem) {
    reg.setCell(idxReg, mem.getCell(MemAddr));
}

void CU::load(int idxReg, string val, Register& reg) {
    reg.setCell(idxReg, val);
}

void CU::store(int idxReg, int idxMem, Register& reg, Memory& mem) {
    mem.setCell(idxMem, reg.getCell(idxReg));
}

void CU::move(int idxReg1, int idxReg2, Register& reg) {
    reg.setCell(idxReg2, reg.getCell(idxReg1));
}

void CU::jump(int idxReg, int MemAddr, Register& reg, int& PC) {
    if (reg.getCell(idxReg) == reg.getCell(0)) {
        PC = MemAddr;
    }
}

void CU::Rotate(int idxReg, int step, ALU& alu, Register& reg) {
    int n = alu.hexaToDecimal(reg.getCell(idxReg));

    string num = alu.decimalToBin(n);

    while (num.size() < 8) {
        num = '0' + num;
    }

    step = step % num.size();

    string Sub = num.substr(num.size() - step, step);
    string result = Sub + num.substr(0, num.size() - step);

    int content = alu.binToDecimal(result);
    string val=alu.decimalToHex(content);
    if(val.size()==1) {
        val = '0' + val;
    }
    reg.setCell(idxReg, val);
}

void CU::halt() {
    cout << "Program halted." << endl;
}

Register& CPU::getRegister() {
    return registers;
}

ALU& CPU::getALU() {
    return alu;
}

// Machine class methods
void Machine::loadProgramFile(string& filename) {
    ifstream inputFile(filename);
    while (!inputFile.is_open()) {
        cout << "Error: Could not open file " << filename << endl;

    }

    string line;
    int address = 10;
    while (inputFile >> line) {
        if (line.size() > 4){
            continue;
        }
        counter++;
        getMemory().setCell(address, line.substr(0, 2));
        address++;
        getMemory().setCell(address, line.substr(2, 2));
        address++; // Increment address for the next instruction
    }
    if (getMemory().getCell(address - 2) != "C0"){
        getMemory().setCell(address, "C0");
        address++;
        getMemory().setCell(address, "00");
        address++;
    }
    inputFile.close();
}

void Machine::outputState() {
    for (int i = 0; i < 16; i++) {
        cout << "Register #" << getProcessor().getALU().decimalToHex(i) << ": " << getProcessor().getRegister().getCell(i) << endl;
    }
    cout << "=============================================\n";
    cout << "Memory content\n";
    cout << "    ";
    for (int i = 0; i < 16; ++i) {
        cout << " " << getProcessor().getALU().decimalToHex(i) << "  ";
    }
    cout << endl;
    for (int i = 0; i < 16; ++i) {
        cout << getProcessor().getALU().decimalToHex(i) << " | ";
        for (int j = 0; j < 16; ++j) {
            cout << " " << getMemory().getCell(i*16+j) << " ";
        }
        cout << endl;
    }
}

CPU& Machine::getProcessor() {
    return processor;
}

Memory& Machine::getMemory() {
    return memory;
}



void Machine::run() {
    CPU cpu;

         while (true) {


            if(cpu.Fetch(getMemory())==true){
                 cpu.programCounter = cpu.DecodeAndExecute(getMemory(),"C000");
                break;
            }
            //cpu.programCounter = cpu.DecodeAndExecute(getMemory(),cpu.Fetch(getMemory()));


        }

}
