#include "global.h"
/*
-----------------------------------------------------------------------------
 Class: ScreenInputOptions

 Desc: See header.

 Copyright (c) 2001-2002 by the person(s) listed below.  All rights reserved.
	Chris Danford
-----------------------------------------------------------------------------
*/

#include "ScreenInputOptions.h"
#include "RageUtil.h"
#include "RageSounds.h"
#include "ScreenManager.h"
#include "PrefsManager.h"
#include "GameConstantsAndTypes.h"
#include "StepMania.h"
#include "PrefsManager.h"
#include "RageLog.h"
#include "ThemeManager.h"


enum {
	IO_AUTO_MAP_JOYSTICKS = 0,
	IO_DEDICATED_MENU_BUTTONS,
	IO_AUTOPLAY,
	IO_DELAYED_ESCAPE,
	IO_OPTIONS_NAVIGATION,
	IO_WHEEL_SPEED,
	NUM_INPUT_OPTIONS_LINES
};

/* Hmm.  Ignore JoyAxes and Back Delayed probably belong in "key/joy config",
 * preferably alongside button configuration. */
OptionRow g_InputOptionsLines[NUM_INPUT_OPTIONS_LINES] = {
	OptionRow( "Auto Map\nJoysticks",	true, "OFF","ON (recommended)" ),
	OptionRow( "Menu\nButtons",			true, "USE GAMEPLAY BUTTONS","ONLY DEDICATED BUTTONS" ),
	OptionRow( "AutoPlay",				true, "OFF","ON" ),
	OptionRow( "Back\nDelayed",			true, "INSTANT","HOLD" ),
	OptionRow( "Options\nNavigation",	true, "SM STYLE","ARCADE STYLE" ),
	OptionRow( "Wheel\nSpeed",			true, "SLOW","NORMAL","FAST","REALLY FAST" ),
};

ScreenInputOptions::ScreenInputOptions() :
	ScreenOptions("ScreenInputOptions",false)
{
	LOG->Trace( "ScreenInputOptions::ScreenInputOptions()" );

	Init( 
		INPUTMODE_TOGETHER, 
		g_InputOptionsLines, 
		NUM_INPUT_OPTIONS_LINES,
		true );
	m_Menu.m_MenuTimer.Disable();

	SOUND->PlayMusic( THEME->GetPathToS("ScreenInputOptions music") );
}

void ScreenInputOptions::ImportOptions()
{
	m_iSelectedOption[0][IO_AUTO_MAP_JOYSTICKS]		= PREFSMAN->m_bAutoMapJoysticks ? 1:0;
	m_iSelectedOption[0][IO_DEDICATED_MENU_BUTTONS]	= PREFSMAN->m_bOnlyDedicatedMenuButtons ? 1:0;
	m_iSelectedOption[0][IO_AUTOPLAY]				= PREFSMAN->m_bAutoPlay;
	m_iSelectedOption[0][IO_DELAYED_ESCAPE]			= PREFSMAN->m_bDelayedEscape ? 1:0;
	m_iSelectedOption[0][IO_OPTIONS_NAVIGATION]		= PREFSMAN->m_bArcadeOptionsNavigation ? 1:0;
	m_iSelectedOption[0][IO_WHEEL_SPEED] =
		(PREFSMAN->m_iMusicWheelSwitchSpeed <= 5)? 0:
		(PREFSMAN->m_iMusicWheelSwitchSpeed <= 10)? 1:
		(PREFSMAN->m_iMusicWheelSwitchSpeed <= 15)? 2:3;
}

void ScreenInputOptions::ExportOptions()
{
	PREFSMAN->m_bAutoMapJoysticks		= m_iSelectedOption[0][IO_AUTO_MAP_JOYSTICKS] == 1;
	PREFSMAN->m_bOnlyDedicatedMenuButtons= m_iSelectedOption[0][IO_DEDICATED_MENU_BUTTONS] == 1;
	PREFSMAN->m_bDelayedEscape			= m_iSelectedOption[0][IO_DELAYED_ESCAPE] == 1;
	PREFSMAN->m_bAutoPlay				= m_iSelectedOption[0][IO_AUTOPLAY] == 1;
	PREFSMAN->m_bArcadeOptionsNavigation= m_iSelectedOption[0][IO_OPTIONS_NAVIGATION] == 1;
	switch(m_iSelectedOption[0][IO_WHEEL_SPEED])
	{
	case 0: PREFSMAN->m_iMusicWheelSwitchSpeed = 5; break;
	case 1: PREFSMAN->m_iMusicWheelSwitchSpeed = 10; break;
	case 2: PREFSMAN->m_iMusicWheelSwitchSpeed = 15; break;
	case 3: PREFSMAN->m_iMusicWheelSwitchSpeed = 25; break;
	default: ASSERT(0);
	}
}

void ScreenInputOptions::GoToPrevState()
{
	SCREENMAN->SetNewScreen( "ScreenOptionsMenu" );
}

void ScreenInputOptions::GoToNextState()
{
	PREFSMAN->SaveGlobalPrefsToDisk();
	GoToPrevState();
}
