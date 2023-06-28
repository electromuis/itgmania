#ifndef PluginDriver_H
#define PluginDriver_H

#include <vector>
#include <memory>

#define DYNALO_DEMANGLE
#define DYNALO_EXPORT_SYMBOLS
#include "dynalo/symbol_helper.hpp"
#include "dynalo/dynalo.hpp"
using namespace dynalo;

#include "LoadedPlugin.h"

class DYNALO_EXPORT PluginBase;
DYNALO_EXPORT typedef PluginBase* (*GetPluginFunc)();

struct PluginDetails
{
	const char* apiVersion;
	const char* fileName;
	const char* className;
	const char* pluginName;
	const char* pluginVersion;
	const char* pluginAuthor;
	GetPluginFunc initializeFunc;
};

#define PLUGIN_API_VERSION "test"
#define STANDARD_PLUGIN_STUFF PLUGIN_API_VERSION, __FILE__

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)


#define REGISTER_DETAILS(classType, pluginVersion, pluginAuthor)        \
DYNALO_EXPORT PluginDetails exports = {									\
	PLUGIN_API_VERSION,													\
	__FILE__,															\
	#classType,                                                         \
	STRINGIFY(classType),                                               \
	pluginVersion,													    \
	pluginAuthor,                                                       \
	getPlugin_##classType												\
};

#define REGISTER_PLUGIN(classType, pluginVersion, pluginAuthor)								\
extern "C" {																				\
    DYNALO_EXPORT PluginBase* getPlugin_##classType()										\
    {																						\
        return new classType();																\
    }																						\
    REGISTER_DETAILS(classType, pluginVersion, pluginAuthor)								\
}

class PluginDriver
{
public:
	virtual ~PluginDriver() {};
	static PluginDriver* Create();
	virtual void GetAvailablePlugins(std::vector<LoadedPlugin*>& out) = 0;
};

#endif
