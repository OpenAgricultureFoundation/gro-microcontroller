/** 
 *  \file sensor_ds18b20.h
 *  \brief Sensor module for water temperature. 
 *  \author Jake Rye
 */
#ifndef SENSOR_DS18B20_H
#define SENSOR_DS18B20_H

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include "module_handler.h"
#include "support_moving_average.h"
#include "support_one_wire.h"

/**
 * \brief Sensor module for water ph, ec, and temperature.
 */
class SensorDs18b20 : SensorActuatorModule {
  public:
    // Public Functions
    /*
     * \brief Class constructor.
     */
    SensorDs18b20(int temperature_pin, String temperature_instruction_code, int temperature_id);

    /**
     * \brief Called once to setup module.
     * Declares objects, configures initial state, sets configuration & calibration parameters.
     */
    void begin(void);
    
    /**
     * \brief Returns JSON string with module data.
     * Module data: temperature
     * Data: "<instruction_code> <instruction_id> <value>".
     * Example: "SWTM 1 1", 
     */
    String get(void);

    /**
     * \brief Reserved to passing data string to object
     * Currently unused. Exists to comply with SAModule interface.
     */
    String set(String instruction_code, int instruction_id, String instruction_parameter);

    // Public Variables
    float temperature_raw; // degrees C
    float temperature_filtered; // degrees C
    
  private:
    // Private Functions
    void getSensorData(void);
    float getTemperature(void);
    float avergeArray(int* arr, int number);
    void startTempertureConversion(void);
    float TempProcess(bool ch);

    // Private Variables
    int temperature_pin_;
    String temperature_instruction_code_;
    int temperature_id_;
    int prev_update_time_;
    byte temperature_data_[12];
    byte temperature_address_[8];
    OneWire *ds_;
    MovingAverageFilter *temperature_filter_;
};

#endif // SENSOR_DS18B20_H_


