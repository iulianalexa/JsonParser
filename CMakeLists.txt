cmake_minimum_required(VERSION 3.22)
project(JsonParser)

set(CMAKE_CXX_STANDARD 20)

include_directories(${CMAKE_SOURCE_DIR})

add_executable(JsonParser main.cpp
        JsonArray.cpp
        JsonLexer.cpp
        JsonObject.cpp
        JsonExceptions.cpp
        Json.cpp
        Json.h
        JsonArray.h
        JsonExceptions.h
        JsonObject.h
        JsonLexer.h
        utils.h
        Box.h
        CommandLexer.h
        CommandLexer.cpp
        CommandExceptions.h
        CommandExceptions.cpp
        CommandBit.cpp
        CommandBit.h
        CommandParser.cpp
        CommandParser.h
        JsonValue.cpp
        JsonValue.h
)