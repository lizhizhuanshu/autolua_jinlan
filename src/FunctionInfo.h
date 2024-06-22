
#ifndef FUNCTION_INFO_H
#define FUNCTION_INFO_H

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


#endif // FUNCTION_INFO_H