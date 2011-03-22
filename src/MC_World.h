#ifndef _MC_WORLD
#define _MC_WORLD

#include <string>
using namespace std;

class MC_World
{
    public:
        
        // ya i know, laziness.
        
        static string WorldPath;
        static string RegionPath;
        static string PlayerPath;
        static void LoadConfiguration();
        
};

#endif
