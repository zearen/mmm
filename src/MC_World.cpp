#include <fstream>
#include <iostream>

#include "MC_World.h"
#include "util.h"


// load configuration will override
string MC_World::WorldPath = "world/";
string MC_World::RegionPath = "world/region/";
string MC_World::PlayerPath = "world/players/";


void MC_World::LoadConfiguration(void)
{    // load server properties
    
    MC_World::WorldPath = "world";

    if(!Utilities::FileExists("server.properties"))
    {
        cout << "ERROR: Cannot open server.properties... Are you in the minecraft folder?" << endl;
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
              
            MC_World::WorldPath = dataFileLine.substr(11);
           }

        }
        
    }


}
