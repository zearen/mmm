#include "Block.h"

Block::Block(byte newId, byte light, byte extra) {
    id = newId;
    data = (light << 4) & 0xF0 | extra & 0x0F;
}

void setLight(byte light) {
    data |= (light << 4) & 0xF0;
}

byte getLight() {
    return light >> 4;
}

void setExtra(byte extra) {
    data | = extra & 0x0F;
}

byte getExtra() {
    return data & 0x0F;
}
