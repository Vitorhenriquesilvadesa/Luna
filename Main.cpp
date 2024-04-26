#include <iostream>

#include "src/Util/FileReader.h"
#include "src/Mercury/Mercury.h"
#include "src/Venus/Venus.h"

using namespace Luna::Internal::Util;
using namespace Luna::Internal::Mercury;
using namespace Luna::Internal::Venus;


int main() {
    FileReader reader = FileReader("/home/vitor/Github/LunaLang/src/SourceFiles/test.luna");
    std::string sourceCode = reader.readAllLines();
    Mercury lexer = Mercury(sourceCode);
    std::vector<Token> tokens = lexer.scanTokens();
    Venus parser = Venus(tokens);
    std::vector<std::shared_ptr<Statement>> statements = parser.parseTokens();

    for (const std::shared_ptr<Statement> &statement: statements) {
        std::cout << statement->toString() << std::endl;
    }

    return 0;
}
