/** 
 *  \file sensor_dfr0161_0300.cpp
 *  \brief Sensor module for water ph, ec, and temperature. 
 *  \details See sensor_dfr0161_0300.h for details.
 *  \author Jake Rye
 */
#include "sensor_ds18b20.h"

//------------------------------------------PUBLIC FUNCTIONS----------------------------------------//
SensorDs18b20::SensorDs18b20(int temperature_pin, String temperature_instruction_code, int temperature_id) {
  temperature_pin_ = temperature_pin;
  temperature_instruction_code_ = temperature_instruction_code;
  temperature_id_ = temperature_id;
}

void SensorDs18b20::begin(void) {
  // Construct Objects
  ds_ = new OneWire(temperature_pin_);
  temperature_filter_ = new MovingAverageFilter(10);
}

String SensorDs18b20::get(void) {
  // Get PH, Temperature, & EC Data
  getSensorData();

  // Initialize Message
  String message = "";

  // Append Temperature
  message += "\"";
  message += temperature_instruction_code_;
  message += " ";
  message += temperature_id_;
  message += "\":";
  message += String(temperature_filtered,1);
  message += ",";

  // Return Message
  return message;
}

String SensorDs18b20::set(String instruction_code, int instruction_id, String instruction_parameter) {
  return "";
}

//------------------------------------------PRIVATE FUNCTIONS----------------------------------------//
void SensorDs18b20::getSensorData(void) {
  // Update Temperature
  temperature_raw = getTemperature();
  temperature_filtered = (float)round(temperature_filter_->process(temperature_raw)*10)/10; // set accuracy to +-0.05
}


float SensorDs18b20::getTemperature(void) {
  float temperature_value;
  // Read Temperature
  byte present = ds_->reset();
  ds_->select(temperature_address_);    
  ds_->write(0xBE); // Read Scratchpad            
  for (int i = 0; i < 9; i++) { // we need 9 bytes
    temperature_data_[i] = ds_->read();
  }         
  ds_->reset_search();           
  byte MSB = temperature_data_[1];
  byte LSB = temperature_data_[0];        
  float tempRead = ((MSB << 8) | LSB); // using two's compliment
  temperature_value = tempRead / 16;

  // Start Conversion For Next Temperature Reading
  if ( !ds_->search(temperature_address_)) {
     // Serial.println("no more sensors on chain, reset search!");
      ds_->reset_search();
  }      
  if ( OneWire::crc8(temperature_address_, 7) != temperature_address_[7]) {
     // Serial.println("CRC is not valid!");
  }        
  if ( temperature_address_[0] != 0x10 && temperature_address_[0] != 0x28) {
     // Serial.print("Device is not recognized!");
  }      
  ds_->reset();
  ds_->select(temperature_address_);
  ds_->write(0x44,1); // start conversion, with parasite power on at the end

  // Return Temperature
  return temperature_value;
}

float SensorDs18b20::avergeArray(int* arr, int number){
  int i;
  int max,min;
  float avg;
  long amount=0;
  if(number<=0){
    Serial.println("Error number for the array to averaging!/n");
    return 0;
  }
  if(number<5){   //less than 5, calculated directly statistics
    for(i=0;i<number;i++){
      amount+=arr[i];
    }
    avg = amount/number;
    return avg;
  }else{
    if(arr[0]<arr[1]){
      min = arr[0];max=arr[1];
    }
    else{
      min=arr[1];max=arr[0];
    }
    for(i=2;i<number;i++){
      if(arr[i]<min){
        amount+=min;        //arr<min
        min=arr[i];
      }else {
        if(arr[i]>max){
          amount+=max;    //arr>max
          max=arr[i];
        }else{
          amount+=arr[i]; //min<=arr<=max
        }
      }//if
    }//for
    avg = (float)amount/(number-2);
  }//if
  return avg;
}


