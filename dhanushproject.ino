#include <WiFi.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Fingerprint.h>
#include <Firebase_ESP_Client.h>
#include "time.h"

/* WIFI */
#define WIFI_SSID "project"
#define WIFI_PASSWORD "12345678"

/* FIREBASE */
#define API_KEY "AIzaSyA2agIFvlbFSwsgMZIEJZLVAPv8JtXCUCo"
#define DATABASE_URL "https://rabies-vaccine-system-default-rtdb.firebaseio.com/"

/* RFID */
#define SS_PIN 5
#define RST_PIN 4
MFRC522 rfid(SS_PIN, RST_PIN);

/* OLED */
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 lcd(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

/* Fingerprint */
HardwareSerial mySerial(2);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

/* Buttons */
#define BTN_PRESENT 32
#define BTN_REGISTER 33

/* Firebase */
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

bool signupOK=false;
String uid="";

int doseDays[5]={0,3,7,14,28};

void showOLED(String line1, String line2="")
{
  lcd.clearDisplay();
  lcd.setCursor(0,0);
  lcd.println(line1);

  if(line2!="")
  {
    lcd.setCursor(0,20);
    lcd.println(line2);
  }

  lcd.display();
}

void updateLatestScan(String scanType, String statusMsg)
{
  time_t now;
  time(&now);

  int fingerID = 0;

  String fingerPath = "Patients/" + uid + "/fingerID";

  if (Firebase.RTDB.getInt(&fbdo, fingerPath))
  {
    fingerID = fbdo.intData();
  }

  Firebase.RTDB.setString(&fbdo, "LatestScan/uid", uid);
  Firebase.RTDB.setInt(&fbdo, "LatestScan/fingerID", fingerID);
  Firebase.RTDB.setString(&fbdo, "LatestScan/scanType", scanType);
  Firebase.RTDB.setString(&fbdo, "LatestScan/status", statusMsg);
  Firebase.RTDB.setInt(&fbdo, "LatestScan/timestamp", now);

  Serial.println("LatestScan updated");
}

void setup()
{
  Serial.begin(115200);

  pinMode(BTN_PRESENT,INPUT_PULLUP);
  pinMode(BTN_REGISTER,INPUT_PULLUP);

  /* OLED */
  Wire.begin(21,22);

  if(!lcd.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println("OLED failed");
    while(true);
  }

  lcd.clearDisplay();
  lcd.setTextSize(1);
  lcd.setTextColor(SSD1306_WHITE);

  showOLED("System Starting");

  /* RFID */
  SPI.begin(18,19,23,5);
  rfid.PCD_Init();

  /* Fingerprint */
  mySerial.begin(57600,SERIAL_8N1,16,17);
  finger.begin(57600);

  if (finger.verifyPassword())
    Serial.println("Fingerprint OK");
  else
    Serial.println("Fingerprint FAIL");

  /* WiFi */
  WiFi.begin(WIFI_SSID,WIFI_PASSWORD);

  showOLED("Connecting WiFi");

  while(WiFi.status()!=WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  showOLED("WiFi Connected");

  /* Firebase */
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  if(Firebase.signUp(&config,&auth,"",""))
  {
    signupOK=true;
  }

  Firebase.begin(&config,&auth);
  Firebase.reconnectWiFi(true);

  configTime(0,0,"pool.ntp.org");

  showOLED("Scan RFID");
}

void loop()
{
  if (!rfid.PICC_IsNewCardPresent())
  {
    delay(200);
    return;
  }

  if (!rfid.PICC_ReadCardSerial())
  {
    return;
  }

  uid="";

  for (byte i = 0; i < rfid.uid.size; i++)
  {
    uid += String(rfid.uid.uidByte[i], HEX);
  }

  uid.toUpperCase();

  updateLatestScan("RFID", "SCANNED");

  showOLED("Rabies Vaccine");

  delay(2000);

  checkPatient();

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();

  showOLED("Scan RFID");
}

void checkPatient()
{
  String path="Patients/"+uid+"/fingerID";

  if(Firebase.RTDB.getInt(&fbdo,path))
  {
    int dose;

    Firebase.RTDB.getInt(&fbdo,"Patients/"+uid+"/dose");
    dose=fbdo.intData();

    checkSchedule(dose);
  }
  else
  {
    showOLED("New Patient","Press Register");

    while(digitalRead(BTN_REGISTER)==HIGH);

    registerPatient();
  }
}

void registerPatient()
{
  showOLED("Scan Finger");

  int id=enrollFinger();

  if(id>0)
  {
    Firebase.RTDB.setInt(&fbdo,"Patients/"+uid+"/fingerID",id);
    Firebase.RTDB.setInt(&fbdo,"Patients/"+uid+"/dose",0);

    time_t now;
    time(&now);

    Firebase.RTDB.setInt(&fbdo,"Patients/"+uid+"/day0",now);

    updateLatestScan("RFID", "REGISTERED");

    showOLED("Registered");

    delay(2000);
  }
}

void checkSchedule(int dose)
{
  time_t now;
  time(&now);

  int day0;

  Firebase.RTDB.getInt(&fbdo,"Patients/"+uid+"/day0");
  day0=fbdo.intData();

  int days=(now-day0)/86400;

  if(dose >= 5)
  {
    showOLED("Completed");
    updateLatestScan("RFID", "COMPLETED");
    delay(2000);
    return;
  }

  if(days==doseDays[dose])
  {
    showOLED("Give Vaccine","Press Present");

    while(digitalRead(BTN_PRESENT)==HIGH);

    dose++;

    Firebase.RTDB.setInt(&fbdo,"Patients/"+uid+"/dose",dose);

    showOLED("Dose Updated");

    updateLatestScan("RFID", "DOSE_UPDATED");

    delay(2000);
  }
  else if(days>doseDays[dose])
  {
    showOLED("Missed Dose!");

    updateLatestScan("RFID", "MISSED_DOSE");

    delay(2000);
  }
  else
  {
    showOLED("Not Due Yet");

    updateLatestScan("RFID", "NOT_DUE_YET");

    delay(2000);
  }
}

int enrollFinger()
{
  int id=random(1,200);

  while(finger.getImage()!=FINGERPRINT_OK);

  finger.image2Tz(1);

  delay(2000);

  while(finger.getImage()!=FINGERPRINT_OK);

  finger.image2Tz(2);

  if(finger.createModel()==FINGERPRINT_OK)
  {
    finger.storeModel(id);
    return id;
  }

  return -1;
}