#pragma warning(once: 4290)

#include "rvrmeander.h"
#include "interface.h"


int main(int argc, char** argv)
{
    using namespace std;
	int result = 0;

    if (argc < 2)
    {
        fprintf(stderr, "[ERROR]  No file specified\n");
        fprintf(stderr, "Usage: %s CONFIG_FILE [LOG_FILE] where LOG_FILE is optional.\n", argv[0]);
        exit(0);
    }

	bool isIni = false;
	std::string cfgFileExt = std::string(argv[1]);
	if (cfgFileExt.length() > 4)
	{
		cfgFileExt = cfgFileExt.substr(cfgFileExt.length() - 3);
		cfgFileExt[0] = toupper(cfgFileExt[0]);
		cfgFileExt[1] = toupper(cfgFileExt[1]);
		cfgFileExt[2] = toupper(cfgFileExt[2]);
		if (cfgFileExt == "INI")
			isIni = true;
	}

	char* logFile = NULL;
    if (argc > 2)
		logFile = argv[2];

	if (isIni)
		result = RVRDLL_Run(argv[1], logFile);
	else
        result = RVRDLL_Run_libconfig(argv[1], logFile);

    if (result < 0)
        return 1;
    else
        return 0;
}
