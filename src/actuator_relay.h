/** 
 *  \file actuator_relay.h
 *  \brief Actuator module for an active low SPST-NO relay.
 *  \author Jake Rye
 */
 
#ifndef ACTUATOR_RELAY_H
#define ACTUATOR_RELAY_H

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include "module_handler.h"

/**
 * \brief Actuator module for an active low SPST-NO relay.
 */
class ActuatorRelay : SensorActuatorModule {
  //------------------------------------------------PUBLIC---------------------------------------------//
  public:
    /* Public Functions */
    /**
     * \brief Used to construct an instance of the ActuatorRelay class.
     * @param[in] pin is the control line for the relay
     * @param[in] instruction_code is the 4-letter instruction
     * code associated with the actuation the relay is switching. Instruction 
     * codes are not necessarily unique.
     * @param[in] instruction_id is the ID associated with the specific
     * actuator that is being switched. Each ID is unique.
     * 
     * To clarify how parameters are used, consider the following case.
     * Imagine there are two air heaters each connected to their own relay. They would
     * have the same instruction code: AAHE (Actuator Air Heater) but would both
     * need to be addressable. Thus the instruction_ids of the heaters would be
     * 1 and 2.
     */
    ActuatorRelay(int pin, String instruction_code, int instruction_id);
    
    /**
     * \brief Called once to setup module.
     * Sets specified control pin to be in OUTPUT mode.
     */
    void begin(void); 

    /**
     * \brief Returns JSON string with module data.
     * Data: "<instruction_code> <instruction_id> <value>",
     * Example: "AAHU 1 1",
     */
    String get(void); 

    /**
     * \brief Sets relay to be on / off.
     * If instruction_code & instruction_id match the code and id 
     * module was instantiated with, the relay will enter the state
     * passed in through instruction_parameter.
     * If instruction_parameter = "1", relay is ON (switch closed).
     * If instruction_paremerter = "0", relay is OFF (switch open).
     */
    String set(String instruction_code, int instruction_id, String instruction_parameter);

    // Public Variables
    int value_;
    
  //------------------------------------------------PUBLIC---------------------------------------------// 
  private:
    // Private Functions
    void turnOn(void);
    void turnOff(void);
    
    // Private Variables
    int pin_;
    int instruction_id_;
    String instruction_code_;
};

#endif // ACTUATOR_RELAY_H_
