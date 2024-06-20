const int crucePorCeroPin = 2;  
const int salidaPin = 3;         

void setup() {
  pinMode(crucePorCeroPin, INPUT);  
  pinMode(salidaPin, OUTPUT);        
}

void loop() {
  while(digitalRead(crucePorCeroPin) == LOW) {
    delayMicroseconds(10); 
  }
  
  delayMicroseconds(2586);
  
  digitalWrite(salidaPin, HIGH);
  delayMicroseconds(10);  
  digitalWrite(salidaPin, LOW);
}
