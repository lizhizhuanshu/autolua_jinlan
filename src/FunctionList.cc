
#include "FunctionList.h"
#include <qwidget.h>
#include <QFileInfo>
#include <QDebug>
FunctionList::FunctionList(QWidget* parent):QListWidget(parent){
  setAcceptDrops(true);
  connect(this, &QListWidget::itemClicked, this, [this](QListWidgetItem* clickedItem) {
    auto id = clickedItem->data(Qt::UserRole).toInt();
    emit callFunction(id);
  });
  setSpacing(3);
  setStyleSheet("QListWidget::item { padding: 5px; }");
  
}

void FunctionList::addFuncation(int id,QString name){
  QListWidgetItem* item = new QListWidgetItem(name);
  item->setFont(QFont("Arial", 16));
  item->setTextAlignment(Qt::AlignCenter);
  item->setData(Qt::UserRole,id);

  addItem(item);
}

void FunctionList::resetFunctionInfo(const FunctionInfo &info){
  clear();
  for (const auto &item:info.functions) {
    addFuncation(item.id, QString(item.name.c_str()));
  }
}

