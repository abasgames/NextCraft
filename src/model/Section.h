//
// Created by twome on 22/01/2020.
//

#ifndef NEXTCRAFT_SECTION_H
#define NEXTCRAFT_SECTION_H

#include <cstdint>

namespace chunk {

    struct BlockData {
        uint8_t id;
        uint8_t meta;
    };

    struct Section {
    public:
        BlockData *data;

        Section();

        ~Section();

        BlockData *GetBlockData(int x, int y, int z);

        void SetBlockData(int x, int y, int z, BlockData block);

        void SetBlock(int x, int y, int z, uint8_t id);

        void SetMeta(int x, int y, int z, uint8_t meta);

    };

}

#endif //NEXTCRAFT_SECTION_H
