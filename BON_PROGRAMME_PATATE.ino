//****************************************************************************************
// Illutron take on Disney style capacitive touch sensor using only passives and Arduino
// Dzl 2012
//****************************************************************************************


//                              10n
// PIN 9 --[10k]-+-----10mH---+--||-- OBJECT
//               |            |
//              3.3k          |
//               |            V 1N4148 diode
//              GND           |
//                            |
//Analog 0 ---+------+--------+
//            |      |
//          100pf   1MOmhm
//            |      |
//           GND    GND



#define SET(x,y) (x |=(1<<y))				//-Bit set/clear macros
#define CLR(x,y) (x &= (~(1<<y)))       		// |
#define CHK(x,y) (x & (1<<y))           		// |
#define TOG(x,y) (x^=(1<<y))            		//-+



#define N 160  //How many frequencies

float results[N];            //-Filtered result buffer
float freq[N];            //-Filtered result buffer
int sizeOfArray = N;
int pinLed1, pinLed2, pinLed3, pinLed4;


 
   
   

void setup()
{
     pinLed1 = 3;
     pinLed2 = 4;
     pinLed3 = 5;
     pinLed4 = 6;
     pinMode(pinLed1, OUTPUT);
     pinMode(pinLed2, OUTPUT);
     pinMode(pinLed3, OUTPUT);
     pinMode(pinLed4, OUTPUT);
       digitalWrite(pinLed1, LOW);
       digitalWrite(pinLed2, LOW);
       digitalWrite(pinLed3, LOW);
       digitalWrite(pinLed4, LOW);
       
  TCCR1A=0b10000010;        //-Set up frequency generator
  TCCR1B=0b00011001;        //-+
  ICR1=110;
  OCR1A=55;

  pinMode(9,OUTPUT);        //-Signal generator pin
  pinMode(8,OUTPUT);        //-Sync (test) pin

  Serial.begin(115200);

  for(int i=0;i<N;i++)      //-Preset results
    results[i]=0;         //-+
}

void loop()
{
  unsigned int d;
  int vMIN, dMAX;
  vMIN = 0;
  dMAX = 0;

  int counter = 0;
  for(unsigned int d=0;d<N;d++)
  {
    int v=analogRead(0);    //-Read response signal
    CLR(TCCR1B,0);          //-Stop generator
    TCNT1=0;                //-Reload new frequency
    ICR1=d;                 // |
    OCR1A=d/2;              //-+
    SET(TCCR1B,0);          //-Restart generator

    results[d]=results[d]*0.5+(float)(v)*0.5; //Filter results
   
    freq[d] = d;

   

   

 //   plot(v,0);              //-Display
 //   plot(results[d],1);
  // delayMicroseconds(1);  
  if (v > vMIN)
    {
      dMAX = d;
     vMIN = v;
     digitalWrite(pinLed1, LOW);
       digitalWrite(pinLed2, LOW);
       digitalWrite(pinLed3, LOW);
       digitalWrite(pinLed4, LOW);  
  
 if ( dMAX< 60)
    {
      digitalWrite(pinLed1, HIGH);
  digitalWrite(pinLed2, LOW);
  digitalWrite(pinLed3, LOW);
  digitalWrite(pinLed4, LOW);
 
    }
 
     if ((60< dMAX) &&( dMAX < 75))
     {
      digitalWrite(pinLed1, LOW);
  digitalWrite(pinLed2, HIGH);
  digitalWrite(pinLed3, LOW);
  digitalWrite(pinLed4, LOW);
     }
  

     if ((75< dMAX)&& ( dMAX < 84))
     {
      digitalWrite(pinLed1, LOW);
  digitalWrite(pinLed2, LOW);
  digitalWrite(pinLed3, HIGH);
  digitalWrite(pinLed4, LOW);
     }
  
   
   if (84< dMAX)
   {
    digitalWrite(pinLed1, LOW);
  digitalWrite(pinLed2, LOW);
  digitalWrite(pinLed3, LOW);
  digitalWrite(pinLed4, HIGH);
   }  
   }
  }
 
  

PlottArray(1,freq,results); 
 

  TOG(PORTB,0);            //-Toggle pin 8 after each sweep (good for scope)
}
   
