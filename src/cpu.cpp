#include "cpu.hpp"

namespace lc3
{
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
        while (running)
        {
            uint16_t instr = memory->read(reg[PC]++);
            execute_instruction(instr);
        }
    }

    void CPU::update_flags(uint16_t value)
    {
        if (value == 0)
        {
            reg[COND] = ZRO;
        }
        else if (value & 0x8000) // Negative
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