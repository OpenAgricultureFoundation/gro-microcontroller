/** 
 *  \file sensor_tsl2561.cpp
 *  \brief Sensor module for light intensity and par.
 *  \details See sensor_tsl2561.h for details.
 */
#include "sensor_tsl2561.h"

//----------------------------------------------PUBLIC------------------=----------------------------//
SensorTsl2561::SensorTsl2561(String lux_instruction_code, int lux_instruction_id, String par_instruction_code, int par_instruction_id) {
  lux_instruction_code_ = lux_instruction_code;
  lux_instruction_id_ = lux_instruction_id;
  par_instruction_code_ = par_instruction_code;
  par_instruction_id_ = par_instruction_id;
  
}

void SensorTsl2561::begin() {
  Wire.begin();
  writeRegister(TSL2561_Address,TSL2561_Control,0x03);  // POWER UP
  writeRegister(TSL2561_Address,TSL2561_Timing,0x00);  //No High Gain (1x), integration time of 13ms
  writeRegister(TSL2561_Address,TSL2561_Interrupt,0x00);
  writeRegister(TSL2561_Address,TSL2561_Control,0x00);  // POWER Down
  
  calibrtion_to_vernier_lux_ = 0.78;
  calibration_to_vernier_par_ = 0.02;
  measuring_indoor_par_correction_ = 0.86; //reduction by 14%
  read_register_timeout_ = 5; // milliseconds
}

String SensorTsl2561::get(void) {
  // Get Sensor Data
  getSensorData();

    // Initialize Message
  String message = "";

  // Handle Errors
  if (read_register_error_) {
    message += "\"GERR 4\":\"tsl2561 read register timeout\",";
    lux_ = 0;
  }

  // Append Light Intensity
  message += "\"";
  message += lux_instruction_code_;
  message += " ";
  message += lux_instruction_id_;
  message += "\":";
  message += lux_;
  message += ",";

  // Append Light Par
  message += "\"";
  message += par_instruction_code_;
  message += " ";
  message += par_instruction_id_;
  message += "\":";
  message += floatToString(par_, 100);
  message += ",";

  // Return Message
  return message;
}


String SensorTsl2561::set(String instruction_code, int instruction_id, String instruction_parameter){
  return "";
}


//------------------------------------------------PRIVATE--------------------------------------------//

void SensorTsl2561::getSensorData(void) {
  float lux_average = 0;
  writeRegister(TSL2561_Address,TSL2561_Control,0x03);  // POWER UP
  delay(14);

  float samples = 40;
  int i;
  for (i=0; i<samples; i++) {
    getLux();
    if(ch0/ch1 < 2 && ch0 > 4900) {
      lux_ = -1;
      return;  //ch0 out of range, but ch1 not. the lux is not valid in this situation.
    }
    lux_average += (float) calculateLux(0, 0, 0);
  }
  lux_average /= samples;
  lux_ = lux_average*calibrtion_to_vernier_lux_;
  par_ = lux_average*calibration_to_vernier_par_*measuring_indoor_par_correction_;
  
  writeRegister(TSL2561_Address,TSL2561_Control,0x00);  // POWER Down
}

uint8_t SensorTsl2561::readRegister(int deviceAddress, int address) {
  read_register_error_ = 0;
  uint8_t value;
  Wire.beginTransmission(deviceAddress);
  Wire.write(address);                // register to read
  Wire.endTransmission();
  Wire.requestFrom(deviceAddress, 1); // read a byte

  uint32_t start_time = millis();
  while (1) {
    if (Wire.available()) {
      break;
    }
    if (millis() - start_time > read_register_timeout_) {
      read_register_error_ = 1;
      return 0;
    }
  }
  value = Wire.read();
  return value;
}

void SensorTsl2561::writeRegister(int deviceAddress, int address, uint8_t val) {
  Wire.beginTransmission(deviceAddress);  // start transmission to device
  Wire.write(address);                    // send register address
  Wire.write(val);                        // send value to write
  Wire.endTransmission();                 // end transmission
  //delay(100);
}

void SensorTsl2561::getLux(void) {
  CH0_LOW=readRegister(TSL2561_Address,TSL2561_Channal0L);
  CH0_HIGH=readRegister(TSL2561_Address,TSL2561_Channal0H);
  //read two bytes from registers 0x0E and 0x0F
  CH1_LOW=readRegister(TSL2561_Address,TSL2561_Channal1L);
  CH1_HIGH=readRegister(TSL2561_Address,TSL2561_Channal1H);

  ch0 = (CH0_HIGH<<8) | CH0_LOW;
  ch1 = (CH1_HIGH<<8) | CH1_LOW;
}

unsigned long SensorTsl2561::calculateLux(unsigned int iGain, unsigned int tInt,int iType) {
  switch (tInt) {
    case 0:  // 13.7 msec
      chScale = CHSCALE_TINT0;
      break;
    case 1: // 101 msec
      chScale = CHSCALE_TINT1;
      break;
    default: // assume no scaling
      chScale = (1 << CH_SCALE);
      break;
  }
  if (!iGain) {
    chScale = chScale << 4; // scale 1X to 16X
  }
  // scale the channel values
  channel0 = (ch0 * chScale) >> CH_SCALE;
  channel1 = (ch1 * chScale) >> CH_SCALE;

  ratio1 = 0;
  if (channel0!= 0) {
    ratio1 = (channel1 << (RATIO_SCALE+1))/channel0;
  }
  // round the ratio value
  unsigned long ratio = (ratio1 + 1) >> 1;

  switch (iType) {
    case 0: // T package
      if ((ratio >= 0) && (ratio <= K1T)) {
        b=B1T; m=M1T;
      }
      else if (ratio <= K2T) {
        b=B2T; m=M2T;
      }
      else if (ratio <= K3T) {
        b=B3T; m=M3T;
      }
      else if (ratio <= K4T) {
        b=B4T; m=M4T;
      }
      else if (ratio <= K5T) {
        b=B5T; m=M5T;
      }
      else if (ratio <= K6T) {
        b=B6T; m=M6T;
      }
      else if (ratio <= K7T) {
        b=B7T; m=M7T;
      }
      else if (ratio > K8T) {
        b=B8T; m=M8T;
      }
      break;
    case 1:// CS package
      if ((ratio >= 0) && (ratio <= K1C)) {
        b=B1C; m=M1C;
      }
      else if (ratio <= K2C) {
        b=B2C; m=M2C;
      }
      else if (ratio <= K3C) {
        b=B3C; m=M3C;
      }
      else if (ratio <= K4C) {
        b=B4C; m=M4C;
      }
      else if (ratio <= K5C) {
        b=B5C; m=M5C;
      }
      else if (ratio <= K6C) {
        b=B6C; m=M6C;
      }
      else if (ratio <= K7C) {
        b=B7C; m=M7C;
      }
  }
  temp=((channel0*b)-(channel1*m));
  if(temp<0) {
    temp=0;
  }
  temp+=(1<<(LUX_SCALE-1));
  // strip off fractional portion
  lux=temp>>LUX_SCALE;
  return (lux);
}

String SensorTsl2561::floatToString( double val, unsigned int precision) {
// prints val with number of decimal places determine by precision
// NOTE: precision is 1 followed by the number of zeros for the desired number of decimial places
// example: printDouble( 3.1415, 100); // prints 3.14 (two decimal places)
  String str = "";
  str += int(val);  //prints the int part
  str += "."; // print the decimal point
  unsigned int frac;
  if(val >= 0) {
    frac = (val - int(val)) * precision;
  }
  else {
    frac = (int(val)- val ) * precision;
  }
  str += int(frac);
  return str;
} 
