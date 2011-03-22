#ifndef _PLAYER
#define _PLAYER

#include "NBTDefines.h"
#include "NBT.h"

class Player
{
    private:
        
        NBT nbtProcessor;
        TAG_Compound* root;
        NBT_StringHolder playerName;
        
        NBT_BYTE OnGround; 
        NBT_SHORT HurtTime;
        NBT_SHORT Health;
        NBT_INT Dimension;
        NBT_SHORT Air;
        
        NBT_SHORT SleepTimer;
        NBT_BYTE Sleeping;
        NBT_SHORT Fire;
        NBT_FLOAT FallDistance;
        NBT_SHORT DeathTime;
        
        double Pos[3];
        float Rotation[2];
        
        void createNew(const char * name);
        void importByName(const string & playerPath);
        
    protected:
        
    public:
        Player(const char * name);
        ~Player();

        void loadFromFile();
        void reset();

        void display();
};

#endif
