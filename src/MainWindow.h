
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "CustomFunction.h"
#include"DataModel.h"
#include"ImageWidget.h"
#include "ToolbarWidget.h"
#include <QHBoxLayout>
#include <QMainWindow>
#include <QSplitter>
#include <QWidget>

class MainWindow :public QMainWindow 
{
  Q_OBJECT
public:
  explicit MainWindow(DataModelPtr dataModel,  CustomFunctionPtr interpreter,
   QWidget *parent = nullptr);
protected:
  void closeEvent(QCloseEvent *event) override;
private:
  QHBoxLayout* layout;
  ImageWidget* leftWidget;
  ToolbarWidget* rightToolbar;
  CustomFunctionPtr interpreter;
  void initializeSize();
};


#endif // MAINWINDOW_H
