#include <iostream>
#include <fstream>
#include <string>

#include "CommandParser.h"
#include "JsonObject.h"

int main(int argc, char **argv) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " [filename] " << '"' << "command" << '"' << "\n";
        return -1;
    }

    std::string command = argv[2];

    std::ifstream fin(argv[1]);
    if (!fin) {
        std::cerr << "Cannot open file.\n";
        return -1;
    }

    std::string json_str;
    std::string line;
    while (getline(fin, line)) {
        json_str += line;
        line = "";
    }

    auto json_object = JsonObject(json_str);
    auto parser = CommandParser(command, json_object);
    std::cout << parser.execute() << "\n";

    return 0;
}
