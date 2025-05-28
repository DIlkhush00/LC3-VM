#pragma once

#include <algorithm>

namespace lc3
{
    
    class Memory
    {
    private:
        uint16_t *memory = new uint16_t[UINT16_MAX];

        int read_image(const char *image_path);
        void read_image_file(FILE *file);

    public:
        Memory(uint16_t *initial, size_t initial_size);
        Memory(const char* filePath);

        ~Memory();

        uint16_t read(uint16_t address) const;
        void write(uint16_t address, uint16_t value);
        uint16_t get_size() const noexcept;
    };

}