#ifndef MAGNIFIERVIEW_H
#define MAGNIFIERVIEW_H
#include <QWidget>

class MagnifierView : public QWidget
{
    Q_OBJECT
public:
    explicit MagnifierView(QWidget *parent = nullptr);
    void setImage(const QImage &image);
    void updateMagnifier(const QPoint &pos);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    [[nodiscard]] int magnifierSize() const{
      return pointNumber*zoomFactor;
    }
    QImage sourceImage;
    QPoint centerPos;
    int pointNumber = 25;      // 放大镜中显示的像素点数量
    int zoomFactor = 15;       // 放大倍数
};
#endif // MAGNIFIERVIEW_H