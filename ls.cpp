#include <iostream>
#include <filesystem>
#include <string>
#include <expected>

enum Error {
    NotADirectory,
    InvalidOption,
    TooManyDirectory
};

std::string errorToString(Error e) {
    switch (e) {
        case Error::NotADirectory:   
            return "Not a directory";
        case Error::InvalidOption:
            return "Invalid options";
        case Error::TooManyDirectory:
            return "Too many directory";
        default:                     
            return "Unknown error";
    }
}


struct ParsedArguments {
    std::filesystem::directory_entry dir;
    int options;
};

std::expected<ParsedArguments, Error> parseArguments(const int argc, const char** argv) {
    int options{};

    std::string_view dirArg = ".";
    int countDirArg{};

    int i{1};

    while (i < argc && std::string_view{argv[i]} != "--") {
        std::string_view currentArg = argv[i];

        if (currentArg == "-l") {
            options |= 1;
        }
        else if (currentArg == "-a") {
            options |= 2;
        }
        else if (currentArg.starts_with("-")) {
            return std::unexpected(InvalidOption);
        }
        else {
            dirArg = currentArg; 
            countDirArg++;
            if (countDirArg > 1) {
                return std::unexpected(TooManyDirectory);
            }
        }
        i++;
    }
   
    i++;
    while (i < argc) {
        std::string_view currentArg = argv[i];

        dirArg = currentArg;
        countDirArg++;
        if (countDirArg > 1) {
            return std::unexpected(TooManyDirectory);
        }
        i++;
    }

    std::filesystem::directory_entry dir{dirArg};
    if (!dir.is_directory()) {
        return std::unexpected(NotADirectory);
    }

    return ParsedArguments{dir, options};
}


int main(const int argc, const char** argv) {

    std::expected<ParsedArguments, Error> parsedArguments = parseArguments(argc, argv);

    if (!parsedArguments) {
        std::cout << errorToString(parsedArguments.error()) << "\n";
        return 1;
    }

    std::filesystem::directory_entry dir{parsedArguments->dir};
    int options{parsedArguments->options};

    std::string sep = (options & 1) == 1 ? "\n" : " ";

    for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator{dir}) {
        std::string entryString{entry.path().filename().string()};
        if (!entryString.starts_with(".") || (options & 2) == 2) {
            std::cout << entryString << sep;
        }
    }

    return 0;
}
