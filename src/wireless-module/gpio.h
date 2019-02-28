const int PIN_DATA_OUT = 1;  // Red (or orange)
const int PIN_DATA_IN = 2;   // Orange (or red)
const int PIN_CLOCK = 3;     // Green

void setupGpio() {
  pinMode(PIN_DATA_OUT, OUTPUT);
  pinMode(PIN_DATA_IN, INPUT);
  pinMode(PIN_CLOCK, OUTPUT);

  digitalWrite(PIN_CLOCK, HIGH);
}

int gpioTransferBit(int outBit) {
  int inBit;

  digitalWrite(PIN_DATA_OUT, outBit);
  digitalWrite(PIN_CLOCK, LOW);
  delayMicroseconds(61);
  inBit = digitalRead(PIN_DATA_IN);
  digitalWrite(PIN_CLOCK, HIGH);
  delayMicroseconds(61);

  return inBit;
}

byte gpioTransferByte(byte outByte) {
  byte inByte = gpioTransferBit((outByte >> 7) & 0x01);
  inByte = (inByte << 1) | gpioTransferBit((outByte >> 6) & 0x01);
  inByte = (inByte << 1) | gpioTransferBit((outByte >> 5) & 0x01);
  inByte = (inByte << 1) | gpioTransferBit((outByte >> 4) & 0x01);
  inByte = (inByte << 1) | gpioTransferBit((outByte >> 3) & 0x01);
  inByte = (inByte << 1) | gpioTransferBit((outByte >> 2) & 0x01);
  inByte = (inByte << 1) | gpioTransferBit((outByte >> 1) & 0x01);
  inByte = (inByte << 1) | gpioTransferBit((outByte >> 0) & 0x01);

  return inByte;
}
