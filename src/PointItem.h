
#ifndef POINT_ITEM_H
#define POINT_ITEM_H
#include <QWidget>
#include<QHBoxLayout>
#include<QLabel>
#include<QPushButton>
#include<QColor>
#include <qevent.h>



class PointItem : public QWidget
{
  Q_OBJECT
public:
  enum class Action: int{
    CopyPoint = 0,
    CopyColor = 1,
    CopyAll = 2,
    Delete = 3,
    DeleteAll = 4
  };
  PointItem(const QString &text, const QColor &color, QWidget *parent = nullptr);
  [[nodiscard]] QColor color() const { return m_color; }
protected:
  void mouseReleaseEvent(QMouseEvent* event) override;
signals:
  void requested(Action action);
private:
  QLabel *m_label;
  QColor m_color;
};

#endif // POINT_ITEM_H