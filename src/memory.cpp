#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <unistd.h>

#include "memory.hpp"
#include "io.hpp"

namespace lc3
{
    Memory::Memory(uint16_t *initial, size_t initial_size)
    {

        if (initial_size > get_size())
            memory = new uint16_t[initial_size];

        std::copy(initial, initial + initial_size, memory);
    }

    Memory::Memory(const char *filePath)
    {
        // Load memory from a binary file
        if (!read_image(filePath))
        {
            printf("failed to load image: %s\n", filePath);
            exit(1);
        }
    }

    Memory::~Memory()
    {
        delete[] memory;
    }

    uint16_t Memory::read(uint16_t address)
    {
        if (address == MR_KBSR)
        {
            if (IO::check_key())
            {
                memory[MR_KBSR] = (1 << 15);
                memory[MR_KBDR] = getchar(); // getchar() directly updates MR_KBDR
            }
            else
            {
                memory[MR_KBSR] = 0;
            }
            return memory[MR_KBSR]; // Return the value of MR_KBSR
        }
        // For MR_KBDR or any other address:
        else if (address < get_size())
        { // Ensure get_size() is UINT16_MAX
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

    bool Memory::read_image(const char *image_path)
    {
        FILE *file = fopen(image_path, "rb");
        if (!file)
        {
            delete[] memory;
            memory = nullptr; // avoid dangling pointer
            return false;
        }
        read_image_file(file);
        fclose(file);
        return true;
    }

    void Memory::read_image_file(FILE *file)
    {
        // the origin tells us where in memory to place the image
        uint16_t origin;
        fread(&origin, sizeof(origin), 1, file);
        origin = swap16(origin);

        // we know the maximum file size so we only need one fread
        uint16_t max_read = UINT16_MAX - origin;
        uint16_t *p = memory + origin;
        size_t read = fread(p, sizeof(uint16_t), max_read, file);

        // swap to little endian
        while (read-- > 0)
        {
            *p = swap16(*p);
            ++p;
        }
    }

    // Swap bytes for little-endian representation
    uint16_t Memory::swap16(uint16_t x)
    {
        return (x << 8) | (x >> 8);
    }
}