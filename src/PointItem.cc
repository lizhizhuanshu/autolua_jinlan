
#include "PointItem.h"
#include <QListWidget>
#include <QDebug>
#include <QApplication>

PointItem::PointItem(const QString &text, const QColor &color, QWidget *parent)
    : QWidget(parent), m_color(color)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0,0, 0);

    m_label = new QLabel(text);

    m_label->setAlignment(Qt::AlignCenter);
    
    QColor textColor = color.lightness() > 128 ? Qt::black : Qt::white;
    m_label->setStyleSheet(QString("background-color: %1; color: %2;")
                            .arg(color.name())
                            .arg(textColor.name()));
    QFont font("Arial", 16);  // 字体名称和大小（点）
    m_label->setFont(font);
    QPushButton *deleteButton = new QPushButton();
    deleteButton->setIcon(QIcon(":/icons/delete.svg"));  // 确保你有这个图标
    deleteButton->setFixedSize(24, 24);
    deleteButton->setStyleSheet("QPushButton { border: none; }");

    layout->addWidget(m_label);
    layout->addWidget(deleteButton);

    connect(deleteButton, &QPushButton::clicked, this, [this](){
        emit requested(Action::Delete);
    });
}


void PointItem::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        if(QApplication::keyboardModifiers()& Qt::ControlModifier){
            emit requested(Action::DeleteAll);
        }else{
            emit requested(Action::Delete);
        }
    }else if(event->button() == Qt::LeftButton){
        auto pos = event->localPos();
        int shift = width()/3;
        if(pos.x()<=shift){
            emit requested(Action::CopyPoint);
        }else if(pos.x()<=shift*2){
            emit requested(Action::CopyAll);
        }else{
            emit requested(Action::CopyColor);
        }
    }
}

