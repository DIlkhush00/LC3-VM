#include "cpu.hpp"
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/termios.h>
#include <sys/mman.h>

namespace lc3
{
    const std::array<CPU::InstructionHandler, 16> CPU::op_table = {
        &CPU::ins<0>,  // BR, Branch
        &CPU::ins<1>,  // ADD, Add
        &CPU::ins<2>,  // LD, Load
        &CPU::ins<3>,  // ST, Store
        &CPU::ins<4>,  // JSR, Jump to Subroutine
        &CPU::ins<5>,  // AND, Bitwise AND
        &CPU::ins<6>,  // LDR, Load Register
        &CPU::ins<7>,  // STR, Store Register
        nullptr,       // RTI, Reserved for RTI (not used)
        &CPU::ins<9>,  // NOT, Bitwise NOT
        &CPU::ins<10>, // LDI, Load Indirect
        &CPU::ins<11>, // STI, Store Indirect
        &CPU::ins<12>, // JMP, Jump
        nullptr,       // RES, Reserved (not used)
        &CPU::ins<14>, // LEA, Load Effective Address
        &CPU::ins<15>  // TRAP, Execute Trap
    };

    void CPU::setup()
    {
        running = true;
        reg[PC] = 0x3000; // Initial PC value
    }

    void CPU::execute_instruction(uint16_t instr)
    {
        uint16_t op = instr >> 12;
        if (op < op_table.size() && op_table[op])
        {
            (this->*op_table[op])(instr);
        }
        else
        {
            // Handle invalid opcode
            throw std::runtime_error("Invalid opcode encountered");
        }
    }

    struct termios original_tio;

    void disable_input_buffering()
    {
        tcgetattr(STDIN_FILENO, &original_tio);
        struct termios new_tio = original_tio;
        new_tio.c_lflag &= ~ICANON & ~ECHO;
        tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
    }

    void restore_input_buffering()
    {
        tcsetattr(STDIN_FILENO, TCSANOW, &original_tio);
    }

    void handle_interrupt(int signal)
    {
        restore_input_buffering();
        printf("\n");
        exit(-2);
    }

    void CPU::run()
    {
        signal(SIGINT, handle_interrupt);
        disable_input_buffering();

        setup();
        while (running)
        {
            uint16_t instr = memory->read(reg[PC]++);
            execute_instruction(instr);
        }

        restore_input_buffering();
    }

    void CPU::update_flags(uint16_t value)
    {
        if (value == 0)
        {
            reg[COND] = ZRO;
        }
        else if (value >> 15) // Negative
        {
            reg[COND] = NEG;
        }
        else // Positive
        {
            reg[COND] = POS;
        }
    }

    uint16_t CPU::signExtend(uint16_t x, int bitCount) const
    {
        if ((x >> (bitCount - 1)) & 1)
            x |= (0xffff << bitCount);

        return x;
    }
}