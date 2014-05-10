#ifndef WIDGETGPS_H
#define WIDGETGPS_H

#include <QGraphicsView>
#include <QGeoCoordinate>
#include <QGeoRectangle>
#include <QGeoLocation>
#include <QGraphicsScene>

class MyPoint{
public:
  MyPoint(double x,double y);
  double x;
  double y;
};

class WidgetGPS : public QGraphicsView
{
  Q_OBJECT
public:
  explicit WidgetGPS(QWidget *parent = 0);
  QGeoCoordinate actualCoordinate;
signals:

public slots:
  void reset();
  void addCoordinates(QGeoCoordinate coordinate);
  void setActualCoordinate(QGeoCoordinate coordinate);
protected:
  void paintEvent(QPaintEvent *event);
  void resizeEvent(QResizeEvent *);
private:
  QList<QGeoCoordinate> pointList;
  MyPoint calculateXY(QGeoCoordinate coordinates);

  QGeoLocation  *myGeoLocation;
  QGraphicsScene *myScene;
};

#endif // WIDGETGPS_H
