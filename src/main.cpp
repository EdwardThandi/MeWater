#include <Arduino.h>
#include <FlowMeter.h>
#include "Credentials.h"
#include <ArduinoJson.h>

#if !(ESP8266 || ESP32)
#error This code is intended to run on the ESP8266/ESP32 platform! Please check your Tools->Board setting
#endif

#define MYSQL_DEBUG_PORT Serial

// Debug Level from 0 to 4
#define _MYSQL_LOGLEVEL_ 1

#include <MySQL_Generic.h>

#define USING_HOST_NAME true

#if USING_HOST_NAME
// Optional using hostname, and Ethernet built-in DNS lookup
char server[] = "projectedwardmysqldatabase.cp25gjyejcfg.us-east-1.rds.amazonaws.com"; // change to your server's hostname/URL
#else
IPAddress server(192, 168, 2, 112);
#endif

uint16_t server_port = 3306; // 3306;

char default_database[] = "projectedwardmysqldatabase";
char default_table[] = "iot_water_consumption";

MySQL_Connection conn((Client *)&client);

MySQL_Query *query_mem;

// connect a flow meter to an interrupt pin (see notes on your Arduino model for pin numbers)
FlowMeter *Meter1;
FlowMeter *Meter2;
FlowMeter *Meter3;

// set the measurement update period to 1s (1000 ms)
const unsigned long period = 1000;

// define an 'interrupt service handler' (ISR) for every interrupt pin you use
void Meter1ISR()
{
    // let our flow meter count the pulses
    Meter1->count();
}

// define an 'interrupt service handler' (ISR) for every interrupt pin you use
void Meter2ISR()
{
    // let our flow meter count the pulses
    Meter2->count();
}

// define an 'interrupt service handler' (ISR) for every interrupt pin you use
void Meter3ISR()
{
    // let our flow meter count the pulses
    Meter3->count();
}

double meter1Flowrate = 0.0;
double meter2Flowrate = 0.0;
double meter3Flowrate = 0.0;
double meter1Volume = 0.0;
double meter2Volume = 0.0;
double meter3Volume = 0.0;

float getCurrentFlowrateWithError(float baseFlowrate)
{
    return baseFlowrate + 0.0;
}
float getTotalVolumeWithError(float baseVolume)
{
    return baseVolume + 0.0;
}

String INSERT_SQL = String("INSERT INTO ") + default_database + "." + default_table + " (meter1Flowrate,meter2Flowrate,meter3Flowrate,meter1Volume,meter2Volume,meter3Volume) VALUES ('" + String(meter1Flowrate) + "','" + String(meter2Flowrate) + "','" + String(meter3Flowrate) + "','" + String(meter1Volume) + "','" + String(meter2Volume) + "','" + String(meter3Volume) + "')";
//String INSERT_SQL = "INSERT INTO projectedwardmysqldatabase.hello_arduino (meter1Flowrate,meter2Flowrate,meter3Flowrate,meter1Volume,meter2Volume,meter3Volume) VALUES ('" + String(meter1Flowrate) + "','" + String(meter2Flowrate) + "','" + String(meter3Flowrate) + "','" + String(meter1Volume) + "','" + String(meter2Volume) + "','" + String(meter3Volume) + "')";


void runInsert()
{
    // Initiate the query class instance
    MySQL_Query query_mem = MySQL_Query(&conn);

    if (conn.connected())
    {
        MYSQL_DISPLAY(INSERT_SQL);

        // Execute the query
        // KH, check if valid before fetching
        if (!query_mem.execute(INSERT_SQL.c_str()))
        {
            MYSQL_DISPLAY("Insert error");
        }
        else
        {
            MYSQL_DISPLAY("Data Inserted.");
        }
    }
    else
    {
        MYSQL_DISPLAY("Disconnected from Server. Can't insert.");
    }
}

void setup()
{
    // prepare serial communication
    Serial.begin(115200);

    MYSQL_DISPLAY1("\nStarting Basic_Insert_ESP on", ARDUINO_BOARD);
    MYSQL_DISPLAY(MYSQL_MARIADB_GENERIC_VERSION);

    // Begin WiFi section
    MYSQL_DISPLAY1("Connecting to", ssid);

    WiFi.begin(ssid, pass);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        MYSQL_DISPLAY0(".");
    }

    // print out info about the connection:
    MYSQL_DISPLAY1("Connected to network. My IP address is:", WiFi.localIP());

    MYSQL_DISPLAY3("Connecting to SQL Server @", server, ", Port =", server_port);
    MYSQL_DISPLAY5("User =", user, ", PW =", password, ", DB =", default_database);

    // get a new FlowMeter instance for an uncalibrated flow sensor and let them attach their 'interrupt service handler' (ISR) on every rising edge
    Meter1 = new FlowMeter(digitalPinToInterrupt(18), UncalibratedSensor, Meter1ISR, RISING);

    // do this setup step for every  FlowMeter and ISR you have defined, depending on how many you need
    Meter2 = new FlowMeter(digitalPinToInterrupt(19), UncalibratedSensor, Meter2ISR, RISING);

    // do this setup step for every  FlowMeter and ISR you have defined, depending on how many you need
    Meter3 = new FlowMeter(digitalPinToInterrupt(21), UncalibratedSensor, Meter3ISR, RISING);
}

void loop()
{
    // wait between output updates
    delay(period);

    // process the (possibly) counted ticks
    Meter1->tick(period);
    Meter2->tick(period);
    Meter3->tick(period);

    meter1Flowrate = Meter1->getCurrentFlowrate();
    meter2Flowrate = Meter2->getCurrentFlowrate();
    meter3Flowrate = Meter3->getCurrentFlowrate();
    meter1Volume = Meter1->getTotalVolume();
    meter2Volume = Meter2->getTotalVolume();
    meter3Volume = Meter3->getTotalVolume();

    // output some measurement result
    Serial.println("Meter 1 currently " + String(Meter1->getCurrentFlowrate()) + " l/min, " + String(Meter1->getTotalVolume()) + " l total.");
    Serial.println("Meter 2 currently " + String(Meter2->getCurrentFlowrate()) + " l/min, " + String(Meter2->getTotalVolume()) + " l total.");
    Serial.println("Meter 3 currently " + String(Meter3->getCurrentFlowrate()) + " l/min, " + String(Meter3->getTotalVolume()) + " l total.");

    // if (conn.connect(server, server_port, user, password))
    if (conn.connectNonBlocking(server, server_port, user, password) != RESULT_FAIL)
    {
        delay(500);
        runInsert();
        conn.close(); // close the connection
    }
    else
    {
        MYSQL_DISPLAY("\nConnect failed. Trying again on next iteration.");
    }
}