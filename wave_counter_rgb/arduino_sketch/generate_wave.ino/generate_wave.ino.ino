#define FREQUENCY 250
unsigned long now = 0;
int pulse_time = (1000000 / FREQUENCY) / 2; //pulse time in microseconds

void setup() 
{
  pinMode(PD2, OUTPUT);
}

void loop() 
{
  //on
  now = micros();
  while( (micros() - now) < pulse_time);
  digitalWrite(PD2, 1);

  //off
  now = micros();
  while( (micros() - now) < pulse_time);
  digitalWrite(PD2, 0);
}
