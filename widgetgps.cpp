#include "widgetgps.h"
#include <QPainter>
#include <QDebug>
WidgetGPS::WidgetGPS(QWidget *parent) :
  QGraphicsView(parent)
{
  pointList.clear();
  myScene = new QGraphicsScene(this);
  this->setScene(myScene);
  this->addCoordinates(QGeoCoordinate(0,0));
  actualCoordinate.setLatitude(0);
  actualCoordinate.setLongitude(0);
}
void WidgetGPS::reset(){
  pointList.clear();
  update();
}
void WidgetGPS::addCoordinates(QGeoCoordinate coordinate){
  pointList.append(coordinate);
  this->update();
}

void WidgetGPS::paintEvent(QPaintEvent *event){
  myScene->deleteLater();
  myScene = new QGraphicsScene();

  for (int i=0 ; i<pointList.size(); i++){
    MyPoint point = calculateXY(pointList[i]);
    myScene->addEllipse(point.x,point.y,0.3,0.3,QPen(Qt::transparent),QBrush(Qt::black));
    qDebug()<<"PX"<<point.x;
    qDebug()<<"PY"<<point.y;
  }
  MyPoint point = calculateXY(actualCoordinate);
  myScene->addEllipse(point.x,point.y, 0.3, 0.3,QPen(Qt::transparent),QBrush(Qt::red));
  qDebug()<<"AX"<<point.x;
  qDebug()<<"AY"<<point.y;

  setScene(myScene);
  QGraphicsView::fitInView(scene()->sceneRect(), Qt::KeepAspectRatio );
  QGraphicsView::paintEvent(event);
}

void WidgetGPS::resizeEvent(QResizeEvent *){

}

void WidgetGPS::setActualCoordinate(QGeoCoordinate coordinate){
  actualCoordinate = coordinate;
}
MyPoint WidgetGPS::calculateXY(QGeoCoordinate coordinates){
  MyPoint point(0,0);
  point.x = coordinates.longitude()*10000;
  point.y = coordinates.latitude()*10000;
  return point;
}

MyPoint::MyPoint(double x, double y){
      this->x = x;
      this->y = y;
}
