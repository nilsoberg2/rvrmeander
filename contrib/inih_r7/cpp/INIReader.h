// Read an INI file into easy-to-access name/value pairs.

// inih and INIReader are released under the New BSD license (see LICENSE.txt).
// Go to the project home page for more info:
//
// http://code.google.com/p/inih/

#ifndef __INIREADER_H__
#define __INIREADER_H__

#if defined(_MSC_VER)
#define INIH_DLL_EXPORT __declspec(dllexport)
#define INIH_DLL_IMPORT __declspec(dllimport)
#else
#define INIH_DLL_EXPORT
#define INIH_DLL_IMPORT
#endif
#if defined(INIH_EXPORTS)
#define INIH_DLL_API INIH_DLL_EXPORT
#elif !defined(INIH_STATIC_LINK)
#define INIH_DLL_API INIH_DLL_IMPORT
#else
#define INIH_DLL_API
#endif


#include <map>
#include <string>

// Read an INI file into easy-to-access name/value pairs. (Note that I've gone
// for simplicity here rather than speed, but it should be pretty decent.)
class INIH_DLL_API INIReader
{
public:
    // Construct INIReader and parse given filename. See ini.h for more info
    // about the parsing.
    INIReader(const char* filename);

    // Return the result of ini_parse(), i.e., 0 on success, line number of
    // first error on parse error, or -1 on file open error.
    int ParseError();

    // Get a string value from INI file, returning default_value if not found.
    std::string Get(std::string section, std::string name,
                    std::string default_value);
    std::string Get(std::string key, std::string default_value);

    // Get an integer (long) value from INI file, returning default_value if
    // not found.
    long GetInteger(std::string section, std::string name, long default_value);
    long GetInteger(std::string key, long default_value);

    // Get a double-precision floating point value from INI file, returning
    // default_value if not found.
    double GetDouble(std::string section, std::string name, double default_value);
    double GetDouble(std::string key, double default_value);

    int GetDoubleArray(std::string key, double*& elems_out);

    // Get a boolean value from INI file, returning default_value if not
    // found.
    bool GetBoolean(std::string section, std::string name, bool default_value);
    bool GetBoolean(std::string key, bool default_value);

    // Determine if a key exists.
    bool Exists(std::string section, std::string name);
    bool Exists(std::string key);

private:
    int _error;
    std::map<std::string, std::string> _values;
    static std::string MakeKey(std::string section, std::string name);
    static int ValueHandler(void* user, const char* section, const char* name,
                            const char* value);
    static std::string ToLower(std::string input);
};

#endif  // __INIREADER_H__
