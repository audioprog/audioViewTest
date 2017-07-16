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

#include "xyseriesiodevice.h"
#include <QtCharts/QXYSeries>

XYSeriesIODevice::XYSeriesIODevice(QXYSeries * series,QXYSeries * series2, QObject *parent) :
    QIODevice(parent),
	m_series(series),
	m_series2(series2)
{
}

qint64 XYSeriesIODevice::readData(char * data, qint64 maxSize)
{
    Q_UNUSED(data)
    Q_UNUSED(maxSize)
    return -1;
}

qint64 XYSeriesIODevice::writeData(const char * data, qint64 maxSize)
{
	qint64 range = 3000;
    QVector<QPointF> oldPoints = m_series->pointsVector();
    QVector<QPointF> points;
	QVector<QPointF> oldPoints2 = m_series2->pointsVector();
	QVector<QPointF> points2;
	int resolution = 6;

	if (oldPoints.count() < range) {
		points = m_series->pointsVector();
	} else {
		for (int i = maxSize/resolution; i < oldPoints.count(); i++)
			points.append(QPointF(i - maxSize/resolution, oldPoints.at(i).y()));
	}

	if (oldPoints2.count() < range) {
		points2 = m_series2->pointsVector();
	} else {
		for (int i = maxSize/resolution; i < oldPoints2.count(); i++)
			points2.append(QPointF(i - maxSize/resolution, oldPoints2.at(i).y()));
	}

	const float max24Int = float(quint32(0xffffffff) >> 9);

    qint64 size = points.count();
    for (int k = 0; k < maxSize/resolution; k++)
	{
		const char* iData = data + (resolution * k);
		const qint32& i32Data = (((*(reinterpret_cast<const qint32*>(iData))) & 0x00ffffff) << 8) >> 8;

		points.append(QPointF(k + size, float(i32Data)/max24Int));

		const char* iData2 = data + (resolution * k + 3);
		const qint32& i32Data2 = (((*(reinterpret_cast<const qint32*>(iData2))) & 0x00ffffff) << 8) >> 8;

		points2.append(QPointF(k + size, float(i32Data2)/max24Int));
	}

    m_series->replace(points);
	m_series2->replace(points2);
	return maxSize;
}
