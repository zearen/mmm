#include <fstream>
#include <vector>

using namespace std;

#ifndef __Region_h
#define __Region_h
#define R_SECTOR_SIZE 4096
#define R_GZIP 1
#define R_ZLIB 2
#endif

#ifndef BYTE
#define BYTE
typedef unsigned char byte;
#endif

class FileNotOpenError {};
class FileNotFoundError {};
class ChunkOutOfRangeError {};
class ChunkDoesNotExistError {};

class Region {
    private:
        fstream regionFile;
        vector<bool> freeSectors;
        int allocSpace(byte sectorsToAlloc);
        inline void growFreeSectors(int needSpace);
    protected:
        void setSectorFree(unsigned sector, bool val);
        bool isSectorFree(unsigned sector);
    public:
        struct chunk_t {
            short len;
            byte cmpr; //compression
            byte *data;
            chunk_t(short length=0, byte compression=2);
        };
        
        // propogates file not found
        Region();
        Region(char *filename);
        ~Region();
        
        void open(char *filename);
        void write(byte x, byte z, chunk_t chunk);
        void erase(byte x, byte z);
        chunk_t read(byte x, byte z);
};
