

#include "CustomFunction.h"
#include "LuaInterpreter.h"
#include "MainWindow.h"
#include <QApplication>
#include <QStatusBar>
#include <qsettings.h>
#include "DataModel.h"
#include "Configure.h"



int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  auto dataModel = std::make_shared<DataModel>();
  auto interpreter = std::make_shared<LuaInterpreter>(dataModel);
  MainWindow w(dataModel,interpreter);
  w.show();
  return a.exec();
}