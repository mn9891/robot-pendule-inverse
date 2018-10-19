#define START_CMD_CHAR '>'
#define END_CMD_CHAR '\n'
#define DIV_CMD_CHAR ','
#define value0_max 10 //Valeur max du capteur
#define v_in 9 //Valeur max du voltage (alimentation)
#define v_min 1.6 //Valeur min en volt (début rotation du moteur)
//*********marge d'angles
#define theta_min 88
#define theta_max2 93
// constantes du modéle LQR (calculé par matlab)
#define K1 16
#define K2 88
#define K3 27
#define K4 13
#define x_max 0.2        //on doit l'augmenter pour qu'il donne le maximum de puissance dans une faible distance
#define x_dot_max 0.1   //de même pour xpoint
#define theta_max 0.1    
#define theta_dot_max 10 //de même pour theha point
// *********** constantes du robot ************
#define delta_t 0.02 //période d'échantillonnage
//******constante de regulage
#define prop 0.001;//constante de propostionallité entre la vitesse et la commande


volatile int v_min255; //Valeur min ds l'échelle 255
volatile float theta_dot;//varie entre -10 et 10
volatile float theta;//angle avec le verticale
volatile int theta_int;
volatile float x=0;//position relative
volatile float x_dot;//vitesse rectiligne
volatile float cmd;
volatile int cmd_M;
volatile float power_max=K1*x_max+K2*theta_max+K3*x_dot_max+K4*theta_dot_max;
//float power_max=K2*theta_max+K4*theta_dot_max;
volatile float value0,value1,power=0,angle;
volatile float tab_theta_dot[10];
volatile int i,countS;

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial1.flush();
  pinMode(2, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  for( i = 0 ; i < 10 ; i++ ) { 
    tab_theta_dot[i] = 0; 
  }
  v_min255 = 255 - 255*(v_in - v_min)/v_in;
  while(!Serial1){ //required for the Leonardo
    ;
  }
}

void loop(){
  Serial1.flush();
  Serial.flush();
  int sensorType = 0;
  unsigned long logCount = 0L;
  char getChar = ' '; //read serial
  getChar = Serial1.read();
  if (getChar != START_CMD_CHAR) return; // if no command start flag, return to loop().
  // parse incoming pin# and value
  sensorType = Serial1.parseInt(); // read sensor typr
  logCount = Serial1.parseInt(); // read total logged sensor readings
  value0 = Serial1.parseFloat(); // 1st sensor value
  value1 = Serial1.parseFloat(); // 2nd sensor value
  if (sensorType == 4) {
    theta_dot = value0;//vitesse angulaire en rad/s
     }
  if (sensorType == 3) {
    theta = -(value1+90);
    theta = theta*PI/180;    
  }
tab_theta_dot[0]=theta_dot;
  for( i = 0 ; i < 10 ; i++ ) {
    if( abs( tab_theta_dot[i] ) < 0.02 ) { 
      countS++; 
    }
  }
  if( countS > 9 ) {
    x_dot = 0;
    x = 0;
    theta_dot = 0;
    theta = 0;
  }  //70
  for( i = 9 ; i > 0 ; i-- ) { 
    tab_theta_dot[ i ] = tab_theta_dot[ i-1 ]; 
  }






  power = K1*x+K2*theta+K3*x_dot+K4*theta_dot;
   Serial.println("===================== ");
   Serial.print("theta_dot ");
   Serial.println(theta_dot);
   Serial.println("theta ");
    Serial.println(theta);
    Serial.print("power ");
    Serial.println(power);
  //power = K2*theta+K4*theta_dot;
   cmd  =  max( min( power , 255 ) , -255 );
  cmd_M = map((int)(round(cmd)),-30,3 ,-255,255);
  x_dot = x_dot+cmd; //proportionelle a la commande
  x=x+x_dot;
  if(cmd>0){
    digitalWrite(2, HIGH); 
    analogWrite(6 , LOW);
    analogWrite(5, min(255,cmd_M+45));
    digitalWrite(10, LOW);
    analogWrite(9, min(255,45+cmd_M));
//    Serial.print("cmd_m");
//    Serial.println(cmd_M);

   

  }
  else {
    digitalWrite(2, HIGH);
    digitalWrite(5, LOW);
    analogWrite(6, min(255,-cmd_M+40));
    digitalWrite(9, LOW);
    analogWrite(10, min(255,-cmd_M+40));
//    Serial.print("cmd_m");
//    Serial.println(cmd_M);

  }

}





