#pragma once
#include <climits>


const int reg_num = 15; // regs number excluding counter
const int ram_num = 256;

class VM {

private:
   
    int regs[reg_num];
    int RAM[ram_num];
    int counter;

    const unsigned char operation_mask = 7;
    const unsigned char alu_cond_mask = 8;
    const unsigned char im1_mask = 128;
    const unsigned char im2_mask = 64;
    const unsigned char arg1_ram_mask = 32;
    const unsigned char arg2_ram_mask = 16;
    const unsigned char dest_ram_mask = 48;
    const unsigned char arg_mask = 15;

    
    
    struct instruction {
        bool im1;
        bool im2;
        bool alu_vs_cond;
        unsigned char op;
        bool arg1_ram;
        bool arg2_ram;
        bool dest_ram;
    };

    struct args {
        int arg1;
        int arg2;
        int dest;
    };

    void opcode_validation (instruction);
    instruction op_decoder(unsigned char);
    args decode(unsigned char, unsigned char, unsigned char, instruction);

    int ALU(unsigned char, int, int);
    bool COND(unsigned char, int, int);

public:
    VM();
    const int MAX_PROG_SIZE = 1024;
    void execute(unsigned char*);

};