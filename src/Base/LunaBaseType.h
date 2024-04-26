//
// Created by vitor on 24/04/24.
//

#ifndef LUNALANG_LUNABASETYPE_H
#define LUNALANG_LUNABASETYPE_H

#include <string>

namespace Luna::Internal::Base {

    template<typename T>
    struct LunaBaseType {
        virtual ~LunaBaseType() = default;

        [[nodiscard]] virtual std::string toString() const {
            return "";
        }

        virtual T get() const = 0;
    };

    template<typename T>
    class Object : LunaBaseType<T> {
    public:
        explicit Object(T data) : m_Data(data) {}

        T get() const {
            return m_Data;
        }

    private:
        T m_Data;
    };
}

#endif //LUNALANG_LUNABASETYPE_H
