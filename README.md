# RFID attendance logger 


## The problem with paper attendance: 
- Manual registers take 5–10 min per class, are prone to proxy, and hard to audit
- In a college of 2000 students, that's ~66 hours of attendance time wasted per day
- No real-time visibility for admins or parents
- Segue: "What if a tap of a card could replace all of that?"

## How RFID works
- RFID = Radio Frequency Identification — card stores a unique ID, reader detects it wirelessly
- MFRC522 operates at 13.56 MHz (same as metro cards, office ID cards)

## Our system - what it does
Authorized card → green LED + double beep + name + timestamp logged
Unknown card → red LED + long buzz + "UNKNOWN" flagged in log
Serial Monitor outputs live CSV — copy directly to Excel or Google Sheets
Built on Arduino Uno + MFRC522, simulated on Wokwi 
