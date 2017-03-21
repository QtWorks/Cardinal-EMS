#ifndef BUTTONBAR_H
#define BUTTONBAR_H

#include <QtWidgets>
#include <QFile>

#include <wiringPi.h>

class ButtonBar : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit ButtonBar(QGraphicsObject * parent = 0);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    int numOfButtons = 4;
    int buttonLocation = 1; // 1-Bottom; 2-Top; 3-Left Side; 4-Right Side
    static int buttonDisplay; //1-Menu; 2-Fuel; 3-Settings
    static void button1Clicked();
    static void button2Clicked();
    static void button3Clicked();
    static int buttonPushed;

private:
    QRectF buttonRect1 = QRectF(0,-50.0,100,50);
    QRectF buttonRect2 = QRectF(100,-50.0,100,50);
    QRectF buttonRect3 = QRectF(200,-50.0,100,50);
    QRectF buttonRect4 = QRectF(300,-50.0,100,50);
    QRectF buttonRect5 = QRectF(400,-50.0,100,50);
    QRectF buttonRect6 = QRectF(500,-50.0,100,50);

signals:
    void sendAlarmAck();
    void sendFuelChange(QString changeDirection); //  + or -

public slots:
    void onAlarmFlash(int state);
};

#endif // BUTTONBAR_H
