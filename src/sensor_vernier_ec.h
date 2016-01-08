/** 
 *  \file sensor_vernier_ec.h
 *  \brief Sensor module ec
 *  \details 
 *  \author Jake Rye
 */
#ifndef SENSOR_VERNIER_EC_H
#define SENSOR_VERNIER_EC_H

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include "module_handler.h"

/**
 * \brief Sensor module for ph
 */
class SensorVernierEc : SensorActuatorModule {
  public:
    // Public Functions
    /*
     * \brief Class constructor.
     */
    SensorVernierEc(int ec_pin, String ec_instruction_code, int ec_instruction_id);

    /**
     * \brief Called once to setup module.
     * Declares objects, configures initial state, sets configuration & calibration parameters.
     */
    void begin(void);
    
    /**
     * \brief Returns JSON string with module data.
     * Module data: ec.
     * Data: "<instruction_code> <instruction_id> <value>".
     * Example: "SWEC 1 1", 
     */
    String get(void);

    /**
     * \brief Reserved to passing data string to object
     * Currently unused. Exists to comply with SAModule interface.
     */
    String set(String instruction_code, int instruction_id, String instruction_parameter);

    // Public Variables
    float ec; // ec
    
  private:
    // Private Functions
    float getEc(void);

    // Private Variables
    int ec_pin_;
    String ec_instruction_code_;
    int ec_instruction_id_;
    float ec_calibration_coefficient_;
    float ec_calibration_offset_;
    int ec_decimal_points_;
};

#endif // SENSOR_VERNIER_EC_H_


