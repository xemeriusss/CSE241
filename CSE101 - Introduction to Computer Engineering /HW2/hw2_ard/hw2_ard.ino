int counter=0;                     
int buttonState;                

const int buttonPin = 2;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  pinMode(7,INPUT);
}

void loop() {
  if(Serial.available() > 0){

      char selection = Serial.read()-48; // char to int
      if(selection == 1)
          digitalWrite(ledPin,HIGH);

      else if(selection == 2)
          digitalWrite(ledPin,LOW);

      else if(selection == 3){
          int i=0;
          digitalWrite(ledPin,LOW);
          delay(750);

          for(i=0 ; i<3 ; i++){
             digitalWrite(ledPin,HIGH);
             delay(750);
             digitalWrite(ledPin,LOW);
             delay(750);
          }
      }

      else if(selection == 4){
          Serial.setTimeout(8000);

          int num = Serial.parseInt();
          int square = num*num;

          Serial.println(square);
          delay(3000);   
      }
    
    else if(selection == 5){
      counter = 0;

      while(1){
          buttonState = digitalRead(buttonPin);

          if (buttonState == 1){
            digitalWrite(ledPin,HIGH);
            delay(150);
            counter++;
            delay(300);
            Serial.println(counter);
          } else {
              digitalWrite(ledPin,LOW);
          }
      }
    } 
  }
}
    

