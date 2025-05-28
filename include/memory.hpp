#pragma once

#include <algorithm>
#include <cstdint>
#include <cstdio> 

namespace lc3
{
    class Memory
    {
    private:
        const uint16_t MR_KBSR = 0xFE00; // Keyboard Status Register
        const uint16_t MR_KBDR = 0xFE02; // Keyboard Data Register

        uint16_t *memory = new uint16_t[UINT16_MAX];

        bool read_image(const char *image_path);
        void read_image_file(FILE *file);
        uint16_t swap16(uint16_t x);

    public:
        Memory(uint16_t *initial, size_t initial_size);
        Memory(const char *filePath);

        ~Memory();

        uint16_t read(uint16_t address);
        void write(uint16_t address, uint16_t value);
        uint16_t get_size() const noexcept;
       
    };

}