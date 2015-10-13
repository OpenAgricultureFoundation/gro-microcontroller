/** 
 *  \file sensor_contact_switch.h
 *  \brief Sensor module for all sensors that behave like a contact switch.
 *  \author Jake Rye
 */
#ifndef SENSOR_CONTACT_SWITCH_H
#define SENSOR_CONTACT_SWITCH_H

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include "module_handler.h"

/** 
 *  \brief Sensor module for all sensors that behave like a contact switch.
 */
class SensorContactSwitch : SensorActuatorModule {
  public:
    // Public Functions
    SensorContactSwitch(int pin, String instruction_code, int instruction_id);
    void begin(void); 
    String get(void); 
    String set(String instruction_code, int instruction_id, String instruction_parameter);

    // Public Variables
    bool is_connected_;
    
  private:
    // Private Functions
    bool getData(void);
    
    // Private Variables
    int pin_;
    int instruction_id_;
    String instruction_code_;
};

#endif // SENSOR_CONTACT_SWITCH_H_

