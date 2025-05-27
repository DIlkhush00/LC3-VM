#pragma once
#include <cstdint>

namespace lc3
{
    constexpr uint16_t MEMORY_SIZE = 0x10000; // 64K memory

    class Memory
    {
    public:
        Memory();
        ~Memory();

        uint16_t read(uint16_t address) const;
        void write(uint16_t address, uint16_t value);
        uint16_t get_size() const noexcept;

    private:
        uint16_t mem[MEMORY_SIZE];
    };

    extern Memory memory; // Global memory instance
}