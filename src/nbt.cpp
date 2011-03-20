#include <string>
#include <fstream>
#include "nbt.h"
#include "util.h"
#include "nbt_defines.h"

#include <iostream>

#include <zlib.h>


   NBT::NBT(void)
   {
   }
   
   
   
   
   NBT::~NBT()
   {
        // destroy everything!
    }
   
   
   
   void NBT::DisplayToScreen()
   {
        cout << _rootNode.getName() << endl;
    }
    
   NBT * NBT::DecompressFile(string fileName)
   {
        FILE*_nbtFile;
        
        if(!Utilities::FileExists(fileName))
        {
            return NULL;
        }
        
        _nbtFile = fopen(fileName.c_str(),  "rb");
        if(_nbtFile == NULL)
        {
            return NULL;
        }
        
        // get file size
        fseek(_nbtFile, -4, SEEK_END);
        int compressedSize = ftell(_nbtFile)+4;
        unsigned int uncompressedSize = fread(&uncompressedSize, 4, 1, _nbtFile);
        
        if(Utilities::IsLittleEndian)
        {
            uncompressedSize = endianSwap(uncompressedSize);
        };
        
        fclose(_nbtFile);
        
        NBT_BYTE * uncompressedFile =  new NBT_BYTE[uncompressedSize];
        gzFile myFile = gzopen(fileName.c_str(), "rb");
        if(myFile == NULL)
        {
            delete [] uncompressedFile;
            return NULL;
        }
        gzread(myFile, uncompressedFile, uncompressedSize);
        gzclose(myFile);
        
        // skip junk? test with normal first.
        byte * fileBegins = uncompressedFile += 3;
        
        NBT * n = new NBT;
        n->Parse(uncompressedFile);
        
        // All done here!
        delete[] uncompressedFile;
        
        return n;
    }
    
    
   NBT * DecompressMemory(NBT_BYTE * buffer, NBT_INT length)
   {
        // use zlib with gzip
      z_stream zstream;
        zstream.zalloc    = (alloc_func)0;
        zstream.zfree     = (free_func)0;
        zstream.opaque    = (voidpf)0;
        zstream.next_in   = buffer;
        zstream.avail_in  = length;
        zstream.next_out  = 0;
        zstream.avail_out = 0;
        zstream.total_in  = 0;
        zstream.total_out = 0;
        zstream.data_type = Z_BINARY;
        inflateInit(&zstream);
        
        NBT_BYTE * chunkBuffer = new NBT_BYTE[NBT_BUFFER_SIZE];
        
        zstream.next_out = chunkBuffer;
        zstream.avail_out = NBT_BUFFER_SIZE;

        int ret = 0;
        
        if(ret = inflate(&zstream, Z_FINISH) != Z_STREAM_END && ret != Z_BUF_ERROR)
        {
            cout << "Error decompressing memory stream. Sorry." << endl;
            delete [] chunkBuffer;
            return NULL;
        }
        
        inflateEnd(&zstream);
        
        
        NBT * n = new NBT;
        n->Parse(chunkBuffer);
        
        delete [] chunkBuffer;
        
        return n;
    }
    
      
      
   void NBT::Parse(NBT_BYTE * data)
   {
        long int position = 0;
        
        
        
        if(*data == TAGTYPE_COMPOUND)
        {
            _rootNode.parseTag(++data);
        }
        else
        {
            // No root node, fail.
            
            cout << "NO!!!!!!! BAD FILE IS BAD!: SAY: " << (unsigned short int)*data << endl; 
        }
        
         return;
   }

