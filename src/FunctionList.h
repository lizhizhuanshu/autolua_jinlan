

#ifndef FUNCTIONLIST_H
#define FUNCTIONLIST_H

#include <qchar.h>
#include <qlistwidget.h>
#include "CustomFunction.h"

class FunctionList :public QListWidget{
  Q_OBJECT
public:
  explicit FunctionList(QWidget *parent = nullptr);
  void resetFunctionInfo(const FunctionInfo &info);
private:
  void addFuncation(int id,QString name);
signals:
  void callFunction(int id);
};

#endif // FUNCTIONLIST_H