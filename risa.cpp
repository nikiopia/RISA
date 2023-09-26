// INCLUDE
#include <stdio.h>
#include <fstream>
#include <string>

// TYPE DEFINE
struct Machine {
    int ACC = 0;
    int B = 0;
    int PC = 0;
    bool NEGATIVE_FLAG = false;
    bool RUNNING = true;
    int MEM[0x100] = { 0 };
    int ERRORLEVEL = 0;
};

void LDA (Machine &sys, int addr) {
    if (addr < 0 || addr > 0xff) {
        sys.ERRORLEVEL = -1;
        sys.RUNNING = false;
        return;
    }
    sys.ACC = sys.MEM[addr];
    if (sys.ACC < 0) { sys.NEGATIVE_FLAG = true; } else { sys.NEGATIVE_FLAG = false; }
}

void STA (Machine &sys, int addr) {
    if (addr < 0 || addr > 0xff) {
        sys.ERRORLEVEL = -1;
        sys.RUNNING = false;
        return;
    }
    sys.MEM[addr] = sys.ACC;
}

void ADD (Machine &sys) {
    sys.ACC = sys.ACC + sys.B;
    if (sys.ACC < 0) { sys.NEGATIVE_FLAG = true; } else { sys.NEGATIVE_FLAG = false; }
}

void SUB (Machine &sys) {
    sys.ACC = sys.ACC - sys.B;
    if (sys.ACC < 0) { sys.NEGATIVE_FLAG = true; } else { sys.NEGATIVE_FLAG = false; }
}

void MBA (Machine &sys) { sys.B = sys.ACC; }

void JMP (Machine &sys, int addr) {
    if (addr < 0 || addr > 0xff) {
        sys.ERRORLEVEL = -1;
        sys.RUNNING = false;
        return;
    }
    sys.PC = addr;
}

void JN (Machine &sys, int addr) {
    if (addr < 0 || addr > 0xff) {
        sys.ERRORLEVEL = -1;
        sys.RUNNING = false;
        return;
    }
    if (sys.NEGATIVE_FLAG) { sys.PC = addr; } else { sys.PC++; }
}

void OUT (Machine &sys, int addr) {
    if (addr < 0 || addr > 0xff) {
        sys.ERRORLEVEL = -1;
        sys.RUNNING = false;
        return;
    }
    printf("MEM[0x%x] = 0x%x\n", addr, sys.MEM[addr]);
}

void HLT (Machine &sys) { sys.RUNNING = false; }

void Interpreter (Machine &sys) {
    if (sys.PC < 0 || sys.PC > 0xff) {
        sys.ERRORLEVEL = -1;
        sys.RUNNING = false;
        return;
    }
    int OPCODE = sys.MEM[sys.PC] >> 8;
    int PARAM = sys.MEM[sys.PC] - (OPCODE << 8);

    switch (OPCODE) {
        case 0:
            LDA(sys, PARAM);
            sys.PC++;
            break;
        case 1:
            STA(sys, PARAM);
            sys.PC++;
            break;
        case 2:
            ADD(sys);
            sys.PC++;
            break;
        case 3:
            SUB(sys);
            sys.PC++;
            break;
        case 4:
            MBA(sys);
            sys.PC++;
            break;
        case 5:
            JMP(sys, PARAM);
            break;
        case 6:
            JN(sys, PARAM);
            break;
        case 7:
            OUT(sys, PARAM);
            sys.PC++;
            break;
        default:
            HLT(sys);
    }
}

void importCode (Machine &sys, std::string fileName) {
    std::ifstream codeText;
    std::string line;
    codeText.open(fileName);
    if (codeText.is_open()) {
        printf("Found code file, importing...\n");
        int memIndex = 0;
        int memValue = 0;
        std::string A,B;
        while (codeText) {
            if (memIndex < 0 || memIndex > 0xff) {
                sys.ERRORLEVEL = -1;
                sys.RUNNING = false;
                break;
            }
            std::getline(codeText, line);
            if (line[0] == '?') {
                A = line.substr(1,4);
                memIndex = std::stoul(A, nullptr, 16);
            }
            if (isalpha(line[0])) {
                A = line.substr(0,3);
                memValue = 0;
                if (A == "LDA") {
                    memValue += std::stoul(line.substr(4,4), nullptr, 16);
                } else if (A == "STA") {
                    memValue += 1 << 8;
                    memValue += std::stoul(line.substr(4,4), nullptr, 16);
                } else if (A == "ADD") {
                    memValue += 2 << 8;
                } else if (A == "SUB") {
                    memValue += 3 << 8;
                } else if (A == "MBA") {
                    memValue += 4 << 8;
                } else if (A == "JMP") {
                    memValue += 5 << 8;
                    memValue += std::stoul(line.substr(4,4), nullptr, 16);
                } else if (A == "JN ") {
                    memValue += 6 << 8;
                    memValue += std::stoul(line.substr(4,4), nullptr, 16);
                } else if (A == "OUT") {
                    memValue += 7 << 8;
                    memValue += std::stoul(line.substr(4,4), nullptr, 16);
                } else {
                    memValue += 8 << 8;
                }
                sys.MEM[memIndex] = memValue;
                memIndex++;
            }
            if (isdigit(line[0]) || line[0] == '-') {
                sys.MEM[memIndex] = std::stoi(line, nullptr, 10);
                memIndex++;
            }
        }
        codeText.close();
    } else {
        printf("Cannot locate file, stopping machine...\n");
        sys.RUNNING = false;
        sys.ERRORLEVEL = -2;
    }
}

int main (int argc, char* argv[]) {
    // Machine init
    Machine sys;
    int WATCHDOG_TIMER = 1000;

    // Import code file
    if (argv[1]) {
        std::string arg1(argv[1]);
        if (arg1 == "help" or arg1 == "h") {
            printf("Instruction Set:\n");
            printf("0 - LDA addr -- load ACC with value at addr\n");
            printf("1 - STA addr -- store acc value at addr\n");
            printf("2 - ADD      -- add b to acc\n");
            printf("3 - SUB      -- subtract b from acc\n");
            printf("4 - MBA      -- move a to b\n");
            printf("5 - JMP addr -- jump to addr\n");
            printf("6 - JN  addr -- jump if negative flag to addr\n");
            printf("7 - OUT addr -- display value at addr\n");
            printf("8 - HLT      -- halt system\n");
            printf("Command line options:\n");
            printf("-h or -help : print this message\n");
            printf("[filename] [optional: initial program counter address, default 0x00]\n");
            printf("Error levels:  0 -> ok\n");
            printf("              -1 -> attempt access mem index out of range\n");
            printf("              -2 -> cannot open/read code file\n");
            printf("\nExited with code: %i\n", sys.ERRORLEVEL);
            return sys.ERRORLEVEL;
        } else {
            importCode(sys, arg1);
        }
    } else {
        sys.RUNNING = false;
    }

    // Handle optional PC definition from CMD line
    if (argv[2]) {
        std::string arg2(argv[2]);
        sys.PC = std::stoul(arg2, nullptr, 10);
    } else {
        sys.PC = 0x0;
    }

    if (sys.RUNNING) {
        printf("Starting...\n\n");
    }

    while (sys.RUNNING) {
        if (WATCHDOG_TIMER <= 0) { printf("WATCHDOG EXIT\n"); break; }
        Interpreter(sys);
        WATCHDOG_TIMER--;
    }

    printf("\nExited with code: %i\n", sys.ERRORLEVEL);
    return sys.ERRORLEVEL;
}
