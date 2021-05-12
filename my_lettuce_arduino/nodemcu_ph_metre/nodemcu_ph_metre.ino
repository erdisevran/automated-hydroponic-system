#include <DateTime.h>
#include <DateTimeTZ.h>
#include <ESPDateTime.h>
#include <TimeElapsed.h>


#include <NTPClient.h>
#include <WiFiUdp.h>

#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <FirebaseArduino.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
 
#define FIREBASE_HOST "marulum-b1044-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "HS4mH99WWTycrm97WhZKe8Mi94eFmNX9pIx1Oyi4"
//#define WIFI_SSID "Sevran"
//#define WIFI_PASSWORD "z08M9ksO"
 #define WIFI_SSID "Sevran mini"
 #define WIFI_PASSWORD "2011212010"

int timezone = 7 *3600;
int dst = 0;

 int      ph_value;
 float    ph_voltage,
          ph_low,
          ph_high;
 String   ph_statu;
 int      ph_times,
          ph_stop_time,
          ph_sayac;
         
float     calibration = 21.9;
const int analogInPin = A0; 
      int sensorValue = 0; 
unsigned long int avgValue; 
float b;
int   buf[10],temp;

    void setup() {
      Serial.begin(9600);
      Serial.print("Başlatıldı");
  
      //pins

      // connect to wifi.
      WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
      Serial.print("connecting");
      while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
      }
      Serial.println();
      Serial.print("connected: ");
      Serial.println(WiFi.localIP());
      
      // Firebase
      Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); 
      Serial.println("FB Başlatıldı"); 
      delay(1000);  

  configTime(timezone, dst, "pool.ntp.org","time.nist.gov");
    
      while(!time(nullptr)){
         Serial.print("*");
         delay(1000);
      } 
    }
    
    void loop() { 
      
  
        delay(1000);
  
        PhMetre();
        delay(1000);
    }
  
    void PhMetre(){
        Serial.print("PH Sayac:");
        Serial.println(ph_sayac); 
        
        ph_statu = FbPhOku();  
        if( ph_sayac >= ph_times && ph_statu == "X" ) {
           FbPhKapat(); 
           ph_sayac = 0;
        }else if(ph_statu == "X"){
            if(ph_sayac == 0){
              Serial.print("Ph Açıldı!"); 
              phRun();
              ph_sayac = 1;
            }
            else{
              phRun();
              ph_sayac = ph_sayac + 1;
            }    
          }
      }
      
    String FbPhOku(){
      
      FirebaseObject phmetre = Firebase.get("/phmetre"); 
      delay(500);
      ph_times     = phmetre.getInt(String("times")); 
      ph_statu     = phmetre.getString(String("statu")); 
      Serial.print("Ph Statu:");
      Serial.println(ph_statu);  
      setTime();
      return ph_statu;
    }
    
    void FbPhKapat(){
      Serial.print("Ph Kapatıldı!"); 
      Firebase.setString("phmetre/statu", "");
    }

    void setTime(){
      
  time_t now = time(nullptr);
  struct tm* p_tm = localtime(&now);
  
String con_time = (String)(p_tm->tm_hour - 4);
       con_time.concat(":");
       con_time.concat((String)p_tm->tm_min);
       con_time.concat(":");
       con_time.concat((String)p_tm->tm_sec);
      Firebase.setString("phmetre/con_time", con_time);
    }
    
    void phRun(){
     for(int i=0;i<10;i++) { 
     buf[i]=analogRead(analogInPin);
     delay(30);
     }
     for(int i=0;i<9;i++){
      for(int j=i+1;j<10;j++) {
         if(buf[i]>buf[j]) 
         {
           temp=buf[i];
           buf[i]=buf[j];
           buf[j]=temp; }
           }
       }
       avgValue=0;
       for(int i=2;i<8;i++)
       avgValue+=buf[i];
       float pHVol=(float)avgValue*3.2/1024/6;
       ph_voltage = -5.70 * pHVol + calibration;
       Serial.print("ph_value: ");
       Serial.println(ph_voltage);
       Firebase.setFloat("phmetre/phValue", ph_voltage);
       delay(500);
    }



    /*
      
  time_t now = time(nullptr);
  struct tm* p_tm = localtime(&now);
  Serial.print(p_tm->tm_mday);
  Serial.print("/");
  Serial.print(p_tm->tm_mon + 1);
  Serial.print("/");
  Serial.print(p_tm->tm_year + 1900);
  
  Serial.print(" ");
  
  Serial.print(p_tm->tm_hour);
  Serial.print(":");
  Serial.print(p_tm->tm_min);
  Serial.print(":");
  Serial.println(p_tm->tm_sec);
  */
