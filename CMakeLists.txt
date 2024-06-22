cmake_minimum_required(VERSION 3.20)

project(autolua_jinlan)


set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS Core Widgets Gui Svg)



add_definitions(-DLUA_DL_DLOPEN -DLUA_COMPAT_5_3 -DLUA_USE_LINUX)
set(MY_LUA_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rd/lua)
include_directories(${MY_LUA_DIR})
aux_source_directory(${MY_LUA_DIR}  LUA_DIR)
list(REMOVE_ITEM LUA_DIR "${MY_LUA_DIR}/lua.c"
        "${MY_LUA_DIR}/luac.c" "${MY_LUA_DIR}/onelua.c")
add_library(lua_static STATIC ${LUA_DIR})

add_subdirectory(3rd/sol2/single)

include_directories(${CMAKE_BINARY_DIR}/3rd/sol2/single/include)



set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
add_executable(autolua_jinlan ./src/main.cc 
  ./src/MainWindow.cc
  ./src/ToolbarWidget.cc
  ./src/ImageWidget.cc
  ./src/MagnifierView.cc
  ./src/PointItem.cc
  ./src/FunctionList.cc
  ./src/LuaInterpreter.cc
  ./src/DataModel.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/resources/resources.qrc)

target_link_libraries(autolua_jinlan lua_static  Qt5::Core Qt5::Widgets Qt5::Gui Qt5::Svg)