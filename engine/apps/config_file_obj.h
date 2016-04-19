#ifndef __CONFIG_FILE_OBJ_H__20100304__
#define __CONFIG_FILE_OBJ_H__20100304__

#include <libconfig.h++>
#include <INIReader.h>
#include "enums.h"


struct GenericConfigObj
{
    libconfig::Config* conf;
    INIReader* ini;
    ConfigFileType type;
    GenericConfigObj();
    ~GenericConfigObj();
    void Cleanup();
};


#endif//__CONFIG_FILE_OBJ_H__20100304__
