// Sensors : pins
const int SENSOR_L = 12;
const int SENSOR_R = 11;

// Left motor : pins
const int ENA = 5;
const int IN1 = 10; // if rever turn backward switch it 
const int IN2 = 9; // with this one.

// Right motor : pins
const int ENB = 6;
const int IN3 = 8; //  if rover turn backward switch it 
const int IN4 = 7; // with this one.

//kindly check before pluging or directly flasing the code into the board, manule weather senser are working or not before wireing 

// Settings 
const int  BLACK   = HIGH;  // use LOW if sensor outputs LOW on black
const int  SPEED   = 100;   // straight speed
const int  TURN    = 100;   // turning speed
// lowest PWM where both motors reliably spin so it doesnt stops when there no engough momentum the as per the costant speed ,also set in way that it does over come the friction,
const int  MIN_PWM = 65;   // reduce if it feels speedy.

// as rover or driod was sometimes move far ahead of the line so now it should be moving along the black line only
// or if in case it doesnt then it will move back to previews steps so it can move back 

int last = 0;  // -1 = last turned left, 1 = last turned right
unsigned long lostAt = 0;
const unsigned long SEARCH_MS = 12000;   // 12 second

// as there was issue with the code itself as sometimee the mode 
void motor(int s, int a, int b, int en) {
  
  digitalWrite(a, s > 0);
  digitalWrite(b, s < 0);
  analogWrite(en, s == 0 ? 0 : max(abs(s), MIN_PWM));
}

void drive(int l, int r) {
  motor(l, IN1, IN2, ENA);
  motor(r, IN3, IN4, ENB);
}

void setup() {
  // from the code.
  TCCR0B = TCCR0B & B11111000 | B00000010;  // ~7.8 kHz PWM on D5/D6

  pinMode(SENSOR_L, INPUT);
  pinMode(SENSOR_R, INPUT);
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  drive(0, 0);
  unsigned long lostAt = 0;
  const unsigned long SEARCH_MS = 12000;  // about sec 

}

void loop() {
  bool l = digitalRead(SENSOR_L) == BLACK; // KINDLY CHECK WETHER THE THIS IS CORRECT OR NOT 
  bool r = digitalRead(SENSOR_R) == BLACK;

  if (l && r) {
    drive(SPEED, SPEED);
    lostAt = 0;
  }
  else if (l && !r) {
    drive(-TURN, TURN); // turn left: if speed is less then add or sub vise-versa.
    last = -1;
    lostAt = 0;
  }
  else if (!l && r) {
    drive(TURN, -TURN); // turn right : like wise above so it would be like (TURN+50, -TURN)
    last = 1;
    lostAt = 0;
  } 
  else {                                  
  // In case it lost the line it will serach fro the line so it can be back on where it left,
  // in case it didnt find a thing then it will go back to sleep.
    if (lostAt == 0) lostAt = millis();
    if (millis() - lostAt < SEARCH_MS) {
      if (last < 0)      drive(-TURN, TURN);
      else if (last > 0) drive(TURN, -TURN);
      else               drive(0, 0);
    } else {
      drive(0, 0);                        // searched too long: stop
    }
  }
}