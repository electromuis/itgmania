#include "global.h"
#include "RageFileManager.h"
#include "PluginDriver_Win32.h"
#include "RageLog.h"

LoadedPluginWin32::LoadedPluginWin32(RString libraryPath)
	:LoadedPluginLibrary(libraryPath)
{
	
}

void PluginDriver_Win32::GetAvailablePlugins(std::vector<LoadedPlugin*>& out)
{
	PluginDriver::GetAvailablePlugins(out);

	std::vector<RString> files = std::vector<RString>();
	FILEMAN->GetDirListing("Plugins\\*.dll", files, false, true);

	for (RString file : files)
	{
		file = FILEMAN->ResolvePath(file);
		file = file.substr(1);

		try {
			LoadedPlugin* lp = new LoadedPluginWin32(file);
			out.push_back(lp);
		}
		catch (std::exception e) {
			LOG->Info("Failed loading plugin (Lib): %s, exception: %s", file.c_str(), e.what());
		}
		catch (std::string e) {
			LOG->Info("Failed loading plugin (Lib): %s, exception: %s", file.c_str(), e.c_str());
		}
		catch (...) {
			LOG->Info("Failed loading plugin (Plug): %s", file.c_str());
		}
	}
}
