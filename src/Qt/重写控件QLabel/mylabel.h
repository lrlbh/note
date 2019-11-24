#ifndef MYLABEL_H
#define MYLABEL_H

#include <QLabel>

class MyLabel : public QLabel
{
    Q_OBJECT
public:
    explicit MyLabel(QWidget *parent = nullptr);

    //重写鼠标进入事件
    void enterEvent(QEvent *event);
    //重写鼠标离开事件
    void leaveEvent(QEvent *event);
    //重写鼠标移动
    void mouseMoveEvent(QMouseEvent *ev);
    //重写鼠标按下
    void mousePressEvent(QMouseEvent *ev);
    //重写鼠标释放
    void mouseReleaseEvent(QMouseEvent *ev);


    //重写事件分发
    bool event(QEvent *e);

signals:

public slots:
};

#endif // MYLABEL_H
