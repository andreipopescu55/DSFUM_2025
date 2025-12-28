
void setup() {
  // put your setup code here, to run once:
    for(int i=8; i<=13; i++)
      pinMode(i, OUTPUT);
}

void first_animation(){
    int index = 0,reverse = 0;
    for (int i = 8; i <= 13; i++) 
      digitalWrite(i, LOW);

    if(!reverse)
    {
        digitalWrite(8 + index, HIGH);
        digitalWrite(13 - index , HIGH);

        if(index == 2)
           reverse = 1;
        else
          ++index;
    } else if(reverse)
    {
        digitalWrite(8 + index, HIGH);
        digitalWrite(13 - index , HIGH);

        if(index == 0)
           reverse = 0;
        else
          --index;
    }
}

void second_animation()
{
    int index = 0,reverse = 0;
    for(int i = 8; i <= 11; i++)
        digitalWrite(i, LOW);

    if(!reverse){
      digitalWrite(8 + index,HIGH);

      if(index == 3){
        index = 0;
        reverse = 1;
      } else ++index;
    } else if(reverse)
      {
        digitalWrite(11 - index, HIGH);

        if(index == 3){
          index = 0;
          reverse = 0;
        } else ++index;
      }
}

void third_animation()
{
  for(int i = 8; i <= 13; i++)
      digitalWrite(i, LOW);

  for(int i = 8; i <= 13; i++)
      digitalWrite(i,millis() / 200 & 1);
}

void loop() {
  // put your main code here, to run repeatedly:
    first_animation();
    delay(1000);
    second_animation();
    delay(1000);
    third_animation();
    delay(1000);
}