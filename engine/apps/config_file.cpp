#pragma warning(once: 4290)

#include "config_file.h"
#include "config_file_obj.h"
#include "config_file_util.h"
#include "rvrmeander.h"
#include "util.h"		//Added by Nils on March 25 2011


static std::string makeLibconfigKey(std::string key)
{
    return std::string("rvrmeander.") + key;
}


GenericConfigObj::GenericConfigObj()
{
    type = CFT_Unknown;
    conf = NULL;
    ini = NULL;
}

GenericConfigObj::~GenericConfigObj()
{
    Cleanup();
}


void GenericConfigObj::Cleanup()
{
    if (conf != NULL)
        delete conf;
    if (ini != NULL)
        delete ini;
}


ConfigFileContents::ConfigFileContents()
{
    Channel = NULL;
    Preprocessing = NULL;
    Hydrodynamics = NULL;
    BankErosion = NULL;
    Migration = NULL;
    Smoothing = NULL;
    Plotting = NULL;
}


ConfigFileContents::~ConfigFileContents()
{
    if (Channel != NULL)
        delete Channel;
    if (Preprocessing != NULL)
        delete Preprocessing;
    if (Hydrodynamics != NULL)
        delete Hydrodynamics;
    if (BankErosion != NULL)
        delete BankErosion;
    if (Migration != NULL)
        delete Migration;
    if (Smoothing != NULL)
        delete Smoothing;
    if (Plotting != NULL)
        delete Plotting;
}


std::string DetermineMethod(const char* file, ConfigFileType type)
{
	using namespace std;

    GenericConfigObj config;
    config.type = type;


    // Handle the libconfig-style config file.
    if (type == CFT_libconfig)
    {
	    using namespace libconfig;

        config.conf = new Config();

	    try
        {
            config.conf->readFile(file);
	    }
        catch(...)
        {
            return "";
        }
    }

    // Handle the INI-style config file.
    else if (type == CFT_INI)
    {
        try
        {
            config.ini = new INIReader(file);

            if (config.ini->ParseError())
                return "";
        }
        catch(...)
        {
            return "";
        }
    }

    // If the style isn't one of the above, then crash.
    else
        return "";


    // Now lookup the method.
    std::string method;
    configStringLookup(config, method, "rvrmeander.method");

    return method;
}



bool RVRMeander::Load(std::string file, ConfigFileType type)
{
	using namespace std;

    m_configFile = file;

    GenericConfigObj config;
    config.type = type;


    // Handle the libconfig-style config file.
    if (type == CFT_libconfig)
    {
	    using namespace libconfig;

        config.conf = new Config();

	    try
        {
            config.conf->readFile(file.c_str());
	    }
        catch(ParseException& ex)
        {
            logmsg("[ERROR]  Loading config file, error on line %d: %s", ex.getLine(), ex.getError());
            return false;
        }
        catch(SettingNotFoundException nfex)
        {
            logmsg("[ERROR]  Loading config file, setting not found: %s", nfex.getPath());
            return false;
        }
        catch(ConfigException& cex)
        {
            logmsg("[ERROR]  Loading config file, exception: %s", cex.what());
            return false;
        }
        catch(...)
        {
            logmsg("[ERROR]  Loading config file");
            return false;
        }
    }

    // Handle the INI-style config file.
    else if (type == CFT_INI)
    {
        try
        {
            config.ini = new INIReader(file.c_str());

			int code = config.ini->ParseError();
            if (code)
            {
				logmsg("[ERROR]  Unable to parse INI file '%s': '%d'", file, code);
                return false;
            }
        }
        catch(...)
        {
            logmsg("[ERROR]  Loading config file");
            return false;
        }
    }

    // If the style isn't one of the above, then crash.
    else
        return false;

	return loadConfigFile(config, m_configData);
}

void configCleanup2DArray(double**& arr, int size)
{
    for (int i = 0; i < size; i++)
        delete[] arr[i];
    delete[] arr;
    arr = NULL;
}

int configDoubleArrayLookup(GenericConfigObj& obj, double*& elems, std::string key)
{
    if (obj.type == CFT_INI)
    {
        if (obj.ini->Exists(std::string(key)))
            return obj.ini->GetDoubleArray(key, elems);
        else
            return 0;
    }
    else if (obj.type == CFT_libconfig)
    {
        key = makeLibconfigKey(key);
        if (obj.conf->exists(key))
        {
            libconfig::Setting& coeffs_s = obj.conf->lookup(key);
	        
            int size = coeffs_s.getLength();
	        elems = new double[size];
	        
            for (int i = 0; i < size; i++)
		        elems[i] = coeffs_s[i];
            
            return size;
        }
        else
            return 0;
    }

    return 0;
}

bool configStringLookup(GenericConfigObj& obj, std::string& var, std::string key)
{
    if (obj.type == CFT_INI)
    {
		key = ToLower(key);			//Added by Nils on March 25 2011
        if (obj.ini->Exists(key))	//Modified by Nils on March 25 2011
            var = obj.ini->Get(key, "");
        else
            return false;

        return true;
    }
    else if (obj.type == CFT_libconfig)
    {
        key = makeLibconfigKey(key);
        if (obj.conf->exists(key))
        {
            libconfig::Setting& method_s = obj.conf->lookup(key);
		    var = (const char*)method_s;
        }
        else
            return false;
            
        return true;
    }

    return false;
}

bool configDoubleLookup(GenericConfigObj& obj, double& var, std::string key)
{
    double temp1;
    int temp2;

    if (obj.type == CFT_INI)
    {
        if (obj.ini->Exists(key))
            var = obj.ini->GetDouble(key, 0.0);
        else
            return false;

        return true;
    }
    else if (obj.type == CFT_libconfig)
    {
        key = makeLibconfigKey(key);
        if (obj.conf->lookupValue(key, temp1))
            var = (double)temp1;
        else if (obj.conf->lookupValue(key, temp2))
            var = (double)((int)temp2);
        else
            return false;

        return true;
    }

    return false;
}


bool configIntLookup(GenericConfigObj& obj, int& var, std::string key)
{
    double temp1;
    int temp2;

    if (obj.type == CFT_INI)
    {
        if (obj.ini->Exists(key))
            var = obj.ini->GetInteger(key, 0);
        else
            return false;

        return true;
    }
    else if (obj.type == CFT_libconfig)
    {
        key = makeLibconfigKey(key);
        if (obj.conf->lookupValue(key, temp2))
            var = (int)temp2;
        else if (obj.conf->lookupValue(key, temp1))
            var = (int)((double)temp1);
        else
            return false;

        return true;
    }

    return false;
}


bool configBoolLookup(GenericConfigObj& obj, bool& var, std::string key)
{
	bool temp;

    if (obj.type == CFT_INI)
    {
        if (obj.ini->Exists(key))
            var = obj.ini->GetBoolean(key, false);
        else
            return false;

        return true;
    }
    else if (obj.type == CFT_libconfig)
    {
        key = makeLibconfigKey(key);
        if (obj.conf->lookupValue(key, temp))
            var = (bool)temp;
        else
            return false;

        return true;
    }

    return false;
}

