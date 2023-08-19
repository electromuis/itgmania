#pragma once
#include "global.h"

#include "argparse/argparse.hpp"
#include <string>
#include "Steps.h"

struct CommandConvert : public argparse::Args {
    std::optional<std::string>& inputFile      = arg("Input file");
    std::optional<std::string>& outputFile     = arg("Output file");
    std::string& inputFormat    = kwarg("if,input_format", "Chart input format");
    std::string& outputFormat   = kwarg("of,output_format", "Chart output format");

    int run();
};

struct CommandHash : public argparse::Args {
    std::optional<std::string>& inputFile       = arg("Input file");
    std::string& inputFormat                    = kwarg("if,input_format", "Chart input format");
    HashType& hashVersion                       = kwarg("version", "Hash version to calculate").set_default(HASH_TYPE_GS3);

    int run();
};

struct CommandChart : public argparse::Args {
    CommandConvert& convert     = subcommand("convert");
    CommandHash& hash           = subcommand("hash");
};

