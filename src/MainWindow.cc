
#include<QScreen>
#include<QSettings>
#include <qapplication.h>
#include <QVBoxLayout>
#include <QPushButton>
#include <qboxlayout.h>
#include <qevent.h>
#include <QMessageBox>
#include <qmessagebox.h>
#include<QDebug>

#include "MainWindow.h"
#include "Configure.h"
#include "DataModel.h"
#include "ImageWidget.h"

MainWindow::MainWindow(DataModelPtr dataModel,
 CustomFunctionPtr interpreter,QWidget *parent) : 
  interpreter(interpreter),
  QMainWindow(parent), 
  layout(new QHBoxLayout(this)),
  leftWidget(new ImageWidget(this)),
  rightToolbar(new ToolbarWidget(std::move(dataModel),std::move(interpreter),this))
{

  setWindowTitle("AutoLua 金兰取色器");
  QWidget *centralWidget = new QWidget(this);
  // 创建水平布局
  QHBoxLayout *layout = new QHBoxLayout(centralWidget);
  this->setBackgroundRole(QPalette::ColorRole::Dark);
  layout->addWidget(leftWidget,1);
  layout->addWidget(rightToolbar,0);
  setCentralWidget(centralWidget);
  initializeSize();
  setAcceptDrops(true);
  connect(leftWidget, &ImageWidget::selectedPoint, rightToolbar, &ToolbarWidget::addPointColor);
  connect(leftWidget, &ImageWidget::selectedRect, rightToolbar, &ToolbarWidget::selectedRect);

}

void MainWindow::initializeSize()
{
  QSettings settings(autolua::CompanyName,autolua::AppName);
  settings.beginGroup("MainWindow");
  if(settings.contains("size"))
  {
    resize(settings.value("size").toSize());
  }
  else
  {
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int height = screenGeometry.height();
    int width = screenGeometry.width();
    resize(width * 0.8, height * 0.8);
  }
  if(settings.contains("pos"))
  {
    move(settings.value("pos").toPoint());
  }
} 


void MainWindow::closeEvent(QCloseEvent *event)
{
  QSettings settings(autolua::CompanyName, autolua::AppName);
  settings.beginGroup("MainWindow");
  settings.setValue("size", size());
  settings.setValue("pos", pos());
  settings.endGroup();
  QMainWindow::closeEvent(event);
}

static std::string readAll(const QString &filePath)
{
  QFile file(filePath);
  auto r = file.open(QIODevice::ReadOnly);
  if(!r)
  {
    return "";
  }
  QByteArray data = file.readAll();
  file.close();
  return data.toStdString();
}




