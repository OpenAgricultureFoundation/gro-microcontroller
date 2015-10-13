/** 
 *  \file module_handler.cpp
 *  \brief Handles all module integration. 
 *  \details See module_handler.h for details.
 *  \author Jake Rye
 */
#include "module_handler.h"

// Include Module Libraries
#include "communication.h"
#include "sensor_dfr0161_0300.h"
#include "sensor_tsl2561.h"
#include "sensor_dht22.h"
#include "sensor_gc0011.h"
#include "actuator_relay.h"
#include "sensor_contact_switch.h"

// Declare Module Objects
Communication communication;
SensorTsl2561 sensor_tsl2561_light_intensity_default("SLIN", 1, "SLPA", 1);
SensorDfr01610300 sensor_dfr01610300_water_ph_temperature_ec_default(A1, "SWPH", 1, 5, "SWTM", 1, A2, "SWEC", 1, 22);
SensorDht22 sensor_dht22_air_temperature_humidity_default(A0, "SATM", 1, "SAHU", 1);
SensorGc0011 sensor_gc0011_air_co2_temperature_humidity_default(12, 11, "SACO", 1, "SATM", 2, "SAHU", 2);
SensorContactSwitch sensor_contact_switch_general_shell_open_default(4, "SGSO", 1);
SensorContactSwitch sensor_contact_switch_general_window_open_default(3, "SGWO", 1);
ActuatorRelay actuator_relay_air_heater_default(6, "AAHE", 1); // AC port 4
ActuatorRelay actuator_relay_light_panel_default(8, "ALPN", 1); // AC port 2
ActuatorRelay actuator_relay_air_humidifier_default(9, "AAHU", 1); // AC port 1
ActuatorRelay actuator_relay_air_vent_default(14, "AAVE", 1);
ActuatorRelay actuator_relay_air_circulation_default(15, "AACR", 1);
ActuatorRelay actuator_relay_light_chamber_illumination_default(53, "ALPN", 2); 
ActuatorRelay actuator_relay_light_motherboard_illumination_default(52, "ALMI", 1);

void initializeModules(void) {
  communication.begin();
  sensor_dfr01610300_water_ph_temperature_ec_default.begin();
  sensor_tsl2561_light_intensity_default.begin();
  sensor_dht22_air_temperature_humidity_default.begin();
  sensor_gc0011_air_co2_temperature_humidity_default.begin();
  sensor_contact_switch_general_shell_open_default.begin();
  sensor_contact_switch_general_window_open_default.begin();
  actuator_relay_air_heater_default.begin();
  actuator_relay_air_humidifier_default.begin();
  actuator_relay_air_vent_default.begin();
  actuator_relay_air_circulation_default.begin();
  actuator_relay_light_panel_default.begin();
  actuator_relay_light_chamber_illumination_default.begin();
  actuator_relay_light_motherboard_illumination_default.begin();

  // Set Default States
  actuator_relay_air_circulation_default.set("AACR", 1, "1");
  actuator_relay_light_motherboard_illumination_default.set("ALMI", 1, "1");
  actuator_relay_air_vent_default.set("AAVE", 1, "1");
}

void updateIncomingMessage(void) {
  // Check for Message(s) And Handle If Necessary
  String response_message = "";
  while (communication.available()) { // read in message(s) until nothing in serial buffer
    response_message += handleIncomingMessage();
  }
  // Append Responses From Message(s) Then Send
  if (response_message != "") {
    response_message = "\"GTYP\":\"Response\"," + response_message;
    response_message += "\"GEND\":0";
    communication.send(response_message);
  }
}

void updateStreamMessage(void) {
  // Initialize Stream Message
  String stream_message = "\"GTYP\":\"Stream\",";

  // Get Stream Message
  stream_message += sensor_dfr01610300_water_ph_temperature_ec_default.get();
  stream_message += sensor_tsl2561_light_intensity_default.get();
  stream_message += sensor_dht22_air_temperature_humidity_default.get(); // does not work on 1.0
  stream_message += sensor_gc0011_air_co2_temperature_humidity_default.get();
  stream_message += sensor_contact_switch_general_shell_open_default.get();
  stream_message += sensor_contact_switch_general_window_open_default.get();
  stream_message += actuator_relay_air_heater_default.get();
  stream_message += actuator_relay_air_humidifier_default.get();
  stream_message += actuator_relay_air_vent_default.get();
  stream_message += actuator_relay_air_circulation_default.get();
  stream_message += actuator_relay_light_panel_default.get();
  stream_message += actuator_relay_light_chamber_illumination_default.get();
  stream_message += actuator_relay_light_motherboard_illumination_default.get();

  // Return Stream Message
  stream_message += "\"GEND\":0";

  // Send Stream Message
  communication.send(stream_message);
}

String handleIncomingMessage(void) {
  // Parse Message into: Instruction Code - ID - Parameter
  String return_message = "";
  String incoming_message = communication.receive();
  Instruction instruction = parseIncomingMessage(incoming_message);

  // Pass Parsed Message To All Objects and Update Return Message if Applicable
  if (instruction.valid) {
    return_message += sensor_dfr01610300_water_ph_temperature_ec_default.set(instruction.code, instruction.id, instruction.parameter);
    return_message += sensor_tsl2561_light_intensity_default.set(instruction.code, instruction.id, instruction.parameter);
    return_message += sensor_dht22_air_temperature_humidity_default.set(instruction.code, instruction.id, instruction.parameter);
    return_message += sensor_gc0011_air_co2_temperature_humidity_default.set(instruction.code, instruction.id, instruction.parameter);
    return_message += sensor_contact_switch_general_shell_open_default.set(instruction.code, instruction.id, instruction.parameter);
    return_message += sensor_contact_switch_general_window_open_default.set(instruction.code, instruction.id, instruction.parameter);
    return_message += actuator_relay_air_heater_default.set(instruction.code, instruction.id, instruction.parameter);
    return_message += actuator_relay_air_humidifier_default.set(instruction.code, instruction.id, instruction.parameter);
    return_message += actuator_relay_air_vent_default.set(instruction.code, instruction.id, instruction.parameter);
    return_message += actuator_relay_air_circulation_default.set(instruction.code, instruction.id, instruction.parameter);
    return_message += actuator_relay_light_panel_default.set(instruction.code, instruction.id, instruction.parameter);
    return_message += actuator_relay_light_chamber_illumination_default.set(instruction.code, instruction.id, instruction.parameter);
    return_message += actuator_relay_light_motherboard_illumination_default.set(instruction.code, instruction.id, instruction.parameter);
  }
  return return_message;
}

Instruction parseIncomingMessage(String message) {
  // Initialize Instruction
  Instruction instruction;
  instruction.valid = 0;

  // Get Instruction Data
  int len = message.length();
  int first_space = message.indexOf(" ");
  if ((first_space > 0) && (len > first_space)) {
    int second_space = message.indexOf(" ", first_space + 1);
    if ((second_space > 0) && (second_space < len - 1)) {
      // Received good message
      instruction.code = message.substring(0, 4);
      instruction.id = (message.substring(first_space, second_space)).toInt();
      instruction.parameter = message.substring(second_space + 1, len);
      instruction.valid = 1;
    }
  }

  // Return Instruction Data
  return instruction;
}

