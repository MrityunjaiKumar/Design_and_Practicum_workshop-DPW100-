//------------------------------------------------------------------------------
// Include the IRremote library header
//
#include <IRremote.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//------------------------------------------------------------------------------
// Tell IRremote which Arduino pin is connected to the IR Receiver (TSOP4838)
//
int recvPin = 15;
IRrecv irrecv(recvPin);

//+=============================================================================
// Configure the Arduino
//
void  setup ( )
{
  Serial.begin(115200);   // Status message will be sent to PC at 9600 baud
  irrecv.enableIRIn();  // Start the receiver
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,20);
  display.display(); 
}

//+=============================================================================
// Display IR code
//
void  ircode (decode_results *results)
{
  
  // Panasonic has an Address
  if (results->decode_type == PANASONIC) {
    display.println("PANASONIC");
    display.display();
    Serial.print(":");
  }

  // Print Code
  Serial.print(results->value, HEX);
}

//+=============================================================================
// Display encoding type
//
void  encoding (decode_results *results)
{
  switch (results->decode_type) {
    default:
    case UNKNOWN:    
    display.println("UNKNOWN");
    display.display();  
    break ;
    case NEC:          
    display.println("NEC");           
    break ;
    case SONY:         
    display.println("SONY");         
    break ;
    case RC5:          
    display.println("RC5");          
    break ;
    case RC6:          
    display.println("RC6");           
    break ;
    case DISH:         
    Serial.print("DISH");          
    break ;
    case SHARP:        
    display.println("SHARP");         
    break ;
    case JVC:          
    display.println("JVC");           
    break ;
    case SANYO:        
    display.println("SANYO");         
    break ;
    case MITSUBISHI:   
    display.println("MITSUBISHI");    
    break ;
    case SAMSUNG:      
    display.println("SAMSUNG");       
    break ;
    case LG:           display.println("LG");            break ;
    case WHYNTER:      display.println("WHYNTER");       break ;
    case AIWA_RC_T501: display.println("AIWA_RC_T501");  break ;
    case PANASONIC:    display.println("PANASONIC");     break ;
    case DENON:        display.println("Denon");         break ;
  }
  display.display();
}

//+=============================================================================
// Dump out the decode_results structure.
//
void  dumpInfo (decode_results *results)
{
  // Check if the buffer overflowed
  if (results->overflow) {
    Serial.println("IR code too long. Edit IRremoteInt.h and increase RAWBUF");
    return;
  }

  // Show Encoding standard
  Serial.print("Encoding  : ");
  encoding(results);
  Serial.println("");

  // Show Code & length
  Serial.print("Code      : ");
  ircode(results);
  Serial.print(" (");
  Serial.print(results->bits, DEC);
  Serial.println(" bits)");
}

//+=============================================================================
// Dump out the decode_results structure.
//
void  dumpRaw (decode_results *results)
{
  // Print Raw data
  Serial.print("Timing[");
  Serial.print(results->rawlen-1, DEC);
  Serial.println("]: ");

  for (int i = 1;  i < results->rawlen;  i++) {
    unsigned long  x = results->rawbuf[i] * USECPERTICK;
    if (!(i & 1)) {  // even
      Serial.print("-");
      if (x < 1000)  Serial.print(" ") ;
      if (x < 100)   Serial.print(" ") ;
      Serial.print(x, DEC);
    } else {  // odd
      Serial.print("     ");
      Serial.print("+");
      if (x < 1000)  Serial.print(" ") ;
      if (x < 100)   Serial.print(" ") ;
      Serial.print(x, DEC);
      if (i < results->rawlen-1) Serial.print(", "); //',' not needed for last one
    }
    if (!(i % 8))  Serial.println("");
  }
  Serial.println("");                    // Newline
}

//+=============================================================================
// Dump out the decode_results structure.
//
void  dumpCode (decode_results *results)
{
  // Start declaration
  Serial.print("unsigned int  ");          // variable type
  Serial.print("rawData[");                // array name
  Serial.print(results->rawlen - 1, DEC);  // array size
  Serial.print("] = {");                   // Start declaration

  // Dump data
  for (int i = 1;  i < results->rawlen;  i++) {
    Serial.print(results->rawbuf[i] * USECPERTICK, DEC);
    if ( i < results->rawlen-1 ) Serial.print(","); // ',' not needed on last one
    if (!(i & 1))  Serial.print(" ");
  }

  // End declaration
  Serial.print("};");  // 

  // Comment
  Serial.print("  // ");
  encoding(results);
  Serial.print(" ");
  ircode(results);

  // Newline
  Serial.println("");

  // Now dump "known" codes
  if (results->decode_type != UNKNOWN) {

    // Some protocols have an address
    if (results->decode_type == PANASONIC) {
      Serial.print("unsigned int  addr = 0x");
      Serial.print(results->address, HEX);
      Serial.println(";");
    }

    // All protocols have data
    Serial.print("unsigned int  data = 0x");
    Serial.print(results->value, HEX);
    Serial.println(";");
  }
}

//+=============================================================================
// The repeating section of the code
//
void  loop ( )
{
  decode_results  results;        // Somewhere to store the results

  if (irrecv.decode(&results)) {  // Grab an IR code
    dumpInfo(&results);           // Output the results
    dumpRaw(&results);            // Output the results in RAW format
    dumpCode(&results);           // Output the results as source code
    Serial.println("");           // Blank line between entries
    irrecv.resume();              // Prepare for the next value
  }
}
