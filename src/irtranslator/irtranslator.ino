/* IRtranslator v1.0
 * -----------------
 *  Written by Matt Collinge.
 *  https://github.com/mattcuk/IRtranslator
 *  Arduino IR Translator. Converts incoming IR codes from one manufacturer to another set of IR codes from a different manufacturer.
 */

#include <IRremote.h> // >v3.0.0
                                           
#define PIN_RECV 2
#define PIN_SEND 3

void setup()  
{  
  Serial.begin(115200); //initialize serial connection to print on the Serial Monitor of the Arduino IDE

  Serial.print("Initialise IrSender on PIN ");
  Serial.println(PIN_SEND);
  IrSender.begin(PIN_SEND, false); // Initialise the IR sender

  // Uncomment this block if you want this to happen during boot-up
  // Send POWER ON to speaker IR as the board boots up (power applied from the TV's USB port)
  /*delay(10000); // 10 second delay when powering up.. helped fix a problem where the Samsung TV powered the USB port momentarily after 2+ hours :/
  Serial.println("Sending alternative POWER command (first boot when TV's USB port activated)");
  // Raw data for Edifier R1855DB power button
  unsigned int rawData[] = { 8980,4420, 580,520, 580,520, 580,570, 580,520, 630,1620, 580,520, 580,520, 630,520, 580,1620, 630,1620, 630,1620, 580,520, 580,520, 580,1670, 580,1620, 630,1620, 630,470, 630,1620, 630,1570, 630,520, 580,520, 630,520, 580,1620, 630,520, 580,1670, 580,470, 630,520, 580,1620, 630,1620, 630,1620, 630,470, 630,1620, 580 };
  IrSender.sendRaw(rawData, sizeof(rawData)/sizeof(int), 38);
  delay(5); // Make sure data got sent */

  Serial.print("Initialise IrReceiver on PIN ");
  Serial.println(PIN_RECV);
  IrReceiver.begin(PIN_RECV, true); // Initializes the IR receiver
}  

int sendDelay = 5; // Can be adjusted (increased from 5) if you have issues with the spacing of IR transmits & your equipment seeing multiple sends as one long code
int justBooted = true; // Set to false if you don't want special behaviour on the first button press after booting (see line 57 onwards)

// Command sequence matching (look for a pattern & do something special)
int seqLen = 4; // Need to know the sequence length
char seqMatch[] = { 'M', 'M', 'M', 'U' }; // Sequence we're looking for
char seqCurrent[4]; // Current sequence

void loop()  
{
  if (IrReceiver.decode()) {
    IrReceiver.printIRResultShort(&Serial); // Prints a summary of the received data

    // Output the raw data (helps learn your particular remote)
    IrReceiver.compensateAndPrintIRResultAsCArray(&Serial, true); // Output the results as uint16_t source code array of micros

    // Amazon Basics Soundbar Commands/Buttons (this will vary depending on what manufacturer you choose to map into your Fire TV remote)
    if (IrReceiver.decodedIRData.address == 0) {

        delay(5); // Seems to need 5ms to cool-down from recieving an IR signal
        char currCommand = 'X';

        if (justBooted) {
          
          // This can be used to power on your speakers when one of the remote buttons is pressed for the first time after booting the Arduino
          // It's useful when the POWER ON block is causing you issues in setup(). To disable this behaviour simply set the justBooted variable to false on line 36.
          justBooted = false;
          Serial.println("Sending alternative POWER command (first booted)");
          // Raw data for Edifier R1855DB power button
          unsigned int rawData[] = { 8980,4420, 580,520, 580,520, 580,570, 580,520, 630,1620, 580,520, 580,520, 630,520, 580,1620, 630,1620, 630,1620, 580,520, 580,520, 580,1670, 580,1620, 630,1620, 630,470, 630,1620, 630,1570, 630,520, 580,520, 630,520, 580,1620, 630,520, 580,1670, 580,470, 630,520, 580,1620, 630,1620, 630,1620, 630,470, 630,1620, 580 };
          IrSender.sendRaw(rawData, sizeof(rawData)/sizeof(int), 38);
          delay(sendDelay); // Make sure data got sent */
          
        } else {

          // Look for the buttons we want mapped from the Fire TV remote to the Edifier speakers
          if (IrReceiver.decodedIRData.command == 0x40) {
              Serial.println("Received POWER .. sending alternative command");
              // Raw data for Edifier R1855DB power button
              unsigned int rawData[] = { 8980,4420, 580,520, 580,520, 580,570, 580,520, 630,1620, 580,520, 580,520, 630,520, 580,1620, 630,1620, 630,1620, 580,520, 580,520, 580,1670, 580,1620, 630,1620, 630,470, 630,1620, 630,1570, 630,520, 580,520, 630,520, 580,1620, 630,520, 580,1670, 580,470, 630,520, 580,1620, 630,1620, 630,1620, 630,470, 630,1620, 580 };
              IrSender.sendRaw(rawData, sizeof(rawData)/sizeof(int), 38);
              currCommand = 'P';
              delay(40000); // 40 sec delay so that power doesn't accidently get toggled (the TVs USB power should auto off after 20 sec.. powering this all down)
          } else if (IrReceiver.decodedIRData.command == 0x41) {
              Serial.println("Received VOL Up .. sending alternative command");
              // Raw data for Edifier R1855DB volume up
              unsigned int rawData[] = { 8880,4520, 530,570, 530,570, 580,570, 530,570, 580,1670, 530,570, 530,570, 580,570, 530,1670, 580,1670, 530,1670, 580,570, 530,570, 530,1720, 530,1670, 580,1670, 530,570, 580,1670, 530,1670, 580,570, 530,570, 530,620, 530,570, 580,520, 580,1670, 530,570, 580,570, 530,1670, 580,1670, 530,1670, 580,1670, 530,1670, 580 };
              IrSender.sendRaw(rawData, sizeof(rawData)/sizeof(int), 38);
              currCommand = 'U';
              delay(sendDelay);
          } else if (IrReceiver.decodedIRData.command == 0x45) {
              Serial.println("Received VOL Down .. sending alternative command");
              // Raw data for Edifier R1855DB volume down
              unsigned int rawData[] = { 8930,4420, 580,570, 580,520, 580,520, 580,570, 580,1620, 580,570, 580,520, 580,520, 580,1670, 580,1620, 630,1620, 580,520, 580,570, 580,1620, 580,1670, 580,1670, 580,1620, 580,1670, 580,1620, 580,570, 530,620, 530,520, 630,1620, 580,520, 580,570, 580,520, 580,570, 580,1620, 580,1670, 580,1620, 580,570, 580,1620, 580 };
              IrSender.sendRaw(rawData, sizeof(rawData)/sizeof(int), 38);
              currCommand = 'D';
              delay(sendDelay);
          } else if (IrReceiver.decodedIRData.command == 0x48) {
              Serial.println("Received VOL Mute .. sending alternative command");
              // Raw data for Edifier R1855DB mute button
              unsigned int rawData[] = { 8930,4470, 580,520, 580,520, 630,520, 580,520, 580,1670, 580,520, 580,570, 580,520, 580,1620, 580,1670, 580,1670, 580,520, 580,520, 580,1670, 580,1670, 580,1620, 580,1670, 580,520, 580,520, 630,520, 580,520, 630,470, 630,1620, 580,520, 630,520, 580,1620, 580,1670, 580,1670, 580,1620, 580,1670, 580,520, 580,1670, 580 };
              IrSender.sendRaw(rawData, sizeof(rawData)/sizeof(int), 38);
              currCommand = 'M';
              delay(sendDelay);
          }
          
        }

        // Look for a sequence of remote commands (like a Konami code) to send a POWER command (helpful if speaker power gets out of sync)
        arrayPush(seqCurrent, currCommand, seqLen);
        Serial.print("Current sequence = ");
        arrayPrint(seqCurrent, seqLen);
  
        if (isArrayMatch(seqCurrent, seqMatch, seqLen)) {
          Serial.println("Sequence recognised.. sending alternative POWER command");

          arrayPush(seqCurrent, ' ', seqLen); // Reset the sequence
          
          // Raw data for Edifier R1855DB power button
          delay(100);
          unsigned int rawData[] = { 8980,4420, 580,520, 580,520, 580,570, 580,520, 630,1620, 580,520, 580,520, 630,520, 580,1620, 630,1620, 630,1620, 580,520, 580,520, 580,1670, 580,1620, 630,1620, 630,470, 630,1620, 630,1570, 630,520, 580,520, 630,520, 580,1620, 630,520, 580,1670, 580,470, 630,520, 580,1620, 630,1620, 630,1620, 630,470, 630,1620, 580 };
          IrSender.sendRaw(rawData, sizeof(rawData)/sizeof(int), 38);
          delay(50);
        }
    }
    
    Serial.println("---------------------------------------------------------------------------------------");
    Serial.flush();
    IrReceiver.resume(); // Receive the next IR signal
  }
}

// Do the arrays match?
bool isArrayMatch(char arr1[], char arr2[], int len) {
  bool isMatch = true;
  for(int i=0; i<len; i++) {
    if(arr1[i]!=arr2[i]) {
      isMatch = false;
      break;
    }
  }
  return isMatch;
}

// Push a value to the end of the array by shifting all current values to the left
void arrayPush(char arr[], char val, int len) {
  for(int i=1; i<len; i++) {
    arr[i-1]=arr[i];
  }
  arr[len-1]=val;
}

// Print the array contents to Serial (helps with debugging)
void arrayPrint(char arr[], int len) {
  for(int i=0; i<len; i++) {
    Serial.print(arr[i]);
    Serial.print(" ");
  }
  Serial.println();
}
