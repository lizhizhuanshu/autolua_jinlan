#ifndef TOOLBARWIDGET_H
#define TOOLBARWIDGET_H

#include "FunctionList.h"
#include"DataModel.h"
#include "CustomFunction.h"

#include <QLabel>
#include <QListWidget>
#include <QWidget>
#include <qlistwidget.h>
#include <qpoint.h>
#include <qpushbutton.h>


class ToolbarWidget:public QWidget
{
  Q_OBJECT
public:
  explicit ToolbarWidget(DataModelPtr dataModel,CustomFunctionPtr interpreter,  QWidget *parent = nullptr);
private:
  DataModelPtr dataModel;
  CustomFunctionPtr interpreter;
  FunctionList* functionList;
  QPushButton* lastRect;
  QListWidget* pointColorList;
  void reloadFunction(const QString &filePath);
  
protected:
  void dragEnterEvent(QDragEnterEvent *event) override;
  void dropEvent(QDropEvent *event) override;

public slots:
  void addPointColor(const QPoint &pos,const QColor &color);
  void selectedRect(const QRect &rect);
  void callFunction(int id);
};

#endif // TOOLBARWIDGET_H
