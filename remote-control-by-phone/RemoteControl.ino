/*
  RemoteControl

  Demonstrates remotely controlling the Arduino Yun by mobile phone. Uses the Nexmo and 
  Google Spreadsheet APIs via the Temboo Arduino Yun SDK.
  
  This sketch phones a user when dramatic changes in light level are sensed. The user
  can choose to turn on a LED or ignore the Yun's call. The value of 
  each notable light change is logged to a Google Spreadsheet.
  
  Requires a light dependent resister (photocell) attached to pin A0.
*/

#include <Bridge.h>
#include <Temboo.h>
#include "TembooAccount.h"

/*** SUBSTITUTE YOUR VALUES BELOW: ***/

// Note that for additional security and reusability, you could
// use #define statements to specify these values in a .h file.

// you NEXMO API key & secret
const String NEXMO_API_KEY = "xxxxxxxxx";
const String NEXMO_API_SECRET = "xxxxxxxxx";

// the phone number you want to Yun to call
// make sure to include the international access code,
// for example, a US number would be: 12062954698
const String NUMBER_TO_CALL = "xxxxxxxxxxx";

// your google account details - make sure use your full email address 
// e.g., john.smith@gmail.com as opposed to just 'john.smith'. 
const String GOOGLE_USERNAME = "xxxxxxxxxx@xxxxx.xxx";
const String GOOGLE_PASSWORD = "xxxxxxxxxxxxx";

// the title of the Google Spreadsheet you want to write to.
// make sure that the spreadsheet has one named column. 
// In other words, your spreadsheet should look like:
  
// | Light reading  |    |    |    |  
// -----------------+-----------------
// |                |    |    |    |
// -----------------------------------
// |                |    |    |    | 
  
const String GOOGLE_SPREADSHEET_TITLE = "xxxxxxxxx";

// the value read by a light dependent resistor when the room is 'bright'
// make sure to adjust this based on the room that your Yun is in
// you'll see the value in the serial monitor when you run the sketch
const int BASELINE = 600; 

int ldr = A0; // the LDR sensor pin
int led = 13; // the LED pin

void setup(){
  Serial.begin(9600);
  
  delay(4000);
  while(!Serial);
  Bridge.begin();
  
  // initialize pin 13 as an output
  pinMode(led, OUTPUT);  
}

void loop(){
  // read the LDR sensor value
  int LDRReading = analogRead(ldr); 
  //just here to slow down the output and make it easier to read 
  delay(250); 
  // print the light level value 
  Serial.println(LDRReading); 
  
  
  // when the light level dips by 30%
  if (LDRReading < (BASELINE * .7)) {
    Serial.println("\nUh-oh! It's getting dark - better make a call. Dialing now...\n");
    digitalWrite(led, LOW); // turn off the light in case it's on from a previous event
    
    // if the user chooses option 1 when they receive the phone call
    if (makeNexmoCall() == "1") {
      Serial.println("Turning on the lights!\n");
      digitalWrite(led, HIGH); // turn on the light on pin 13
    } else {
      Serial.println("They didn't care :-(\n");
    }
  // when the user's choice has been made, and the lights have been turned on or not, 
  // log the light change data that triggered to phone call to a Google Spreadsheet
  saveData(LDRReading); 
  }
}


/*
  Trigger a voice call via a Temboo Nexmo Choreo.Call the user, give them a menu of options
  and return the selection they make on their phone keypad as an integer. 
*/
String makeNexmoCall() {
  // a string to represent the choice made by the user who receives the phone call
  String choice = "";
  
  // create the choreo
  TembooChoreo CaptureTextToSpeechPromptChoreo;

  // invoke the Temboo client
  CaptureTextToSpeechPromptChoreo.begin();
    
  // set Temboo account credentials
  CaptureTextToSpeechPromptChoreo.setAccountName(TEMBOO_ACCOUNT);
  CaptureTextToSpeechPromptChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
  CaptureTextToSpeechPromptChoreo.setAppKey(TEMBOO_APP_KEY);
    
  // set choreo inputs
  CaptureTextToSpeechPromptChoreo.addInput("Text", "Hey there, it's getting dark! Press one to turn on the lights, or press zero to ignore.");
  CaptureTextToSpeechPromptChoreo.addInput("APIKey", NEXMO_API_KEY);
  CaptureTextToSpeechPromptChoreo.addInput("To", NUMBER_TO_CALL);
  CaptureTextToSpeechPromptChoreo.addInput("MaxDigits", "1");
  CaptureTextToSpeechPromptChoreo.addInput("APISecret", NEXMO_API_SECRET);
  CaptureTextToSpeechPromptChoreo.addInput("ByeText", "Ok, your wish is my command. Goodbye!");
    
  // identify choreo to run
  CaptureTextToSpeechPromptChoreo.setChoreo("/Library/Nexmo/Voice/CaptureTextToSpeechPrompt");
  
  // create an output filter to return only the digit that the user pressed when they made their choice
  CaptureTextToSpeechPromptChoreo.addOutputFilter("choice", "/digits", "CallbackData");
    
  // run choreo
  CaptureTextToSpeechPromptChoreo.run();
  
  // parse the results
  while(CaptureTextToSpeechPromptChoreo.available()) {
    // read the name of the next output item
    String name = CaptureTextToSpeechPromptChoreo.readStringUntil('\x1F');
    name.trim(); // use “trim” to get rid of newlines

    // read the value of the next output item
    String data = CaptureTextToSpeechPromptChoreo.readStringUntil('\x1E');
    data.trim(); // use “trim” to get rid of newlines
    
    // parse the user's choice out of the response data
    if (name == "choice") {
      choice = data;
    } 
  }
  // close the choreo 
  CaptureTextToSpeechPromptChoreo.close();
  
  // return the choice that the user made 
  return choice;
}

/*
  Save data to a Google Spreadsheet. Remember that your Google Spreadsheet
  must have at least one column containing a title.
*/
void saveData(int data) {
    //create the choreo
    TembooChoreo AppendRowChoreo;  

    // invoke the Temboo client
    AppendRowChoreo.begin();
    
    // set Temboo account credentials
    AppendRowChoreo.setAccountName(TEMBOO_ACCOUNT);
    AppendRowChoreo.setAppKeyName(TEMBOO_APP_KEY_NAME);
    AppendRowChoreo.setAppKey(TEMBOO_APP_KEY);
    
    // Set choreo inputs
    AppendRowChoreo.addInput("Password", GOOGLE_PASSWORD);
    AppendRowChoreo.addInput("Username", GOOGLE_USERNAME);
    AppendRowChoreo.addInput("RowData", String(data));
    AppendRowChoreo.addInput("SpreadsheetTitle", GOOGLE_SPREADSHEET_TITLE);
    
    // identify the choreo to run
    AppendRowChoreo.setChoreo("/Library/Google/Spreadsheets/AppendRow");
    
    // print out that we're adding the light reading to your Google Spreadsheet
    Serial.println("Adding " + String(data) + " to the \"" + GOOGLE_SPREADSHEET_TITLE  + "\" Spreadsheet - please wait...");
    // run the choreo
    AppendRowChoreo.run();
    
    Serial.println("Done!\n");
    
    // close the choreo
    AppendRowChoreo.close();
}
