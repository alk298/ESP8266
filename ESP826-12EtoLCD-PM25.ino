

/*********************************************************************
This is an example sketch for ESP8266-12E with DHT22+ PMS3003 (PM2.5 sensor)  
+ 1602/2004A  LCD Display .

Written by alk298

*********************************/ 

  #include <Wire.h> 
  #include <LiquidCrystal_I2C.h>
  #include <ESP8266WiFi.h>
  #include <WiFiClientSecure.h>

 #include <ESP8266Ping.h>
 const IPAddress remote_ip(8, 8, 8, 8); // Remote host

  #include <SoftwareSerial.h>// for PM2.5 definition
   SoftwareSerial mySerial2(12, 14); // GPIO12--> to PM303 TX, GPIO14 ----> PM303 RX
   #define pmsDataLen 24 
    unsigned char buf[pmsDataLen];
    int idx = 0;
    int pm10 = 0;
    int pm25 = 0;
    int pm100 = 0;
    int uppm25=0;

    bool hasPm25Value = false;

 // for CO2 definition --TBC
     int upco2=0;

  #include "DHT.h"
  #define DHTPIN 13 // GPIO 13
  #define DHTTYPE DHT22    

  
// LiquidCrystal_I2C lcd(0x27,16,2); // set for LCD 1602

// set the LCD 2004 address to 0x3f for a 20 chars and 4 line display
//  0x3f for B2 AirQuality System
//  0x27 for B4 Air Quality System 
 
 LiquidCrystal_I2C lcd(0x27,20,4);  


const char* ssid= "XXXX";
const char* password = "XXXX";






String  MACAddress = "";
String LocalIP ="";
String MAC="";

const int httpPort = 443;
const char* host = "XX.XX.XX";//Your Nas DNS or IP



int AP=0;
 int Autodisconnect=0;
  int exitcount =0;




// Use web browser to view and copy
// SHA1 fingerprint of the certificate
//const char* fingerprint = "CF 05 98 89 CA FF 8E D8 5E 5C E0 C2 E4 F7 E6 C3 C7 50 DD 5C";


const char* fingerprint = "DA 39 A3 EE 5E 6B 4B 0D 32 55 BF EF 95 60 18 90 AF D8 07 09";



DHT dht(DHTPIN, DHTTYPE, 15);


  
  void setup() 
  {
    dht.begin();

  ESP.wdtDisable();
  ESP.wdtEnable(WDTO_8S);
    

    Wire.begin(4, 5);  // GPIO :4=SDA, GPIO 5 =SCL for LCD 1602/2004
    
    mySerial2.begin(9600); // PMS 3003 UART has baud rate 9600


     lcd.init();                      // initialize the lcd 
     lcd.backlight();
     lcd.home();
     lcd.print("DQE T/H Record");
     lcd.setCursor(0, 1);//for 2004aLCD
     lcd.print("WiFi Connection !!!");//for 2004aLCD

 
     delay (2000);
 
     WiFi.mode(WIFI_STA);

     Serial.begin(115200);
     
  
  WiFi.begin(ssid, password);  


  
    // while (WiFi.status() != WL_CONNECTED) {
 
    while ( !Ping.ping(remote_ip)) {

    delay(2000);
    Serial.print(".");
    Serial.println(WiFi.localIP());

    
    lcd.setCursor(0, 1);//for 2004aLCD
    lcd.print("IP=");//for 2004aLCD
    lcd.print(WiFi.localIP());//for 2004A LCD
    lcd.setCursor(0, 2);//for 2004aLCD
    lcd.print("Check WiFi Status !!!");//for 2004aLCD
   
  }//  while ( !Ping.ping(remote_ip)) 

     MACAddress=WiFi.macAddress();
     MAC=MACAddress.substring(12, 17);

     lcd.clear(); // for 1602 LCD
     delay (700);// Dealy for stablize LCD after LCD clear();
   
    lcd.setCursor(0, 2);//for 2004aLCD
    lcd.print("IP=");//for 2004aLCD
    lcd.print(WiFi.localIP());//for 2004A LCD
    lcd.setCursor(0, 1);//for 2004aLCD
    lcd.print(MACAddress);//for 2004A LCD
/*
   lcd.setCursor(0, 0);//for 1602 LCD
   lcd.print("IP=");//for 1602 LDC
   lcd.print(WiFi.localIP());//for 1602 LCD
      delay (5000);
    lcd.clear(); // for 1602 LCD
   delay (700);// Dealy for stablize LCD after LCD clear();
   lcd.setCursor(0, 0);//for 1602 LCD
   lcd.print("MAC=");//for 1602 LCD
   lcd.print(MAC);//for 1602 LCD



   
*/
     Serial.print("Pinging ip : ");
     Serial.println(remote_ip);
 
    // Ping
     if (Ping.ping(remote_ip)) 
         {
          Serial.println("Ping Success!!");
            lcd.setCursor(0, 3);//for 2004aLCD
            lcd.print("Ping Success!!!");//for 2004aLCD
          } else {
            Serial.println("Ping Error :");

           lcd.setCursor(0, 3);//for 2004aLCD
           lcd.print("Ping Error !!!");//for 2004aLCD
           
            }


     delay (5000);
     lcd.clear(); // for 1602 LCD
     delay (700);// Dealy for stablize LCD after LCD clear();

   
  }//  void setup() 



  
  void loop()  
  
  {
    ESP.wdtFeed();
    exitcount=0;

     MACAddress=WiFi.macAddress();
     MAC=MACAddress.substring(12, 17);

 /*    
   lcd.clear(); // for 1602 LCD
   delay (700);// Dealy for stablize LCD after LCD clear();

   lcd.setCursor(0, 0);//for 1602 LCD
   lcd.print("MAC=");//for 1602 LCD
   lcd.print(MAC);//for 1602 LCD
*/


    
    lcd.setCursor(0, 0);//for 2004aLCD
    lcd.print(MACAddress);//for 2004A LCD


   if(MACAddress =="")
     {

      
       lcd.clear(); // for 1602 LCD
       delay (700);// Dealy for stablize LCD after LCD clear();

  //     lcd.setCursor(0, 0);//for 1602 LCD
  //     lcd.print("MAC=Empty.!");//for 1602 LCD

       lcd.setCursor(0, 0);//for 2004 LCD
       lcd.print("MAC=Empty.!!!!!");//for 2004 LCD

       
      return;
     }


// WiFiClient client;

  // Use WiFiClientSecure class to create TLS connection
  WiFiClientSecure client;
/*
    Autodisconnect = Autodisconnect+1;
    if(Autodisconnect >=2)
    {
       WiFi.disconnect();
       Serial.println ("Autodisconnect=1\n");

       Serial.println (WiFi.status());


    }
    */

   Serial.println (WiFi.status());

   
   if (!client.connect(host, httpPort)) 

  { 
    
 //  lcd.setCursor(0, 0);// for 1602 LCD

   Serial.println("connection failed");

   lcd.setCursor(0, 1);// for 2004A LCD

    lcd.print("PHP Failed!!!!!!");
    
     lcd.setCursor(0, 2);//for 2004aLCD
    lcd.print("IP=");//for 2004aLCD
    lcd.print(WiFi.localIP());//for 2004A LCD

    

      Serial.print("Pinging ip : ");
      Serial.println(remote_ip);
 
    // Ping
     if (Ping.ping(remote_ip)) {
           Serial.println("Ping Success !!!!");
            lcd.setCursor(0, 3);//for 2004aLCD
            lcd.print("Ping Success!!");//for 2004aLCD
          } else {
           lcd.setCursor(0, 3);//for 2004aLCD
           lcd.print("Ping Error !!!!");//for 2004aLCD
           
           Serial.println("Ping Error :");

           
            }
          
    
    delay(10000);

    
 if (AP==0)
  {

      WiFi.begin(ssid, password); 
      delay(2000);
      Serial.println ("WiFi.Beging=1\n");

       Serial.println (WiFi.status());
    
  //  while (WiFi.status() != 3)
    while (!Ping.ping(remote_ip))

    
     {
        delay(2000);
        WiFi.begin(ssid, password); 
       delay(2000);
       Serial.println ("WiFi.Beging=2\n");

       Serial.println (WiFi.status());
     
 //  lcd.setCursor(0, 0);//for 1602 LCD

   lcd.setCursor(0, 1);//for 2004A LCD
 
    lcd.print("Connecting ............");

    Serial.print(".");
     exitcount = exitcount+1;

    if (exitcount >10)
    {

      Serial.println("wifi-rety 10 cycles faile  !!!!\n");
       Serial.println (WiFi.status());
     //  WiFi.disconnect(true);
     // WiFi.mode(WIFI_OFF);
      delay (5000);// Dealy for stablize LCD after LCD clear(); lcd.clear(); // for 1602 LCD
              
      lcd.clear(); // for 1602 LCD
      delay (700);// Dealy for stablize LCD after LCD clear(); lcd.clear(); // for 1602 LCD


      return;   

      }
     
    
      }//while (WiFi.status() != WL_CONNECTED) {
    

    }// if (AP==0)

     
      lcd.clear(); // for 1602 LCD
      delay (700);// Dealy for stablize LCD after LCD clear(); lcd.clear(); // for 1602 LCD
      return;   
      

  }// if (!client.connect(host, httpPort)) 


/*
     lcd.setCursor(0, 1);//for 1602 LCD
     lcd.print("IP=");//for 1602 LDC
     lcd.print(WiFi.localIP());//for 1602 LCD
*/

    lcd.setCursor(0, 1);  //for 2004aLCD
    lcd.print("IP=");   //for 2004aLCD
    lcd.print(WiFi.localIP());  //for 2004A LCD


         delay (2000);
    

   float h = dht.readHumidity();
   float t = dht.readTemperature();
   float f = dht.readTemperature(true);


    if (isnan(h) || isnan(t) || isnan(f)) 
    {
      // lcd.setCursor(0, 0);// for 1602 LDC
        lcd.setCursor(0, 2);// for 2004A LCD
       
       lcd.print("Read Fail DHT !!!!!!!");
            delay(10000);


        Serial.println("Failed to read from DHT sensor!");
        return;
   } 




     // Show LCD for Temp and Humid & PM2.5
    /* 
      lcd.clear(); // for 1602 LCD
      lcd.setCursor(0, 1);// for 1602 LDC
      lcd.print("T=");// for 1602 LDC
      lcd.print(t);// for 1602 LDC
      lcd.setCursor(8, 1);// for 1602 LDC
      lcd.print("H=");// for 1602 LDC
      lcd.print(h);// for 1602 LDC
*/
     
 

      lcd.setCursor(0, 2);// for 2004A LCD
      lcd.print("T=");// for 2004A LDC
      lcd.print(t);// for for 2004A LDC
      lcd.setCursor(9, 2);// for 2004A LDC
      lcd.print("H=");// for for 2004A LDC
      lcd.print(h);// for for 2004A LDC

      retrievePM25Value();
     
      delay(10000);
      

  


     //  t=25.55;
     //  h=88.88;
 
    String url = "/ESP8266/esp.php?";
    url += "&mac="; 
    url +=MACAddress; 
    
    url += "&pm25="; 
    url +=uppm25; 
    
     url += "&co2="; 
    url +=upco2; 
    
    url += "&t="; 
    url += t;
    url += "&h="; 
    url += h;

    
    Serial.println(url);

    
   // client.print(String("GET ") + url + " HTTP/1.1\r\n" +
   //            "Host: " + host + "\r\n" + 
   //            "Connection: close\r\n\r\n");  

 if (client.connect(host, httpPort)) 

{
      Serial.println("Server respond ok !!!\n");

     client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");

               
}            

  //  delay (5000);

  // Read all the lines of the reply from server and print them to Serial
  // while(client.available()){
  // String line = client.readStringUntil('\r');
  // Serial.println(line);
 // }
   
    
    //  lcd.scrollDisplayLeft();
    // delay (400);
 
     delay(120000);
         //     delay(20000);

    lcd.clear(); // for 1602/2004
    delay (700);// Dealy for stablize LCD after LCD clear();

  }//Loop



void retrievePM25Value()
{
 Serial.println("Enter PMsensor d for Reading") ;
  hasPm25Value = false;
  int  count = 0;
  int ckecksum = 0 ;
  int exptsum = 0 ;
  memset(buf, 0, pmsDataLen);

  
//Wayne


 // while (( Serial2.available()>0) && (count < pmsDataLen) )
  while (( mySerial2.available()>0) && (count < pmsDataLen) )

  
  {
    //    Serial1.readBytes(buf,pmsDataLen);
    //buf[count] = Serial2.read();

    buf[count] = mySerial2.read();

    
  Serial.print(".");
    /*
      Serial.print("NO(") ;
      Serial.print(count) ;
      Serial.print(") , ") ;
      Serial.print( buf[count],DEC) ;
      Serial.print("\n") ;
    */
    if (buf[0] == 0x42 && count == 0 )
    {
      count = 1 ;
      continue ;

    }
    if (buf[1] == 0x4d && count == 1 )
    {
      count = 2 ;
      continue ;

    }
    if (count >= 2 )
    {
      count++ ;
      if (count >= (pmsDataLen) )
      {
        hasPm25Value = true ;
        break ;
      }
      continue ;
    }

  } //void retrievePM25Value()


 if (hasPm25Value )
  {
    for (int i = 0 ; i < (pmsDataLen - 2) ; i++)
      ckecksum = ckecksum + buf[i] ;
    exptsum = ((unsigned int)buf[22] << 8 ) + ((unsigned int)buf[23]) ;
    if (ckecksum == exptsum)
    {
      hasPm25Value = true ;
    }
    else
    {
      hasPm25Value = false ;
      Serial.print("\n \n ERROR Check sum");
      Serial.print("\n Sensor Check sum is : ");
      Serial.print(exptsum);
      Serial.print(", \n And Data Check sum is :");
      Serial.print(ckecksum);
      Serial.println("");
      return ;
    }
    pm25 = ( buf[12] << 8 ) | buf[13];
    pm10 = ( buf[10] << 8 ) | buf[11];
    pm100 = ( buf[14] << 8 ) | buf[15];

    Serial.print("\n pm2.5: ");
    Serial.print(pm25);
    Serial.print(" ug/m3\t");
    Serial.print("pm1.0: ");
    Serial.print(pm10);
    Serial.print(" ug/m3\t");
    Serial.print("pm10: ");
    Serial.print(pm100);
    Serial.print(" ug/m3");
    Serial.println("");
    /*
     //Show PM25 to LCD 1602
      lcd.setCursor(0, 0);//for 1602 LCD  
      lcd.print("PM25=");//for 1602 LDC
      lcd.print(pm25);//for 1602 LCD
      lcd.print(" ug/m3");//for 1602 LDC
      */

   //Show PM25 to LCD 2004
      uppm25=pm25;
      lcd.setCursor(0, 3);//for 2004 LCD  
      lcd.print("PM2.5= ");//for 2004 LCD
      lcd.print(pm25);///for 2004 LCD
      lcd.print(" ug/m3");//for 2004 LCD
      

  }


}







  
