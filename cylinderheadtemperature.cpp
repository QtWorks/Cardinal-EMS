//////////////////////////////////////////////////////////////////////////
//                                                                      //
// EngineMonitor, a graphical gauge to monitor an aircraft's engine     //
// Copyright (C) 2012 Tobias Rad                                        //
//                                                                      //
// This program is free software: you can redistribute it and/or modify //
// it under the terms of the GNU General Public License as published by //
// the Free Software Foundation, either version 3 of the License, or    //
// (at your option) any later version.                                  //
//                                                                      //
// This program is distributed in the hope that it will be useful,      //
// but WITHOUT ANY WARRANTY; without even the implied warranty of       //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        //
// GNU General Public License for more details.                         //
//                                                                      //
// You should have received a copy of the GNU General Public License    //
// along with this program. If not, see <http://www.gnu.org/licenses/>. //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "cylinderheadtemperature.h"

CylinderHeadTemperature::CylinderHeadTemperature(QGraphicsObject *parent) : QGraphicsObject(parent)
  , minValue(0.0)
  , maxValue(0.0)
  , greenYellowValue(0.0)
  , yellowRedValue(0.0)
{
	currentValues << 0.0 << 0.0 << 0.0 << 0.0;
}

QRectF CylinderHeadTemperature::boundingRect() const
{
	return QRectF(-125, -170, 250, 230);
}

/*! \brief Handles drawing of the object
*
* This member handles all of the painting logic used to draw the item and the associated style.
*/
void CylinderHeadTemperature::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
	//Set Clipping Rect
	painter->setClipRect(boundingRect());

	//Draw the side legend
	painter->setBrush(Qt::green);
	painter->setPen(QPen(Qt::green, 0));
	painter->drawRect(QRectF(QPointF(50.0, calculateLocalValue(minValue)), QPointF(80.0, calculateLocalValue(greenYellowValue))));
	painter->setBrush(Qt::yellow);
	painter->setPen(QPen(Qt::yellow, 0));
	painter->drawRect(QRectF(QPointF(50.0, calculateLocalValue(greenYellowValue)), QPointF(80.0, calculateLocalValue(yellowRedValue))));
	painter->setBrush(Qt::red);
	painter->setPen(QPen(Qt::red, 0));
	painter->drawRect(QRectF(QPointF(50.0, calculateLocalValue(yellowRedValue)), QPointF(80.0, calculateLocalValue(maxValue))));

	//Set painter for texts
	painter->setPen(QPen(Qt::white, 1));
    painter->setFont(QFont("Arial", 18));

	//Draw the static texts
    painter->drawText(QRectF(75.0, -165.0, 50.0, 20.0), Qt::AlignCenter | Qt::AlignVCenter, "CHT");
    painter->drawText(QRectF(90.0, 40.0, 40.0, 20.0), Qt::AlignRight | Qt::AlignBottom, QString::fromUtf8("°C"));

	//Draw the ticks and numbers
	foreach(double value, betweenValues)
	{
		painter->drawLine(80, calculateLocalValue(value), 90, calculateLocalValue(value));
		painter->drawText(QRectF(90.0, calculateLocalValue(value)-10.0, 35.0, 20.0), Qt::AlignRight | Qt::AlignVCenter, QString::number(value, 'f', 0));
	}

	//Draw the red line to define warning range
	painter->setPen(Qt::red);
	painter->drawLine(-125, calculateLocalValue(yellowRedValue), 35, calculateLocalValue(yellowRedValue));

	//Draw center dashed lines
	painter->setPen(QPen(Qt::white, 1, Qt::DashLine));
	for(int i = 0; i < 4; i++)
	{
		painter->drawLine(i*40-105, -120, i*40-105, 60);
	}

	//Draw the bar graphes
	for(int i = 0; i < 4; i++)
	{
		QRectF barRect = QRectF(QPointF(i*40-120, 60), QPointF(i*40-90, calculateLocalValue(currentValues.value(i))));
		if(currentValues.at(i) > yellowRedValue)
		{
			//If value is in warning area, bar is drawn red
			painter->setBrush(Qt::red);
            cylinderAlarm = 3;
		}
		else if(currentValues.at(i) > greenYellowValue)
		{
			//If value is in caution area, bar is drawn yellow
			painter->setBrush(Qt::yellow);
            cylinderAlarm = 2;
		}
		else
		{
			//In all other cases, bar is drawn green
			painter->setBrush(Qt::green);
            cylinderAlarm = 1;
		}

		if((currentValues.at(i) > minValue) &&
				(currentValues.at(i) < maxValue))
		{
			//If value is in visible range, draw the bar
			painter->setPen(painter->brush().color());
			painter->drawRect(barRect);
		}
		else
		{
			//If value is outside the displayed range, draw a red cross
			painter->setPen(QPen(Qt::red, 2));
			painter->drawLine(barRect.left(), 60, barRect.right(), -120);
			painter->drawLine(barRect.left(), -120, barRect.right(), 60);
		}

		if(painter->brush().color() == Qt::green)
		{
			//If value is in normal range, draw text in white
			painter->setPen(Qt::white);
		}

		//Define text position and move to current column
        QRectF textRect(-35, -20, 40, 20);
		textRect.moveCenter(QPointF(i*40-105, -135));

		if(i%2)
		{
			//All odd values should be raised
			textRect.translate(QPointF(0.0, -20.0));
		}

        //
        if ((isAlarmedRed == true) && (cylinderAlarm == 3)) {
            if (flashState) {
                painter->setPen(Qt::red);
                painter->setBrush(Qt::red);
                painter->drawRect(textRect);
                painter->setPen(Qt::white);
                painter->drawText(textRect, Qt::AlignCenter, QString::number(currentValues.at(i), 'f', 0));

            } else {
                painter->setPen(Qt::red);
                painter->drawText(textRect, Qt::AlignCenter, QString::number(currentValues.at(i), 'f', 0));
            }

        } else if ((isAlarmedYellow) && (cylinderAlarm == 2)) {
            if (flashState) {
                painter->setPen(Qt::yellow);
                painter->setBrush(Qt::yellow);
                painter->drawRect(textRect);
                painter->setPen(Qt::black);
                painter->drawText(textRect, Qt::AlignCenter, QString::number(currentValues.at(i), 'f', 0));

            } else {
                painter->setPen(Qt::yellow);
                painter->drawText(textRect, Qt::AlignCenter, QString::number(currentValues.at(i), 'f', 0));
            }
        } else {
            //Draw the readout
            painter->drawText(textRect, Qt::AlignCenter, QString::number(currentValues.at(i), 'f', 0));
        }
        update();
	}

}

double CylinderHeadTemperature::calculateLocalValue(double value) const
{
	return -(value-minValue)/(maxValue-minValue)*180.0+60.0;
}

void CylinderHeadTemperature::addBetweenValue(double value)
{
	betweenValues.append(value);
}

void CylinderHeadTemperature::setValues(double val1, double val2, double val3, double val4)
{

	currentValues.replace(0, val1);
	currentValues.replace(1, val2);
	currentValues.replace(2, val3);
	currentValues.replace(3, val4);

    if ((((yellowRedValue > val1) && (val1 >= greenYellowValue))
         || ((yellowRedValue > val2) && (val2 >= greenYellowValue))
         || ((yellowRedValue > val3) && (val3 >= greenYellowValue))
         || ((yellowRedValue > val4) && (val4 >= greenYellowValue)))
            && (isAlarmedYellow == false && isAlarmedRed == false))
    {
        emit sendAlarm("CHT", Qt::yellow, true);
        isAlarmedYellow = true;
    }
    else if ((val1 >= yellowRedValue || val2 >= yellowRedValue || val3 >= yellowRedValue || val4 >= yellowRedValue) && (isAlarmedRed == false))
    {
        if (isAlarmedYellow)
        {
            emit cancelAlarm("CHT");
            isAlarmedYellow = false;

            emit sendAlarm("CHT", Qt::red, true);
            isAlarmedRed = true;
        }
        else
        {
            emit sendAlarm("CHT", Qt::red, true);
            isAlarmedRed = true;
        }

    }
    else if ((isAlarmedRed) && (val1 < yellowRedValue && val2 < yellowRedValue && val3 < yellowRedValue && val4 < yellowRedValue))
    {
        emit cancelAlarm("CHT");

        isAlarmedRed = false;
    }
    else if ((isAlarmedYellow) && (val1 < greenYellowValue && val2 < greenYellowValue && val3 < greenYellowValue && val4 < greenYellowValue))
    {
        emit cancelAlarm("CHT");

        isAlarmedYellow = false;
    }


}

void CylinderHeadTemperature::setBorders(double minimum, double maximum, double yellowBorder, double redBorder)
{
	minValue = minimum;
	maxValue = maximum;
	greenYellowValue = yellowBorder;
	yellowRedValue = redBorder;
}

void CylinderHeadTemperature::changeFlashState()
{
    if (flashState == false) {
        flashState  = true;
    } else {
        flashState = false;
    }
}
