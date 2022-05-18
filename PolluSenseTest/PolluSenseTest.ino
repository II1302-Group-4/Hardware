#include "src/PolluSense/PolluSense.h"
#include "src/ArduinoUnit/src/ArduinoUnit.h"

const String WIFI_GOTBERG = "wifi_adefcade";
const String PWD_GOTBERG = "therobotsaretakingourjobs";
const String SERVER = "pollusenseserver.azurewebsites.net";
const String SERVER_PORT = "80";
const String DAYTIME_SERVER = "java.lab.ssvl.kth.se";
const String DAYTIME_SERVER_PORT = "13";

PolluSense instance(2, 3, true);
const long currentUnixTime = 1652866557; //Needs to be updated before every test.


test(getEpoch_validation_compareUnixTime)
{
    instance.wifiModule->closeTCP();
    const int maxerr = 300; // Five minute relative error, since 'currentUnixTime' is not the exact time.
    long res = 0;
    res = instance.getEpoch(DAYTIME_SERVER, DAYTIME_SERVER_PORT);
    assertNear(currentUnixTime, res, maxerr);
    delay(5000);
}

test(getEpoch_wrongPort)
{
    instance.wifiModule->closeTCP();
    long res = 0;
    res = instance.getEpoch(DAYTIME_SERVER, "7");
    assertEqual(res, 0);
    delay(5000);
}

test(getEpoch_wrongHost)
{
    instance.wifiModule->closeTCP();
    long res = 0;
    res = instance.getEpoch("cava.lab.ssvl.kth.se", DAYTIME_SERVER_PORT);
    assertEqual(res, 0);
}
test(postData_validation)
{
    instance.wifiModule->closeTCP();
    instance.wifiModule->openTCP(SERVER, SERVER_PORT);
    bool res = instance.postData("1" , "400", "400");
    assertEqual(res, true);
    delay(5000);
}

test(postData_wrongData)
{
    instance.wifiModule->closeTCP();
    instance.wifiModule->openTCP(SERVER, SERVER_PORT);
    bool res = instance.postData("0" , "400", "400");
    assertEqual(res, false);
}
void setup()
{
  delay(2000);
  Serial.begin(9600);
  while(!Serial) {}
  instance.wifiModule->basicInit();
  instance.wifiModule->connectToAP(WIFI_GOTBERG, PWD_GOTBERG);
  Serial.println("--- PolluSense-Testing ---");
}

void loop()
{
  Test::run();
}
