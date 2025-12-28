
char input[30];
int idx = 0;

void setup() {
  Serial.begin(9600);    

  for (int i = 0; i < 14; i++) 
    pinMode(i, OUTPUT);
}

void loop() {
  if (Serial.available()) {
    char c = Serial.read();

    if (c == '\n') {  
      input[idx] = '\0';
      idx = 0;

      int led_start = 0, led_stop = 0, time_ms = 0;
      char program[10];

      if (strchr(input, '-')) {
        sscanf(input, "%d-%d,%[^,],%d", &led_start, &led_stop, program, &time_ms);
      } else {
          sscanf(input, "%d,%[^,],%d", &led_start, program, &time_ms);
          led_stop = led_start;
      }
    
      if (strcmp(program, "blink") == 0) {
        for (int i = 0; i < 5; i++) {  
          for (int j = led_start; j <= led_stop; j++) digitalWrite(8+j, HIGH);
            delay(time_ms);
          for (int j = led_start; j <= led_stop; j++) digitalWrite(8+j, LOW);
            delay(time_ms);
        }
      } else if(strcmp(program, "fadein") == 0)
      {
        for (int j = led_start; j <= led_stop; j++)
          for(int fadeValue = 0; fadeValue< 255; fadeValue +=5){
            analogWrite(8+j,fadeValue);
            delay(time_ms/255);
          }
      } else if(strcmp(program, "fadeout") == 0)
      {
        for (int j = led_start; j <= led_stop; j++)
          for(int fadeValue = 255; fadeValue >= 0; fadeValue -= 5){
            analogWrite(8+j,fadeValue);
            delay(time_ms/255);
          }
      }

    } else 
        input[idx++] = c;
  }
}