#pragma once
#include <cstdint>

namespace lc3
{
    
    class Memory
    {
    private:
        uint16_t *memory = new uint16_t[UINT16_MAX];

    public:
        Memory(uint16_t *initial, size_t initial_size) {}
        ~Memory();

        uint16_t read(uint16_t address) const;
        void write(uint16_t address, uint16_t value);
        uint16_t get_size() const noexcept;
    };

}