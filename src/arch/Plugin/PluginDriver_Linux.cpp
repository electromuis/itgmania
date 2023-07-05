#include "global.h"
#include "RageFileManager.h"
#include "PluginDriver_Linux.h"
#include "RageLog.h"

#include <iostream>

LoadedPluginLinux::LoadedPluginLinux(RString libraryPath)
	:LoadedPluginLibrary(libraryPath)
{

}

void PluginDriver_Linux::GetAvailablePlugins(std::vector<LoadedPlugin*>& out)
{
	PluginDriver::GetAvailablePlugins(out);

	std::vector<RString> files = std::vector<RString>();
	FILEMAN->GetDirListing("Plugins\\*.so", files, false, true);

	for (RString file : files)
	{
		file = FILEMAN->ResolvePath(file);

		try {
			LoadedPlugin* lp = new LoadedPluginLinux(file);
			out.push_back(lp);
		}
		catch (std::exception e) {
			std::cout << "E1" << e.what() << std::endl;
			LOG->Trace("Failed loading plugin (Scan): %s, exception: %s", file.c_str(), e.what());
		}
		catch (std::string e) {
			std::cout << "E2" << e << std::endl;
			LOG->Trace("Failed loading plugin (Scan): %s, exception: %s", file.c_str(), e.c_str());
		}
		catch (...) {
			std::cout << "E3" << std::endl;
			LOG->Trace("Failed loading plugin (Scan): %s", file.c_str());
		}
	}
}
