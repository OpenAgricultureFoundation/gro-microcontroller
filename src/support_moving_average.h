/** 
 *  \file support_moving_average.h
 *  \brief Support module that creates a moving average filter for data.
 *  \details Use is very easy. Construct an instance of the class specifying the number of
 *  data points to be used in the filter. Pass in new data points with the 
 *  *.process method. Method returns updated moving average filtered value.
 *  Found at: https://github.com/sebnil/Moving-Avarage-Filter--Arduino-Library-
 */
#ifndef SUPPORT_MOVING_AVERAGE_H
#define SUPPORT_MOVING_AVERAGE_H

#define MAX_DATA_POINTS 20

class MovingAverageFilter {
public:
  //construct without coefs
  MovingAverageFilter(unsigned int newDataPointsCount);

  float process(float in);

private:
  float values[MAX_DATA_POINTS];
  int k; // k stores the index of the current array read to create a circular memory through the array
  int dataPointsCount;
  float out;
  int i; // just a loop counter
};
#endif // SUPPORT_MOVING_AVERAGE_H_

