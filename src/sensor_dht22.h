/** 
 *  \file sensor_dht22.h
 *  \brief Sensor module for air temperature and humidity.
 */

// Library based off: DHT library from Seeed Studio
// Library found at: https://github.com/Seeed-Studio/Grove_Temperature_And_Humidity_Sensor
// Component found at: http://www.seeedstudio.com/depot/grove-temperaturehumidity-sensor-pro-p-838.html?cPath=144_147
// Modified by: Jake Rye

#ifndef SensorDht22_H
#define SensorDht22_H
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include "module_handler.h"

// 8 MHz(ish) AVR ---------------------------------------------------------
#if (F_CPU >= 7400000UL) && (F_CPU <= 9500000UL)
#define COUNT 3
// 16 MHz(ish) AVR --------------------------------------------------------
#elif (F_CPU >= 15400000UL) && (F_CPU <= 19000000L)
#define COUNT 6
#else
#error "CPU SPEED NOT SUPPORTED"
#endif

// how many timing transitions we need to keep track of. 2 * number bits + extra
#define MAXTIMINGS 85

/** 
 *  \brief Sensor module for air temperature and humidity.
 */
class SensorDht22 : SensorActuatorModule {
  public:
    // Public Functions
    SensorDht22(int pin, String temperature_instruction_code, int temperature_instruction_id, String humidity_instruction_code, int humidity_instruction_id);
    void begin(void);
    String get(void);
    String set(String instruction_code, int instruction_id, String parameter);

    // Public Variables
    float humidity;
    float temperature;
    
  private:
    // Private Functions
    void getSensorData(void);
    boolean read(void);
    void getRawSensorData(void);
    void filterSensorData(void);
    String floatToString( double val, unsigned int precision);
    
    // Private Variables
    int pin_;
    String humidity_instruction_code_;
    int humidity_instruction_id_;
    String temperature_instruction_code_;
    int temperature_instruction_id_;

    uint8_t data[6];
    uint8_t count_;
    uint32_t last_read_time_;
    boolean first_reading_;
    float humidity_raw_;
    float temperature_raw_;
};

#endif // SensorDht22_H_
