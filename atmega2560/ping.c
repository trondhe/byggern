/*
 long duration, cm;

 // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
 // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
 
 // Set as output
 DDRC |= (1 << PC0);
 
 // Set low
 PORTC &=~(1 << PC0);
 _delay_us(2);
 
 // Set high
 PORTC |= (1 << PC0);
 _delay_us(5);
 
 // Set low
 PORTC &=~(1 << PC0);
 
 // Set as input
 DDRC &=~(1 << PC0);


 pinMode(pingPin, INPUT);
 duration = pulseIn(pingPin, HIGH);

 cm = microsecondsToCentimeters(duration);


 
 long microsecondsToCentimeters(long microseconds) {
	 // The speed of sound is 340 m/s or 29 microseconds per centimeter.
	 // The ping travels out and back, so to find the distance of the
	 // object we take half of the distance travelled.
	 return microseconds / 29 / 2;
 }
 */