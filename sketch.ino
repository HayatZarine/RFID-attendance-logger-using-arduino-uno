#include <SPI.h>
#include <MFRC522v2.h>
#include <MFRC522DriverSPI.h>
#include <MFRC522DriverPinSimple.h>

#define BUZZER    7
#define LED_GREEN 5
#define LED_RED   4

MFRC522DriverPinSimple ss_pin(10);
MFRC522DriverSPI driver{ss_pin};
MFRC522 rfid{driver};

//known cards 
const byte UID1[4] = {0x01, 0x02, 0x03, 0x04}; // Blue Card  → Zarine Hayat
const byte UID2[4] = {0xAA, 0xBB, 0xCC, 0xDD}; // Red Card   → Tanvi
const byte UID3[4] = {0x11, 0x22, 0x33, 0x44}; // Green Card → Plakshi
const char* NAME1  = "ZarineHayat";
const char* NAME2  = "Tanvi";
const char* NAME3  = "Plakshi";

// log 
#define MAX_LOG 50
byte  logUID[MAX_LOG][4];
char  logName[MAX_LOG][12];
long  logTime[MAX_LOG];
int   logCount = 0;

bool uidEqual(byte* a, const byte* b) {
  return (a[0]==b[0] && a[1]==b[1] && a[2]==b[2] && a[3]==b[3]);
}

void greenBeep() {
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_RED,   LOW);
  tone(BUZZER, 1000); delay(120); noTone(BUZZER);
  delay(80);
  tone(BUZZER, 1400); delay(120); noTone(BUZZER);
  delay(300);
  digitalWrite(LED_GREEN, LOW);
}

void redBeep() {
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED,   HIGH);
  tone(BUZZER, 300); delay(600); noTone(BUZZER);
  delay(300);
  digitalWrite(LED_RED, LOW);
}

void printRow(int i) {
  Serial.print(i + 1);
  Serial.print("\t");
  Serial.print(logUID[i][0], HEX); Serial.print(":");
  Serial.print(logUID[i][1], HEX); Serial.print(":");
  Serial.print(logUID[i][2], HEX); Serial.print(":");
  Serial.print(logUID[i][3], HEX);
  Serial.print("\t");
  Serial.print(logName[i]);
  Serial.print("\t");
  Serial.print(logTime[i]);
  Serial.println("s");
}

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.PCD_Init();

  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED,   OUTPUT);
  pinMode(BUZZER,    OUTPUT);

  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED,   LOW);

  // startup LED test
  digitalWrite(LED_GREEN, HIGH); delay(300); digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED,   HIGH); delay(300); digitalWrite(LED_RED,   LOW);

  Serial.println("=== RFID Attendance Logger ===");
  Serial.println("Scan a card...");
  Serial.println();
  Serial.println("#\tUID\t\t\t\tName\tTime");
  Serial.println("--\t---\t\t\t\t----\t----");
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent()) { delay(50); return; }
  if (!rfid.PICC_ReadCardSerial())   { delay(50); return; }

  byte* uid = rfid.uid.uidByte;
  long  now = millis() / 1000;

  const char* name = "UNKNOWN";
  bool authorized  = false;

  if      (uidEqual(uid, UID1)) { name = NAME1; authorized = true; }
  else if (uidEqual(uid, UID2)) { name = NAME2; authorized = true; }
  else if (uidEqual(uid, UID3)) { name = NAME3; authorized = true; }

  if (logCount < MAX_LOG) {
    logUID[logCount][0] = uid[0];
    logUID[logCount][1] = uid[1];
    logUID[logCount][2] = uid[2];
    logUID[logCount][3] = uid[3];
    strncpy(logName[logCount], name, 11);
    logName[logCount][11] = '\0';
    logTime[logCount] = now;
    printRow(logCount);
    logCount++;
  }

  if (authorized) greenBeep();
  else            redBeep();

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
  delay(1500);
}
