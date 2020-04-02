// ATtiny85 based envelope
// Made by Simon Juhl
// www.simonjuhl.net
// should work with other microcontrolles

int env = 0; // envelope state (0-255)
int len;  // length of env
long int time; // time
unsigned long  lastTime; // time since last PWM change
unsigned long triggerTime; // time since triggered
bool gateRise = true;
bool triggered = false; // is running env

void setup() {
  pinMode(0, OUTPUT); // pwm
  pinMode(1, OUTPUT); // active gate
  pinMode(2, OUTPUT); // edge trigger
  pinMode(4, INPUT);  // gate input
  pinMode(3, INPUT);  // length ctrl
}

void loop() {
  time = micros(); // get time
  len = analogRead(3); // set length

  if (gateRise && digitalRead(4) == HIGH) { // check if gate input is high
    digitalWrite(2, HIGH); // set rising edge trigger to HIGH
    triggerTime = time; // get trigger time
    gateRise = false;
    triggered = true;
    env = 255; // set env to max
  }

  if (triggered && triggerTime + 8000 <= time) { // 8ms pulse length
    digitalWrite(2, LOW);
    triggerTime = 0;
  }

  if (gateRise == false && digitalRead(4) == LOW) {
    gateRise = true;
  }

  if (time >= lastTime + (len + 2 ) * 4 && triggered) { // PWM env
    analogWrite(0, env); // set PWM
    lastTime = time; 
    env--; // decay/release
  }

  if (env <= 0 && triggered) { // turn of PWM on end
    triggered = false;
    analogWrite(0, 0);
  }

  if (triggered) { // active gate output
    digitalWrite(1, HIGH);
  } else {
    digitalWrite(1, LOW);
  }
}
