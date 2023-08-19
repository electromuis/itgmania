#include "global.h"
#include "RageUtil.h"

#include <iostream>
#include "CommandChart.h"

#include "RageLog.h"
#include "ThemeManager.h"
#include "RageFileManager.h"
#include "PrefsManager.h"
#include "GameState.h"
#include "CryptManager.h"

// struct CommandMain : public argparse::Args {
//     CommandChart& chart = subcommand("chart");
// };

void Init(const char** argv)
{
    LUA		= new LuaManager;

    FILEMAN = new RageFileManager(argv[0]);

    LOG = new RageLog(false, true);
    CRYPTMAN = new CryptManager;
    PREFSMAN = new PrefsManager;
    GAMESTATE = new GameState;
    THEME = new ThemeManager;
    THEME->ReloadSubscribers();
}

void Deinit()
{
    SAFE_DELETE( THEME );
    SAFE_DELETE( GAMESTATE );
    SAFE_DELETE( CRYPTMAN );
    SAFE_DELETE( PREFSMAN );
    SAFE_DELETE( LOG );
    SAFE_DELETE( FILEMAN );
	SAFE_DELETE( LUA );
}

int main(int argc, const char** argv)
{
    Init(argv);
    auto args = argparse::parse<CommandChart>(argc, argv);

    auto ret = args.run_subcommands();
    Deinit();

    return ret;
}
