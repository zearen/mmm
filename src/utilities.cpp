#include <string>
#include <sys/stat.h> // used for 'file exists' stat
#include "utilities.h"
#include <fstream>




bool Utilities::FileExists(string fileName)
{

     struct stat stFileInfo;
     bool doesExist = false;
     if(stat(fileName.c_str(), &stFileInfo) == 0)
     {
         doesExist = true;
     }
     return doesExist;
}
