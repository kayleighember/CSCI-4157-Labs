#include "TextFile.h"
#include <fstream>

TextFile::TextFile() {

}

void TextFile::Trim(std::string& str) {
    const std::string delimiters = " \f\n\r\t\v";
    str.erase(str.find_last_not_of(delimiters) + 1);
    str.erase(0, str.find_first_not_of(delimiters));
}

void TextFile::TextToStringStream(std::string filePath) {
    std::stringstream ss;
    std::ifstream fin{};
    fin.open(filePath.c_str());
    if (fin.fail()) {
        ss << "Could not open: " << filePath << std::endl;
        Log(ss.str());
        return;
    }

    std::string line;
    while (!fin.eof()) {
        getline(fin, line);
        Trim(line);
        if (line != "") { // Skip blank lines
            ss << line << std::endl;
        }
    }
    Log("Successfully read files!");
    data = ss.str();
    fin.close();
}
