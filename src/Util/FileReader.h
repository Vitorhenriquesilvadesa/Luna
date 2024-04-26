//
// Created by vitor on 24/04/24.
//

#ifndef LUNALANG_FILEREADER_H
#define LUNALANG_FILEREADER_H

#include <string>
#include <fstream>


namespace Luna::Internal::Util {

    class FileReader {
    public:
        explicit FileReader(const std::string &path);

        std::string getFileContent() { return fileContent; }

        std::string readLine();

        std::string readAllLines();

        void close();

        char readChar();

        bool isOpen();

        std::streampos fileSize();

    private:
        std::string fileContent;
        std::string filePath;
        std::ifstream fileStream;

        void openIfIsClosed();
    };

} // Internal
// Luna

#endif //LUNALANG_FILEREADER_H
