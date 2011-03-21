#include <cstdlib>
#include <iostream>

#include <fstream>

#include "nbt.h"
#include "util.h"


using namespace std;

static string WORLD_PATH = "";
static string PLAYER_PATH = "";
static string REGION_PATH = "";


/* Load Server Configuration from server.properties */

void LoadConfiguration(void)
{    // load server properties
    
    WORLD_PATH = "world";

    if(!Utilities::FileExists("server.properties"))
    {
        cout << "ERROR: Cannot open server.properties... Are you in the minecraft folder?" << endl;
        cout << "Assuming default world path of 'world' for now." << endl;
        cout << "WARNING: Program will crash if world folder doesn't exist.";
    }
    else
    {
        ifstream serverProperties ("server.properties", ifstream::binary);
        string dataFileLine;
    
        while(serverProperties.good())
        {
            getline(serverProperties, dataFileLine);
           // find it
           remove(dataFileLine.begin(),dataFileLine.end(),' ');
           if(dataFileLine.compare(0,11,"level-name=") == 0)
           {
              // found the info you want!
              
            WORLD_PATH = dataFileLine.substr(11);
           }

        }
        
    }
    WORLD_PATH = WORLD_PATH + "/";
    PLAYER_PATH = WORLD_PATH + "players/";
    REGION_PATH = WORLD_PATH + "region/";


}



int main(int argc, char *argv[])
{
    
    // Determine if system is big or little endian (sets Utilities::IsLittleEndian )
    Utilities::DetectBigEndian();    
    
    
    cout << "Reading server properties . . . " << endl;
    
    LoadConfiguration();
        
    cout << "Using map: " << WORLD_PATH << endl;  
       
    cout << "Lol!" << endl;
    
    NBT * player = NBT::DecompressFile(PLAYER_PATH + "zearen.dat");
    //player.CloseFile();
    player->DisplayToScreen();
       
    system("PAUSE");
    return EXIT_SUCCESS;
}

