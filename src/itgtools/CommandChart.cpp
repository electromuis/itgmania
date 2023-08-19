#include "global.h"
#include "CommandChart.h"

#include <iostream>
#include <fstream>

#include "Song.h"
#include "Steps.h"

#include "NotesLoaderDWI.h"
#include "NotesLoaderJson.h"
#include "NotesLoaderSM.h"
#include "NotesLoaderSMA.h"
#include "NotesLoaderSSC.h"

#include "NotesWriterDWI.h"
#include "NotesWriterJson.h"
#include "NotesWriterSM.h"
#include "NotesWriterSSC.h"

#include "RageFile.h"

bool ReadSimfile(Song& song, std::string inputFile, std::string inputFormat)
{
    RageFile f;
    if( !f.Open( "/@mem/tmpchart", RageFile::WRITE ) ) {
        return false;
    }

    if(inputFile.length() == 0) {
        std::string line;
		bool first = true;

        while (std::getline(std::cin, line)) {
			if(!first)
				f.Write("\n");

			first = false;
            f.Write(line);
        }
    }
    else {
        std::ifstream file;
        file.open(inputFile);
        if (!file.is_open()) {
            return false;
        }

        std::string line;
		bool first = true;
        while (std::getline (file, line)) {
			if (!first)
				f.Write("\n");

			first = false;
            f.Write(line);
        }
    }

    if(inputFormat == "dwi") {
        Steps* steps = song.CreateSteps();
        if(!DWILoader::LoadNoteDataFromSimfile("/@mem/tmpchart", *steps)) {
            return false;
        }

        return true;
    }

    else if(inputFormat == "json") {
        if(!NotesLoaderJson::LoadFromJsonFile("/@mem/tmpchart", song)) {
            return false;
        }

        return true;
    }

    else if(inputFormat == "sm") {
        SMLoader smloader;
        if(!smloader.LoadFromSimfile("/@mem/tmpchart", song)) {
            return false;
        }

        return true;
    }

    else if(inputFormat == "sma") {
        SMALoader smaloader;
        if(!smaloader.LoadFromSimfile("/@mem/tmpchart", song)) {
            return false;
        }

        return true;
    }

    else if(inputFormat == "ssc") {
        SSCLoader sscloader;
        if(!sscloader.LoadFromSimfile("/@mem/tmpchart", song)) {
            return false;
        }

        return true;
    }

    return false;
}

bool WriteSimfile(Song& song, std::string outputFormat, RString& out)
{
	RageFile f;
	if (!f.Open("/@mem/tmpchartout", RageFile::WRITE)) {
		return false;
	}

	if (outputFormat == "sm") {
		if (!NotesWriterSM::Write(f, song, song.GetAllSteps())) {
			return false;
		}
	}

	else if (outputFormat == "ssc") {
		if (!NotesWriterSSC::Write("/@mem/tmpchartout", song, song.GetAllSteps(), false)) {
			return false;
		}
	}

	else if (outputFormat == "json") {
		if (!NotesWriterJson::WriteSong("/@mem/tmpchartout", song, true)) {
			return false;
		}
	}

	else {
		return false;
	}

	if (!f.Open("/@mem/tmpchartout", RageFile::READ)) {
		return false;
	}

	f.Seek(0);
	f.Read(out);

	return true;
}

int CommandConvert::run()
{
    Song song;
    if(!ReadSimfile(song, inputFile.value_or(""), inputFormat)) {
        std::cerr << "Reading failed\n";
        return 1;
    }

	song.m_sSongFileName = "";

	RString result;
	if (!WriteSimfile(song, outputFormat, result)) {
		std::cerr << "Writing failed\n";
		return 1;
	}

	std::cout << result;

    return 0;
}

int CommandHash::run()
{
    Song song;
    if(!ReadSimfile(song, inputFile.value_or(""), inputFormat)) {
        std::cerr << "Reading failed\n";
        return 1;
    }

    bool first = true;
    for(auto& c : song.GetAllSteps()) {
        if(!first)
            std::cout << std::endl;

        first = false;
        RString hash = c->GetHash(hashVersion);
        std::cout << hash;
    }

    return 0;
}
