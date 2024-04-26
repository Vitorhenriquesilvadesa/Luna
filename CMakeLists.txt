cmake_minimum_required(VERSION 3.27)
project(LunaLang)

set(CMAKE_CXX_STANDARD 23)

add_executable(LunaLang Main.cpp
        src/Mercury/Mercury.cpp
        src/Mercury/Mercury.h
        src/Mercury/Token.h
        src/Base/LunaBaseType.h
        src/Error/Error.h
        src/Util/FileReader.cpp
        src/Util/FileReader.h
        src/Venus/Venus.cpp
        src/Venus/Venus.h
        src/Venus/Expression.cpp
        src/Venus/Expression.h
        src/Venus/Expression.cpp
        src/Venus/Statement.cpp
        src/Venus/Statement.h
        src/Earth/Earth.cpp
        src/Earth/Earth.h
        src/Earth/Environment.cpp
        src/Earth/Environment.h)
