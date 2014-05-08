#include "widgetgps.h"
#include <QPainter>
#include <QDebug>
WidgetGPS::WidgetGPS(QWidget *parent) :
  QGraphicsView(parent)
{
  myScene = new QGraphicsScene(this);
  this->setScene(myScene);
  addPoint(GPSCoordinates(50,2,27.78,'N',21,59,56.76,'E'));
  addPoint(GPSCoordinates(50,2,28.78,'N',21,59,56.90,'E'));

}
void WidgetGPS::reset(){
  pointList.clear();
  update();
}

void WidgetGPS::addPoint(GPSCoordinates coordinate){
  pointList.append(coordinate);
  this->update();
}

void WidgetGPS::paintEvent(QPaintEvent *event){
  myScene->clear();

  for (int i=0 ; i<pointList.size(); i++){
      QPoint point = calculateXY(pointList[i]);
      myScene->addEllipse(point.x(),point.y(),1,1,QPen(Qt::black),QBrush(Qt::black));
    }
  QGraphicsView::paintEvent(event);
}

void WidgetGPS::resizeEvent(QResizeEvent *){

}

void WidgetGPS::setActualCoordinate(GPSCoordinates coordinate){
  actualCoordinate = coordinate;
}
QPoint WidgetGPS::calculateXY(GPSCoordinates coordinates){
  double x = coordinates.latitudeDegrees+coordinates.latitudeMinutes/60+coordinates.latitudeSeconds/3600;
  double y = coordinates.longnitudeDegrees+coordinates.longnitudeDegrees/60+coordinates.latitudeSeconds/3600;

  x=x*10;
  y=y*10;
  return QPoint(x,y);
}
