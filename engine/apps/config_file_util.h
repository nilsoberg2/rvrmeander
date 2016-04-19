#ifndef __CONFIG_FILE_UTIL_H__20101004093636__
#define __CONFIG_FILE_UTIL_H__20101004093636__

#include <string>
#include "config_file_obj.h"


bool configStringLookup(GenericConfigObj& obj, std::string& var, std::string key);
bool configDoubleLookup(GenericConfigObj& obj, double& var, std::string key);
bool configIntLookup(GenericConfigObj& obj, int& var, std::string key);
bool configBoolLookup(GenericConfigObj& obj, bool& var, std::string key);
int configDoubleArrayLookup(GenericConfigObj& obj, double*& elems, std::string key);
void configCleanup2DArray(double**& arr, int size);
static std::string makeLibconfigKey(std::string key);


#endif//__CONFIG_FILE_UTIL_H__20101004093636__
