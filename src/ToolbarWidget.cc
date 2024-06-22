
#include <QVBoxLayout>
#include <qapplication.h>
#include <qchar.h>
#include <qdebug.h>
#include <qevent.h>
#include <qglobal.h>
#include <qlistwidget.h>
#include <qnamespace.h>
#include <QPushButton>
#include "ToolbarWidget.h"
#include "DataModel.h"
#include "FunctionList.h"
#include "PointItem.h"
#include <QEvent>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QFileInfo>
#include <qpushbutton.h>
#include <QClipboard>
ToolbarWidget::ToolbarWidget(DataModelPtr dataModel,
  CustomFunctionPtr interpreter,
  QWidget *parent) : 
  dataModel(std::move(dataModel)),
  interpreter(std::move(interpreter)), 
  functionList(new FunctionList(this)),
  pointColorList(new QListWidget(this)),
  QWidget(parent)
{
  setFixedWidth(240); // 设置固定宽度
  setBackgroundRole(QPalette::ColorRole::Dark);

  auto *layout = new QVBoxLayout(this);
  auto rect = this->dataModel->selectedRect();
  lastRect = new QPushButton(QString("%1,%2,%3,%4").arg(rect.x1).arg(rect.y1).arg(rect.x2).arg(rect.y2 ));
  lastRect->setFont(QFont("Arial", 16));
  lastRect->setStyleSheet("color: #000000;");
  connect(lastRect, &QPushButton::clicked, this, [this](){
    QApplication::clipboard()->setText(lastRect->text());
  });
  


  pointColorList->setSpacing(3);
  layout->setContentsMargins(0,0,0,0);
  layout->addWidget(lastRect);
  layout->addWidget(pointColorList);
  layout->addWidget(functionList);
  layout->setStretch(0, 0);
  layout->setStretch(1, 4);
  layout->setStretch(2,2);

  setAcceptDrops(true);

  connect(functionList, &FunctionList::callFunction, this, &ToolbarWidget::callFunction);

  functionList->resetFunctionInfo(this->interpreter->functionInfo());
}

void ToolbarWidget::addPointColor(const QPoint &pos, const QColor &color)
{
  QString text = QString("%1,%2,0x%3").arg(pos.x()).arg(pos.y()).arg(color.rgb() & 0xFFFFFF, 0, 16);
  PointItem *itemWidget = new PointItem(text, color);
  QListWidgetItem *item = new QListWidgetItem(pointColorList);
  item->setSizeHint(itemWidget->sizeHint());
  pointColorList->addItem(item);
  pointColorList->setItemWidget(item, itemWidget);

  dataModel->feature().add(MPointFeature(pos.x(), pos.y(), color.rgb() & 0xFFFFFF));
  connect(itemWidget, &PointItem::requested, this, 
  [this, item](PointItem::Action action){
    int row = pointColorList->row(item);
    switch (action) {
    case PointItem::Action::CopyPoint:{
      auto pointColor = dataModel->feature()[row];
      std::string text = interpreter->encodePoint(pointColor.x, pointColor.y);
      QApplication::clipboard()->setText(QString::fromStdString(text));
      break;
    }
    case PointItem::Action::CopyColor:{
      auto pointColor = dataModel->feature()[row];
      std::string text = interpreter->encodeColor(pointColor.color);
      QApplication::clipboard()->setText(QString::fromStdString(text));
      break;
    }
    case PointItem::Action::CopyAll:{
      auto pointColor = dataModel->feature()[row];
      std::string text = interpreter->encodePointColor(pointColor.x, pointColor.y, pointColor.color);
      QApplication::clipboard()->setText(QString::fromStdString(text));
      break;
    }
    case PointItem::Action::Delete:{
      QListWidgetItem *removedItem = pointColorList->takeItem(row);
      delete removedItem;
      dataModel->feature().remove(row);
      break;
    }
    case PointItem::Action::DeleteAll:{
      pointColorList->clear();
      dataModel->feature().clear();
      break;
    }
    }
  });
  // lastRect->setText(text);
}

void ToolbarWidget::callFunction(int id)
{
  auto r = interpreter->callFunction(id);
  if(!r){
    qDebug()<<"callFunction failed,error:"<<interpreter->getError().c_str();
  }
}


void ToolbarWidget::dragEnterEvent(QDragEnterEvent *event)
{
  if (event->mimeData()->hasUrls()) {
    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.count() == 1) { // 我们只接受一个文件
      QFileInfo info(urls[0].toLocalFile());
      if (info.suffix().toLower() == "lua") {
        event->acceptProposedAction();
        return;
      }
    }
  }
  event->ignore();
}

void ToolbarWidget::dropEvent(QDropEvent *event)
{
  QList<QUrl> urls = event->mimeData()->urls();
  if (!urls.isEmpty()) {
    QString filePath = urls[0].toLocalFile();
    reloadFunction(filePath);
  }
}

void ToolbarWidget::reloadFunction(const QString &filePath)
{
  auto r = interpreter->reloadFunction(filePath.toStdString());
  if(r){
    functionList->resetFunctionInfo(interpreter->functionInfo());
  }else{
    qDebug()<<"reloadFunction failed";
  }
}



void ToolbarWidget::selectedRect(const QRect &rect)
{

  QString text = QString("%1,%2,%3,%4").
    arg(rect.left()).
    arg(rect.top()).
    arg(rect.right()).
    arg(rect.bottom());
  lastRect->setText(text);

  MRect mRect;
  mRect.x1 = rect.left();
  mRect.y1 = rect.top();
  mRect.x2 = rect.right();
  mRect.y2 = rect.bottom();
  std::string str = interpreter->encodeRect(mRect.x1, mRect.y1, mRect.x2, mRect.y2);

  QApplication::clipboard()->setText(QString::fromStdString(str));
  dataModel->setSelectedRect(mRect);
}
