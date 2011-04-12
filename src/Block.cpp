#include "Block.h"

Block::Block();

Block::Block(byte newId, byte skylight, byte blocklight, byte extra) {
    id = newId;
    light = (skylight << 4) & 0xF0 | blocklight & 0x0F;
    data = extra & 0x0F;
}

void Block::setSkyLight(byte skylight) {
    light = light & 0x0F | (light << 4) & 0xF0;
}

byte Block::getSkyLight() {
    return light >> 4;
}

void Block::setBlockLight(byte blocklight) {
    light = data & 0xF0 | blocklight & 0x0F;
}

byte Block::getBlockLight() {
    return light & 0x0F;
}

void Block::setExtra(byte extra) {
    data = extra & 0x0F;
}

byte Block::getExtra() {
    return data;
}
