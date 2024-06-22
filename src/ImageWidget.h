

#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H
#include <QWidget>
#include <QScrollArea>
#include <QLabel>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QPoint>
#include <qimage.h>
#include <qpixmap.h>

#include "MagnifierView.h"


class CustomLabel : public QLabel
{
  Q_OBJECT
public:
  explicit CustomLabel(QWidget *parent = nullptr) : QLabel(parent) {}
  void setImage(const QImage &image) {
    originalPixmap = QPixmap::fromImage(image);
    updatePixmap();
  }
  void setHighlightRect(const QRect &rect) {
    if(rect == highlightRect)return;
    highlightRect = rect;
    updatePixmap();
  }

  

private:
  QPixmap originalPixmap;
  QPixmap displayPixmap;
  QRect highlightRect;
  void updatePixmap();
};

class ImageWidget:public QWidget {
  Q_OBJECT
public:
  explicit ImageWidget(QWidget *parent = nullptr);
  void setImage(const QImage &image);
signals:
  void selectedPoint(const QPoint &pos,const QColor &color);
  void selectedRect(const QRect &rect);
protected:
  void dragEnterEvent(QDragEnterEvent *event) override;
  void dropEvent(QDropEvent *event) override;
  bool eventFilter(QObject *obj, QEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override;
  void keyReleaseEvent(QKeyEvent *event) override;
private:
  QScrollArea *scrollArea;
  CustomLabel *imageLabel;
  QPoint dragStartPosition;
  QImage currentImage;
  MagnifierView *magnifier;
  QPoint selectOrigin;
  QRect selectingRect;
  bool isCtrlPressed;
  bool isDragging;
  bool isSelectingRect;

  void loadImage(const QString &filePath);
  void startDragging(const QPoint &pos);
  void stopDragging();
  void doDragging(const QPoint &pos);
  void sendSelectedPoint(const QPoint &pos);

  void startSelectRect(const QPoint &pos);
  void updateSelectRect(const QPoint &pos);
  void stopSelectRect();


  QPoint getScrollPosition() const;
  QPoint getImagePositionUnderMouse(const QPoint &mousePos) const;
  QColor getColorAtPosition(const QPoint &pos) const;

  void updateMagnifier(const QPoint &pos);
};

#endif // IMAGEWIDGET_H