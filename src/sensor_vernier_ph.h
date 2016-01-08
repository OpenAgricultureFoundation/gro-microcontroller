/** 
 *  \file sensor_vernier_ph.h
 *  \brief Sensor module ph
 *  \details 
 *  \author Jake Rye
 */
#ifndef SENSOR_VERNIER_PH_H
#define SENSOR_VERNIER_PH_H

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include "module_handler.h"

/**
 * \brief Sensor module for ph
 */
class SensorVernierPh : SensorActuatorModule {
  public:
    // Public Functions
    /*
     * \brief Class constructor.
     */
    SensorVernierPh(int ph_pin, String ph_instruction_code, int ph_instruction_id);

    /**
     * \brief Called once to setup module.
     * Declares objects, configures initial state, sets configuration & calibration parameters.
     */
    void begin(void);
    
    /**
     * \brief Returns JSON string with module data.
     * Module data: ph, temperature, ec.
     * Data: "<instruction_code> <instruction_id> <value>".
     * Example: "SWPH 1 1", "SWTM 1 1", "SWEC 1 1", 
     */
    String get(void);

    /**
     * \brief Reserved to passing data string to object
     * Currently unused. Exists to comply with SAModule interface.
     */
    String set(String instruction_code, int instruction_id, String instruction_parameter);

    // Public Variables
    float ph; // pH
    
  private:
    // Private Functions
    float getPh(void);

    // Private Variables
    int ph_pin_;
    String ph_instruction_code_;
    int ph_instruction_id_;
    float ph_calibration_coefficient_;
    float ph_calibration_offset_;
    int ph_decimal_points_;
};

#endif // SENSOR_VERNIER_PH_H_


