//
// Created by Twometer on 19/09/2019.
//

#ifndef NEXTCRAFT_MCBUFFER_H
#define NEXTCRAFT_MCBUFFER_H

#include <cstdint>
#include "../util/ZLib.h"

class McBuffer {

private:
    uint8_t *data = nullptr;

    int dataSize = 0;

    int offset = 0;

    bool isBufferReplaced = false;

public:
    McBuffer();

    McBuffer(uint8_t *buf, int length);

    ~McBuffer();

    char *ReadString();

    int32_t ReadInt();

    int32_t ReadVarInt();

    int64_t ReadLong();

    uint64_t ReadULong();

    double ReadDouble();

    float ReadFloat();

    int16_t ReadShort();

    uint8_t ReadByte();

    uint16_t ReadBlockData();

    bool ReadBool();

    void Read(void *target, int len);

    void Write(void *data, int len);

    void Write(McBuffer &buf);

    void WriteVarInt(int32_t value);

    void WriteUShort(uint16_t value);

    void WriteULong(uint64_t value);

    void WriteString(const char *value);

    void WriteDouble(double value);

    void WriteFloat(float value);

    void WriteBool(bool value);

    void Skip(int len);

    void DecompressRemaining(int sizeUncompressed);

    int32_t GetAllocated();

    uint8_t *GetBytes();

private:
    static inline void Reverse(void *ptr, int len);

    inline uint8_t *GetPosition() {
        return data + offset;
    }

};


#endif //NEXTCRAFT_MCBUFFER_H
