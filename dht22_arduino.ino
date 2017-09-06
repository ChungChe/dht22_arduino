#include "DHT.h"
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>


#define DHTPIN1 2     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)

IPAddress ip(192, 168, 0, 19);
unsigned int localPort = 8888;      // local port to listen on
EthernetUDP udp;

byte mac[] = { 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };

float TGS2602_REG_X3[15][4]={   //3次方多項式回歸
{ 0.0067 , -0.026, -0.2567 , 1.494 },
{ 0.005  , -0.013, -0.29   , 1.532 },
{ 0.0033 , 0     , -0.3233 , 1.57   },
{ 0.0017 , 0.013 , -0.3567 , 1.608 },
{ 0      , 0.026 , -0.39   , 1.646 },
{ -0.0017, 0.039 , -0.4233 , 1.684 },
{ -0.0033, 0.052 , -0.4567 , 1.722 },
{ -0.005 , 0.065 , -0.49   , 1.76   },
{ -0.0054, 0.0688, -0.5058 , 1.7975},
{ -0.0058, 0.0725, -0.5217 , 1.835 },
{ -0.0063, 0.0763, -0.5375 , 1.8725},
{ -0.0067, 0.08  , -0.5533 , 1.91  },
{ -0.0071, 0.0838, -0.5692 , 1.9475},
{ -0.0075, 0.0875, -0.585  , 1.985 },
{ -0.0079, 0.0913, -0.6008 , 2.0225}};


float getGasCorrection_2602 (float Temp ,float Humid)
{
    float ftemp= Temp / 10.0;
    int ihumid= (Humid - 30) / 5;
    float poly = 0;
    float rt = 0;
    float sum = 0;
    float out_y = 0;
    for (int i = 3; i >= 0; i--)
    {
        poly = TGS2602_REG_X3[ihumid][3-i];
        rt = pow(ftemp,i);
        sum = rt * poly;
        out_y = out_y + sum;
    }
    return out_y; 
}

DHT dht1(DHTPIN1, DHTTYPE);
 
void setup() {
    Serial.begin(9600); 
    Ethernet.begin(mac,ip);
    udp.begin(localPort);
    dht1.begin();
}

void send(const String addr, int port, const String str)
{
    udp.beginPacket(addr.c_str(), port);
    udp.write(str.c_str());
    udp.endPacket();
}
 
void loop() {
    // Wait a few seconds between measurements.
    delay(2000);
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float h1 = dht1.readHumidity();
    float t1 = dht1.readTemperature();

    // Check if any reads failed and exit early (to try again).
    if (isnan(h1) || isnan(t1)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }
  
    Serial.print("Humidity1: "); 
    Serial.print(h1);
    Serial.print(" %\t");
    Serial.print("Temperature1: "); 
    Serial.print(t1);
    Serial.println(" *C ");

    String temp(t1);
    String humd(h1);

    int gasSensor_2602 = A1; // analog pin A1

    int read_2602 = analogRead(gasSensor_2602);  //自己自訂讀取腳位
    Serial.print("A1 Analog: ");
    Serial.print(read_2602);
    float Correction_2602 = getGasCorrection_2602(t1, h1); //從DHT22讀取溫溼度計
    Serial.print("Correct val: ");
    Serial.println(Correction_2602);

    float gasval_2602 = read_2602 * Correction_2602;

    Serial.print("Final Val: ");
    Serial.println(gasval_2602);

    String finalVal(gasval_2602);

    String remote_ip("192.168.0.101");
    String data(temp + " " + humd + " " + finalVal);
    send(remote_ip, 15000, data);
}
