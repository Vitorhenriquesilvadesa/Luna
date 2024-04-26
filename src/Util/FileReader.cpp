//
// Created by vitor on 24/04/24.
//

#include <fstream>
#include <iostream>
#include "../Error/Error.h"
#include "FileReader.h"


namespace Luna::Internal::Util {


    FileReader::FileReader(const std::string &path) : filePath(path) {
        std::ifstream file(path);
    }

    std::string FileReader::readLine() {
        openIfIsClosed();
        if (fileStream.is_open()) {
            std::string line;
            std::getline(fileStream, line);
            return line;
        }

        ERROR("Unable to read file: " + filePath);
    }

    std::string FileReader::readAllLines() {
        openIfIsClosed();
        if (fileStream.is_open()) {
            std::string line;
            while (std::getline(fileStream, line)) {
                fileContent.append(line);
                fileContent.append("\n");
            }
            fileStream.close();
            return fileContent;
        }
        ERROR("Unable to read file: " + filePath);
    }

    void FileReader::close() {
        if (fileStream.is_open()) fileStream.close();
    }

    char FileReader::readChar() {
        openIfIsClosed();
        if (fileStream.is_open()) {
            char c;
            fileStream.get(c);
            return c;
        }

        ERROR("Unable to open file: " + filePath);
    }

    bool FileReader::isOpen() {
        return fileStream.is_open();
    }

    std::streampos FileReader::fileSize() {
        openIfIsClosed();
        if (fileStream.is_open()) {
            fileStream.seekg(0, std::ios::end);
            const std::streampos size = fileStream.tellg();
            fileStream.seekg(0, std::ios::beg);
            return size;
        }
        ERROR("Cannot get the file size because file is closed.");
    }

    void FileReader::openIfIsClosed() {
        if (!fileStream.is_open()) {
            fileStream.open(filePath);
        }
    }
} // Internal
// Luna