/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "widget.h"
#include <QtMultimedia/QAudioDeviceInfo>
#include <QtMultimedia/QAudioInput>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChart>
#include <QtWidgets/QVBoxLayout>
#include <QtCharts/QValueAxis>
#include "xyseriesiodevice.h"

QT_CHARTS_USE_NAMESPACE

Widget::Widget(QWidget *parent)
	: QWidget(parent)
{
    m_chart = new QChart;
    QChartView *chartView = new QChartView(m_chart);
    chartView->setMinimumSize(800, 600);
    m_series = new QLineSeries;
    m_chart->addSeries(m_series);
    QValueAxis *axisX = new QValueAxis;
	axisX->setRange(0, 3000);
    axisX->setLabelFormat("%g");
    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(-1, 1);
    axisY->setTitleText("Audio level");
    m_chart->setAxisX(axisX, m_series);
    m_chart->setAxisY(axisY, m_series);
    m_chart->legend()->hide();
    m_chart->setTitle("Data from the microphone");

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(chartView);

	m_chart2 = new QChart;
	QChartView *chartView2 = new QChartView(m_chart2);
	chartView2->setMinimumSize(800, 600);
	m_series2 = new QLineSeries;
	m_chart2->addSeries(m_series2);
	QValueAxis *axisX2 = new QValueAxis;
	axisX2->setRange(0, 3000);
	axisX2->setLabelFormat("%g");
	QValueAxis *axisY2 = new QValueAxis;
	axisY2->setRange(-1, 1);
	axisY2->setTitleText("Audio level");
	m_chart2->setAxisX(axisX2, m_series2);
	m_chart2->setAxisY(axisY2, m_series2);
	m_chart2->legend()->hide();

	mainLayout->addWidget(chartView2);

	setLayout(mainLayout);

    QAudioFormat formatAudio;
	formatAudio.setSampleRate(44100);
	formatAudio.setChannelCount(2);
	formatAudio.setSampleSize(24);
    formatAudio.setCodec("audio/pcm");
    formatAudio.setByteOrder(QAudioFormat::LittleEndian);
    formatAudio.setSampleType(QAudioFormat::UnSignedInt);

    QAudioDeviceInfo inputDevices = QAudioDeviceInfo::defaultInputDevice();
    m_audioInput = new QAudioInput(inputDevices,formatAudio, this);

	m_device = new XYSeriesIODevice(m_series, m_series2, this);
    m_device->open(QIODevice::WriteOnly);

    m_audioInput->start(m_device);
}

Widget::~Widget()
{
    m_audioInput->stop();
    m_device->close();
}
