#include <iostream>
#include <fstream>
#include <ios>
#include <climits>
#include "Virtual_Machine.h"
#include <string>


unsigned char* read_prog(std::string file_name, int program_size) {
    unsigned char* program = new unsigned char[program_size] ();
    std::ifstream fin;
    fin.open(file_name, std::ios::binary | std::ios::in);
    fin.read((char*)program, program_size);
    return program;
}

int main()
{
    VM vm;
    
    std::string file_name;
    std::cout << "Enter the file path." << std::endl;
    std::cin >> file_name;
    unsigned char* program = read_prog(file_name, vm.MAX_PROG_SIZE);
    vm.execute(program);
    delete program;
    return 0;
}