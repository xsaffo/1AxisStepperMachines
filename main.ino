#define BTN_STOP 11 /*Pulsante di Stop*/
#define BTN_START 12 /*Pulsante di Start*/
#define SW_DIR 10 /*Deviatore di Direzione 1=SU, 0=GIU*/

#define AN_VELOCITA A0 /*Selettore (partitore resistivo) per velocita 0->5V*/


#define EN 8      /* Enable pin for all stepper outputs */

#define X_DIR 5   /* Direction pin for X axis */
#define X_STEP 2  /* Step pin for X axis */

#define Y_DIR 6   /* Direction pin for Y axis */
#define Y_STEP 3  /* Step pin for Y axis */

// Se impostiamp i microstep a 1/16 moltiplicare per 16
// Se impostiamp i microstep a 1/8 moltiplicare per 8
// Se impostiamp i microstep a 1/4 moltiplicare per 4
// Se impostiamp i microstep a 1/2 moltiplicare per 2 
const int numStepMotore = 200; //E' il numero di step per rotazione del motore (potrebbe differire in base al modello)

//const int debounceMs = 25;

const long velocita_millisec = 20; //Si tratta di millisecondi tra un impulso e l'altro sul pin STEP
const long velocita_microsec = 20000; //Si tratta di microsecondi tra un impulso e l'altro sul pin STEP 
const long velocita = 600; //Si tratta di microsecondi tra un impulso e l'altro sul pin STEP 

const long tPasso = 1000; //Microsecondi

long tDelay = 10; //Millisecondi
//const long tDelay = 1; //Millisecondi
//const long tDelay = 3; //Millisecondi
//const long tDelay = 10; //Millisecondi


bool val_btn_movimento = false;
int val_btn_start = -1;
int val_btn_stop = -1;
int val_sw_dir=-1;
int val_an_velocita=-1;

int s_dir = -1;
int s_start = -1;
int s_stop = -1;
int s_velocita = -1;

bool direzione = false;

void setup() {
  //inizializzo i PIN come OUTPUT
  pinMode(X_STEP, OUTPUT);
  pinMode(Y_STEP, OUTPUT);
  pinMode(X_DIR, OUTPUT);
  pinMode(Y_DIR, OUTPUT);
  pinMode(EN, OUTPUT);
  
  //Pulsantiera Controller
  pinMode(BTN_STOP, INPUT);
  pinMode(BTN_START, INPUT);
  pinMode(SW_DIR, INPUT);
  
  //Per Abilitare LOW
  digitalWrite(EN, LOW);
  
  //Inizializzo la porta seriale per comunicazione
  Serial.begin(9600);
  
  s_dir = val_sw_dir = digitalRead(SW_DIR);
  s_start = val_btn_start = digitalRead(BTN_START);
  s_stop = val_btn_stop = digitalRead(BTN_STOP);
  
  s_velocita = analogRead(AN_VELOCITA);
  val_an_velocita =  s_velocita;
  val_btn_movimento = false;
  direzione = !digitalRead(SW_DIR);
  
  //definiamo la direzione del motore
  digitalWrite(X_DIR, direzione);
  digitalWrite(Y_DIR, !direzione);


  Serial.print("VELOCITA = ");
  Serial.println(val_an_velocita);  
  
  tDelay = map(val_an_velocita, 0, 1023, 0, 30);
  Serial.print("VELOCITA_MAP = ");
  Serial.println(tDelay);
  
  Serial.print("DIR = ");
  Serial.println(val_sw_dir); 
}



int state_Start = HIGH;      // the current state of the output pin
int reading_Start;           // the current reading from the input pin
int previous_Start = LOW;    // the previous reading from the input pin



int state_Stop = LOW;      // the current state of the output pin
int reading_Stop;           // the current reading from the input pin
int previous_Stop = LOW;    // the previous reading from the input pin


// the follow variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long time_Start = 0;         // the last time the output pin was toggled
long time_Stop = 0;         // the last time the output pin was toggled
long debounce = 200;   // the debounce time, increase if the output flickers


void loop() {

  reading_Start = digitalRead(BTN_START);

  reading_Stop = digitalRead(BTN_STOP);

  
  for (int i = 0; i < 6; i++) 
  {
      s_velocita = analogRead(AN_VELOCITA);
      delayMicroseconds(10);
  }
  //s_velocita = analogRead(AN_VELOCITA);
 
  //Deviatore Direzione
  s_dir = digitalRead(SW_DIR);
  // if the input just went from LOW and HIGH and we've waited long enough
  // to ignore any noise on the circuit, toggle the output pin and remember
  // the time
  if (reading_Start == HIGH && previous_Start == LOW && millis() - time_Start > debounce) 
  {
    if (state_Start == HIGH)
    {
      state_Start = LOW;
      state_Stop = HIGH;
      Serial.print("Start");
      Serial.println(state_Start);
      val_btn_movimento = true;
      
  digitalWrite(EN, LOW);

    }
    time_Start = millis();
 }

  if (reading_Stop == HIGH && previous_Stop == LOW && millis() - time_Stop > debounce) 
  {
    if (state_Stop == HIGH)
    {
      state_Stop = LOW;
      state_Start = HIGH;
      Serial.print("Stop");
      Serial.println(state_Stop);
      val_btn_movimento = false;
      
  digitalWrite(EN, HIGH);

    }
    time_Stop = millis();
 }

       previous_Start = reading_Start;
       previous_Stop= reading_Stop;


if (s_dir != val_sw_dir)
{
    val_sw_dir = s_dir;
    Serial.print("DIR = ");
    Serial.println(val_sw_dir);  
    
  //definiamo la direzione del motore
  direzione = !val_sw_dir;
  digitalWrite(X_DIR, direzione);
  digitalWrite(Y_DIR, !direzione);
}

int tmp_vl = s_velocita - val_an_velocita;

//if(tmp_vl < 0)
//  tmp_vl = tmp_vl * -1;

if ((tmp_vl < -20) || (tmp_vl > 20)) {
 
   val_an_velocita = s_velocita;
    Serial.print("VELOCITA = ");
    Serial.println(val_an_velocita);  

    tDelay = map(val_an_velocita, 0, 1023, 0, 15);
    Serial.print("VELOCITA_MAP = ");
    Serial.println(tDelay); 
}

  if (val_btn_movimento)
  {
    Movimento();
  }
}

void Movimento() 
{
  digitalWrite(X_STEP, HIGH);
  digitalWrite(Y_STEP, HIGH);
  delayMicroseconds(tPasso);
  digitalWrite(X_STEP, LOW);
  digitalWrite(Y_STEP, LOW);
    //delayMicroseconds(tPasso);
      delay(tDelay);
}
