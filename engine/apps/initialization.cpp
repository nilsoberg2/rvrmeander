#pragma warning(once : 4290)
#pragma warning(disable : 4251)

#include <iostream>
#include "rvrmeander.h"
#include "config_file.h"
#include "config_file_obj.h"

bool RVRMeander::loadConfigFile(GenericConfigObj& config, ConfigFileContents& configData)
{
    configData.Channel = ConfigFile::Channel::Create(config, this);
    if (configData.Channel == NULL)
        return false;

    configData.Preprocessing = ConfigFile::Preprocessing::Create(config, this);
    if (configData.Preprocessing == NULL)
        return false;

    configData.Hydrodynamics = ConfigFile::Hydrodynamics::Create(config, this);
    if (configData.Hydrodynamics == NULL)
        return false;

    configData.BankErosion = ConfigFile::BankErosion::Create(config, this);
    if (configData.BankErosion == NULL)
        return false;

    configData.Migration = ConfigFile::Migration::Create(config, this);
    if (configData.Migration == NULL)
        return false;

    configData.Smoothing = ConfigFile::Smoothing::Create(config, this);
    if (configData.Smoothing == NULL)
        return false;

    configData.Plotting = ConfigFile::Plotting::Create(config, this);	//DM-FEB2011	
    if (configData.Plotting == NULL)
        return false;
	
	return true;
}
