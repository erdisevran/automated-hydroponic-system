#include <ESP8266WiFi.h>
#include <SoftwareSerial.h>
#include <FirebaseArduino.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
 
#define FIREBASE_HOST "marulum-b1044-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "HS4mH99WWTycrm97WhZKe8Mi94eFmNX9pIx1Oyi4"
//#define WIFI_SSID "Sevran"
//#define WIFI_PASSWORD "z08M9ksO"
  #define FIREBASE_AUTH "HS4mH99WWTycrm97WhZKe8Mi94eFmNX9pIx1Oyi4"
  #define WIFI_SSID "Sevran mini"
  #define WIFI_PASSWORD "2011212010"


 
  
 const int pompa_pin = 5; 
       int pompa_sayac;
       int pompa_times;
    String pompa_statu;
  
 int      ph_value;
 float    ph_voltage,
          ph_low,
          ph_high;
 String   ph_statu;
 int      ph_times,
          ph_stop_time,
          ph_sayac;
         
float     calibration = 21.3; //change this43E value to calibrate
const int analogInPin = A0; 
      int sensorValue = 0; 
unsigned long int avgValue; 
float b;
int   buf[10],temp;
  
  String dp1_statu, //yükseltici
         dp2_statu; //düşürücü
  int    dp1_times,
         dp2_times,
         dp1_sayac,
         dp2_sayac;
          
  const int dp1_pin  = 2; 
  const int dp2_pin  = 3; //4,5,13
  
    void setup() {
      Serial.begin(9600);
      Serial.print("Başlatıldı");
  
      //pins
      pinMode(pompa_pin, OUTPUT);
      digitalWrite(pompa_pin, LOW);
      pinMode(ph_value, INPUT); 
      pinMode(dp1_pin, OUTPUT); 
      pinMode(dp2_pin, OUTPUT); 
  
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
    }
    
    void loop() { 
        Pompa();
   //     PhMetre();
   //     DozajOku();
   //     DozajUp(); //DP1 eğer ph düşükse yükseltmesi için calısacak 
   //     DozajDown(); //DP2 eğer ph yüksekse düşürmesi için calısacak 
    
      delay(1000);
    }
      void Pompa(){
      Serial.print("Pompa Sayac:");
      Serial.println(pompa_sayac); 
      
      pompa_statu = FbPompaOku();
      if( pompa_sayac >= pompa_times && pompa_statu == "X" ) { //1800 yarım saat calıssın
         FbPompaKapat();
         digitalWrite(pompa_pin, LOW); 
         pompa_sayac = 0;
      }else if(pompa_statu == "X"){
          if(pompa_sayac == 0){
            Serial.print("Pompa Açıldı!"); 
            digitalWrite(pompa_pin, HIGH);
            pompa_sayac = 1;
          }
          else{
            digitalWrite(pompa_pin, HIGH);
            pompa_sayac = pompa_sayac + 1;
          }    
        }    
        else if (pompa_statu == "" && pompa_sayac >= 1 )  {//statuyu boşa cektiğinde dursun
          digitalWrite(pompa_pin, LOW); 
          Serial.print("Pompa Kapatıldı!"); 
          Firebase.setString("pompa/statu", "");
          pompa_sayac = 0;
        }  
    }
    
    String FbPompaOku(){
      
      FirebaseObject pompa = Firebase.get("/pompa"); 
      pompa_times = pompa.getInt(String("times")); 
      pompa_statu = pompa.getString(String("statu"));  
      
      Serial.print("Pompa Statu:");
      Serial.println(pompa_statu);  
      Serial.print("Pompa Times:");
      Serial.println(pompa_times);  
      return pompa_statu;
    }
  
    void FbPompaKapat(){
      Serial.print("Pompa Kapatıldı!"); 
      Firebase.setString("pompa/statu", "");
    }
  
  //*-----*
  
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
          //eger ph_times dk ölçüm yapıldıktan sonra ph degerleri normalse ph ölçüm dursun böylece dozajlar da durur  
          if(ph_sayac > ph_stop_time && ph_voltage > ph_low && ph_voltage < ph_high )
            FbPhKapat(); 
      }
      
    String FbPhOku(){
      
      FirebaseObject phmetre = Firebase.get("/phmetre"); 
      ph_times     = phmetre.getInt(String("times")); 
      ph_statu     = phmetre.getString(String("statu")); 
      ph_stop_time = phmetre.getFloat(String("stop_time")); 
      ph_low       = phmetre.getFloat(String("low")); 
      ph_high      = phmetre.getFloat(String("high")); 
      
      Serial.print("Ph Statu:");
      Serial.println(ph_statu);  
      
      return ph_statu;
    }
    
    void FbPhKapat(){
      Serial.print("Ph Kapatıldı!"); 
      Firebase.setString("phmetre/statu", "");
    }
    
    void phRun(){
     for(int i=0;i<10;i++) { 
     buf[i]=analogRead(analogInPin);
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
       float pHVol=(float)avgValue*5.0/1024/6;
       ph_voltage = -5.70 * pHVol + calibration;
       Serial.print("ph_value: ");
       Serial.println(ph_voltage);
/*        ph_value = analogRead(A0);
        Serial.print("ph_value:" ); 
        ph_voltage = ph_value * (5.0 / 1023.0); 
        Serial.println(ph_voltage); 
        Firebase.setFloat("phmetre/phValue", ph_voltage);
*/
    }
    
  //*---*
    void DozajOku(){
      FirebaseObject dozaj = Firebase.get("/dozaj"); 
      dp1_statu = dozaj.getString(String("dp1_statu")); 
      dp2_statu = dozaj.getString(String("dp2_statu")); 
      dp1_times = dozaj.getInt(String("dp1_times")); 
      dp2_times = dozaj.getInt(String("dp2_times")); 
    }
    
  //*---*
    void DozajUp(){
      dp1_sayac = dp1_sayac + 1;
          Serial.print("dp1_sayac: "); 
          Serial.println(dp1_sayac); 
      if(dp1_statu == "X" && dp1_times > dp1_sayac ){
          digitalWrite(dp1_pin , HIGH);  
          Serial.println("Yükseltici calisti"); 
      }else{
      if(ph_statu == "X" && ph_voltage < ph_high && dp1_times > dp1_sayac){
          digitalWrite(dp1_pin , HIGH);  
          Serial.println("Yükseltici calisti"); 
          }
         else{
          digitalWrite(dp1_pin , LOW);      
          Serial.println("Yükseltici durduruldu");     
          dp1_sayac = 0;    
          DozajClose("dp1_statu") ;   
         }
       }
    }
      
  //*---*
    void DozajDown(){
      dp2_sayac = dp2_sayac + 1;
          Serial.print("dp2_sayac: "); 
          Serial.println(dp2_sayac); 
      
      if(dp2_statu == "X" && dp2_times > dp2_sayac ){
          digitalWrite(dp2_pin , HIGH);  
          Serial.println("Düşürücü calisti"); 
      }else{
      if(ph_statu == "X" && ph_voltage > ph_high && dp2_times > dp2_sayac){
          digitalWrite(dp2_pin, HIGH); 
          Serial.println("Düşürücü calisti"); 
          }
         else{
          digitalWrite(dp2_pin, LOW);     
          Serial.println("Düşürücü durduruldu");  
          dp2_sayac = 0;      
          DozajClose("dp2_statu") ;    
         }
       }
    }
  
    void DozajClose(String DX ){
      Serial.print("Dozaj ");
      Serial.print(DX); 

      String path = "dozaj/";
             path.concat(DX); 
      Serial.println(" kapatıldı."); 

      Firebase.setString(path, "");
    }
  //*---* 

  //baglanınca fb ye zamanı söyle
