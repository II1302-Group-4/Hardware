#include "src/PolluSense/PolluSense.h"
#include "src/ArduinoUnit/src/ArduinoUnit.h"

const String WIFI_GOTBERG = "wifi_adefcade";
const String PWD_GOTBERG = "therobotsaretakingourjobs";
const String SERVER = "pollusenseserver.azurewebsites.net";
const String SERVER_PORT = "80";
const String DAYTIME_SERVER = "java.lab.ssvl.kth.se";
const String DAYTIME_SERVER_PORT = "13";

PolluSense instance(2, 3, true);
const char * ut = "==UNIXTIME==";
const long currentUnixTime = 1652808280; //Needs to be updated before every test.

test(getEpoch_validation)
{

    long res = 0;
    res = instance.getEpoch(DAYTIME_SERVER, DAYTIME_SERVER_PORT);

    assertMore(res, 0);
    instance.wifiModule->closeTCP();
    delay(20000);
}

test(getEpoch_validation_compareUnixTime)
{
    const int maxerr = 900; //Max error is 15 minutes, 15 minutes is the delay between uploads to the server.
    long res = 0;
    res = instance.getEpoch(DAYTIME_SERVER, DAYTIME_SERVER_PORT);
    //if(res == 0)
    //    fail();
    //assertRelativelyNear(currentUnixTime, res, maxerr);
    assertEqual(res, currentUnixTime);
    instance.wifiModule->closeTCP();
    delay(10000);
}

//test(getEpoch_wrongPort)
//{
//    PolluSense instance(2, 3, true);
//    instance.wifiModule->basicInit();
//    instance.wifiModule->connectToAP(WIFI_GOTBERG, PWD_GOTBERG);
//
//    int res = instance.getEpoch(DAYTIME_SERVER, "7");
//    assertEqual(res, 0);
//}

test(postData_validation)
{
    instance.wifiModule->openTCP(SERVER, SERVER_PORT);
    bool res = instance.postData("1" , "400", "400");
    assertEqual(res, true);
    instance.wifiModule->closeTCP();
    delay(10000);
}

test(postData_wrongData)
{
    instance.wifiModule->openTCP(SERVER, SERVER_PORT);
    bool res = instance.postData("0" , "400", "400");
    assertEqual(res, false);
}
void setup()
{
  delay(2000);
  Serial.begin(9600);
  while(!Serial) {}
  Serial.println("\n--- PolluSense Testing ---");
  instance.wifiModule->basicInit();
  instance.wifiModule->connectToAP(WIFI_GOTBERG, PWD_GOTBERG);
}

void loop()
{
  Test::run();
}
