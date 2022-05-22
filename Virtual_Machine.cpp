#include "Virtual_Machine.h"
#include <iostream>

VM::VM() {
    counter = 0;
    for (int i = 0; i < reg_num; i++) {
        regs[i] = 0;
    }
    for (int i = 0; i < ram_num; i++) {
        RAM[i] = 0;
    }
}

void VM::opcode_validation (instruction inst) {
    if ((inst.im1 && inst.arg1_ram) || (inst.im2 && inst.arg2_ram)) {
        std::cerr << "Can't be an immediate and read from RAM at the same time!";
        std::exit(0);
    }
}


VM::instruction VM::op_decoder (unsigned char x) {
    instruction inst;
    inst.im1 = x & im1_mask;
    inst.im2 = x & im2_mask;
    inst.alu_vs_cond = x & alu_cond_mask;
    inst.op = x & operation_mask;
    inst.arg1_ram = (x & arg1_ram_mask) == arg1_ram_mask;
    inst.arg2_ram = (x & arg2_ram_mask) == arg2_ram_mask;
    inst.dest_ram = (x & dest_ram_mask) == dest_ram_mask;
    return inst;
}

VM::args VM::decode (unsigned char s, unsigned char t, unsigned char f, instruction inst) {

    args argument;
    if (!(inst.im1 || inst.arg1_ram)) {
        argument.arg1 = s & arg_mask;
    }
    else {
        argument.arg1 = s;
    }

    if (!(inst.im2 || inst.arg2_ram)) {
        argument.arg2 = t & arg_mask;
    }
    else {
        argument.arg2 = t;
    }
    if (!(inst.dest_ram || !inst.alu_vs_cond)) {
        argument.dest = f & arg_mask;
    }
    else {
        argument.dest = f;
    }
    return argument;
}




void VM::execute(unsigned char* prog) {

    for (counter = 0; counter < MAX_PROG_SIZE;) {
        instruction inst;
        inst = op_decoder(prog[counter]);
        opcode_validation(inst);
        args arguments;
        arguments = decode(prog[counter + 1], prog[counter + 2], prog[counter + 3], inst);

        int value_1;
        int value_2;

        if (inst.im1) {
            value_1 = arguments.arg1;
        }
        else if (inst.arg1_ram) {
            value_1 = RAM[arguments.arg1];
        }
        else {
            value_1 = regs[arguments.arg1];
        }


        if (inst.im2) {
            value_2 = arguments.arg2;
        }
        else if (inst.arg2_ram) {
            value_2 = RAM[arguments.arg2];
        }
        else {
            value_2 = regs[arguments.arg2];
        }

        int result;
        if (inst.alu_vs_cond) {
            if (COND(inst.op, value_1, value_2)) {
                if (inst.dest_ram) {
                    counter = RAM[arguments.dest];
                }
                else {
                    counter = regs[arguments.dest];
                }

                if (counter % 4 != 0) {
                    counter -= (counter % 4);
                    std::cout << "WARNING! Invalid counter value was changed by default!";
                }
                continue;
            }
        }

        else {
            result = ALU(inst.op, value_1, value_2);
            if (inst.dest_ram) {
                RAM[arguments.dest] = result;
            }
            else {
                regs[arguments.dest] = result;
            }
        }

        counter += 4;
    }
}

int VM::ALU (unsigned char operation, int arg_1, int arg_2) {

    unsigned char o = operation;
    switch (o) {
    case 0:
        return (arg_1 | arg_2);
    case 1:
        return (arg_1 & arg_2);
    case 2:
        return ~(arg_1 | arg_2);
    case 3:
        return ~(arg_1 & arg_2);
    case 4:
        return arg_1 + arg_2;
    case 5:
        return arg_1 - arg_2;
    case 6:
        return arg_1 * arg_2;
    default:
        return 0;
    }
}

bool VM::COND (unsigned char operation, int arg_1, int arg_2) {

    unsigned char o = operation;
    switch (o) {
    case 0:
        return (arg_1 == arg_2);
    case 1:
        return (arg_1 != arg_2);
    case 2:
        return (arg_1 > arg_2);
    case 3:
        return (arg_1 >= arg_2);
    case 4:
        return (arg_1 < arg_2);
    case 5:
        return (arg_1 <= arg_2);
    case 6:
        return 1; // always true
    case 7:
        return 0; // never true
    default:
        return 0;
    }
}