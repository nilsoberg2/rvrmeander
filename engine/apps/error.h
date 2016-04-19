#ifndef __ERROR_CODES_H__20100330__
#define __ERROR_CODES_H__20100330__


#define RVR_ERR_UNABLE_TO_LOAD_FILE         -100
#define RVR_ERR_UNABLE_TO_CREATE_LOG        -101


#define CONFIG_ERR(val)     { logger->logmsg("[ERROR]  Loading config file, invalid type for %s\n", val); }


#endif//__ERROR_CODES_H__20100330__
