#include "mylabel.h"
#include<QDebug>
#include<QEvent>
#include<QMouseEvent>

MyLabel::MyLabel(QWidget *parent) : QLabel(parent)
{
    //设置鼠标为跟踪模式，不用按下鼠标也能追踪鼠标移动
    this->setMouseTracking(true);

}

//重写鼠标进入事件
void MyLabel::enterEvent(QEvent *event)
{
    qDebug()<<"鼠标进入了";
}

//重写鼠标离开事件
void MyLabel::leaveEvent(QEvent *event)
{
    qDebug()<<"鼠标离开了";
}

//重写鼠标按下状态的移动
void  MyLabel::mouseMoveEvent(QMouseEvent *ev)
{
     qDebug()<<"鼠标移动了";
}

//重写鼠标按下
void  MyLabel::mousePressEvent(QMouseEvent *ev)
{
    qDebug()<<"鼠标按下了";
    qDebug()<<"鼠标按下位置的全局坐标"<<ev->globalX()<<ev->globalY();
    qDebug()<<"鼠标按下位置的当前控件坐标"<<ev->x()<<ev->y();
}
//重写鼠标释放
void  MyLabel::mouseReleaseEvent(QMouseEvent *ev)
{
    //qDebug()<<"鼠标释放了";
}

bool MyLabel::event(QEvent *e)
{
    //拦截指定事件的事件
	//鼠标按下和单击事件 需要同时拦截双击事件才可以
    if(e->type() == QEvent::MouseButtonPress  || e->type() == QEvent::MouseButtonDblClick)
    {
        QMouseEvent * ev = (QMouseEvent*)e;
         qDebug()<<"拦截！！鼠标按下了";
         qDebug()<<"拦截！！鼠标按下位置的全局坐标"<<ev->globalX()<<ev->globalY();
         qDebug()<<"拦截！！鼠标按下位置的当前控件坐标"<<ev->x()<<ev->y();
        //返回true表示当前事件不要在处理
        return true;
    }

//    static int a;
//    qDebug()<<++a;

    //其他事件交还父类处理
    return QLabel::event(e);

}
