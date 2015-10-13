/** 
 *  \file module_handler.h
 *  \brief Handles all module integration.
 *  \author Jake Rye
 */
/**
 * @mainpage Groduino
 * @author Jake Rye
 * \details This is the microcontroller code. It is uploaded to the Arduino Mega. It's purpose is to 
 * be the firmware for applicable sensor/actuator modules. It has been written 
 * in such a way that each new type of sensor and actuator is its own module. Each 
 * sensor/actuator module must contain a class with the following methods: void begin(void), 
 * String get(void), String set(String instruction_code, int instruction_id, String instruction_parameter).
 * The existance of these methods are enforced by using the SensorActuatorModule interface. Each 
 * sensor/actuator must also be instantiated such that its modularity is prioritized. For example,
 * passing in pins, instruction codes, and instruction ids (all parameters that are subject
 * to change depending on the context the module is used in) would look something like:
 * ModuleName(int pin, String instruction_code, int instruction_id).
 * Clearly this example is not representative of all modules that will be created so it 
 * is up to the programmer to use their best judgement. Another important note is that this 
 * code documentation is generated with doxygen so all markdown should follow compliant formats.
 */
#ifndef MODULE_HANDLER_H
#define MODULE_HANDLER_H

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

/**
 * \brief Abstract class used as the interface for all Sensor Actuator Modules
 */
class SensorActuatorModule {
  public:
    /**
     * \brief Called once at beginning of program to initialize modules.
     */
    virtual void begin(void) = 0;
    
    /**
     * \brief Called once per loop iteration to update and report module data to controller.
     */
    virtual String get(void) = 0;

    /**
     * \brief Called once per loop iteration to update module state.
     * If response is generated from updating, reports response to controller.
     */
    virtual String set(String instruction_code, int instruction_id, String instruction_parameter) = 0;
};

/**
 * \brief Called once to initialize all modules.
 *  Runs once at the beginning of the program.
 *  Calls all module *.begin() functions
 */
void initializeModules(void);

/**
 * \brief Handles all incoming messages from the controller.
 * If new message is available, receive message and pass to handler function. 
 * If handler function returns response message, send out.
 */
void updateIncomingMessage(void);

/**
 * \brief Handles all outgoing messages to the controller.
 * Polls all objects using their *.get() function and appends to message stream
 * Sends message stream to controller.
 */
void updateStreamMessage(void);

/**
 * \brief A structure to represent instruction parameters
 * @param code is a 4-letter instruction code, not necessarily unique
 * @param id is the unique ID for the instance of the module
 * @param parameter is the string that contains the message addressed to that specific instruction
 * code and id pair
 * @param valid indicates whether or not the instruction is valid
 */
struct Instruction {
  String code;
  int id;
  String parameter;
  bool valid;
};

/**
 * \brief Messages from controller are handled by this function.
 * Each message is a single instruction string. Instruction string gets
 * broken into instruction code, id, and parameter. Passed in piecewise to 
 * <module>.set function. If a return message is generated from the 
 * <module>.set function, this function returns that message.
 */
String handleIncomingMessage(void);

/** 
 *  \brief Formats an instruction string into an instruction struct.
 *  Message is broken into 3 parts: Instruction Code, Instruction ID, Instruction Parameter 
 *  then returns the parts as an Instruction Object
 *  Also, the Instruction Valid parameter is set accordingly.
 */
Instruction parseIncomingMessage(String message);

#endif // MODULE_HANDLER_H_
