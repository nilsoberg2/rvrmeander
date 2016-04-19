#ifndef __CONFIG_FILE_H__20100310__
#define __CONFIG_FILE_H__20100310__

#pragma warning(disable : 4290)

#include <string>
#include "config_file_obj.h"
#include "cf_bankerosion.h"
#include "cf_channel.h"
#include "cf_hydrodynamics.h"
#include "cf_migration.h"
#include "cf_plotting.h"
#include "cf_preprocessing.h"
#include "cf_smoothing.h"
#include "error.h"


struct ConfigFileContents
{
    ConfigFile::Channel* Channel;
    ConfigFile::Preprocessing* Preprocessing;
    ConfigFile::Hydrodynamics* Hydrodynamics;
    ConfigFile::BankErosion* BankErosion;
    ConfigFile::Migration* Migration;
    ConfigFile::Smoothing* Smoothing;
    ConfigFile::Plotting* Plotting;

    ConfigFileContents();
    ~ConfigFileContents();
};


#endif//__CONFIG_FILE_H__20100310__
