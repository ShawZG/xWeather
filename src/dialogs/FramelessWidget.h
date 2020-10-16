#ifndef FRAMELESSWIDGET_H
#define FRAMELESSWIDGET_H

#include <QWidget>

class FramelessWidget : public QObject
{
Q_OBJECT
public:
    explicit FramelessWidget(QObject *parent = nullptr);

public slots:
    //设置边距
    void setPadding(int padding);
    void setFramelessWidgetMoveEnable(bool moveEnable);
    void setFramelessWidgetResizeEnable(bool resizeEnable);

protected:
    bool eventFilter(QObject *watched, QEvent *event) override ;

private:
    void setFramelessWidget(QWidget *widget);

    void updatePaddingRect();
    void setMouseCursor(QPoint point);
    void updateGeometry(int offsetX, int offsetY);
    void updateMousePressPaddingRect();

    void x11mouseMoveEvent(int globalX, int globalY);

    int padding = 10;                       //边距
    bool moveEnable = true;                 //可移动
    bool resizeEnable = true;               //可拉伸
    QWidget *widget = nullptr;              //无边框窗体

    bool pressed = false;                   //鼠标按下
    bool pressedLeft = false;               //鼠标按下左侧
    bool pressedRight = false;              //鼠标按下右侧
    bool pressedTop = false;                //鼠标按下上侧
    bool pressedBottom = false;             //鼠标按下下侧
    bool pressedLeftTop = false;            //鼠标按下左上侧
    bool pressedRightTop = false;           //鼠标按下右上侧
    bool pressedLeftBottom = false;         //鼠标按下左下侧
    bool pressedRightBottom = false;        //鼠标按下右下侧

    int rectX, rectY, rectW, rectH;         //窗体坐标+宽高
    QPoint lastPos;                         //鼠标按下处坐标

    QRect rectLeft;                 //左侧区域
    QRect rectRight;                //右侧区域
    QRect rectTop;                  //上侧区域
    QRect rectBottom;               //下侧区域
    QRect rectLeftTop;              //左上侧区域
    QRect rectRightTop;             //右上侧区域
    QRect rectLeftBottom;           //左下侧区域
    QRect rectRightBottom;          //右下侧区域

};

#endif // FRAMELESSWIDGET_H
