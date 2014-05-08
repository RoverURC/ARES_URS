#ifndef GPSCOORDINATES_H
#define GPSCOORDINATES_H
#include <QChar>
class GPSCoordinates
{
public:
  GPSCoordinates();
  GPSCoordinates(double latitudeDegrees, double latitudeMinutes, double latitudeSeconds, QChar latitudeDesignator,
                 double longnitudeDegrees, double longnitudeMinutes, double longnitudeSeconds, QChar longnitudeeDesignator);

  double latitudeDegrees;
  double latitudeMinutes;
  double latitudeSeconds;
  QChar latitudeDesignator;

  double longnitudeDegrees;
  double longnitudeMinutes;
  double longnitudeSeconds;
  QChar longnitudeeDesignator;


};

#endif
