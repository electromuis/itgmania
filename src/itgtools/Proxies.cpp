// Create empty symbols to "fix" linking errors and prevent dependany hell

#include "global.h"

#include <iostream>

#include "arch/ArchHooks/ArchHooks.h"
#include "arch/Dialog/Dialog.h"
#include "arch/InputHandler/InputHandler.h"

#if defined(UNIX)
#include "arch/ArchHooks/ArchHooks_Unix.h"
#endif

#include "ActorUtil.h"
#include "Attack.h"
#include "BackgroundUtil.h"
#include "Command.h"
#include "CommonMetrics.h"
#include "FontCharAliases.h"
#include "GameLoop.h"
#include "GameState.h"
#include "ImageCache.h"
#include "InputMapper.h"
#include "MessageManager.h"
#include "NoteSkinManager.h"
#include "NotesLoader.h"
#include "PlayerOptions.h"
#include "PrefsManager.h"
#include "ProfileManager.h"
#include "Profile.h"
#include "UnlockManager.h"
#include "RageDisplay.h"
#include "RageInput.h"
#include "RageSurface.h"
#include "RageSurface_Load.h"
#include "RageTextureManager.h"
#include "RageTextureID.h"
#include "SongManager.h"
#include "SongUtil.h"
#include "Song.h"
#include "ThemeManager.h"

#include "NotesLoaderKSF.h"
#include "NotesLoaderBMS.h"

const RString NO_SONG_BG_FILE           = "-nosongbg-";
const RString SBE_Centered              = "Centered";
const RString SBE_StretchRewind         = "StretchRewind";
const RString SBE_StretchNoLoop         = "StretchNoLoop";

const RString EDIT_STEPS_SUBDIR    = "Edits/";

bool g_bAutoRestart = false;

static const char *MusicWheelUsesSectionsNames[] = {
	"Never",
	"Always",
	"ABCOnly",
};
XToString( MusicWheelUsesSections );
StringToX( MusicWheelUsesSections );
LuaXType( MusicWheelUsesSections );

static const char *AllowW1Names[] = {
	"Never",
	"CoursesOnly",
	"Everywhere",
};
XToString( AllowW1 );
StringToX( AllowW1 );
LuaXType( AllowW1 );

static const char *MaybeNames[] = {
	"Ask",
	"No",
	"Yes",
};
XToString( Maybe );
StringToX( Maybe );
LuaXType( Maybe );

static const char *GetRankingNameNames[] = {
	"Off",
	"On",
	"List",
};
XToString( GetRankingName );
StringToX( GetRankingName );
LuaXType( GetRankingName );


static const char *RandomBackgroundModeNames[] = {
	"Off",
	"Animations",
	"RandomMovies",
};
XToString( RandomBackgroundMode );
StringToX( RandomBackgroundMode );
LuaXType( RandomBackgroundMode );

static const char *ShowDancingCharactersNames[] = {
	"Off",
	"Random",
	"Select",
};
XToString( ShowDancingCharacters );
StringToX( ShowDancingCharacters );
LuaXType( ShowDancingCharacters );

static const char *ImageCacheModeNames[] = {
	"Off",
	"LowResPreload",
	"LowResLoadOnDemand",
	"Full"
};
XToString( ImageCacheMode );
StringToX( ImageCacheMode );
LuaXType( ImageCacheMode );

static const char *HighResolutionTexturesNames[] = {
	"Auto",
	"ForceOff",
	"ForceOn",
};
XToString( HighResolutionTextures );
StringToX( HighResolutionTextures );
LuaXType( HighResolutionTextures );

static const char *AttractSoundFrequencyNames[] = {
	"Never",
	"EveryTime",
	"Every2Times",
	"Every3Times",
	"Every4Times",
	"Every5Times",
};
XToString( AttractSoundFrequency );
StringToX( AttractSoundFrequency );
LuaXType( AttractSoundFrequency );

static const char *CourseSortOrdersNames[] = {
	"Preferred",
	"Songs",
	"Meter",
	"MeterSum",
	"MeterRank",
};
XToString( CourseSortOrders );
StringToX( CourseSortOrders );
LuaXType( CourseSortOrders );

static const char *BackgroundFitModeNames[] = {
	"CoverDistort",
	"CoverPreserve",
	"FitInside",
	"FitInsideAvoidLetter",
	"FitInsideAvoidPillar",
};
XToString( BackgroundFitMode );
StringToX( BackgroundFitMode );
LuaXType( BackgroundFitMode );

static const char* ProfileSortOrderNames[] = {
	"Priority",
	"Recent",
	"Alphabetical"
};
XToString(ProfileSortOrder);
StringToX(ProfileSortOrder);
LuaXType(ProfileSortOrder);

GameState*	    GAMESTATE = nullptr;
ImageCache*     IMAGECACHE = nullptr;
InputMapper*	INPUTMAPPER = nullptr;
MessageManager* MESSAGEMAN = nullptr;
NoteSkinManager* NOTESKIN = nullptr;
PrefsManager* 	PREFSMAN = nullptr;
ProfileManager*	PROFILEMAN = nullptr;
UnlockManager*	UNLOCKMAN = nullptr;
RageDisplay*    DISPLAY = nullptr;
SongManager*    SONGMAN = nullptr;
RageTextureManager* TEXTUREMAN = nullptr;
ThemeManager*   THEME = nullptr;

bool KSFLoader::LoadNoteDataFromSimfile( const RString & cachePath, Steps &out ) { return false; }
bool BMSLoader::LoadNoteDataFromSimfile( const RString & cachePath, Steps &out ) { return false; }

void FontCharAliases::ReplaceMarkers( RString &sText ) { }

RString Command::GetName() const { return ""; }
void ParseCommands( const RString &sCommands, Commands &vCommandsOut, bool bLegacy ) {}

// Metrics

ThemeMetric<RString>		CommonMetrics::IMAGES_TO_CACHE	("Common","ImageCache");
ThemeMetric<int>			CommonMetrics::MAX_COURSE_ENTRIES_BEFORE_VARIOUS("Common","MaxCourseEntriesBeforeShowVarious");

PrefsManager::PrefsManager():
	m_sCurrentGame			( "CurrentGame",		"" ),

	m_sAnnouncer			( "Announcer",			"" ),
	m_sTheme			( "Theme",			"base" ),
	m_sDefaultModifiers		( "DefaultModifiers",		"" ),

	m_bWindowed			( "Windowed",			true ),
	m_sDisplayId			( "DisplayId", "" ),
	m_iDisplayWidth			( "DisplayWidth",		854 ),
	m_iDisplayHeight		( "DisplayHeight",		480 ),
	m_fDisplayAspectRatio		( "DisplayAspectRatio",		16/9.f ),
	m_iDisplayColorDepth		( "DisplayColorDepth",		32 ),
	m_iTextureColorDepth		( "TextureColorDepth",		32 ),
	m_iMovieColorDepth		( "MovieColorDepth",		32 ),
	m_bStretchBackgrounds		( "StretchBackgrounds",		false ),
	m_BGFitMode			( "BackgroundFitMode",		BFM_CoverPreserve),
	m_HighResolutionTextures	( "HighResolutionTextures",	HighResolutionTextures_Auto ),
	m_iMaxTextureResolution		( "MaxTextureResolution",	2048 ),
	m_iRefreshRate			( "RefreshRate",		REFRESH_DEFAULT ),
	m_bAllowMultitexture		( "AllowMultitexture",		true ),
	m_bFullscreenIsBorderlessWindow( "FullscreenIsBorderlessWindow", true ),
	m_bShowStats			( "ShowStats",			false),
	m_bShowBanners			( "ShowBanners",		true ),
	m_bShowMouseCursor		( "ShowMouseCursor",		false ),

	m_bHiddenSongs			( "HiddenSongs",		false ),
	m_bVsync			( "Vsync",			true ),
	m_FastNoteRendering		( "FastNoteRendering",		false),
	m_bInterlaced			( "Interlaced",			false ),
	m_bPAL				( "PAL",			false ),
	m_bDelayedTextureDelete		( "DelayedTextureDelete",	false ),
	m_bDelayedModelDelete		( "DelayedModelDelete",		false ),
	m_ImageCache			( "ImageCache",			IMGCACHE_LOW_RES_PRELOAD ),
	m_bFastLoad			( "FastLoad",			true ),
	m_NeverCacheList		( "NeverCacheList", ""),

	m_bOnlyDedicatedMenuButtons	( "OnlyDedicatedMenuButtons",	false ),
	m_bMenuTimer			( "MenuTimer",			false ),

	m_fLifeDifficultyScale		( "LifeDifficultyScale",	1.0f ),

	m_bRateModsAffectTweens		( "RateModsAffectFGChanges",	false ),

	m_iRegenComboAfterMiss		( "RegenComboAfterMiss",	5 ),
	m_iMaxRegenComboAfterMiss	( "MaxRegenComboAfterMiss",	5 ), // this was 10 by default in SM3.95 -dguzek
	m_bMercifulDrain		( "MercifulDrain",		false ),	// negative life deltas are scaled by the players life percentage
	m_HarshHotLifePenalty		( "HarshHotLifePenalty",	true ),
	m_bMinimum1FullSongInCourses	( "Minimum1FullSongInCourses",	false ),	// FEoS for 1st song, FailImmediate thereafter
	m_bFailOffInBeginner		( "FailOffInBeginner",		false ),
	m_bFailOffForFirstStageEasy	( "FailOffForFirstStageEasy",	false ),
	m_bMercifulBeginner		( "MercifulBeginner",		false ),
	m_bMercifulSuperMeter		( "MercifulSuperMeter",		true ),
	m_bDelayedBack			( "DelayedBack",		true ),
	m_AllowHoldForOptions		( "AllowHoldForOptions",	true ),
	m_bShowInstructions		( "ShowInstructions",		true ),
	m_bShowCaution			( "ShowCaution",		true ),
	m_bShowNativeLanguage		( "ShowNativeLanguage",		true ),
	m_iArcadeOptionsNavigation	( "ArcadeOptionsNavigation",	0 ),
	m_ThreeKeyNavigation		( "ThreeKeyNavigation",		false ),
	m_MusicWheelUsesSections	( "MusicWheelUsesSections",	MusicWheelUsesSections_ALWAYS ),
	m_iMusicWheelSwitchSpeed	( "MusicWheelSwitchSpeed",	15 ),
	m_AllowW1			( "AllowW1",			ALLOW_W1_EVERYWHERE ),
	m_bEventMode			( "EventMode",			true ),
	m_iCoinsPerCredit		( "CoinsPerCredit",		1 ),
	m_iMaxNumCredits		( "MaxNumCredits",		20 ),
	m_iSongsPerPlay			( "SongsPerPlay",		3 ),
	m_bDelayedCreditsReconcile	( "DelayedCreditsReconcile",	false ),
	m_bComboContinuesBetweenSongs	( "ComboContinuesBetweenSongs",	false ),
	m_AllowMultipleToasties		( "AllowMultipleToasties",	true ),
	m_MinTNSToHideNotes		( "MinTNSToHideNotes",		TNS_W3 ),
	m_ShowSongOptions		( "ShowSongOptions",		Maybe_NO ),
	m_bPercentageScoring		( "PercentageScoring",		true ),
	// Wow, these preference names are *seriously* long -Colby
	m_fMinPercentageForMachineSongHighScore		( "MinPercentageForMachineSongHighScore",	0.0001f ), // This is for home, who cares how bad you do?
	m_fMinPercentageForMachineCourseHighScore	( "MinPercentageForMachineCourseHighScore",	0.0001f ), // don't save course scores with 0 percentage
	m_bDisqualification		( "Disqualification",			false ),
	m_bAutogenSteps			( "AutogenSteps",			false ),
	m_bAutogenGroupCourses		( "AutogenGroupCourses",		true ),
	m_bOnlyPreferredDifficulties	( "OnlyPreferredDifficulties",		false ),
	m_bBreakComboToGetItem		( "BreakComboToGetItem",		false ),
	m_bLockCourseDifficulties	( "LockCourseDifficulties",		false ),
	m_ShowDancingCharacters		( "ShowDancingCharacters",		SDC_Random ),
	m_bUseUnlockSystem		( "UseUnlockSystem",			false ),
	m_fGlobalOffsetSeconds		( "GlobalOffsetSeconds",		-0.008f ),
	m_iProgressiveLifebar		( "ProgressiveLifebar",			0 ),
	m_iProgressiveStageLifebar	( "ProgressiveStageLifebar",		0 ),
	m_iProgressiveNonstopLifebar	( "ProgressiveNonstopLifebar",		0 ),
	m_bShowBeginnerHelper		( "ShowBeginnerHelper",			false ),
	m_bDisableScreenSaver		( "DisableScreenSaver",			true ),
	m_sLanguage			( "Language",				"" ),	// ThemeManager will deal with this invalid language
	m_sMemoryCardProfileSubdir	( "MemoryCardProfileSubdir",		"" ),
	m_iProductID			( "ProductID",				1 ),
	m_iCenterImageTranslateX	( "CenterImageTranslateX",		0 ),
	m_iCenterImageTranslateY	( "CenterImageTranslateY",		0 ),
	m_fCenterImageAddWidth		( "CenterImageAddWidth",		0 ),
	m_fCenterImageAddHeight		( "CenterImageAddHeight",		0 ),
	m_AttractSoundFrequency		( "AttractSoundFrequency",		ASF_EVERY_TIME ),
	m_bAllowExtraStage		( "AllowExtraStage",			false ),
	m_iMaxHighScoresPerListForMachine	( "MaxHighScoresPerListForMachine",	10 ),
	m_iMaxHighScoresPerListForPlayer	( "MaxHighScoresPerListForPlayer",	3 ),
	m_bAllowMultipleHighScoreWithSameName	( "AllowMultipleHighScoreWithSameName",	true ),
	m_DisableUploadDir("DisableUploadDir", false),
	m_bCelShadeModels		( "CelShadeModels",			false ),	// Work-In-Progress.. disable by default.
	m_bPreferredSortUsesGroups	( "PreferredSortUsesGroups",		true ),
	m_fDebounceCoinInputTime	( "DebounceCoinInputTime",		0 ),

	m_fPadStickSeconds		( "PadStickSeconds",			0 ),
	m_EditRecordModeLeadIn		("EditRecordModeLeadIn",		1.0f ),
	m_EditClearPromptThreshold	("EditClearPromptThreshold",		50),
	m_bForceMipMaps			( "ForceMipMaps",			false ),
	m_bTrilinearFiltering		( "TrilinearFiltering",			false ),
	m_bAnisotropicFiltering		( "AnisotropicFiltering",		false ),

	m_bSignProfileData		( "SignProfileData",			false ),
	m_ProfileSortOrder		( "ProfileSortOrder",			ProfileSortOrder_Priority ),
	m_bProfileSortOrderAscending		( "ProfileSortOrderAscending",			true ),
	m_CourseSortOrder		( "CourseSortOrder",			COURSE_SORT_SONGS ),
	m_bSubSortByNumSteps		( "SubSortByNumSteps",			false ),
	m_GetRankingName		( "GetRankingName",			RANKING_ON ),
	m_sAdditionalSongFoldersReadOnly( "AdditionalSongFoldersReadOnly",	"", nullptr, PreferenceType::Immutable ),
	m_sAdditionalSongFoldersWritable( "AdditionalSongFoldersWritable",	"", nullptr, PreferenceType::Immutable ),
	m_sAdditionalCourseFoldersReadOnly( "AdditionalCourseFoldersReadOnly",	"", nullptr, PreferenceType::Immutable ),
	m_sAdditionalCourseFoldersWritable( "AdditionalCourseFoldersWritable",	"", nullptr, PreferenceType::Immutable ),
	m_sAdditionalFoldersReadOnly	( "AdditionalFoldersReadOnly",		"", nullptr, PreferenceType::Immutable ),
	m_sAdditionalFoldersWritable	( "AdditionalFoldersWritable",		"", nullptr, PreferenceType::Immutable ),
	m_sDefaultTheme			( "DefaultTheme",			"Simply Love" ),
	m_sLastSeenVideoDriver		( "LastSeenVideoDriver",		"" ),
	m_sVideoRenderers		( "VideoRenderers",			"" ),	// StepMania.cpp sets these on first run:
	m_bSmoothLines			( "SmoothLines",			true ),
	m_iSoundWriteAhead		( "SoundWriteAhead",			0 ),
	m_iSoundDevice			( "SoundDevice",			"" ),
	m_iRageSoundSampleCountClamp	("RageSoundSampleCountClamp", 0), //some sound drivers mask the sample location number, the most popular number for this is 2^27, this causes lockup after ~50 minutes at 44.1khz sample rate
	m_iSoundPreferredSampleRate	( "SoundPreferredSampleRate",		0 ),
	m_sLightsStepsDifficulty	( "LightsStepsDifficulty",		"hard,medium" ),
	m_bLightsSimplifyBass		( "LightsSimplifyBass",		false),
	m_bAllowUnacceleratedRenderer	( "AllowUnacceleratedRenderer",		false ),
	m_bThreadedInput		( "ThreadedInput",			true ),
	m_bThreadedMovieDecode		( "ThreadedMovieDecode",		true ),
	m_sTestInitialScreen		( "TestInitialScreen",			"" ),
	m_bDebugLights			( "DebugLights",			false ),
	m_bMonkeyInput			( "MonkeyInput",			false ),
	m_sMachineName			( "MachineName",			"" ),
	m_sCoursesToShowRanking		( "CoursesToShowRanking",		"" ),
	m_MuteActions			( "MuteActions",			false ),
	m_bAllowSongDeletion		( "AllowSongDeletion",			false ),

	m_bQuirksMode			( "QuirksMode",		false ),

	m_custom_songs_enable("CustomSongsEnable", false),
	m_custom_songs_max_count("CustomSongsMaxCount", 1000), // No limit. -- 2 Unlimited
	m_custom_songs_load_timeout("CustomSongsLoadTimeout", 5.f),
	m_custom_songs_max_seconds("CustomSongsMaxSeconds", 120.f),
	m_custom_songs_max_megabytes("CustomSongsMaxMegabytes", 5.f),

	/* Debug: */
	m_bLogToDisk			( "LogToDisk",		true ),
#if defined(DEBUG)
	m_bForceLogFlush		( "ForceLogFlush",	true ),
	m_bShowLogOutput		( "ShowLogOutput",	true ),
#else
	m_bForceLogFlush		( "ForceLogFlush",	false ),
	m_bShowLogOutput		( "ShowLogOutput",	false ),
#endif
	m_bLogSkips			( "LogSkips",		false ),
	m_bLogCheckpoints		( "LogCheckpoints",	false ),
	m_bShowLoadingWindow		( "ShowLoadingWindow",	true ),
	m_bPseudoLocalize		( "PseudoLocalize",	false ),
	m_show_theme_errors		( "ShowThemeErrors",	false ),
	m_sAdditionalSongFolders	( "AdditionalSongFolders",		"", nullptr, PreferenceType::Deprecated ),
	m_sAdditionalCourseFolders	( "AdditionalCourseFolders",		"", nullptr, PreferenceType::Deprecated ),
	m_sAdditionalFolders		( "AdditionalFolders",			"", nullptr, PreferenceType::Deprecated )

{

}

PrefsManager::~PrefsManager() { }

// Game

const Style* GameState::GetCurrentStyle(PlayerNumber pn) const { return nullptr; }
void GameState::SetProcessedTimingData(TimingData * t) {}
bool GameState::IsCourseDifficultyShown( CourseDifficulty cd ) { return false; }
TimingData * GameState::GetProcessedTimingData() const { return this->processedTiming; }
GameState::GameState():
processedTiming(nullptr),
	m_pCurGame(				Message_CurrentGameChanged ),
	m_pCurStyle(			Message_CurrentStyleChanged ),
	m_PlayMode(				Message_PlayModeChanged ),
	m_iCoins(				Message_CoinsChanged ),
	m_sPreferredSongGroup(	Message_PreferredSongGroupChanged ),
	m_sPreferredCourseGroup(	Message_PreferredCourseGroupChanged ),
	m_PreferredStepsType(	Message_PreferredStepsTypeChanged ),
	m_PreferredDifficulty(	Message_PreferredDifficultyP1Changed ),
	m_PreferredCourseDifficulty(	Message_PreferredCourseDifficultyP1Changed ),
	m_SortOrder(			Message_SortOrderChanged ),
	m_pCurSong(				Message_CurrentSongChanged ),
	m_pCurSteps(			Message_CurrentStepsP1Changed ),
	m_pCurCourse(			Message_CurrentCourseChanged ),
	m_pCurTrail(			Message_CurrentTrailP1Changed ),
	m_bGameplayLeadIn(		Message_GameplayLeadInChanged ),
	m_bDidModeChangeNoteSkin(	false ),
	m_bIsUsingStepTiming(		true ),
	m_bInStepEditor(		false ),
	m_stEdit(				Message_EditStepsTypeChanged ),
	m_cdEdit(				Message_EditCourseDifficultyChanged ),
	m_pEditSourceSteps(		Message_EditSourceStepsChanged ),
	m_stEditSource(			Message_EditSourceStepsTypeChanged ),
	m_iEditCourseEntryIndex(	Message_EditCourseEntryIndexChanged ),
	m_sEditLocalProfileID(		Message_EditLocalProfileIDChanged )
{

}

GameState::~GameState() { }

bool NoteSkinManager::DoNoteSkinsExistForGame( const Game *pGame ) { return false; }
void NotesLoader::GetMainAndSubTitlesFromFullTitle( const RString &sFullTitle, RString &sMainTitleOut, RString &sSubTitleOut ) {}
bool NotesLoader::LoadFromDir( const RString &sPath, Song &out, std::set<RString> &BlacklistedImages, bool load_autosave ) { return false; }

void GameLoop::ChangeGame(const RString& new_game, const RString& new_theme) {}

RString BackgroundChange::ToString() const { return ""; }
void BackgroundUtil::AddBackgroundChange( std::vector<BackgroundChange> &vBackgroundChanges, BackgroundChange seg ) {}

const InputScheme *InputMapper::GetInputScheme() const { return m_pInputScheme; }
const char *InputScheme::GetGameButtonName( GameButton gb ) const { return ""; }

void InputHandler::Create( const RString &drivers_, std::vector<InputHandler *> &Add ) {}

bool Attack::operator== ( const Attack &rhs ) const { return false; }
Attack Attack::FromGlobalCourseModifier( const RString &sModifiers )
{
    Attack a;
    return a;
}

void Attack::GetAttackBeats( const Song *pSong, float &fStartBeat, float &fEndBeat ) const {}
bool AttackArray::ContainsTransformOrTurn() const { return false; }

// Theme

// For self-registering metrics
#include "SubscriptionManager.h"
static SubscriptionManager<IThemeMetric> g_Subscribers;

ThemeManager::ThemeManager()
{
	m_sCurThemeName = "base";
}
ThemeManager::~ThemeManager() { }

void ThemeManager::Subscribe( IThemeMetric *p )
{
	g_Subscribers.Subscribe( p );
}
void ThemeManager::Unsubscribe( IThemeMetric *p )
{
	g_Subscribers.Unsubscribe( p );
}

void ThemeManager::ReloadSubscribers()
{
	// reload subscribers
	if( g_Subscribers.m_pSubscribers )
	{
		for (IThemeMetric *metric : *g_Subscribers.m_pSubscribers)
			metric->Read();
	}
}

bool ThemeManager::IsThemeSelectable(RString const& name) { return false; }
bool ThemeManager::HasString( const RString &sMetricsGroup, const RString &sValueName ) { return false; }
RString ThemeManager::GetMetric( const RString &sMetricsGroup, const RString &sValueName ) { return ""; }
RString ThemeManager::GetString( const RString &sMetricsGroup, const RString &sValueName_ ) { return ""; }
void ThemeManager::GetMetric( const RString &sMetricsGroup, const RString &sValueName, LuaReference &valueOut )
{
	Lua* L = LUA->Get();
	lua_pushnil(L);
	valueOut.SetFromStack(L);
	LUA->Release(L);
}


// Song

bool SongManager::IsGroupNeverCached(const RString& group) const { return false; }
Song *SongManager::FindSong( RString sPath ) const { return nullptr; }
const std::vector<Song*> &SongManager::GetSongs( const RString &sGroupName ) const
{
    static const std::vector<Song*> vEmpty;
    return vEmpty;
}

int UnlockManager::SongIsLocked( const Song *pSong ) const { return 0; }
bool UnlockManager::StepsIsLocked( const Song *pSong, const Steps *pSteps ) const { return false; }

Steps* SongUtil::GetOneSteps(
	const Song *pSong,
	StepsType st,
	Difficulty dc,
	int iMeterLow,
	int iMeterHigh,
	const RString &sDescription,
	const RString &sCredit,
	unsigned uHash,
	bool bIncludeAutoGen
	) { return nullptr; }

Steps* SongUtil::GetStepsByDifficulty( const Song *pSong, StepsType st, Difficulty dc, bool bIncludeAutoGen ) { return nullptr; }

void SongUtil::AdjustDuplicateSteps( Song *pSong ) {}

bool SongCriteria::Matches( const Song *pSong ) const { return false; }

void SongID::FromSong( const Song *p )
{
	if( p )
		sDir = p->GetSongDir();
	else
		sDir = "";

	// HACK for backwards compatibility:
	// Strip off leading "/".  2005/05/21 file layer changes added a leading slash.
	if( sDir.Left(1) == "/" )
		sDir.erase( sDir.begin() );
}

// Player
const Profile* ProfileManager::GetProfile( PlayerNumber pn ) const { return nullptr; }
const Profile* ProfileManager::GetProfile( ProfileSlot slot ) const { return nullptr; }
bool ProfileManager::IsPersistentProfile( ProfileSlot slot ) const { return false; }
const RString& ProfileManager::GetProfileDir( ProfileSlot slot ) const { return ""; }

void PlayerOptions::FromString( const RString &sMultipleMods ) { }
bool PlayerOptions::FromOneModString( const RString &sOneMod, RString &sErrorOut ) { return false; }
bool PlayerOptions::ContainsTransformOrTurn() const { return false; }

int Profile::GetStepsNumTimesPlayed( const Song* pSong, const Steps* pSteps ) const { return 0; }

// Arch

void ArchHooks::MountInitialFilesystems( const RString &sDirOfExecutable ) {}
void ArchHooks::MountUserFilesystems( const RString &sDirOfExecutable ) {}

void Win32RestartProgram() {}
std::int64_t ArchHooks::GetMicrosecondsSinceStart( bool bAccurate ) { return 0; }

void Dialog::OK( RString sMessage, RString sID )
{
    std::cout << sMessage << " - " << sID << std::endl;
}

Dialog::Result Dialog::AbortRetryIgnore( RString sMessage, RString sID )
{
    std::cerr << sMessage << " - " << sID << std::endl;
    return Dialog::Result::abort;
}

void RageTextureID::Init() { }
void RageTextureID::SetFilename( const RString &fn ) { }

RageSurface::~RageSurface() {}
RageSurface *RageSurfaceUtils::LoadFile( const RString &sPath, RString &error, bool bHeaderOnly ) { return nullptr; }

RageTexture* RageTextureManager::LoadTexture( RageTextureID ID ) { return nullptr; }
bool RageTextureManager::IsTextureRegistered( RageTextureID ID ) const { return false; }

#if defined(UNIX)

namespace
{
	clockid_t g_Clock = CLOCK_REALTIME;

	void OpenGetTime()
	{
		static bool bInitialized = false;
		if( bInitialized )
			return;
		bInitialized = true;

		/* Check whether the clock is actually supported. */
		timespec ts;
		if( clock_getres(CLOCK_MONOTONIC, &ts) == -1 )
			return;

		/* If the resolution is worse than a millisecond, fall back on CLOCK_REALTIME. */
		if( ts.tv_sec > 0 || ts.tv_nsec > 1000000 )
			return;

		g_Clock = CLOCK_MONOTONIC;
	}
};

clockid_t ArchHooks_Unix::GetClock()
{
	OpenGetTime();
	return g_Clock;
}

#endif

// Messaging

Message::Message( const RString &s ) {}
Message::~Message() {}
void Message::SetParamFromStack( lua_State *L, const RString &sName ) {}
void MessageManager::Broadcast( Message &msg ) const {}
void MessageManager::Broadcast( const RString& sMessage ) const {}

void ImageCache::LoadImage( RString sImageDir, RString sImagePath ) {}

// Actor

typedef std::map<RString, FileType> etft_cont_t;
typedef std::map<FileType, std::vector<RString> > fttel_cont_t;
etft_cont_t ExtensionToFileType;
fttel_cont_t FileTypeToExtensionList;

void ActorUtil::InitFileTypeLists()
{
	// This function creates things to serve two purposes:
	// 1.  A map from extensions to filetypes, so extensions can be converted.
	// 2.  A reverse map for things that need a list of extensions to look for.
	// The first section creates the map from extensions to filetypes, then the
	// second section uses that map to build the reverse map.
	ExtensionToFileType["lua"]= FT_Lua;

	ExtensionToFileType["xml"]= FT_Xml;

	ExtensionToFileType["ini"]= FT_Ini;

	// Update RageSurfaceUtils when adding new image formats.
	ExtensionToFileType["bmp"]= FT_Bitmap;
	ExtensionToFileType["gif"]= FT_Bitmap;
	ExtensionToFileType["jpeg"]= FT_Bitmap;
	ExtensionToFileType["jpg"]= FT_Bitmap;
	ExtensionToFileType["png"]= FT_Bitmap;

	// Update RageSoundReader_FileReader when adding new sound formats.
	ExtensionToFileType["mp3"]= FT_Sound;
	ExtensionToFileType["oga"]= FT_Sound;
	ExtensionToFileType["ogg"]= FT_Sound;
	ExtensionToFileType["wav"]= FT_Sound;

	// ffmpeg takes care of loading videos, not sure whether this list should
	// have everything ffmpeg supports.
	ExtensionToFileType["avi"]= FT_Movie;
	ExtensionToFileType["f4v"]= FT_Movie;
	ExtensionToFileType["flv"]= FT_Movie;
	ExtensionToFileType["mkv"]= FT_Movie;
	ExtensionToFileType["mp4"]= FT_Movie;
	ExtensionToFileType["mpeg"]= FT_Movie;
	ExtensionToFileType["mpg"]= FT_Movie;
	ExtensionToFileType["mov"]= FT_Movie;
	ExtensionToFileType["ogv"]= FT_Movie;
	ExtensionToFileType["webm"]= FT_Movie;
	ExtensionToFileType["wmv"]= FT_Movie;

	ExtensionToFileType["sprite"]= FT_Sprite;

	ExtensionToFileType["txt"]= FT_Model;

	// When adding new extensions, do not add them below this line.  This line
	// marks the point where the function switches to building the reverse map.
	for(etft_cont_t::iterator curr_ext= ExtensionToFileType.begin();
		curr_ext != ExtensionToFileType.end(); ++curr_ext)
	{
		FileTypeToExtensionList[curr_ext->second].push_back(curr_ext->first);
	}
}

std::vector<RString> const& ActorUtil::GetTypeExtensionList(FileType ft)
{
	return FileTypeToExtensionList[ft];
}

FileType ActorUtil::GetFileType( const RString &sPath )
{
	RString sExt = GetExtension( sPath );
	sExt.MakeLower();

	etft_cont_t::iterator conversion_entry= ExtensionToFileType.find(sExt);
	if(conversion_entry != ExtensionToFileType.end())
	{
		return conversion_entry->second;
	}
	else if(sPath.size() > 0 && sPath[sPath.size()-1] == '/')
	{
		return FT_Directory;
	}
	/* Do this last, to avoid the IsADirectory in most cases. */
	else if(IsADirectory(sPath))
	{
		return FT_Directory;
	}
	return FileType_Invalid;
}
