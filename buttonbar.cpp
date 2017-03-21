#include "buttonbar.h"

ButtonBar::ButtonBar(QGraphicsObject *parent) : QGraphicsObject(parent)
{

    wiringPiSetupSys();
    pinMode(6, INPUT);
    pinMode(13, INPUT);
    pinMode(19, INPUT);
    pinMode(24, OUTPUT);

    wiringPiISR(6,INT_EDGE_FALLING, &button1Clicked);
    wiringPiISR(13,INT_EDGE_FALLING, &button2Clicked);
    wiringPiISR(19,INT_EDGE_FALLING, &button3Clicked);
}

int ButtonBar::buttonDisplay = 1;
int ButtonBar::buttonPushed = 0;

QRectF ButtonBar::boundingRect() const
{
    return QRectF(0.0, -50.0, 600.0, 50.0);
}


void ButtonBar::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    painter->setBrush(Qt::darkBlue);
    painter->setPen(Qt::white);
    painter->setFont(QFont("Arial", 14, QFont::Bold));

    //Save thje painter and deactivate Antialising for rectangle drawing
    //painter->save();
    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->setRenderHint(QPainter::SmoothPixmapTransform, false);
    //painter->restore();


    switch(buttonDisplay) {
    case 1: if (buttonPushed == 1) {
                emit sendAlarmAck();
            } else if (buttonPushed == 2) {
                buttonDisplay = 2;
            } else if (buttonPushed == 3) {
                buttonDisplay = 3;
            }

            buttonPushed = 0;

            painter->drawRect(buttonRect1);
            painter->drawRect(buttonRect2);
            painter->drawRect(buttonRect3);

            painter->drawText(buttonRect1, Qt::AlignCenter,"Ack");
            painter->drawText(buttonRect2, Qt::AlignCenter,"Fuel");
            painter->drawText(buttonRect3, Qt::AlignCenter,"Settings");
            break;

    case 2: if (buttonPushed == 1) {
                buttonDisplay = 1;
            } else if (buttonPushed == 2) {
                // emit a signal to increase the fuel quantity
                emit sendFuelChange("+");
            } else if (buttonPushed == 3) {
                // emit a signal to increase the fuel quantity
                emit sendFuelChange("-");
            }

            buttonPushed = 0;

            painter->drawRect(buttonRect1);

            painter->drawRect(buttonRect2);
            painter->drawRect(buttonRect3);

            painter->drawText(buttonRect1, Qt::AlignCenter,"<-Menu");
            painter->drawText(buttonRect2, Qt::AlignCenter,"+");
            painter->drawText(buttonRect3, Qt::AlignCenter,"-");
            break;

    case 3: if (buttonPushed == 1) {
                buttonDisplay = 1;
            } else if (buttonPushed == 2) {
            } else if (buttonPushed == 3) {
            }

            buttonPushed = 0;

            painter->drawRect(buttonRect1);
            painter->drawRect(buttonRect2);
            painter->drawRect(buttonRect3);

            painter->drawText(buttonRect1, Qt::AlignCenter,"<-Menu");
            painter->drawText(buttonRect2, Qt::AlignCenter,"Export");
            painter->drawText(buttonRect3, Qt::AlignCenter,"Update");
            break;
    }

    update();
}

void ButtonBar::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QPointF clickedPos = event->buttonDownPos(Qt::LeftButton);

    qDebug() << clickedPos;

//    if (buttonPushed == 1) {
//        emit sendAlarmAck();
//        buttonPushed = 0;
//    }

    switch(buttonDisplay) {
    case 1: if ((clickedPos.x() > buttonRect1.x() && clickedPos.x() < buttonRect1.x() + buttonRect1.width()) && (clickedPos.y() > buttonRect1.y())) {
            emit sendAlarmAck();

        } else if ((clickedPos.x() > buttonRect2.x() && clickedPos.x() < buttonRect2.x() + buttonRect2.width()) && (clickedPos.y() > buttonRect2.y())) {
            buttonDisplay = 2;

        } else if ((clickedPos.x() > buttonRect3.x() && clickedPos.x() < buttonRect3.x() + buttonRect3.width()) && (clickedPos.y() > buttonRect3.y())) {
            buttonDisplay = 3;
        }

    case 2: if ((clickedPos.x() > buttonRect1.x() && clickedPos.x() < buttonRect1.x() + buttonRect1.width()) && (clickedPos.y() > buttonRect1.y())) {
            buttonDisplay = 1;

        } else if ((clickedPos.x() > buttonRect2.x() && clickedPos.x() < buttonRect2.x() + buttonRect2.width()) && (clickedPos.y() > buttonRect2.y())) {
            // emit a signal to increase the fuel quantity
            emit sendFuelChange("+");

        } else if ((clickedPos.x() > buttonRect3.x() && clickedPos.x() < buttonRect3.x() + buttonRect3.width()) && (clickedPos.y() > buttonRect3.y())) {
            // emit a signal to decrease the fuel quantity
            emit sendFuelChange("-");
        }

    case 3: if ((clickedPos.x() > buttonRect1.x() && clickedPos.x() < buttonRect1.x() + buttonRect1.width()) && (clickedPos.y() > buttonRect1.y())) {
            buttonDisplay = 1;

        } else if ((clickedPos.x() > buttonRect2.x() && clickedPos.x() < buttonRect2.x() + buttonRect2.width()) && (clickedPos.y() > buttonRect2.y())) {
            // emit a signal to show log file export dialog

        } else if ((clickedPos.x() > buttonRect3.x() && clickedPos.x() < buttonRect3.x() + buttonRect3.width()) && (clickedPos.y() > buttonRect3.y())) {
            // emit a signal to show a software update dialog
            QFile::copy("app.exe", "update.exe");
        }
    }

    QGraphicsItem::mousePressEvent(event);
}

void ButtonBar::button1Clicked()
{
    qDebug() << "Button 1 Pushed!";
    ButtonBar::buttonPushed = 1;
}

void ButtonBar::button2Clicked()
{
    qDebug() << "Button 2 Pushed!";
    ButtonBar::buttonPushed = 2;
}

void ButtonBar::button3Clicked()
{
    qDebug() << "Button 3 Pushed!";
    ButtonBar::buttonPushed = 3;
}

void ButtonBar::onAlarmFlash(int state) {
    if (state==1) {
        digitalWrite(24, HIGH);
    } else {
        digitalWrite(24, LOW);
    }
}
