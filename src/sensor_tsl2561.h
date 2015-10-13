/** 
 *  \file sensor_tsl2561.h
 *  \brief Sensor module for light intensity and par.
 */

// Library based off: Digital_Light_TSL2561 library from Seeed Studio
// Library found at: http://www.seeedstudio.com/wiki/Grove_-_Digital_Light_Sensor
// Component found at: http://www.seeedstudio.com/depot/Grove-Digital-Light-Sensor-p-1281.html
// Modified by: Jake Rye
// Date: July 2, 2015

#ifndef SENSOR_TSL2561_H
#define SENSOR_TSL2561_H

#include <Arduino.h>

#include "support_wire.h"
#include "module_handler.h"

#define  TSL2561_Control  0x80
#define  TSL2561_Timing   0x81
#define  TSL2561_Interrupt 0x86
#define  TSL2561_Channal0L 0x8C
#define  TSL2561_Channal0H 0x8D
#define  TSL2561_Channal1L 0x8E
#define  TSL2561_Channal1H 0x8F

#define TSL2561_Address  0x29       //device address

#define LUX_SCALE 14           // scale by 2^14
#define RATIO_SCALE 9          // scale ratio by 2^9
#define CH_SCALE 10            // scale channel values by 2^10
#define CHSCALE_TINT0 0x7517   // 322/11 * 2^CH_SCALE
#define CHSCALE_TINT1 0x0fe7   // 322/81 * 2^CH_SCALE

#define K1T 0x0040   // 0.125 * 2^RATIO_SCALE
#define B1T 0x01f2   // 0.0304 * 2^LUX_SCALE
#define M1T 0x01be   // 0.0272 * 2^LUX_SCALE
#define K2T 0x0080   // 0.250 * 2^RATIO_SCA
#define B2T 0x0214   // 0.0325 * 2^LUX_SCALE
#define M2T 0x02d1   // 0.0440 * 2^LUX_SCALE
#define K3T 0x00c0   // 0.375 * 2^RATIO_SCALE
#define B3T 0x023f   // 0.0351 * 2^LUX_SCALE
#define M3T 0x037b   // 0.0544 * 2^LUX_SCALE
#define K4T 0x0100   // 0.50 * 2^RATIO_SCALE
#define B4T 0x0270   // 0.0381 * 2^LUX_SCALE
#define M4T 0x03fe   // 0.0624 * 2^LUX_SCALE
#define K5T 0x0138   // 0.61 * 2^RATIO_SCALE
#define B5T 0x016f   // 0.0224 * 2^LUX_SCALE
#define M5T 0x01fc   // 0.0310 * 2^LUX_SCALE
#define K6T 0x019a   // 0.80 * 2^RATIO_SCALE
#define B6T 0x00d2   // 0.0128 * 2^LUX_SCALE
#define M6T 0x00fb   // 0.0153 * 2^LUX_SCALE
#define K7T 0x029a   // 1.3 * 2^RATIO_SCALE
#define B7T 0x0018   // 0.00146 * 2^LUX_SCALE
#define M7T 0x0012   // 0.00112 * 2^LUX_SCALE
#define K8T 0x029a   // 1.3 * 2^RATIO_SCALE
#define B8T 0x0000   // 0.000 * 2^LUX_SCALE
#define M8T 0x0000   // 0.000 * 2^LUX_SCALE
#define K1C 0x0043   // 0.130 * 2^RATIO_SCALE
#define B1C 0x0204   // 0.0315 * 2^LUX_SCALE
#define M1C 0x01ad   // 0.0262 * 2^LUX_SCALE
#define K2C 0x0085   // 0.260 * 2^RATIO_SCALE
#define B2C 0x0228   // 0.0337 * 2^LUX_SCALE
#define M2C 0x02c1   // 0.0430 * 2^LUX_SCALE
#define K3C 0x00c8   // 0.390 * 2^RATIO_SCALE
#define B3C 0x0253   // 0.0363 * 2^LUX_SCALE
#define M3C 0x0363   // 0.0529 * 2^LUX_SCALE
#define K4C 0x010a   // 0.520 * 2^RATIO_SCALE
#define B4C 0x0282   // 0.0392 * 2^LUX_SCALE
#define M4C 0x03df   // 0.0605 * 2^LUX_SCALE
#define K5C 0x014d   // 0.65 * 2^RATIO_SCALE
#define B5C 0x0177   // 0.0229 * 2^LUX_SCALE
#define M5C 0x01dd   // 0.0291 * 2^LUX_SCALE
#define K6C 0x019a   // 0.80 * 2^RATIO_SCALE
#define B6C 0x0101   // 0.0157 * 2^LUX_SCALE
#define M6C 0x0127   // 0.0180 * 2^LUX_SCALE
#define K7C 0x029a   // 1.3 * 2^RATIO_SCALE
#define B7C 0x0037   // 0.00338 * 2^LUX_SCALE
#define M7C 0x002b   // 0.00260 * 2^LUX_SCALE
#define K8C 0x029a   // 1.3 * 2^RATIO_SCALE
#define B8C 0x0000   // 0.000 * 2^LUX_SCALE
#define M8C 0x0000   // 0.000 * 2^LUX_SCALE

/** 
 *  \brief Sensor module for light intensity and par.
 */
class SensorTsl2561 : SensorActuatorModule {
  public:
    // Public Functions
    SensorTsl2561(String lux_instruction_code, int lux_instruction_id, String par_instruction_code, int par_instruction_id);
    void begin(void);
    String get(void);
    String set(String instruction_code, int instruction_id, String instruction_parameter);

    // Public Variables
    int lux_; // lux
    float par_; // (umol)*(m^-2)*(s^-1)
    // note: PAR is likely only valid for Erligpowht 45W LED Red Blue Indoor Garden Plant Grow Light Hanging Lightpanel
    // found @ http://www.amazon.com/gp/product/B00S2DPYQM?psc=1&redirect=true&ref_=oh_aui_detailpage_o08_s03
  private:
    // Private Functions
    void getSensorData(void);
    void getLux(void);
    unsigned long calculateLux(unsigned int iGain, unsigned int tInt,int iType);
    uint8_t readRegister(int deviceAddress, int address);
    void writeRegister(int deviceAddress, int address, uint8_t val);
    String floatToString( double val, unsigned int precision);
    
    // Private Variables
    String lux_instruction_code_;
    int lux_instruction_id_;
    String par_instruction_code_;
    int par_instruction_id_;
    float calibrtion_to_vernier_lux_;
    float calibration_to_vernier_par_;
    float measuring_indoor_par_correction_; //reduction by 14%
    uint32_t read_register_timeout_;
    bool read_register_error_;
    uint8_t CH0_LOW,CH0_HIGH,CH1_LOW,CH1_HIGH;
    uint16_t ch0,ch1;
    unsigned long chScale;
    unsigned long channel1;
    unsigned long channel0;
    unsigned long  ratio1;
    unsigned int b;
    unsigned int m;
    unsigned long temp;
    unsigned long lux;
 };

#endif // SENSOR_TSL2561_H_




