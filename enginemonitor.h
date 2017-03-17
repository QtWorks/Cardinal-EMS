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

#ifndef ENGINEMONITOR_H
#define ENGINEMONITOR_H

#include <QtGui>

#include "rpmindicator.h"
#include "bargraph.h"
#include "fuelmanagement.h"
#include "manifoldpressure.h"
#include "alarmBox.h"
#include "textBoxGauge.h"
#include "fueldisplay.h"
#include "chtegtgauge.h"
#include <buttonbar.h>

class EngineMonitor : public QGraphicsView
{
	Q_OBJECT
public:
	EngineMonitor(QWidget *parent = 0);
	~EngineMonitor();
private:
    void setupAlarm();
	void setupRpmIndicator();
	void setupBarGraphs();
	void setupExhaustGasTemperature();
	void setupCylinderHeadTemperature();
	void setupStatusItem();
	void setupTimeToDestinationItem();
	void setupFuelManagement();
	void setupManifoldPressure();
	void setupLogFile();
    void setupChtEgt();
	void saveSceneToSvg(const QString fileName = "./out/output.svg");
    void handleAlarm(int alarmColor, int alarmSeverity, QString alarmText, QString alarmGauge);
    void cancelAlarm(QString alarmGauge);
	QGraphicsScene graphicsScene;
    RpmIndicator rpmIndicator;
	BarGraph oilTemperature;
	BarGraph oilPressure;
	BarGraph voltMeter;
	BarGraph ampereMeter;
	BarGraph fuelFlow;
    TextBox insideAirTemperature;
    TextBox outsideAirTemperature;
	QGraphicsTextItem statusItem;
	QGraphicsTextItem timeToDestinationItem;
	FuelManagement fuelManagement;
    FuelDisplay fuelDisplay;
	ManifoldPressure manifoldPressure;
	QFile *logFile;
    QSettings settings;
    QSettings gaugeSettings;
    QString sensorInterfaceType;
    AlarmBox alarmWindow;
    int warmupTemp;
    QTimer flashTimer;
    ChtEgt chtEgt;
    ButtonBar buttonBar;

private slots:
	void demoFunction();
	void writeLogFile();
public slots:
    void setValuesBulkUpdate(quint16 rpm, quint16 fuelFlow, quint16 oilTemp, quint16 oilPress, quint16 amps, quint16 volts, quint16 egt1, quint16 egt2, quint16 egt3, quint16 egt4, quint16 cht1, quint16 cht2, quint16 cht3, quint16 cht4, quint16 oat, quint16 iat);
    void setFuelData(double fuelFlowValue, double fuelAbsoluteValue);

    // These are slots for the RDAC
    void setDataMessage1(double fuelFlowValue, double fuelAbsoluteValue);
    void setDataMessage2(double insideAirTemperatureValue, double outsideAirTemperatureValue, double ampereValue, double oilTemperatureValue, double oilPressureValue, double voltageValue, double manifoldPressureValue);
    void setDataMessage3(double revolutionPerMinute);
    void setDataMessage4egt(quint16 egt1, quint16 egt2, quint16 egt3, quint16 egt4);
    void setDataMessage4cht(quint16 cht1, quint16 cht2, quint16 cht3, quint16 cht4);
    void setTimeToDestination(double time);
    void userMessageHandler(QString title, QString content, bool endApplication);
    void showStatusMessage(QString text, QColor color);

};

#endif // ENGINEMONITOR_H
