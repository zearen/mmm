#include <stdio.h>
#include <time.h>

#include "util.h"
#include "Region.h"

union intBlock {
    int i;
    char c[4];
};

int getOffset(byte x, byte z);

void printSectors(vector<bool>& fs) {
    for (int i = 0; i < fs.size(); i++) {
        printf("%d",(int)fs[i]);
    }
    printf("\n");
}

Region::chunk_t::chunk_t(short length, byte compression) {
    cmpr = compression;
    if (length > 0) {
        len = length;
        data = new byte[len];
    }
    else {
        len = 0;
        data = NULL;
    }
}

Region::Region() {};

Region::Region(char *filename) {
    open(filename);
}

Region::~Region() {
    if (regionFile.is_open())
        regionFile.close();
}

void Region::open(char *filename) {
    intBlock inblk;
    int offset, size, i;
    printf("Openning file...\n");
    regionFile.open(filename, ios::in | ios::out | ios::binary);
    if (!regionFile.is_open())
        throw FileNotFoundError();
    regionFile.seekg(0, ios::end);
    size = regionFile.tellg();
    if (size < R_SECTOR_SIZE * 2) {
        printf("    Growing file\n");
        // we need to grow the file
        int bytesNeeded = R_SECTOR_SIZE * 2 - size;
        const byte zero = 0;
        for (i = 0; i < bytesNeeded; i++) {
            regionFile.write((char*)&zero, 1);
        }
    }
    
    // Let's set all the free sectors
    regionFile.seekg(0);
    freeSectors.resize((size >> 12) + 1, true);
    setSectorFree(0, false); // offsets
    setSectorFree(1, false); // timestamps
    for (i = 2; i < 1024; i++) {
        regionFile.read(inblk.c, 4);
        if (inblk.i) { // If chunk exists
            offset = endianSwap<int>(inblk.i) >> 8;
            for(int j = 0; j < inblk.c[3]; j++) {
                setSectorFree(offset+j, false);
            }
        }
    }
    printSectors(freeSectors);
    printf("\nDone.\n\n");
}

void Region::write(byte x, byte z, chunk_t chunk) {
    intBlock inblk;
    byte outByte;
    int offset=getOffset(x, z), curSize;
    int i, endIndex;

    if (!regionFile.is_open())
        throw FileNotOpenError();
    
    printf("Writing...\n");
    printf("    Chunk data: len=%08x\n", chunk.len);
    // Write timestamp
    regionFile.seekp(offset + R_SECTOR_SIZE);
    inblk.i = endianSwap<int>((int)time(NULL));
    regionFile.write(inblk.c, 4);
    printf("    Timestamped at 0x%08x\n", (int)regionFile.tellp()-4);
    
    // First we need to figure out where to put the data
    chunk.len += 4; // We're going to write an extra 4 bytes to be safe
    regionFile.seekg(offset);
    regionFile.read(inblk.c, 4);
    curSize = R_SECTOR_SIZE * ((int)inblk.c[3]);
    printf("    curSize=0x%08x\n    chunk.len=0x%08x is ", curSize, chunk.len);
    
    if (chunk.len < curSize - R_SECTOR_SIZE) {
        // It's smaller, so all we have to do is report it
        //+ and overwrite
        printf("smaller\n");
        outByte = (chunk.len >> 12) + 1; // len / 4096 + 1
        regionFile.seekp(offset + 3);
        regionFile.write((char*)&outByte, 1);
        endIndex = (endianSwap<int>(inblk.i) >> 8) + (curSize >> 12);
        for (i = endIndex - (curSize >> 12) + outByte; i < endIndex; i++) {
            setSectorFree(i, true);
        }
        printSectors(freeSectors);
        regionFile.seekp(R_SECTOR_SIZE * (endianSwap<int>(inblk.i) >> 8));
    }
    
    else if (chunk.len > curSize) {
        // We're bigger, so we need to grow in sectors
        printf("bigger\n");
        outByte = (chunk.len >> 12) + 1; // len / R_SECTOR_SIZE + 1
        regionFile.seekp(offset + 3);
        regionFile.write((char*)&outByte, 1);
        if (inblk.i) {
            // If we've allocated sectors, unallocate them.
            endIndex = (endianSwap<int>(inblk.i) >> 8) + (curSize >> 12);
            for (i = endIndex - (curSize >> 12); i <= endIndex; i++) {
                setSectorFree(i, true);
            }
            printSectors(freeSectors);
        }
        offset = allocSpace(outByte);
        regionFile.seekp(-4, ios::cur);
        inblk.i = endianSwap<int>(offset);
        regionFile.write((char*)(inblk.c + 1), 3);
        regionFile.seekp(R_SECTOR_SIZE * offset);
    }
    
    else {
        // The allocated space and needed space match, we're set
        printf("the same\n");
        regionFile.seekp(R_SECTOR_SIZE * (endianSwap<int>(inblk.i) >> 8));
    }
    
    // At this point we should be on the data,
    //+ so we write
    printf("\n    Committing data at 0x%08x\n", (int)regionFile.tellp());
    chunk.len -= 4; // We need the actual value now
    offset = endianSwap<int>(chunk.len + 1);
    regionFile.write((char*)&offset, 4);
    regionFile.write((char*)&chunk.cmpr, 1);
    regionFile.write((char*)chunk.data, chunk.len - 1);

    // top off sector
    offset = 0;
    regionFile.write((char*)&offset, 4);
    regionFile.flush();
    printSectors(freeSectors);
    printf("Done.\n\n");
}

// Called when we need to find more space to insert.
int Region::allocSpace(byte sectorsToAlloc) {
    bool notDone = true;
    int offset=0, run=0;
    int oldpos, size, needSize;
    const byte zero=0;
    printf("    Allocating space\n");
    while (run < sectorsToAlloc) {
        if (isSectorFree(offset)) {
            run++;
        }
        else {
            run = 0;
        }
        offset++;
    }
    
    for (run = offset - sectorsToAlloc; run < offset; run++) {
        setSectorFree(run, false);
    }
    offset -= sectorsToAlloc;
    
    oldpos = regionFile.tellp();
    regionFile.seekp(0, ios::end);
    size = regionFile.tellp();
    needSize = R_SECTOR_SIZE * offset;
    // We need to grow the last sector
    for (int i = size; i <= needSize; i++) {
        regionFile.write((char*)&zero, 1);
    }
    regionFile.seekp(oldpos);
            
    
    return offset;
    printf("    Done.\n");
}

Region::chunk_t Region::read(byte x, byte z) {
    int inint;
    byte inbyte;
    chunk_t ret;
    
    if (!regionFile.is_open())
        throw FileNotOpenError();
    
    printf("Reading...\n");
    // Get to the start of data
    regionFile.seekg(getOffset(x, z));
    regionFile.read((char*)&inint, 4);
    printf("    Found 0x%08x\n", endianSwap<int>(inint));
    if (!inint)
        throw ChunkDoesNotExistError();
    regionFile.seekg(R_SECTOR_SIZE * (endianSwap<int>(inint) >> 8));
    printf("     Sought to sector 0x%08x\n", (int)regionFile.tellg());
    regionFile.read((char*)&inint, 4);
    regionFile.read((char*)&inbyte, 1);
    ret = chunk_t(endianSwap<int>(inint) - 1, inbyte);
    regionFile.read((char*)ret.data, ret.len);
    printf("Done.");
    return ret;
}

void Region::erase(byte x, byte z) {
    const int zero = 0;
    intBlock inblk;
    int offset = getOffset(x,z), endIndex;
    
    printf("Deleting...\n");
    regionFile.seekg(offset);
    regionFile.read(inblk.c, 4);
    endIndex = (endianSwap<int>(inblk.i) >> 8) + inblk.c[3];
    for (int i = endIndex - inblk.c[3]; i < endIndex; i++) {
        setSectorFree(i, true);
    }
    printSectors(freeSectors);
    regionFile.seekp(offset);
    regionFile.write((char*)&zero,4);
    
    printf("Done.\n\n");
}

inline int getOffset(byte x, byte z) {
    if (x > 31 || z > 31)
        throw ChunkOutOfRangeError();
    return (z*32 + x) << 2; // 4*(...)
}


inline void Region::growFreeSectors(int needSpace) {
    for (int i = 0; i < needSpace; i++) {
        freeSectors.push_back(true);
    }
}

void Region::setSectorFree(unsigned int sector, bool val) {
    growFreeSectors(sector - freeSectors.size() + 1);
    freeSectors[sector] = val;
}

bool Region::isSectorFree(unsigned int sector) {
    growFreeSectors(sector - freeSectors.size() + 1);
    printf("s%d:%d, ", sector, (int)freeSectors[sector]);
    return freeSectors[sector];
}
