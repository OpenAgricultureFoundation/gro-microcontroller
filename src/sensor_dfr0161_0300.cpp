/** 
 *  \file sensor_dfr0161_0300.cpp
 *  \brief Sensor module for water ph, ec, and temperature. 
 *  \details See sensor_dfr0161_0300.h for details.
 *  \author Jake Rye
 */
#include "sensor_dfr0161_0300.h"

//------------------------------------------PUBLIC FUNCTIONS----------------------------------------//
SensorDfr01610300::SensorDfr01610300(int ph_pin, String ph_instruction_code, int ph_instruction_id, int temperature_pin, String temperature_instruction_code, int temperature_id, int ec_pin, String ec_instruction_code, int ec_id, int ec_enable_pin) {
  ph_pin_ = ph_pin;
  ph_instruction_code_ = ph_instruction_code;
  ph_instruction_id_ = ph_instruction_id;
  temperature_pin_ = temperature_pin;
  temperature_instruction_code_ = temperature_instruction_code;
  temperature_id_ = temperature_id;
  ec_pin_ = ec_pin;
  ec_instruction_code_ = ec_instruction_code;
  ec_id_ = ec_id;
  ec_enable_pin_ = ec_enable_pin;
}

void SensorDfr01610300::begin(void) {
  // Construct Objects
  ds_ = new OneWire(temperature_pin_);
  ph_filter_ = new MovingAverageFilter(10);
  ec_filter_ = new MovingAverageFilter(10);
  temperature_filter_ = new MovingAverageFilter(10);
  
  // Configure Initial State
  pinMode(ec_enable_pin_, OUTPUT);
  digitalWrite(ec_enable_pin_, LOW);
  last_update_was_ec_ = true;
  prev_update_time_ = millis();

  // Set Configuration Parameters
  ec_on_delay_ = 400; // milliseconds
  ec_off_delay_ = 400; // milliseconds
  
  // Set Calibration Parameters
  ph_calibration_coefficient_ = 3.5;
  ph_calibration_offset_ = -0.1;
  ec_calibration_offset_ = 0.15;
}

String SensorDfr01610300::get(void) {
  // Get PH, Temperature, & EC Data
  getSensorData();

  // Initialize Message
  String message = "";

  // Append PH
  message += "\"";
  message += ph_instruction_code_;
  message += " ";
  message += ph_instruction_id_;
  message += "\":";
  message += String(ph_filtered,1);
  message += ",";

  // Append Temperature
  message += "\"";
  message += temperature_instruction_code_;
  message += " ";
  message += temperature_id_;
  message += "\":";
  message += String(temperature_filtered,1);
  message += ",";

  // Append EC
  message += "\"";
  message += ec_instruction_code_;
  message += " ";
  message += ec_id_;
  message += "\":";
  message += String(ec_filtered,1);
  message += ",";

  // Return Message
  return message;
}

String SensorDfr01610300::set(String instruction_code, int instruction_id, String instruction_parameter) {
  return "";
}

//------------------------------------------PRIVATE FUNCTIONS----------------------------------------//
void SensorDfr01610300::getSensorData(void) {
  // Update Temperature
  temperature_raw = getTemperature();
  temperature_filtered = (float)round(temperature_filter_->process(temperature_raw)*10)/10; // set accuracy to +-0.05
  
  // Alternate Updating pH & EC With Enforced Delays
  // Need to do this because EC interferes with PH sensor. EC power lines connected to a relay 
  // controlled by ec_enable_pin_. Need on_delay because takes some time for ec sensor to initialize
  // Need off_delay becasue takes some time for ec noise to dissipate.
  if (last_update_was_ec_ && (millis() - prev_update_time_ > ec_off_delay_)) {
    // Update pH
    ph_raw = getPh();
    ph_filtered = (float)round(ph_filter_->process(ph_raw)*10)/10; // set accuracy to +-0.05
    digitalWrite(ec_enable_pin_, HIGH);
    prev_update_time_ = millis();
    last_update_was_ec_ = false;
  }
  else if (!last_update_was_ec_ && (millis() - prev_update_time_ > ec_on_delay_)) {
    // Update EC
    ec_raw = getEc(temperature_filtered);
    ec_filtered = (float)round(ec_filter_->process(ec_raw)*10)/10; // set accuracy to +-0.05
    digitalWrite(ec_enable_pin_, LOW);
    prev_update_time_ = millis();
    last_update_was_ec_ = true;
  }
}

float SensorDfr01610300::getPh(void) {
  // Sampling Specifications
  int samples = 40;
  int voltage[samples];
  const int sample_time_delta = 20; // milliseconds

  // Acquire Samples
  for (int i=0; i<samples; i++) {
    voltage[i] = analogRead(ph_pin_);
  }

  // Remove Min & Max Samples, Average, Convert to Voltage
  double volts = avergeArray(voltage, samples)*5.0/1024;

  // Convert Average Voltage to pH
  return ph_calibration_coefficient_*volts + ph_calibration_offset_;;
}

float SensorDfr01610300::getTemperature(void) {
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

float SensorDfr01610300::getEc(float temperature_value) { 
  int analog_sum = 0;
  const int samples = 20;
  
  for (int i=0; i<samples; i++) {
    analog_sum += analogRead(ec_pin_);
  }
  float analog_average = (float) analog_sum / samples;
  float analog_voltage = analog_average*(float)5000/1024;
  float temperature_coefficient = 1.0 + 0.0185*(temperature_value - 25.0);
  float voltage_coefficient = analog_voltage / temperature_coefficient; 
  
  if(voltage_coefficient < 0) {
    return 0;
    //Serial.println("No solution!");   //25^C 1413us/cm<-->about 216mv  if the voltage(compensate)<150,that is <1ms/cm,out of the range
  }
  else if (voltage_coefficient > 3300) {
    return 0;
    //Serial.println("Out of the range!");  //>20ms/cm,out of the range
  }
  else { 
    if(voltage_coefficient <= 448) {
      return (6.84*voltage_coefficient-64.32)/1000 + ec_calibration_offset_;   //1ms/cm<EC<=3ms/cm
    }
    else if (voltage_coefficient <= 1457) {
      return (6.98*voltage_coefficient-127)/1000 + ec_calibration_offset_;  //3ms/cm<EC<=10ms/cm
    }
    else {
      return (5.3*voltage_coefficient+2278)/1000 + ec_calibration_offset_; //10ms/cm<EC<20ms/cm
    }
  }
}

float SensorDfr01610300::avergeArray(int* arr, int number){
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


