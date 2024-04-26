//
// Created by vitor on 25/04/24.
//

#ifndef LUNALANG_ENVIRONMENT_H
#define LUNALANG_ENVIRONMENT_H

#include <unordered_map>
#include <string>
#include <memory>
#include <utility>

#include "../Mercury/Token.h"

using namespace Luna::Internal::Mercury;

namespace Luna::Internal::Earth {

    class Environment {
    public:
        Environment() {
            this->enclosing = nullptr;
        }

        explicit Environment(std::shared_ptr<Environment> enclosing) : enclosing(std::move(enclosing)) {}

        ~Environment() = default;

        template<class T>
        std::shared_ptr<T> get(Token& name) {
            if(values.count(name.lexeme) > 0) {
                void* variableValue = values.at(name.lexeme);
                T* typedPointer = static_cast<T*>(variableValue);
                return std::shared_ptr<T>(typedPointer);
            } else {
                return nullptr;
            }
        }

        std::shared_ptr<Environment> enclosing;

    private:
        std::unordered_map<std::string, void *> values;
    };

}

#endif //LUNALANG_ENVIRONMENT_H
