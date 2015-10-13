/** 
 *  \file support_moving_average.cpp
 *  \brief Support module that creates a moving average filter for data.
 *  \details See support_moving_average.h for details.
 */
#include "support_moving_average.h"

MovingAverageFilter::MovingAverageFilter(unsigned int newDataPointsCount) {
  k = 0; //initialize so that we start to write at index 0
  if (newDataPointsCount < MAX_DATA_POINTS)
  dataPointsCount = newDataPointsCount;
  else
  dataPointsCount = MAX_DATA_POINTS;
  
  for (i=0; i<dataPointsCount; i++) {
    values[i] = 0; // fill the array with 0's
  }
}

float MovingAverageFilter::process(float in) {
  out = 0;

  values[k] = in;
  k = (k+1) % dataPointsCount;

  for (i=0; i<dataPointsCount; i++) {
    out += values[i];
  }

  return out/dataPointsCount;
}

