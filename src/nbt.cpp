#include <string>
#include <fstream>
#include "nbt.h"
#include "utilities.h"
#include "nbt_defines.h"

#include "zlib/stdafx.h"
#include "zlib/deflate.h"


   NBT::NBT(void)
   {
      _isReady = false;
   }
   
   bool NBT::IsReady(void)
   {
        return _isReady;
    }
   
   bool NBT::LoadFile(string fileName)
   {
        // close already loaded file
        if (_isReady)
        {
            CloseFile();
        }
        
        bool didLoad = false;
        if(Utilities::FileExists(fileName))
        {
            _nbtFile = fopen(fileName.c_str(),  "rb");
            _isReady = true;
            didLoad = true;
        }
        
        return didLoad;
    }
   
   void NBT::Decompress()
   {
        // get file size
        fseek(_nbtFile, 0, SEEK_END);
        long int fileSize = ftell(_nbtFile);
        
        // reset
        fseek(_nbtFile, 0, SEEK_SET);
        
        Byte * compressedFile = new Byte [fileSize];
        
        int len;
        len = fread(compressedFile, sizeof(Byte), INBUFLEN, _nbtFile);
        
        // decompress now!
        
        // set to nbt root
        //_rootTag = ParseCompoundTag(decompressedData, 0, decompressedSize);
        
        
        return;
    }
   
   
   
   NBT_SHORT NBT::ParseShort(NBT_BYTE * data, long int start, long int maxSize)
   {
        // two bytes of data
        return *((NBT_SHORT*)(data+start));
    }
    
   NBT_StringHolder NBT::ParseString(NBT_BYTE * data, long int start, long int maxSize)
   {
        NBT_SHORT len = ParseShort(data, start, maxSize);
        start += sizeof(NBT_SHORT);
        
        char * name = new char[len+1];
        memcpy(name, (data+start), len);
        name[len] = 0; // terminate string!
        
        NBT_StringHolder v;
        v.length = len;
        v.value = name;
        
        return v;
        
    }
   
   // data = the data file being read
   // position = the position of the tag header
   // length = the size of char * data
   TAG_Compound * NBT::ParseCompoundTag(NBT_BYTE * data, long int start, long int maxSize)
   {
        
        TAG_Compound root;
                
        long int position = start;
        
        
        NBT_StringHolder name = ParseString(data, start, maxSize);
        
        // get name
        root.SetName(&name);
        
        position += name.length + sizeof(NBT_SHORT);

        
        while(position < maxSize)
        {
            switch(*data+position)
            {
                case TAGTYPE_COMPOUND:
                    root.add(ParseCompoundTag(data, position+1, maxSize));
                    break;
                    
            };
        }
        // or does it need to be <=
        
        // fail!
        return NULL;
    }
   
   
   void NBT::Parse(NBT_BYTE * data, long int length)
   {
        long int position = 0;
        
        
        
        if(data[position] == TAGTYPE_COMPOUND)
        {
            _rootNode = ParseCompoundTag(data, position, length);
        }
        else
        {
            // No root node, fail.
        }
        
         return;
   }
   
   void NBT::CloseFile()
   {
        if(_isReady)
        {
            fclose(_nbtFile);
            _isReady = false;
        }
    }
