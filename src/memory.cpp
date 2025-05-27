#include "memory.hpp"
#include <stdexcept>
#include <algorithm>

namespace lc3
{

    Memory::Memory(uint16_t *initial, size_t initial_size)
    {
        if (initial_size > get_size())
            memory = new uint16_t[initial_size];

        std::copy(initial, initial + initial_size, memory);
    }

    Memory::~Memory()
    {
        delete[] memory;
    }

    uint16_t Memory::read(uint16_t address) const
    {
        if (address < get_size())
        {
            return memory[address];
        }
        throw std::out_of_range("Memory read out of bounds");
    }

    void Memory::write(uint16_t address, uint16_t value)
    {
        if (address < get_size())
        {
            memory[address] = value;
            return;
        }
        throw std::out_of_range("Memory write out of bounds");
    }

    uint16_t Memory::get_size() const noexcept
    {
        return UINT16_MAX;
    }
}