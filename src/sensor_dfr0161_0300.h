/** 
 *  \file sensor_dfr0161_0300.h
 *  \brief Sensor module for water ph, ec, and temperature. 
 *  \details These 3 sensors are contained in the same module due to their dependencies.
 *  Water temperature is used to calculate ec and when the ec sensor is on, it 
 *  interferes with the ph sensor. Temperature is updated every time the the *.get()
 *  function is called. Ph and ec will alternate updates assuming the time since the 
 *  previous *.get() was called is greater than the required delay durations for the 
 *  amount of time the ec sensor needs to turn on as well as the the amount of time
 *  the ph sensor needs for ec_sensor noise to settle once turned off. As implied by
 *  the previous statement, the ec sensor's GND and VCC lines are tied to a DPST-NO 
 *  relay (2-TypeA) that is controlled by the ec_enable_pin. As some relays can be very 
 *  loud when switching, a reed relay should be used instead of a latching relay. The audible 
 *  noise produced from the reed relay used in this system is neglidgable. As 
 *  audible noise can be bothersome to have in a system, a reasonable thought might be 
 *  to use a solid-state relay. This will not work as solid state relays do not provide 
 *  complete electrical isolation which will cause the ec sensor to interfere with the ph 
 *  sensor thus defeating the purpose of having the relay. If alternating ph and ec sensor
 *  reading is inadequate to your sensing purposes, analog optoisolators may be of use. The
 *  specific components intended for use with this module are the DFR0161 pH sensor, DFR0300
 *  EC sensor, and the DS18B20 temperature sensor. The temperature sensor is included in the 
 *  DFR0300 package. The relay used is the 8L02-05-01.
 *  \author Jake Rye
 */
#ifndef SENSOR_DFR_0161_0300_H
#define SENSOR_DFR_0161_0300_H

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
class SensorDfr01610300 : SensorActuatorModule {
  public:
    // Public Functions
    /*
     * \brief Class constructor.
     */
    SensorDfr01610300(int ph_pin, String ph_instruction_code, int ph_instruction_id, int temperature_pin, String temperature_instruction_code, 
                      int temperature_id, int ec_pin, String ec_instruction_code, int ec_id, int ec_enable_pin, int ec_power_pin);

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
    float ph_raw; // pH
    float ph_filtered; // pH
    float temperature_raw; // degrees C
    float temperature_filtered; // degrees C
    float ec_raw; // mS/cm
    float ec_filtered; // mS/cm
    
  private:
    // Private Functions
    void getSensorData(void);
    float getPh(void);
    float getTemperature(void);
    float getEc(float temperature);
    float avergeArray(int* arr, int number);
    void startTempertureConversion(void);
    float TempProcess(bool ch);

    // Private Variables
    int ph_pin_;
    String ph_instruction_code_;
    int ph_instruction_id_;
    int temperature_pin_;
    String temperature_instruction_code_;
    int temperature_id_;
    int ec_pin_;
    String ec_instruction_code_;
    int ec_id_;
    int ec_enable_pin_;
    int ec_power_pin_;
    float ph_calibration_coefficient_;
    float ph_calibration_offset_;
    float ec_calibration_coefficient_;
    float ec_calibration_offset_;
    int ec_on_delay_;
    int ec_off_delay_;
    int prev_update_time_;
    bool last_update_was_ec_;
    byte temperature_data_[12];
    byte temperature_address_[8];
    OneWire *ds_;
    MovingAverageFilter *ph_filter_;
    MovingAverageFilter *ec_filter_;
    MovingAverageFilter *temperature_filter_;
};

#endif // SENSOR_DFR0161_0300_H_


