unsigned int chargePinTable[] = {
  13, //Charge Relay 1
  12, //Carge Relay 2
  11,
  10,
  9,
  8
};

unsigned int cabinetPinTable[] {
  7, //Cabinet Relay 1
  6, //Cabinet Relay 2
  5,
  4,
  3,
  A0
};

uint8_t tokenPin = 2;

void setup() {
  Serial.begin(9600);

  for(uint8_t i = 0; i < 6; i++) {
    uint8_t pin = chargePinTable[i];
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }

  for(uint8_t i = 0; i < 6; i++) {
    uint8_t pin = cabinetPinTable[i];
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }

  pinMode(tokenPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(tokenPin), OnTokenReceived, RISING);
}

void OnTokenReceived() {
  Serial.println("Token: Received");
}

String inputMessage = "";
unsigned long lastHeartbeatAt = millis();

void loop() {
  // if there's any serial available, read it:
  while (Serial.available() > 0) {
    char c = Serial.read();
    
    if (c == ';') {
      if (inputMessage.startsWith("CHARGE:")) 
        parseChargeCommand(inputMessage);
      else if (inputMessage.startsWith("OPEN-CAB:"))
        parseCabinetCommand(inputMessage);
        
      inputMessage = "";
    }
    else
      inputMessage += c;
  }

  if (millis() - lastHeartbeatAt > 1000) {
    Serial.print("HeartBeat: ");
    Serial.println(lastHeartbeatAt);

    lastHeartbeatAt = millis();
  }
}

void parseChargeCommand(String& cmd) {
  cmd.replace("CHARGE: ", "");
  uint8_t chargeRelayIndex = isDigit(cmd[0]) ? (uint8_t)(cmd[0] - 48) : 0;
  char temp[3] = "";
  temp[0] = (char)(chargeRelayIndex + 48);
  strcat(temp, ":\0");
  cmd.replace(temp, "");
  bool shouldOpen = (cmd == "ON");

  cmd = "";
  temp[0] = '\0';

  SetChargeRelayState(chargeRelayIndex, shouldOpen ? 1 : 0); //1 True = HIGH, 0 False = LOW
}

void parseCabinetCommand(String& cmd) {
  cmd.replace("OPEN-CAB: ", "");
  uint8_t cabinetRelayIndex = isDigit(cmd[0]) ? (uint8_t)(cmd[0] - 48) : 0;

  cmd = "";

  OpenCabinetRelay(cabinetRelayIndex);
}

void SetChargeRelayState(uint8_t index, uint8_t state) {
  digitalWrite(chargePinTable[index], state == 0 ? LOW : HIGH);

  Serial.print("SarjRelay: ");
  Serial.print(index + 1);
  Serial.print(" ");
  Serial.println(state == 0 ? "kapandi." : "acildi.");
}

void OpenCabinetRelay(uint8_t index) {
  digitalWrite(cabinetPinTable[index], HIGH);
  delay(1000);
  digitalWrite(cabinetPinTable[index], LOW);

  Serial.print("Cekmece: ");
  Serial.print(index + 1);
  Serial.println(" acildi.");
}










