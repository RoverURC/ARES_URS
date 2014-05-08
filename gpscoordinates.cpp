#include <gpscoordinates.h>
#include <QDebug>
GPSCoordinates::GPSCoordinates(double latitudeDegrees, double latitudeMinutes, double latitudeSeconds,
                               QChar latitudeDesignator, double longnitudeDegrees, double longnitudeMinutes,
                               double longnitudeSeconds, QChar longnitudeeDesignator){
  this->latitudeDegrees=latitudeDegrees;
  this->latitudeMinutes=latitudeMinutes;
  this->latitudeSeconds=latitudeSeconds;
  this->latitudeDesignator=latitudeDesignator;
  this->longnitudeDegrees=longnitudeDegrees;
  this->longnitudeMinutes=longnitudeMinutes;
  this->longnitudeSeconds=longnitudeSeconds;
  this->longnitudeeDesignator=longnitudeeDesignator;
}
GPSCoordinates::GPSCoordinates(){

}
