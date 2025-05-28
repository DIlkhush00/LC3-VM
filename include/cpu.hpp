#pragma once

#include <memory>
#include <array>

#include "memory.hpp"

namespace lc3
{
    enum Register : uint16_t
    {
        R0 = 0,
        R1,
        R2,
        R3,
        R4,
        R5,
        R6,
        R7,     // General-purpose registers
        PC,     // Program Counter
        COND,   // Condition code register
        R_COUNT // Total number of registers
    };

    enum Flag : uint16_t
    {
        POS = 1 << 0, // Positive flag
        ZRO = 1 << 1, // Zero flag
        NEG = 1 << 2  // Negative flag
    };

    enum TrapVector : uint16_t
    {
        GETC = 0x20,  // Get character
        OUT = 0x21,   // Output character
        PUTS = 0x22,  // Output string
        IN = 0x23,    // Input character
        PUTSP = 0x24, // Output string with two-byte characters
        HALT = 0x25   // Halt the program
    };

    class CPU
    {
    private:
        bool running = false;
        uint16_t reg[static_cast<uint16_t>(Register::R_COUNT)];
        std::shared_ptr<Memory> memory;

        template <unsigned op>
        void ins(uint16_t instr)
        {
            uint16_t r0, r1, r2, imm5, imm_flag;
            uint16_t pc_plus_off, base_plus_off;

            constexpr uint16_t opbit = (1 << op);

            // Initialize registers based on the instruction bits
            if (0x4EEE & opbit)
            {
                r0 = (instr >> 9) & 0x7;
            }

            // Check if the instruction uses a second register
            if (0x12F3 & opbit)
            {
                r1 = (instr >> 6) & 0x7;
            }

            // Check if the instruction uses an immediate value or a register
            if (0x0022 & opbit)
            {
                imm_flag = (instr >> 5) & 0x1;

                if (imm_flag)
                {
                    imm5 = signExtend(instr & 0x1F, 5);
                }
                else
                {
                    r2 = instr & 0x7;
                }
            }

            // Base + offset
            if (0x00C0 & opbit)
            {
                base_plus_off = reg[r1] + signExtend(instr & 0x3F, 6);
            }

            // Indirect address
            if (0x4C0D & opbit)
            {
                pc_plus_off = reg[PC] + signExtend(instr & 0x1FF, 9);
            }

            // BR, Branch
            if (0x0001 & opbit)
            {
                uint16_t cond = (instr >> 9) & 0x7;
                if (cond & reg[COND])
                {
                    reg[PC] = pc_plus_off;
                }
            }

            // ADD, Add
            if (0x0002 & opbit)
            {
                if (imm_flag)
                {
                    reg[r0] = reg[r1] + imm5;
                }
                else
                {
                    reg[r0] = reg[r1] + reg[r2];
                }
            }

            // AND, Bitwise AND
            if (0x0020 & opbit)
            {
                if (imm_flag)
                {
                    reg[r0] = reg[r1] & imm5;
                }
                else
                {
                    reg[r0] = reg[r1] & reg[r2];
                }
            }

            // NOT, Bitwise NOT
            if (0x0200 & opbit)
            {
                reg[r0] = ~reg[r1];
            }

            // JMP, Jump
            if (0x1000 & opbit)
            {
                reg[PC] = reg[r1];
            }

            // JSR, Jump to Subroutine
            if (0x0010 & opbit)
            {
                uint16_t long_flag = (instr >> 11) & 1;
                reg[R7] = reg[PC];
                if (long_flag) // JSR
                {
                    pc_plus_off = reg[PC] + signExtend(instr & 0x7FF, 11);
                    reg[PC] = pc_plus_off;
                }
                else // JSRR
                {
                    reg[PC] = reg[r1];
                }
            }

            // LD, Load
            if (0x0004 & opbit)
            {
                reg[r0] = memory->read(pc_plus_off);
            }

            // LDI, Load Indirect
            if (0x0400 & opbit)
            {
                reg[r0] = memory->read(memory->read(pc_plus_off));
            }

            // LDR, Load Register
            if (0x0040 & opbit)
            {
                reg[r0] = memory->read(base_plus_off);
            }

            // LEA, Load Effective Address
            if (0x4000 & opbit)
            {
                reg[r0] = pc_plus_off;
            }

            // ST, Store
            if (0x0008 & opbit)
            {
                memory->write(pc_plus_off, reg[r0]);
            }

            // STI, Store Indirect
            if (0x0800 & opbit)
            {
                memory->write(memory->read(pc_plus_off), reg[r0]);
            }

            // STR, Store Register
            if (0x0080 & opbit)
            {
                memory->write(base_plus_off, reg[r0]);
            }

            // Handle TRAP instructions
            if (0x8000 & opbit)
            {
                reg[R7] = reg[PC];

                switch (instr & 0xFF)
                {
                case GETC:
                    reg[R0] = (uint16_t)getchar();
                    update_flags(R0);
                    break;

                case OUT:
                    putc((char)reg[R0], stdout);
                    fflush(stdout);
                    break;

                case PUTS:
                {
                    uint16_t address = reg[R0];
                    uint16_t ch;
                    while ((ch = memory->read(address++)))
                    {
                        putc((char)ch, stdout);
                    }
                    fflush(stdout);
                }
                break;

                case IN:
                {
                    printf("Enter a character: ");
                    char c = getchar();
                    putc(c, stdout);
                    fflush(stdout);
                    reg[R0] = (uint16_t)c;
                    update_flags(R0);
                }
                break;

                case PUTSP:
                {
                    uint16_t address = reg[R0];
                    uint16_t val;
                    while ((val = memory->read(address++)))
                    {
                        char char1 = val & 0xFF;
                        putc(char1, stdout);
                        char char2 = (val >> 8) & 0xFF;
                        if (char2)
                            putc(char2, stdout);
                    }
                    fflush(stdout);
                }
                break;

                case HALT:
                    puts("\n--- HALT ---");
                    fflush(stdout);
                    running = false;
                    break;
                }
            }

            if (0x4666 & opbit)
            {
                update_flags(r0);
            }
        }

        // Instruction table
        using InstructionHandler = void (CPU::*)(uint16_t);
        static const std::array<InstructionHandler, 16> op_table;

        // Helper functions
        uint16_t signExtend(uint16_t x, int bitCount) const;
        void update_flags(uint16_t value);

    public:
        CPU(std::shared_ptr<Memory> mem)
            : memory(std::move(mem)) {}

        void setup();
        void run();
        void execute_instruction(uint16_t instr);
    };
}