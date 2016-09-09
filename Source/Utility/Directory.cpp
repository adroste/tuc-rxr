#include "Directory.h"
#include <dirent.h>

std::vector< std::string > GetDirectoryFilenames(const std::string& folder)
{
	std::vector< std::string > names;

	DIR* dir;
	struct dirent* ent;
	if ((dir = opendir(folder.c_str())) != nullptr)
	{
		while ((ent = readdir(dir)) != nullptr)
		{
			auto s = std::string(ent->d_name);
			if(s != "." && s != "..")
				names.push_back(std::move(s));
		}
			

		closedir(dir);
	}
	return names;
}