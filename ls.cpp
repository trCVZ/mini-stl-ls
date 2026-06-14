#include <iomanip>
#include <iostream>
#include <filesystem>
#include <string>
#include <expected>
#include <chrono>

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

std::string permissionsToString(std::filesystem::perms p) {
    std::string s;

    using std::filesystem::perms;

    s += (p & perms::owner_read) != perms::none ? "r" : "-";
    s += (p & perms::owner_write) != perms::none ? "w" : "-";
    s += (p & perms::owner_exec) != perms::none ? "x" : "-";

    s += (p & perms::group_read) != perms::none ? "r" : "-";
    s += (p & perms::group_write) != perms::none ? "w" : "-";
    s += (p & perms::group_exec) != perms::none ? "x" : "-";

    s += (p & perms::others_read) != perms::none ? "r" : "-";
    s += (p & perms::others_write) != perms::none ? "w" : "-";
    s += (p & perms::others_exec) != perms::none ? "x" : "-";

    return s;
}

std::string fileTypeToString(const std::filesystem::file_type type) {
    namespace fs = std::filesystem;

    switch (type) {
        case fs::file_type::none:
            return "?";

        case fs::file_type::not_found:
            return "!";

        case fs::file_type::regular:
            return "-";

        case fs::file_type::directory:
            return "d";

        case fs::file_type::symlink:
            return "l";

        case fs::file_type::block:
            return "b";

        case fs::file_type::character:
            return "c";

        case fs::file_type::fifo:
            return "p";

        case fs::file_type::socket:
            return "s";

        case fs::file_type::unknown:
            return "u";

        default:
            return "?";
    }
}

std::string lastWriteDate(const std::filesystem::directory_entry& entry) {

    const std::chrono::system_clock::time_point systemTime = std::chrono::clock_cast<std::chrono::system_clock>(entry.last_write_time());

    return std::format("{:%d/%m/%Y}", floor<std::chrono::days>(systemTime));
}

void printLong(const std::filesystem::directory_entry& entry) {
    std::string type = fileTypeToString(entry.status().type());
    std::string perms = permissionsToString(entry.status().permissions());

    std::uintmax_t size{entry.file_size()};

    std::string name = entry.path().filename().string();
    std::string date = lastWriteDate(entry);

    std::cout << perms << " " << type << " " << std::setw(12) <<size << " " << date << " " << name << "\n";
}

int main(const int argc, const char** argv) {

    std::expected<ParsedArguments, Error> parsedArguments = parseArguments(argc, argv);

    if (!parsedArguments) {
        std::cout << errorToString(parsedArguments.error()) << "\n";
        return 1;
    }

    std::filesystem::directory_entry dir{parsedArguments->dir};
    int options{parsedArguments->options};

    for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator{dir}) {
        std::string entryString{entry.path().filename().string()};
        if (!entryString.starts_with(".") || (options & 2) == 2) {
            if ((options & 1) == 1) {
                printLong(entry);
            }
            else {
                std::cout << entryString << " ";
            }
        }
    }

    std::cout << "\n";
    return 0;
}
