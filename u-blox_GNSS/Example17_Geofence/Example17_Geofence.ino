/*
  u-blox M8 geofence example

  This example demonstrates how to use the addGeofence and getGeofenceState functions

   This example powers up the GNSS and reads the fix.
  Once a valid 3D fix has been found, the code reads the latitude and longitude.
  The code then sets four geofences around that position with a radii of 5m, 10m, 15m and 20m with 95% confidence.
  The code then monitors the geofence status.
  The LED will be illuminated if you are inside the _combined_ geofence (i.e. within the 20m radius).

  This code has been tested on the ZOE-M8Q.
*/

#include <board.h>

#define DSerial SerialUSB

#define LED LED_BUILTIN // Change this if your LED is on a different pin


SFE_UBLOX_GNSS myGNSS;

void setup()
{
  pinMode(LED, OUTPUT);

  // Set up the I2C pins
  Wire.begin();

  // Start the console serial port
  DSerial.begin(115200); // You may need to increase this for high navigation rates!
  while (DSerial.read() >= 0);
  delay(5000);

  DSerial.println();
  DSerial.println();
  DSerial.println(F("u-blox M8 geofence example"));
  DSerial.println();
  DSerial.println();

  delay(1000); // Let the GNSS power up

  if (myGNSS.begin() == false) //Connect to the u-blox module using Wire port
  {
    DSerial.println(F("u-blox GNSS not detected at default I2C address. Please check wiring. Freezing."));
    while (1);
  }

  //myGNSS.enableDebugging(); // Enable debug messages
  myGNSS.setI2COutput(COM_TYPE_UBX); // Limit I2C output to UBX (disable the NMEA noise)

  DSerial.println(F("Waiting for a 3D fix..."));

  byte fixType = 0;

  while (fixType < 3)
  {
    fixType = myGNSS.getFixType(); // Get the fix type
    DSerial.print(F("Fix: ")); // Print it
    DSerial.print(fixType);

    if(fixType == 0) DSerial.print(F(" = No fix"));
    else if(fixType == 1) DSerial.print(F(" = Dead reckoning"));
    else if(fixType == 2) DSerial.print(F(" = 2D"));
    else if(fixType == 3) DSerial.print(F(" = 3D"));
    else if(fixType == 4) DSerial.print(F(" = GNSS + Dead reckoning"));
    else if(fixType == 5) DSerial.print(F(" = Time only"));
    DSerial.println();
    delay(1000);
  }

  DSerial.println(F("3D fix found!"));

  long latitude = myGNSS.getLatitude(); // Get the latitude in degrees * 10^-7
  DSerial.print(F("Lat: "));
  DSerial.print(latitude);

  long longitude = myGNSS.getLongitude(); // Get the longitude in degrees * 10^-7
  DSerial.print(F("   Long: "));
  DSerial.println(longitude);

  uint32_t radius = 500; // Set the radius to 5m (radius is in m * 10^-2 i.e. cm)

  byte confidence = 2; // Set the confidence level: 0=none, 1=68%, 2=95%, 3=99.7%, 4=99.99%

  // Call clearGeofences() to clear all existing geofences.
  DSerial.print(F("Clearing any existing geofences. clearGeofences returned: "));
  DSerial.println(myGNSS.clearGeofences());

  // It is possible to define up to four geofences.
  // Call addGeofence up to four times to define them.
  DSerial.println(F("Setting the geofences:"));

  DSerial.print(F("addGeofence for geofence 1 returned: "));
  DSerial.println(myGNSS.addGeofence(latitude, longitude, radius, confidence));

  radius = 1000; // 10m
  DSerial.print(F("addGeofence for geofence 2 returned: "));
  DSerial.println(myGNSS.addGeofence(latitude, longitude, radius, confidence));

  radius = 1500; // 15m
  DSerial.print(F("addGeofence for geofence 3 returned: "));
  DSerial.println(myGNSS.addGeofence(latitude, longitude, radius, confidence));

  radius = 2000; // 20m
  DSerial.print(F("addGeofence for geofence 4 returned: "));
  DSerial.println(myGNSS.addGeofence(latitude, longitude, radius, confidence));
}

void loop()
{
  geofenceState currentGeofenceState; // Create storage for the geofence state

  bool result = myGNSS.getGeofenceState(currentGeofenceState);

  DSerial.print(F("getGeofenceState returned: ")); // Print the combined state
  DSerial.print(result); // Get the geofence state

  if (!result) // If getGeofenceState did not return true
  {
    DSerial.println(F(".")); // Tidy up
    return; // and go round the loop again
  }

  DSerial.print(F(". status is: ")); // Print the status
  DSerial.print(currentGeofenceState.status);

  DSerial.print(F(". numFences is: ")); // Print the numFences
  DSerial.print(currentGeofenceState.numFences);

  DSerial.print(F(". combState is: ")); // Print the combined state
  DSerial.print(currentGeofenceState.combState);

  if (currentGeofenceState.combState == 0)
  {
    DSerial.print(F(" = Unknown"));
    digitalWrite(LED, LOW);
  }
  if (currentGeofenceState.combState == 1)
  {
    DSerial.print(F(" = Inside"));
    digitalWrite(LED, HIGH);
  }
  else if (currentGeofenceState.combState == 2)
  {
    DSerial.print(F(" = Outside"));
    digitalWrite(LED, LOW);
  }

  DSerial.print(F(". The individual states are: ")); // Print the state of each geofence
  for(int i = 0; i < currentGeofenceState.numFences; i++)
  {
    if (i > 0) DSerial.print(F(","));
    DSerial.print(currentGeofenceState.states[i]);
  }
  DSerial.println();

  delay(1000);
}
