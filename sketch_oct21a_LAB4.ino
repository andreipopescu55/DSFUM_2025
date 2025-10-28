byte cifre[10]={
0b11000000,
0b11111001,
0b10100100,
0b10110000,
0b10011001,
0b10010010,
0b10000010,
0b11111000,
0b10000000,
0b10010000,
};
void setup ()
{
  pinMode(4,OUTPUT); // pinul pentru latch (blocare)
  pinMode(7,OUTPUT); // pin pentru sincronizarea ceasului
  pinMode(8,OUTPUT); // pin pentru transmiterea datelor
}
 
void loop()
{for(int i=0;i<10;i++){
   afiseazaValoare(cifre[i], 241); // scriem pe segmentul 241 (primul), valoarea 0.
  delay(1000);
}
 
}
 
void afiseazaValoare(byte valoare, byte segment)
{
  digitalWrite(4,LOW);
  shiftOut(8, 7, MSBFIRST, valoare);
  shiftOut(8, 7, MSBFIRST, segment);
  digitalWrite(4,HIGH);
}