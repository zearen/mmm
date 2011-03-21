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
    
    void NBT::DisplayList(TAG_List* src, string childStr)
    {
            int range=src->size();
            TagType type = src->getItemType();
            
            cout << range << " entries of type " << type << endl;
            cout << childStr << "{ " << endl;
            for(short int i=0;i<range;i++)
            {
                DisplayTag((*src)[i], childStr + "   ");
            }
            cout << childStr << "}" << endl;
    }
   
   void NBT::DisplayTag(NBT_Tag * src, string childStr)
   {
        
        // display tag to screen
        TagType t = src->getType();
        
        // use own handler, ignore it
        if(t == TAGTYPE_COMPOUND)
        {
            // Recursively parse compound tag
            DisplayCompound((TAG_Compound*)src, childStr);
            return;
        }
        
        cout << childStr;

        string tagName = "";
        
        if(src->getNameHolder().length > 0)
        {
        tagName = "(\"";
        tagName += src->getName();
        tagName += "\")";
        }
        switch(t)
        {
            case TAGTYPE_BYTE:
                cout << "TAG_Byte" << tagName.c_str() << ": 0x";
                cout << (unsigned short)(0 << 8 | ((TAG_Byte*)src)->getPayload());
                cout << endl;
                break;
            case TAGTYPE_DOUBLE:
                cout << "TAG_Double" << tagName.c_str() << ": ";
                cout << ((TAG_Double*)src)->getPayload();
                cout << endl;
                break;
                
                
            case TAGTYPE_FLOAT:
                cout << "TAG_Float" << tagName.c_str() << ": ";
                cout << ((TAG_Float*)src)->getPayload();
                cout << endl;
                break;
                
                
            case TAGTYPE_INT:
                cout << "TAG_Int" << tagName.c_str() << ": ";
                cout << ((TAG_Int*)src)->getPayload();
                cout << endl;
                break;
                
            case TAGTYPE_LONG:
                cout << "TAG_Long" << tagName.c_str() << ": ";
                cout << ((TAG_Long*)src)->getPayload();
                cout << endl;
                break;
                
                
            case TAGTYPE_SHORT:
                cout << "TAG_Short" << tagName.c_str() << ": ";
                cout << ((TAG_Short*)src)->getPayload();
                cout << endl;
                break;
                
            case TAGTYPE_STRING:
                cout << "TAG_String" << tagName.c_str() << ": ";
                cout << ((TAG_String*)src)->getPayload()->value;
                cout << endl;
                break;
                
                
                
            case TAGTYPE_LIST:
                cout << "TAG_List" << tagName.c_str() << ": ";
                DisplayList((TAG_List*)src, childStr);
                cout << endl;

                break;
                
            
        }
        
    }
    

    
    void NBT::DisplayCompound(TAG_Compound* root, string childStr)
    {
        
        cout << childStr << "TAG_Compound";
        if(root->getNameHolder().length > 0)
        {
            cout << "(\"" << root->getName() << "\")";
        }
        cout << ": ";
        
        NBT_INT numNodes = root->size();
        
        cout << numNodes << " entries" << endl;
        cout << childStr << "{" << endl;
        
        char ** nodes = root->listTagNames();
        
        
        NBT_INT i = 0;
        while(i < numNodes)
        {
            NBT_Tag * t = (*root)[*nodes];
            if(t)
            {
                DisplayTag(t, childStr + "   ");
            }
            else
            {
                cout << childStr << "ERROR: Null tag result (not found in compound)." << endl;
            }
            i++;
            nodes++;
        }
        
         cout << childStr << "}" << endl;

        
        
        //delete nodes;
    }
    
   void NBT::DisplayToScreen()
   {
        
        DisplayCompound(&_rootNode);
        
    }
    
   NBT * NBT::DecompressFile(string fileName)
   {
        cout << "Beginning to decompress file: " << fileName << endl;
        
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
        unsigned int compressedSize = ftell(_nbtFile)+4;

        NBT_INT uncompressedSize;
        fread(&uncompressedSize, 4, 1, _nbtFile);
        
        /*
        // this is little endian!?
        
        if(Utilities::IsLittleEndian)
        {   
            
            uncompressedSize = endianSwap(uncompressedSize);
        };

        cout << "Decompressing (File size " << uncompressedSize << ") " << endl;
        cout.flush();
        */
        
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
        
        NBT_BYTE * chunkBuffer = new NBT_BYTE[NBT_BUFFER_SIZE + 1];
        
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

