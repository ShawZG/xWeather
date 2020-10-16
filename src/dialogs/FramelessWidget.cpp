#include <QEvent>
#include <QDebug>
#include <QHoverEvent>
#include <QCursor>

#ifdef Q_OS_LINUX
#include <X11/Xlib.h>
#include <QX11Info>
#endif

#include "FramelessWidget.h"

FramelessWidget::FramelessWidget(QObject *parent) : QObject(parent)
{
    if (parent->isWidgetType()) {
        setFramelessWidget(qobject_cast<QWidget *>(parent));
    }
}

void FramelessWidget::updatePaddingRect()
{
    //重新计算八个描点的区域,描点区域的作用还有就是计算鼠标坐标是否在某一个区域内
    int width = widget->width();
    int height = widget->height();

    //左侧描点区域
    rectLeft = QRect(0, padding, padding, height - padding * 2);
    //上侧描点区域
    rectTop = QRect(padding, 0, width - padding * 2, padding);
    //右侧描点区域
    rectRight = QRect(width - padding, padding, padding, height - padding * 2);
    //下侧描点区域
    rectBottom = QRect(padding, height - padding, width - padding * 2, padding);
    //左上角描点区域
    rectLeftTop = QRect(0, 0, padding, padding);
    //右上角描点区域
    rectRightTop = QRect(width - padding, 0, padding, padding);
    //左下角描点区域
    rectLeftBottom = QRect(0, height - padding, padding, padding);
    //右下角描点区域
    rectRightBottom = QRect(width - padding, height - padding, padding, padding);
}

void FramelessWidget::setMouseCursor(QPoint point)
{
    if (rectLeft.contains(point)) {
        widget->setCursor(Qt::SizeHorCursor);
    } else if (rectRight.contains(point)) {
        widget->setCursor(Qt::SizeHorCursor);
    } else if (rectTop.contains(point)) {
        widget->setCursor(Qt::SizeVerCursor);
    } else if (rectBottom.contains(point)) {
        widget->setCursor(Qt::SizeVerCursor);
    } else if (rectLeftTop.contains(point)) {
        widget->setCursor(Qt::SizeFDiagCursor);
    } else if (rectRightTop.contains(point)) {
        widget->setCursor(Qt::SizeBDiagCursor);
    } else if (rectLeftBottom.contains(point)) {
        widget->setCursor(Qt::SizeBDiagCursor);
    } else if (rectRightBottom.contains(point)) {
        widget->setCursor(Qt::SizeFDiagCursor);
    } else {
        widget->setCursor(Qt::ArrowCursor);
    }
}

void FramelessWidget::updateGeometry(int offsetX, int offsetY)
{
    if (pressedLeft) {
        int resizeW = widget->width() - offsetX;
        if (widget->minimumWidth() <= resizeW) {
            widget->setGeometry(widget->x() + offsetX, rectY, resizeW, rectH);
        }
    } else if (pressedRight) {
        widget->setGeometry(rectX, rectY, rectW + offsetX, rectH);
    } else if (pressedTop) {
        int resizeH = widget->height() - offsetY;
        if (widget->minimumHeight() <= resizeH) {
            widget->setGeometry(rectX, widget->y() + offsetY, rectW, resizeH);
        }
    } else if (pressedBottom) {
        widget->setGeometry(rectX, rectY, rectW, rectH + offsetY);
    } else if (pressedLeftTop) {
        int resizeW = widget->width() - offsetX;
        int resizeH = widget->height() - offsetY;
        if (widget->minimumWidth() <= resizeW) {
            widget->setGeometry(widget->x() + offsetX, widget->y(), resizeW, resizeH);
        }
        if (widget->minimumHeight() <= resizeH) {
            widget->setGeometry(widget->x(), widget->y() + offsetY, resizeW, resizeH);
        }
    } else if (pressedRightTop) {
        int resizeW = rectW + offsetX;
        int resizeH = widget->height() - offsetY;
        if (widget->minimumHeight() <= resizeH) {
            widget->setGeometry(widget->x(), widget->y() + offsetY, resizeW, resizeH);
        }
    } else if (pressedLeftBottom) {
        int resizeW = widget->width() - offsetX;
        int resizeH = rectH + offsetY;
        if (widget->minimumWidth() <= resizeW) {
            widget->setGeometry(widget->x() + offsetX, widget->y(), resizeW, resizeH);
        }
        if (widget->minimumHeight() <= resizeH) {
            widget->setGeometry(widget->x(), widget->y(), resizeW, resizeH);
        }
    } else if (pressedRightBottom) {
        int resizeW = rectW + offsetX;
        int resizeH = rectH + offsetY;
        widget->setGeometry(widget->x(), widget->y(), resizeW, resizeH);
    }
}

void FramelessWidget::updateMousePressPaddingRect()
{
    if (rectLeft.contains(lastPos)) {
        pressedLeft = true;
    } else if (rectRight.contains(lastPos)) {
        pressedRight = true;
    } else if (rectTop.contains(lastPos)) {
        pressedTop = true;
    } else if (rectBottom.contains(lastPos)) {
        pressedBottom = true;
    } else if (rectLeftTop.contains(lastPos)) {
        pressedLeftTop = true;
    } else if (rectRightTop.contains(lastPos)) {
        pressedRightTop = true;
    } else if (rectLeftBottom.contains(lastPos)) {
        pressedLeftBottom = true;
    } else if (rectRightBottom.contains(lastPos)) {
        pressedRightBottom = true;
    } else {
        pressed = true;
    }
}

bool FramelessWidget::eventFilter(QObject *watched, QEvent *event)
{
    if (nullptr != widget && watched == widget) {
        if (event->type() == QEvent::Resize) {
            updatePaddingRect();
        } else if (event->type() == QEvent::HoverMove) {
            //设置对应鼠标形状,这个必须放在这里而不是下面,因为可以在鼠标没有按下的时候识别
            QHoverEvent *hoverEvent = dynamic_cast<QHoverEvent *>(event);
            QPoint point = hoverEvent->pos();
            if (resizeEnable) {
                setMouseCursor(point);
            }
            //根据当前鼠标位置,计算XY轴移动了多少
            int offsetX = point.x() - lastPos.x();
            int offsetY = point.y() - lastPos.y();
            //根据按下处的位置判断是否是移动控件还是拉伸控件
            if (true == moveEnable && true == pressed) {
#ifdef Q_OS_LINUX
                // 部分X11平台窗管不支持无边框窗口移出屏幕外
                if (true == QX11Info::isPlatformX11()) {
                    QPoint gPoint = widget->mapToGlobal(hoverEvent->pos());
                    x11mouseMoveEvent(gPoint.x(), gPoint.y());
                }
#endif
                widget->move(widget->x() + offsetX, widget->y() + offsetY);
            }
            if (true == resizeEnable) {
                updateGeometry(offsetX, offsetY);
            }
        } else if (event->type() == QEvent::MouseButtonPress) {
            //记住当前控件坐标和宽高以及鼠标按下的坐标
            QMouseEvent *mouseEvent = dynamic_cast<QMouseEvent *>(event);
            rectX = widget->x();
            rectY = widget->y();
            rectW = widget->width();
            rectH = widget->height();
            lastPos = mouseEvent->pos();
            //判断按下的手柄的区域位置
            updateMousePressPaddingRect();
        } else if (event->type() == QEvent::MouseButtonRelease) {
            //恢复所有
            pressed = false;
            pressedLeft = false;
            pressedRight = false;
            pressedTop = false;
            pressedBottom = false;
            pressedLeftTop = false;
            pressedRightTop = false;
            pressedLeftBottom = false;
            pressedRightBottom = false;
            widget->setCursor(Qt::ArrowCursor);
        }
    }

    return QObject::eventFilter(watched, event);
}

void FramelessWidget::setPadding(int padding)
{
    this->padding = padding;
}

void FramelessWidget::setFramelessWidgetMoveEnable(bool moveEnable)
{
    this->moveEnable = moveEnable;
}

void FramelessWidget::setFramelessWidgetResizeEnable(bool resizeEnable)
{
    this->resizeEnable = resizeEnable;
}

void FramelessWidget::setFramelessWidget(QWidget *widget)
{
    if (nullptr == this->widget) {
        this->widget = widget;
        this->widget->setMouseTracking(true);
        this->widget->installEventFilter(this);
        //设置悬停为真,必须设置这个,不然当父窗体里边还有子窗体全部遮挡了识别不到MouseMove,需要识别HoverMove
        this->widget->setAttribute(Qt::WA_Hover, true);
    }
}

void FramelessWidget::x11mouseMoveEvent(int globalX, int globalY)
{
#ifdef Q_OS_LINUX
    XEvent event;
    memset(&event, 0, sizeof(XEvent));

    Display *display = QX11Info::display();
    event.xclient.type = ClientMessage;
    event.xclient.message_type = XInternAtom(display, "_NET_WM_MOVERESIZE", False);
    event.xclient.display = display;
    //wid 是当前程序的 window id，可以通过QWidget->wId()获得，QWidget必须实例化
    event.xclient.window = static_cast<XID>(widget->winId());
    event.xclient.format = 32;
    event.xclient.data.l[0] = globalX;
    event.xclient.data.l[1] = globalY;
    event.xclient.data.l[2] = 8;
    event.xclient.data.l[3] = Button1;
    event.xclient.data.l[4] = 1;

    XUngrabPointer(display, CurrentTime);
    XSendEvent( display,
                QX11Info::appRootWindow(QX11Info::appScreen()),
                False,
                SubstructureNotifyMask | SubstructureRedirectMask,
                &event);
    XFlush(display);
#endif
}
