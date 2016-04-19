// Read an INI file into easy-to-access name/value pairs.

#define _CRT_SECURE_NO_WARNINGS

#include <cctype>
#include <cstdlib>
#include <cstring>
#include "../ini.h"
#include "INIReader.h"
#include <vector>

using std::string;

INIReader::INIReader(const char* filename)
{
    _error = ini_parse(filename, ValueHandler, this);
}

int INIReader::ParseError()
{
    return _error;
}




string INIReader::Get(string section, string name, string default_value)
{
    string key = MakeKey(section, name);
    return Get(key, default_value);
}

string INIReader::Get(string key, string default_value)
{
    return _values.count(key) ? _values[key] : default_value;
}




long INIReader::GetInteger(string key, long default_value)
{
    string valstr = Get(key, "");
    const char* value = valstr.c_str();
    char* end;
    // This parses "1234" (decimal) and also "0x4D2" (hex)
    long n = strtol(value, &end, 0);
    return end > value ? n : default_value;
}

long INIReader::GetInteger(string section, string name, long default_value)
{
    return GetInteger(MakeKey(section, name), default_value);
}




double INIReader::GetDouble(string key, double default_value)
{
    string valstr = Get(key, "");
    const char* value = valstr.c_str();
    char* end;
    // This parses "1234" (decimal) and also "0x4D2" (hex)
    double n = strtod(value, &end);
    return end > value ? n : default_value;
}

double INIReader::GetDouble(string section, string name, double default_value)
{
    return GetDouble(MakeKey(section, name), default_value);
}




int INIReader::GetDoubleArray(string key, double*& elems_out)
{
    string valstr = Get(key, "");

    if (valstr.length() < 3 || valstr[0] != '[' || valstr[valstr.length()-1] != ']')
        return false;

    char* arrayStr = (char*)(valstr.substr(1, valstr.length()-3).c_str());

    char delims[] = " \t\n\r";
    char* result = NULL;
    char* end;

    result = strtok(arrayStr, delims);

    std::vector<double> elems;

    while (result != NULL)
    {
        if (strlen(result) > 0)
        {
            double n = strtod(result, &end);
            if (end > result)
                elems.push_back(n);
        }
        result = strtok(NULL, delims);
    }

    if (elems.size() == 0)
        return 0;

    std::size_t size = elems.size();
    elems_out = new double[size];
    for (std::size_t i = 0; i < size; i++)
        elems_out[i] = elems.at(i);

    return (int)size;
}




bool INIReader::GetBoolean(string key, bool default_value)
{
    string valstr = ToLower(Get(key, ""));
    if (valstr == "true" || valstr == "1")
        return true;
    else if (valstr == "false" || valstr == "0")
        return false;
    else
        return default_value;
}

bool INIReader::GetBoolean(string section, string name, bool default_value)
{
    return GetBoolean(MakeKey(section, name), default_value);
}




bool INIReader::Exists(string section, string name)
{
    return Exists(MakeKey(section, name));
}

bool INIReader::Exists(string key)
{
    return _values.count(key) ? true : false;
}




string INIReader::MakeKey(string section, string name)
{
    string key = section + "." + name;
    return ToLower(key);
}

string INIReader::ToLower(string input)
{
    // Convert to lower case to make lookups case-insensitive
    for (std::size_t i = 0; i < input.length(); i++)
        input[i] = tolower(input[i]);
    return input;
}

int INIReader::ValueHandler(void* user, const char* section, const char* name,
                            const char* value)
{
    INIReader* reader = (INIReader*)user;
    reader->_values[MakeKey(section, name)] = value;
    return 1;
}
