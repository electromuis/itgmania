set(ITGTOOLS_DIR "${SM_SRC_DIR}/itgtools")

list(APPEND ITGTOOLS_SRC
            "${ITGTOOLS_DIR}/CliMain.cpp"
            "${ITGTOOLS_DIR}/CommandChart.cpp"
            "${ITGTOOLS_DIR}/Proxies.cpp"
            )

list(APPEND ITGTOOLS_HPP
            "${ITGTOOLS_DIR}/CommandChart.h")

source_group("Source Files" FILES ${ITGTOOLS_SRC} ${ITGTOOLS_HPP})

list(APPEND ITGTOOLS_DEPENDANCIES
    "global.cpp"
    "global.h"
    "${SM_SRC_DIR}/generated/verstub.cpp"

    "Song.cpp"
    "Song.h"
    # "SongCacheIndex.cpp"
    # "SongOptions.cpp"
    # "SongPosition.cpp"
    # "SongUtil.cpp"
    # "SongUtil.h"

    "NoteData.cpp"
    "NoteData.h"
    "NoteDataUtil.cpp"
    "NoteDataUtil.h"
    
    "Steps.cpp"
    "Steps.h"
    "StepsUtil.cpp"
    "StepsUtil.h"
    "Style.cpp"
    "Style.h"
    # "StyleUtil.cpp"
    # "StyleUtil.h"

    "LuaBinding.cpp"
    "LuaBinding.h"
    "LuaReference.cpp"
    "LuaReference.h"

    # "BackgroundUtil.cpp"
    # "BackgroundUtil.h"
    "Difficulty.cpp"
    "Difficulty.h"
    "EnumHelper.cpp"
    "EnumHelper.h"
    "Game.cpp"
    "Game.h"
    "JsonUtil.cpp"
    "JsonUtil.h"
    "GameConstantsAndTypes.cpp"
    "GameConstantsAndTypes.h"
    "LocalizedString.cpp"
    "LocalizedString.h"
    "NoteTypes.cpp"
    "NoteTypes.h"
    "PlayerNumber.cpp"
    "PlayerNumber.h"
    "Preference.cpp"
    "Preference.h"
    "RadarValues.cpp"
    "RadarValues.h"
    "TimingData.cpp"
    "TimingData.h"
    "TimingSegments.cpp"
    "TimingSegments.h"
    "TitleSubstitution.cpp"
    "TitleSubstitution.h"
    "Trail.cpp"
    "Trail.h"

    "CryptHelpers.cpp"
    "CryptHelpers.h"

    "NotesLoaderDWI.cpp"
    "NotesLoaderDWI.h"
    "NotesLoaderJson.cpp"
    "NotesLoaderJson.h"
    "NotesLoaderSM.cpp"
    "NotesLoaderSM.h"
    "NotesLoaderSMA.cpp"
    "NotesLoaderSMA.h"
    "NotesLoaderSSC.cpp"
    "NotesLoaderSSC.h"

    "NotesWriterDWI.cpp"
    "NotesWriterDWI.h"
    "NotesWriterJson.cpp"
    "NotesWriterJson.h"
    "NotesWriterSM.cpp"
    "NotesWriterSM.h"
    "NotesWriterSSC.cpp"
    "NotesWriterSSC.h"
    
    ${SMDATA_RAGE_MISC_SRC}
    ${SMDATA_RAGE_MISC_HPP}
    ${SMDATA_RAGE_FILE_SRC}
    ${SMDATA_RAGE_FILE_HPP}
    ${SMDATA_RAGE_UTILS_SRC}
    ${SMDATA_RAGE_UTILS_HPP}
    
    ${SMDATA_ARCH_THREADS_SRC}
    ${SMDATA_ARCH_THREADS_HPP}

    "RageSoundReader.cpp"
    "RageSoundReader.h"
    "RageSoundReader_FileReader.cpp"
    "RageSoundReader_FileReader.h"
    "RageSoundReader_MP3.cpp"
    "RageSoundReader_MP3.h"
    "RageSoundReader_WAV.cpp"
    "RageSoundReader_WAV.h"
    "RageSoundReader_Vorbisfile.cpp"
    "RageSoundReader_Vorbisfile.h"

    ${SMDATA_FILE_TYPES_SRC}
    ${SMDATA_FILE_TYPES_HPP}

    "SpecialFiles.cpp"
    "SpecialFiles.h"

    "CryptManager.cpp"
    "CryptManager.h"
    "GameManager.cpp"
    "GameManager.h"
    "LuaManager.cpp"
    "LuaManager.h"
    # "PrefsManager.cpp"
    # "PrefsManager.h"
    "SongCacheIndex.cpp"
    "SongCacheIndex.h"
)

if(WIN32)
  list(APPEND ITGTOOLS_DEPENDANCIES
      "archutils/Win32/arch_time.cpp"
      "archutils/Win32/Crash.cpp"
      "archutils/Win32/Crash.h"
      "archutils/Win32/ErrorStrings.cpp"
      "archutils/Win32/ErrorStrings.h"
  )
else() # Unix
  list(APPEND ITGTOOLS_DEPENDANCIES
      "archutils/Unix/RunningUnderValgrind.cpp"
      "archutils/Unix/RunningUnderValgrind.h"
      "archutils/Common/PthreadHelpers.cpp"
      "archutils/Common/PthreadHelpers.h"
  )
endif()


set(CMAKE_MFC_FLAG 2)
add_executable("itgtools"
      ${ITGTOOLS_SRC}
      ${ITGTOOLS_HPP}
      ${ITGTOOLS_DEPENDANCIES}         
)

if(MSVC)
    foreach(sm_src_file ${SMDATA_GLOBAL_SINGLETON_SRC})
      get_filename_component(sm_src_raw "${sm_src_file}" NAME_WE)
      if(${sm_src_raw} MATCHES "global")
        set_source_files_properties("${sm_src_raw}.cpp"
                                    PROPERTIES
                                    COMPILE_FLAGS
                                    "/Ycglobal.h")
      elseif(NOT (${sm_src_raw} MATCHES "verstub"))
        set_source_files_properties("${sm_src_raw}.cpp"
                                    PROPERTIES
                                    COMPILE_FLAGS
                                    "/Yuglobal.h")
      endif()
    endforeach()
endif()

if(NOT WIN32)
  target_compile_definitions("itgtools" PRIVATE UNIX)
  if("${CMAKE_SYSTEM}" MATCHES "Linux")
    target_compile_definitions("itgtools" PRIVATE LINUX)
  endif()
endif()

set_target_properties(itgtools PROPERTIES
            CXX_STANDARD 17
            CXX_EXTENSIONS OFF
)

unset(CMAKE_MFC_FLAG)
set_property(TARGET "itgtools" PROPERTY FOLDER "Internal Libraries")

disable_project_warnings("itgtools")

list(APPEND ITGTOOLS_LINK_LIB 
      "zlib"
      "jsoncpp"
      "tomcrypt"
      "lua-5.1"
      "miniz"
      "pcre"
      "vorbisfile"
      "vorbis"
      "ogg"
      "mad"
      "jpeg"
      "png")
    
target_link_libraries("itgtools" ${ITGTOOLS_LINK_LIB})

list(APPEND ITGTOOLS_INCLUDE_DIRS
            "${ITGTOOLS_DIR}"
            "${SM_SRC_DIR}"
            "${SM_SRC_DIR}/generated"
            "${SM_EXTERN_DIR}/argparse/include")

target_include_directories("itgtools"
                           PUBLIC ${ITGTOOLS_INCLUDE_DIRS})

set_target_properties("itgtools"
                      PROPERTIES RUNTIME_OUTPUT_DIRECTORY
                                 "${SM_PROGRAM_DIR}"
                                 RUNTIME_OUTPUT_DIRECTORY_RELEASE
                                 "${SM_PROGRAM_DIR}"
                                 RUNTIME_OUTPUT_DIRECTORY_DEBUG
                                 "${SM_PROGRAM_DIR}"
                                 RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL
                                 "${SM_PROGRAM_DIR}"
                                 RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO
                                 "${SM_PROGRAM_DIR}")

set_target_properties("itgtools"
                      PROPERTIES OUTPUT_NAME
                                 "${SM_NAME_RELEASE}itgtools"
                                 RELEASE_OUTPUT_NAME
                                 "${SM_NAME_RELEASE}itgtools"
                                 DEBUG_OUTPUT_NAME
                                 "${SM_NAME_RELEASE}itgtools"
                                 MINSIZEREL_OUTPUT_NAME
                                 "${SM_NAME_RELEASE}itgtools"
                                 RELWITHDEBINFO_OUTPUT_NAME
                                 "${SM_NAME_RELEASE}itgtools")
