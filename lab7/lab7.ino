void setup()
{
    for(int i=8; i<=13; i++)
        pinMode(i,OUTPUT);
}

void first_animation()
{
    for(int i=11; i<=13; i++)
        digitalWrite(i,HIGH);

      delay(500);

      digitalWrite(13,LOW);
      digitalWrite(11,LOW);
      delay(500);
      digitalWrite(12,LOW);
      delay(1000);

       for(int i=11; i<=13; i++)
        digitalWrite(i,HIGH);
}

void second_animation(long now)
{
     for(int fadeOut = 255; fadeOut>=0; fadeOut-=5)
     {
        analogWrite(10,fadeOut);
        analogWrite(9,fadeOut);
        delay(4000/(255/5));
     }
}

void third_animation()
{
     for(int fadeIn = 0; fadeIn<=255; fadeIn+=5)
     {
        analogWrite(8,fadeIn);
        delay(50/(255/5));
     }

     for(int fadeOut = 255; fadeOut>=0; fadeOut-=5)
     {
        analogWrite(8,fadeOut);
        delay(50/(255/5));
     }
}

void loop()
{

} 