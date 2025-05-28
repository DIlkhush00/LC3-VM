#include <iostream>

#include "cpu.hpp"
#include "io.hpp"

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

    void CPU::run()
    {
        signal(SIGINT, lc3::IO::handle_interrupt);
        lc3::IO::disable_input_buffering();

        setup();

        while (running)
        {
            uint16_t instr = memory->read(reg[PC]++);
            execute_instruction(instr);
        }

        lc3::IO::restore_input_buffering();
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