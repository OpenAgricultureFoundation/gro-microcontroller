/** 
 *  \file communication.h
 *  \brief Handles a character based serial communication protocol.
 *  \details Uses ascii control codes and checksum. Protocol for a
 *  packed message: SOH<message_size>STX<message>ETX<message_checksum>EOT
 *  \author Jake Rye
 */
#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

/** 
 *  \brief Handles a character based serial communication protocol. 
 */
class Communication {
  public:
    // Public Functions
    void begin(void);
    void send(String message);
    bool available(void);
    String receive(void);
    
    // Public Variables
    bool not_connected_;

  private:
    // Private Functions
    String getPackedMessage(String message);
    String getChecksum(String message);
    String getUnpackedMessage(String message);
    bool checkStartOfHeader(String message);
    int parseHeader(String message);
    String parseText(String message);
    String parseFooter(String message);
    
    // Private Variables
    uint32_t kBaudRate;
    uint32_t kEstablishConnectionTimeout; // milliseconds
    uint32_t kReceiveTimeout; // milliseconds
    char kStartOfHeaderChar;
    char kStartOfTextChar;
    char kEndOfTextChar;
    char kEndOfTransmissionChar;
    char kEnquireChar;
    char kAcknowledgeChar; 
};

#endif // COMMUNICATION_H_
