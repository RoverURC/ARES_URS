#ifndef WIDGETGPS_H
#define WIDGETGPS_H

#include <QGraphicsView>
#include <gpscoordinates.h>
#include <QGraphicsScene>
class WidgetGPS : public QGraphicsView
{
  Q_OBJECT
public:
  explicit WidgetGPS(QWidget *parent = 0);
  QList<GPSCoordinates> pointList;
  GPSCoordinates actualCoordinate;
signals:

public slots:
  void reset();
  void addPoint(GPSCoordinates coordinate);
  void setActualCoordinate(GPSCoordinates coordinate);
protected:
  void paintEvent(QPaintEvent *event);
  void resizeEvent(QResizeEvent *);
private:
  QPoint calculateXY(GPSCoordinates coordinates);

  QGraphicsScene *myScene;
};

#endif // WIDGETGPS_H
