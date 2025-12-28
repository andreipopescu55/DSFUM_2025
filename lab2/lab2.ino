
void setup() {
    pinMode(13, OUTPUT);
}

void loop() {
    for(int p = 0; p <= 255; p++)
    {
        int t = millis();

        while(millis() - t < 5)
        {
            if(millis() - t < p/12)
                digitalWrite(13, 1);
            else 
                digitalWrite(13, 0);
        }
    }

    for(int p = 255; p >= 0; p--)
    {
        int t = millis();
        while(millis() - t < 5)
        {
            if(millis() - t < p/12)
                digitalWrite(13, 1);
            else 
                digitalWrite(13, 0);
        }
    }
}