#include "src/PolluSense/PolluSense.h"
#include "src/ArduinoUnit/src/ArduinoUnit.h"

const String WIFI_GOTBERG = "wifi_adefcade";
const String PWD_GOTBERG = "therobotsaretakingourjobs";
const String SERVER = "pollusenseserver.azurewebsites.net";
const String SERVER_PORT = "80";
const String DAYTIME_SERVER = "java.lab.ssvl.kth.se";
const String DAYTIME_SERVER_PORT = "13";

const int currentUnixTime = 1652783452; //Needs to be updated before every test.

test(getEpoch_validation)
{
    PolluSense instance(2, 3, true);
    instance.wifiModule->basicInit();
    instance.wifiModule->connectToAP(WIFI_GOTBERG, PWD_GOTBERG);

    int res = instance.getEpoch(DAYTIME_SERVER, DAYTIME_SERVER_PORT);
    assertMore(res, 0);
}

test(getEpoch_validation_compareUnixTime)
{
    const int maxerr = 900; //Max error is 15 minutes, 15 minutes is the delay between uploads to the server.
    PolluSense instance(2, 3, true);
    instance.wifiModule->basicInit();
    instance.wifiModule->connectToAP(WIFI_GOTBERG, PWD_GOTBERG);

    int res = instance.getEpoch(DAYTIME_SERVER, DAYTIME_SERVER_PORT);
    assertRelativelyNear(currentUnixTime, res, maxerr);
}

test(getEpoch_wrongHostName)
{
    PolluSense instance(2, 3, true);
    instance.wifiModule->basicInit();
    instance.wifiModule->connectToAP(WIFI_GOTBERG, PWD_GOTBERG);

    int res = instance.getEpoch(DAYTIME_SERVER + ".com", DAYTIME_SERVER_PORT);
    assertEqual(res, 0);
}

/*
test(getEpoch_wrongPort)
{
    PolluSense instance(2, 3, true);
    instance.wifiModule->basicInit();
    instance.wifiModule->connectToAP(WIFI_GOTBERG, PWD_GOTBERG);

    int res = instance.getEpoch(DAYTIME_SERVER, "7");
    assertEqual(res, 0);
}

test(getEpoch_WiFiFail)
{
    PolluSense instance(2, 3, true);
    instance.wifiModule->basicInit();
    instance.wifiModule->connectToAP(WIFI_GOTBERG, "wrong_pass");

    int res = instance.getEpoch(DAYTIME_SERVER, PWD_GOTBERG);
    assertEqual(res, 0);
}

test(postData_validation)
{
    PolluSense instance(2, 3, true);
    instance.wifiModule->basicInit();
    instance.wifiModule->connectToAP(WIFI_GOTBERG, PWD_GOTBERG);

    instance.wifiModule->openTCP(SERVER, SERVER_PORT);
    bool res = instance.postData("0" , "400", "400");
    assertEqual(res, true);
}

test(postData_wrongServer)
{
    PolluSense instance(2, 3, true);
    instance.wifiModule->basicInit();
    instance.wifiModule->connectToAP(WIFI_GOTBERG, PWD_GOTBERG);

    instance.wifiModule->openTCP(DAYTIME_SERVER, DAYTIME_SERVER_PORT);
    bool res = instance.postData("0" , "400", "400");
    assertEqual(res, false);
}

test(postData_wrongData)
{
    PolluSense instance(2, 3, true);
    instance.wifiModule->basicInit();
    instance.wifiModule->connectToAP(WIFI_GOTBERG, PWD_GOTBERG);

    instance.wifiModule->openTCP(SERVER, SERVER_PORT);
    bool res = instance.postData("0" , "399", "1188");
    assertEqual(res, false);
}
*/
void setup()
{
  delay(2000);
  Serial.begin(9600);
  while(!Serial) {}
  Serial.println("\n--- PolluSense Testing ---");
}

void loop()
{
  Test::run();
}
