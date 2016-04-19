#pragma warning(once : 4290)

#include <string>


std::string fileName(const std::string& file)
// Analogous to POSIX basename(3), but for C++ string-object pathnames
{
#ifdef _WIN32
	static const std::string dir_separators = "\\/:";
#else
	static const std::string dir_separators = "/";
#endif

	std::string::size_type pos = file.find_last_of(dir_separators);

	if(pos == std::string::npos)
		return file;
	if(pos >= file.size()-1)
		return "";

	return file.substr(pos+1);
}

std::string dirPath(const std::string& file)
// Analogous to POSIX dirname(3), but for C++ string-object pathnames
{
#ifdef _WIN32
	static const std::string dir_separators = "\\/:";
#else
	static const std::string dir_separators = "/";
#endif

	std::string::size_type pos = file.find_last_of(dir_separators);

	if(pos == std::string::npos)
		return "";

	return file.substr(0,pos+1);
}

std::string resolvePath(const std::string& progDir, const std::string& file)
{
#ifdef _WIN32
	
	std::string::size_type pos1 = file.find(":\\");
	std::string::size_type pos2 = file.find(":/");
	
	// Absolute path
	if (pos1 == 1 || pos2 == 1 || progDir.length() == 0)
		return file;
	else
		return progDir + "\\" + file;

#else
	
	// Absolute path
	if ((file.length() > 0 && file.substr(0, 1) == "/") || progDir.length() == 0)
		return file;
	else
		return progDir + "/" + file;

#endif
}
