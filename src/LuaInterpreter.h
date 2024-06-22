
#ifndef LUA_INTERPRETER_H
#define LUA_INTERPRETER_H

#include <string>

#include <sol/sol.hpp>

#include "CustomFunction.h"
#include "DataModel.h"

class LuaInterpreter:public CustomFunction{
public:
  explicit LuaInterpreter(DataModelPtr dataModel);
  bool callFunction(int id) override;
  bool reloadFunction(const std::string &path) override;
  FunctionInfo& functionInfo() override;
  std::string getError() override;
  std::string encodeColor(uint32_t color) override;
  std::string encodePoint(int16_t x, int16_t y)override;
  std::string encodePointColor(int16_t x, int16_t y, uint32_t color)override;
  std::string encodeRect(int16_t x1, int16_t y1, int16_t x2, int16_t y2)override;
private:
  FunctionInfo info;
  DataModelPtr dataModel;
  sol::state lua;
  std::string error;
  void setError(const std::string &error){
    this->error = error;
  }
  void clearError();
  void initLua();
  bool loadFunctionsFromString(const std::string &code);
  void initFunctions();
};


#endif // LUA_INTERPRETER_H