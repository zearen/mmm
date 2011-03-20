#ifndef __BetaRegion_h
#define __BetaRegion_h
#define BR_SECTOR_SIZE 4096
#define BR_GZIP 1
#define BR_ZLIB 2
#endif

#ifndef byte
#define byte unsigned char
#endif

class BetaRegion {
    private:
        FILE *regionFile;
        vector<bool> freeSectors;
        int allocSpace(byte sectorsToAlloc);
        inline void growFreeSectors(unsigned needSpace);
    protected:
        void setSectorFree(unsigned sector, bool val);
        bool isSectorFree(unsigned sector);
    public:
        static class FileNotFound {};
        static class ChunkOutOfRangeError {};
        static class ChunkDoesNotExistError{};
        static struct chunk_t {
            short len;
            byte cmpr; //compression
            byte *data;
            chunk_t(short length=0, byte compression=2);
            ~chunk_t();
        }
        
        // propogates file not found
        BetaRegion(char *filename);
        ~BetaRegion();
        
        void write(byte x, byte z, chunk_t chunk);
        chunk_t read(byte x, byte z);
};
