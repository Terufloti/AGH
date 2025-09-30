#include <SoftwareSerial.h>
#include <NewPing.h>
#include <stdio.h> // for snprintf()

// Used PINS
#define TRIGGER_PIN     10
#define ECHO_PIN        9
#define MIN_DISTANCE_CM 200
#define MAX_DISTANCE_CM 300

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE_CM);

// Frequency and serial ports
SoftwareSerial FC(2, 3);                  // D2 = RX, D3 = TX
constexpr uint32_t MSP_BAUD = 57600;
constexpr uint32_t USB_BAUD = 115200;

// MSP v1
constexpr uint8_t MSP_API_VERSION = 1;
constexpr uint8_t MSP_SET_RAW_RC  = 200;
constexpr uint8_t MSP_RC          = 105;
constexpr uint8_t MSP_NAME        = 10;
constexpr uint8_t MSP_SET_NAME    = 11;

// AUX indexes 
constexpr uint8_t CH_IDX_BEEPER = 9;  // Beeper
constexpr uint8_t CH_IDX_SWITCH = 7;  // Safe mode switch

// States
uint16_t rcNow[16] = {1500};
volatile uint16_t switchRaw = 1500;
enum SwitchPos { SW_DOWN, SW_MID, SW_UP };
volatile SwitchPos switchPos = SW_MID;

char craftName[33] = {0};
char origName[33]  = {0};
bool haveOrigName  = false;

// OSD States
bool osdShown      = false; // Active "Obstacle"
bool osdBlankShown = false; // Active empty name

// MSP core - sending read message to FC
void sendMspV1(uint8_t cmd, const uint8_t *payload, uint8_t len){
  uint8_t csum = 0;
  FC.write('$'); 
  FC.write('M'); 
  FC.write('<');
  FC.write(len); 
  csum ^= len;
  FC.write(cmd); 
  csum ^= cmd;
  for(uint8_t i=0;i<len;i++){ 
    uint8_t b = payload ? payload[i] : 0; 
    if(payload) {
       FC.write(b); 
      csum ^= b; 
    } 
  }
  FC.write(csum);
}

inline void mspRequest(uint8_t cmd){ 
  sendMspV1(cmd, nullptr, 0); 
}

static const uint8_t MAX_MSP_PAYLOAD = 64;

bool readOneMspFrame(uint8_t &cmd, uint8_t *payload, uint8_t &len){
  while (FC.available()){
    if (FC.peek() != '$'){ 
      FC.read(); 
      continue; 
    }
    if (FC.available() < 3) 
      return false;
    FC.read(); 
    if (FC.read()!='M') 
      continue;
    char dir = FC.read();
    if (dir != '>' && dir != '!') 
      continue;
    while (FC.available() < 2) 
      return false;
    uint8_t frameLen = FC.read();
    uint8_t frameCmd = FC.read();
    while (FC.available() < (int)frameLen + 1) 
      return false;
    uint8_t csum = 0;
    csum ^= frameLen;
    csum ^= frameCmd;
    uint8_t bToCopy = 0;
    if(payload != nullptr) {
      if(frameLen < MAX_MSP_PAYLOAD)
        bToCopy = frameLen;
      else
        bToCopy = MAX_MSP_PAYLOAD;
    }
    for(uint8_t i=0;i<bToCopy;i++){
      uint8_t payloadB = FC.read();
      payload[i] = payloadB;
      csum ^=payloadB;
    }
    for(uint8_t i=bToCopy;i<frameLen;i++){
      uint8_t skippedB = FC.read();
      csum ^= skippedB;
    }
    uint8_t recvCsum = (uint8_t)FC.read();
    if(recvCsum != csum)
      continue;
    if(dir == '!')
      continue;
    cmd = frameCmd;
    len = bToCopy;
    return true;
  }
  return false;
}

void pollMsp(){
  static uint8_t payload[64]; uint8_t cmd,len;
  while (readOneMspFrame(cmd,payload,len)){
    if (cmd == MSP_RC){
      uint8_t n = len/2; 
      if (n>16) 
        n=16;
      for(uint8_t i=0;i<n;i++){
        rcNow[i] = (uint16_t)payload[2*i] | ((uint16_t)payload[2*i+1] << 8);
      }
      switchRaw = rcNow[CH_IDX_SWITCH];
      if (switchRaw < 1300) 
        switchPos = SW_DOWN;
      else if (switchRaw > 1700) 
        switchPos = SW_UP;
      else
        switchPos = SW_MID;
    }
    else if (cmd == MSP_NAME){
      uint8_t n = len; 
      if (n>32) 
        n=32;
      memcpy(craftName, payload, n); 
      craftName[n]='\0';
      if (!haveOrigName){ 
        strcpy(origName, craftName); 
        haveOrigName=true; 
      }
      Serial.print(F("[MSP] Craft name: "));
      Serial.print(craftName);
    }
  }
}

// Helper functions
void sendBeeperAUX(uint16_t val){          // val 1000..2000
  uint8_t buf[32];
  uint16_t ch[16];
  for(uint8_t i=0;i<16;i++) 
    ch[i]=1500;
  ch[CH_IDX_BEEPER] = val;
  // pack LE
  for(uint8_t i=0;i<16;i++){ 
    buf[2*i] = ch[i] & 0xFF; 
    buf[2*i+1] = ch[i]>>8; 
  }
  sendMspV1(MSP_SET_RAW_RC, buf, sizeof(buf));
}

void setCraftName(const char* s){
  // max 32 chars
  uint8_t len = 0; 
  while (s[len] && len<32) 
    len++;
  uint8_t csum = 0;
  FC.write('$'); 
  FC.write('M'); 
  FC.write('<');
  FC.write(len); 
  csum ^= len;
  FC.write(MSP_SET_NAME); 
  csum ^= MSP_SET_NAME;
  for(uint8_t i=0;i<len;i++){ 
    uint8_t b=(uint8_t)s[i]; 
    FC.write(b); 
    csum ^= b; 
  }
  FC.write(csum);
}

void setCraftNameBlank(){             // „empty” name – safe one space
  setCraftName(" ");
  osdBlankShown = true;
  osdShown = false;
}

void setCraftNameObstacle(){          // dynamically: „OBSTACLE (<XXcm)!”
  char msg[33];
  snprintf(msg, sizeof(msg), "OBSTACLE (<%ucm)!", (unsigned)MIN_DISTANCE_CM);
  setCraftName(msg);
  osdShown = true;
  osdBlankShown = false;
}

void tickRequestRC(){                 // ask about RC ~20 Hz
  static uint32_t last=0; uint32_t now=millis();
  if (now-last >= 50){ 
    mspRequest(MSP_RC); 
    last=now; 
  }
}
void requestOrigNameOnce(){
  static bool asked=false;
  if (!asked){ 
    mspRequest(MSP_NAME); 
    asked=true; 
  }
}

// SETUP
void setup(){
  Serial.begin(USB_BAUD); 
  delay(200);
  FC.begin(MSP_BAUD);
  delay(300);
  requestOrigNameOnce();
  Serial.println(F("Ready."));
}

void loop(){
  // 1) Receiving MSP (RC + name)
  tickRequestRC(); // mspRequest - sending sendMSPv1
  pollMsp();

  // 2) Distance measurements
  static uint32_t lastPing=0; 
  uint32_t now=millis();
  static bool obstacle=false, obstaclePrev=false;
  if (now - lastPing >= 50){               // ~20 Hz
    unsigned int us = sonar.ping_median(5);
    unsigned int dist = sonar.convert_cm(us);
    obstaclePrev = obstacle;
    obstacle = (dist>0 && dist < MIN_DISTANCE_CM);
    lastPing = now;
  }

  // 3) Reaction to switch CHANGE (edge)
  static SwitchPos prevPos = SW_MID;
  if (switchPos != prevPos) {
    // out from UP -> set original name
    if (prevPos == SW_UP) {
      if (haveOrigName) 
        setCraftName(origName);
      osdShown = false;
      osdBlankShown = false;
    }
    // in UP -> Clear OSD immadiately
    if (switchPos == SW_UP) {
      if (!haveOrigName) 
        requestOrigNameOnce();
      setCraftNameBlank(); // empty name right after in UP 
    }
    prevPos = switchPos;
  }

  // 4) Logic depending on switch
  static uint32_t lastMSP=0;
  if (switchPos == SW_DOWN){
    // Override off
    if ((osdShown || osdBlankShown) && haveOrigName){ 
      setCraftName(origName); 
      osdShown=false; 
      osdBlankShown=false;
    }

  } else {
    // MID/UP: override in BF active – keep Beeper AUX depending on obstacle (~25 Hz)
    if (now - lastMSP >= 40){
      sendBeeperAUX(obstacle ? 2000 : 1000);
      lastMSP = now;
    }

    if (switchPos == SW_UP){
      // in UP: in case of obstacle -> OBSTACLE; without obnstacle -> empty
      if (obstacle && !osdShown){
        setCraftNameObstacle();
      } else if (!obstacle && !osdBlankShown){
        setCraftNameBlank();
      }
    } else { // SW_MID
      // in MID  we want original name (without clear)
      if ((osdShown || osdBlankShown) && haveOrigName){
        setCraftName(origName);
        osdShown=false; 
        osdBlankShown=false;
      }
    }
  }
}
