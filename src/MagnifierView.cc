#include "MagnifierView.h"

#include <QPainter>
#include <qdebug.h>
#include <qglobal.h>

MagnifierView::MagnifierView(QWidget *parent) : QWidget(parent) {
  setAttribute(Qt::WA_TranslucentBackground);
  setFixedSize(magnifierSize(), magnifierSize());
}

void MagnifierView::setImage(const QImage &image) { sourceImage = image; }

void MagnifierView::updateMagnifier(const QPoint &pos) {
  centerPos = pos;
  update();
}

void MagnifierView::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);
  
  if (sourceImage.isNull()) {
    return;
  }

  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);

  // 计算源图像中要显示的区域
  int sourceSize = pointNumber;
  QRect sourceRect(centerPos.x() - sourceSize / 2,
                   centerPos.y() - sourceSize / 2, sourceSize, sourceSize);


  QImage magnifiedImage(magnifierSize(), magnifierSize(), QImage::Format_ARGB32);
  magnifiedImage.fill(Qt::lightGray);  // 使用浅灰色作为背景，您可以根据需要更改

  QPainter imgPainter(&magnifiedImage);

  QRect validSourceRect = sourceRect.intersected(sourceImage.rect());
  
  QPoint startPoint = validSourceRect.topLeft() - sourceRect.topLeft();

  for(int x = 0;x<validSourceRect.width();x++){
    for(int y = 0;y<validSourceRect.height();y++){
      QColor color = sourceImage.pixelColor(validSourceRect.x()+x,validSourceRect.y()+y);
      QRect targetRect((x+startPoint.x())*zoomFactor,(y+startPoint.y())*zoomFactor,zoomFactor,zoomFactor);
      imgPainter.fillRect(targetRect,color);
    }
  }

  // 绘制放大后的图像
  painter.drawImage(rect(), magnifiedImage);

  painter.setPen(QPen(Qt::red, 1));
  painter.drawLine(rect().left(), rect().center().y(), rect().right(), rect().center().y());
  painter.drawLine(rect().center().x(), rect().top(), rect().center().x(), rect().bottom());

  // 显示颜色信息
  QColor centerColor = sourceImage.pixelColor(centerPos);
  QString colorInfo = QString(" %1,%2,0x%3 ")
                          .arg(centerPos.x())
                          .arg(centerPos.y())
                          .arg(centerColor.rgb() & 0xFFFFFF, 6, 16, QChar('0'));

  // 计算文本区域
  QFontMetrics fm(painter.font());

  int textWidth = fm.horizontalAdvance(colorInfo);
  int textHeight = fm.height();
  QRect textRect(rect().center().x() - textWidth / 2,
                  rect().bottom() - textHeight - 5,
                  textWidth,
                  textHeight);

  // 绘制半透明黑色背景
  painter.setPen(Qt::NoPen);
  painter.setBrush(QColor(0, 0, 0, 255));  // 180 是透明度
  painter.drawRect(textRect);

  // 绘制文本
  painter.setPen(Qt::white);
  painter.drawText(textRect, Qt::AlignCenter, colorInfo);
}