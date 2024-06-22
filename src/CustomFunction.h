
#ifndef FUNCTION_INTERPRETER_H
#define FUNCTION_INTERPRETER_H
#include <memory>
#include <string>
#include <vector>

struct FunctionItem{
  int id;
  std::string name;
};

struct FunctionInfo {
  std::string name;
  std::string version;
  std::vector<FunctionItem> functions;
};

class CustomFunction {
public:
  virtual ~CustomFunction() = default;
  virtual bool callFunction(int id) = 0;
  virtual bool reloadFunction(const std::string &path) = 0;
  virtual FunctionInfo& functionInfo() = 0;
  virtual std::string encodeColor(uint32_t color) = 0;
  virtual std::string encodePoint(int16_t x, int16_t y) = 0;
  virtual std::string encodePointColor(int16_t x, int16_t y, uint32_t color) = 0;
  virtual std::string encodeRect(int16_t x1, int16_t y1, int16_t x2, int16_t y2) = 0;
  virtual std::string getError() = 0;
};

using CustomFunctionPtr = std::shared_ptr<CustomFunction>;

#endif // FUNCTION_INTERPRETER_H