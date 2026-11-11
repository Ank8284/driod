const int SENSOR_L = 12;
const int SENSOR_R = 11;

const int ENA = 5;   // left motor PWM
const int IN1 = 10;
const int IN2 = 9;
const int ENB = 6;   // right motor PWM
const int IN3 = 8;
const int IN4 = 7;

const int BLACK = HIGH;
const int SPEED = 60;
const int TURN = 80;


int last = 0;  // -1 = last turned left, 1 = last turned right
unsigned long lostAt = 0;
const unsigned long SEARCH_MS = 12000;

void drive(int l, int r) {
  digitalWrite(IN1, l > 0); digitalWrite(IN2, l < 0);
  digitalWrite(IN3, r > 0); digitalWrite(IN4, r < 0);
  analogWrite(ENA, abs(l));
  analogWrite(ENB, abs(r));
}

void setup() {
  pinMode(SENSOR_L, INPUT);
  pinMode(SENSOR_R, INPUT);
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void loop() {
  bool l = digitalRead(SENSOR_L) == BLACK;
  bool r = digitalRead(SENSOR_R) == BLACK;

  if (l && r)  {  // both on balck: forward : change according to liking 
    drive(SPEED, SPEED);   
    lostAt = 0;
  }      
  else if (l && !r){      // now motor second motor doesnt stop 
  //it reverse so it move smoothly as before it was stopping which casusing the twitching 
  // calibrate accrodingly
    drive(-1, TURN);      // line on left: stop left wheel
    last = 1;
    lostAt = 0;
  }  
  else if (!l && r){
    drive(TURN, -1);      // line on right: stop right wheel
    last = -1;
    lostAt = 0;
  } 
  else{ // second loop which will act as automation solution to find the black line own its own 
    if (lostAt == 0) lostAt = millis();
    if (millis() -lostAt < SEARCH_MS){
      if(last<0)      drive(-1,TURN);
      else if(last>0) drive(TURN,-1);
      else            drive(0,0);
    }
    else{           
    drive(0, 0); //serched for too long so stoping        
    }
  }
  
}