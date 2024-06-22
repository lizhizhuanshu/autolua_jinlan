

#include "LuaInterpreter.h"
#include "DataModel.h"
#include "Configure.h"


#include <qdebug.h>
#include <sol/sol.hpp>

#include <qglobal.h>
#include <QDebug>
#include <QApplication>
#include <QClipboard>
#include <QSettings>
#include <QFile>
#include<QResource>

#include <sstream>
#include <iostream>
#include <iomanip>

LuaInterpreter::LuaInterpreter(DataModelPtr dataModel)
  :dataModel(std::move(dataModel)) {
  initLua();
  initFunctions();
}

void LuaInterpreter::initLua(){
  lua.open_libraries(sol::lib::base, 
  sol::lib::package, 
  sol::lib::string,
  sol::lib::table,
  sol::lib::io,
  sol::lib::os,
  sol::lib::math);

  lua.set_function("print", [this](const std::string &str){
    qDebug()<<str.c_str();
  });

  lua.set_function("ShowMessage",[](const std::string&str){
    
  });

  lua.set_function("SetClipboard", [this](const std::string &str){
    qDebug()<<"SetClipboard"<<str.c_str();
    QApplication::clipboard()->setText(QString::fromStdString(str));
  });

  lua.set_function("GetSelectedRect", [this](){
    auto rect = this->dataModel->selectedRect();
    return std::make_tuple(rect.x1, rect.y1, rect.x2, rect.y2);
  });

  lua.new_usertype<MPointFeature>("PointFeature",
    "x",&MPointFeature::x,
    "y",&MPointFeature::y,
    "color",&MPointFeature::color);


  lua.new_usertype<MFeature>("Feature",
    "size",&MFeature::size,
    "__len",&MFeature::size,
    "__index",[this](MFeature &f, int i){
      if(i < 1 || i > f.size()){
        return sol::make_object(this->lua,sol::nil);
      }
      return sol::make_object(this->lua, f[i-1]);
    });
  

  lua.set_function("GetFeature", [this](){
    return this->dataModel->feature();
  });
}



bool LuaInterpreter::loadFunctionsFromString(const std::string &code) {
  auto r = lua.script(code);
  if(!r.valid()){
    setError(r.get<std::string>());
    return false;
  }
  try{
    FunctionInfo info;
    info.name = lua["Name"];
    info.version = lua["Version"];
    sol::table functions = lua["Functions"];
    info.functions.clear();
    functions.for_each([&info](const sol::object& key, sol::table value){
      FunctionItem item;
      item.id = key.as<int>();
      item.name = value["name"];
      info.functions.push_back(item);
    });
    this->info = info;
    return true;
  }catch(const sol::error &e){
    setError(e.what());
    return false;
  }
}

void LuaInterpreter::initFunctions(){
  QSettings settings(autolua::CompanyName,autolua::AppName);
  settings.beginGroup("CustomFunction");
  std::string code;
  if(settings.contains("filePath")){
    if(reloadFunction(settings.value("filePath").toString().toStdString())){
      return;
    }
    settings.remove("filePath");
    qDebug()<<"loadFunctions failed";
  }
  settings.endGroup();
  if(!reloadFunction(":/functions.lua")){
    qDebug()<<"load default functions failed";
  }
}

bool LuaInterpreter::reloadFunction(const std::string &path) {
  QFile file(QString::fromStdString(path));
  if(!file.open(QIODevice::ReadOnly)){
    qDebug()<<"open file failed"<<path.c_str()<<"error"<<file.errorString();
    return false;
  }
  auto code = file.readAll().toStdString();
  auto r = loadFunctionsFromString(code);
  if(r){
    QSettings settings(autolua::CompanyName,autolua::AppName);
    settings.beginGroup("CustomFunction");
    settings.setValue("filePath",QString::fromStdString(path));
    settings.endGroup();
  }
  return r;
}

bool LuaInterpreter::callFunction(int id) {
  auto f = lua["Functions"][id]["method"];
  if(f.valid()){
    auto r = f();
    if(!r.valid()){
      sol::error e = r;
      setError(e.what());
      return false;
    }
    return true;
  }
  setError("Function not found");
  return false;
}

std::string LuaInterpreter::getError() {
  return error;
}

std::string LuaInterpreter::encodeColor(uint32_t color) {
  auto f = lua["EncodeColor"];
  if(f.get_type() == sol::type::function){
    auto r = f(color);
    if(r.valid()){
      return r.get<std::string>();
    }
  }
  std::ostringstream oss;
  oss<<"0x"<<std::setw(6)<<std::setfill('0')<<std::hex<<color;
  return oss.str();
}

std::string LuaInterpreter::encodePoint(int16_t x, int16_t y) {
  auto f = lua["EncodePoint"];
  if(f.get_type() == sol::type::function){
    auto r = f(x,y);
    if(r.valid()){
      return r.get<std::string>();
    }
  }
  std::ostringstream oss;
  oss<<x<<","<<y;
  return oss.str();
}

std::string LuaInterpreter::encodeRect(int16_t x1, int16_t y1, int16_t x2, int16_t y2) {
  auto f = lua["EncodeRect"];
  if(f.get_type() == sol::type::function){
    auto r = f(x1,y1,x2,y2);
    if(r.valid()){
      return r.get<std::string>();
    }
  }
  std::ostringstream oss;
  oss<<x1<<","<<y1<<","<<x2<<","<<y2;
  return oss.str();
}

std::string LuaInterpreter::encodePointColor(int16_t x, int16_t y, uint32_t color) {
  auto f = lua["EncodePointColor"];
  if(f.get_type() == sol::type::function){
    auto r = f(x,y,color);
    if(r.valid()){
      return r.get<std::string>();
    }
  }
  std::ostringstream oss;
  oss<<x<<","<<y<<","<<"0x"<<std::setw(6)<<std::setfill('0')<<std::hex<<color;
  return oss.str();
}

void LuaInterpreter::clearError() {
  error.clear();
}

FunctionInfo &LuaInterpreter::functionInfo() {
  return info;
}