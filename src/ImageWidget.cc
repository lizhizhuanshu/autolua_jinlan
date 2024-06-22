
#include "ImageWidget.h"
#include <QFileInfo>
#include <QImageReader>
#include <qapplication.h>
#include <qboxlayout.h>
#include <qglobal.h>
#include <qnamespace.h>
#include <qscrollbar.h>
#include <qpainter.h>
#include <QClipboard>

ImageWidget::ImageWidget(QWidget *parent)
    : QWidget(parent), scrollArea(new QScrollArea(this)),
      imageLabel(new CustomLabel(scrollArea)), 
      magnifier(new MagnifierView(this)),
      isCtrlPressed(false),
      isDragging(false),
      isSelectingRect(false) {
  imageLabel->setBackgroundRole(QPalette::ColorRole::Base);
  imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
  imageLabel->setScaledContents(false);
  imageLabel->setMouseTracking(true);

  scrollArea->setWidget(imageLabel);
  scrollArea->setBackgroundRole(QPalette::Dark);
  scrollArea->setWidgetResizable(false);

  auto *layout = new QVBoxLayout(this);
  layout->addWidget(scrollArea);
  setLayout(layout);
  setAcceptDrops(true);


  imageLabel->installEventFilter(this);
  scrollArea->viewport()->installEventFilter(this);
}

void ImageWidget::setImage(const QImage &image) {
  currentImage = image;
  imageLabel->setImage(image);
  imageLabel->adjustSize();
  magnifier->setImage(image);
}

void ImageWidget::dragEnterEvent(QDragEnterEvent *event) {
  // 检查拖入的是否是图像文件
  if (event->mimeData()->hasUrls()) {
    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.count() == 1) { // 我们只接受一个文件
      QFileInfo info(urls[0].toLocalFile());
      if (QImageReader::supportedImageFormats().contains(
              info.suffix().toLower().toUtf8())) {
        event->acceptProposedAction();
        return;
      }
    }
  }
  event->ignore();
}

void ImageWidget::dropEvent(QDropEvent *event) {
  QList<QUrl> urls = event->mimeData()->urls();
  if (!urls.isEmpty()) {
    QString filePath = urls[0].toLocalFile();
    loadImage(filePath);
  }
}

void ImageWidget::loadImage(const QString &filePath) {
  QImage image(filePath);
  if (!image.isNull()) {
    setImage(image);
  } else {
    // 处理加载失败的情况
    qDebug() << "Failed to load image:" << filePath;
  }
}

bool ImageWidget::eventFilter(QObject *obj, QEvent *event) {
  if (obj == imageLabel || obj == scrollArea->viewport()) {
    switch (event->type()) {
    case QEvent::MouseButtonPress: {
      QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
      if (mouseEvent->button() == Qt::LeftButton) {
        if(isCtrlPressed){
          startSelectRect(mouseEvent->pos());
        }else{
          startDragging(mouseEvent->pos());
        }
        return true;
      }
    } break;
    case QEvent::MouseMove: {
      QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
      if(isSelectingRect){
        updateSelectRect(mouseEvent->pos());
        return true;
      }
      if (isDragging) {
        doDragging(mouseEvent->pos());
        return true;
      }
      updateMagnifier(mouseEvent->pos());
      return true;
    } break;

    case QEvent::MouseButtonRelease: {
      QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
      if(mouseEvent->button() == Qt::RightButton){
        imageLabel->setHighlightRect(QRect());
        return true;
      }
      if(isSelectingRect){
        stopSelectRect();
        return true;
      }
      if (isDragging) {
        stopDragging();
        return true;
      }
    } break;
    case QEvent::Leave:
      magnifier->hide();
      return true;
    default:
      break;
    }
  }

  return QWidget::eventFilter(obj, event);
}

void ImageWidget::startDragging(const QPoint &pos) {
  isDragging = true;
  dragStartPosition = pos;
  scrollArea->viewport()->setCursor(Qt::ClosedHandCursor);
}

void ImageWidget::stopDragging() {
  isDragging = false;
  scrollArea->viewport()->setCursor(Qt::ArrowCursor);
}

void ImageWidget::doDragging(const QPoint &pos) {
  if (isDragging) {
    QPoint delta = pos - dragStartPosition;
    QScrollBar *hBar = scrollArea->horizontalScrollBar();
    QScrollBar *vBar = scrollArea->verticalScrollBar();

    hBar->setValue(hBar->value() - delta.x());
    vBar->setValue(vBar->value() - delta.y());
    dragStartPosition = pos;
  }
}

QPoint ImageWidget::getScrollPosition() const {
  return QPoint(scrollArea->horizontalScrollBar()->value(),
                scrollArea->verticalScrollBar()->value());
}

QPoint ImageWidget::getImagePositionUnderMouse(const QPoint &mousePos) const {
  return mousePos;
}

QColor ImageWidget::getColorAtPosition(const QPoint &pos) const {
  if (currentImage.valid(pos)) {
    return QColor(currentImage.pixel(pos));
  }
  return QColor(); // 返回无效颜色
}

void ImageWidget::updateMagnifier(const QPoint &pos) {
  if (currentImage.isNull())
    return;

  QPoint globalPos = imageLabel->mapToGlobal(pos);
  QPoint viewerPos = mapFromGlobal(globalPos);
  QPoint shiftPos;
  int shiftNumber = 50;
  if(viewerPos.x() < this->width()/2){
    shiftPos.setX(shiftNumber);
  }else{
    shiftPos.setX(-shiftNumber-magnifier->width());
  }
  if(viewerPos.y() < this->height()/2){
    shiftPos.setY(shiftNumber);
  }else{
    shiftPos.setY(-shiftNumber-magnifier->height());
  }

  // 更新放大镜位置
  magnifier->move(viewerPos + shiftPos);

  // 计算图像中的实际位置
  QPoint imagePos = getImagePositionUnderMouse(pos);


  magnifier->updateMagnifier(imagePos);
  magnifier->show();
}

void ImageWidget::sendSelectedPoint(const QPoint &pos) {
  QColor color = getColorAtPosition(pos);
  
  emit selectedPoint(pos, color);
}

void ImageWidget::keyPressEvent(QKeyEvent *event) {
  if(currentImage.isNull())
    return;
  if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter){
    QPoint globalPos = QCursor::pos();
    QPoint viewerPos = imageLabel->mapFromGlobal(globalPos);
    QPoint imagePos = getImagePositionUnderMouse(viewerPos);
    sendSelectedPoint(imagePos);
  }
  if (event->key() == Qt::Key_Control){
    isCtrlPressed = true;
  }
}

void ImageWidget::keyReleaseEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_Control){
    isCtrlPressed = false;
  }
}

void ImageWidget::startSelectRect(const QPoint &pos) {
  isSelectingRect = true;
  selectOrigin = pos;
  selectingRect.setTopLeft(pos);
  selectingRect.setBottomRight(pos);
  magnifier->hide();
}

void ImageWidget::updateSelectRect(const QPoint &pos) {
  if(pos.x()>selectOrigin.x()){
    selectingRect.setRight(pos.x());
    selectingRect.setLeft(selectOrigin.x());
  }else{
    selectingRect.setRight(selectOrigin.x());
    selectingRect.setLeft(pos.x());
  }
  if(pos.y()>selectOrigin.y()){
    selectingRect.setBottom(pos.y());
    selectingRect.setTop(selectOrigin.y());
  }else{
    selectingRect.setBottom(selectOrigin.y());
    selectingRect.setTop(pos.y());
  }
  imageLabel->setHighlightRect(selectingRect);
}

void ImageWidget::stopSelectRect() {
  isSelectingRect = false;
  magnifier->show();
  emit selectedRect(selectingRect);
}


void CustomLabel::updatePixmap() {
  if (originalPixmap.isNull()) {
    return;
  }
  displayPixmap = originalPixmap.copy();
  if (!highlightRect.isEmpty()) {
    QPainter painter(&displayPixmap);
    
    QPen whitePen(Qt::white, 2);  // 2像素宽的白色画笔
    painter.setPen(whitePen);
    painter.drawRect(highlightRect);
    QPen pen(Qt::red, 2);
    pen.setStyle(Qt::DashLine);
    pen.setDashPattern({5, 5});
    painter.setPen(pen);
    painter.drawRect(highlightRect);
  }
  setPixmap(displayPixmap);
  displayPixmap = QPixmap();
}