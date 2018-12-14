/** El siguiente codigo fue diseñado para controlar el proyecto "Estacionamiento Inteligente".
 *  Se analizan distintas variables de medio ambiente para modificar otras que seran visualizadas por el usuario final.
 *  Este codigo  analiza los resultados de 10 fotoresistencias y 3 fotodiodos para modificar las siguientes variables de medio ambiente:
 *  -Presencia de auto para cada uno de los cajones del estacionamiento, con leds (rojo:ocupado,verde:disponible).
 *  -Posicion del brazo para entrada y el brazo para salida (abajo impide la entrada al estacionamiento, arriba es lo contrario).
 *  -Iluminacion del estacionamiento (Se enciende el foco a falta de luz, se apaga si hay luz).
 *  -Cantidad de espacios disponibles en el estacionamiento (Se nyestra en un display).
 */
#include <Servo.h>//se importa la libreria para utilizar los servomotores

//variables que se usaran para encender o apagar los leds de los semaforos
//se igualan las variables a los pines del arduino que se quieren utilizar
const int R8 = 52;   
const int V8 = 53;

const int R7 = 50;   
const int V7 = 51;

const int R6 = 48;   
const int V6 = 49;

const int R5 = 46;   
const int V5 = 47;

const int R4 = 44;   
const int V4 = 45;

const int R3 = 42;   
const int V3 = 43;

const int R2 = 40;   
const int V2 = 41;

const int R1 = 38;   
const int V1 = 39;

const int R0 = 36;   
const int V0 = 37;

//entradas digitales que se usaran para evaluar el estado de los semaforos
const int F8 = 12;  //the number of the LDR pin
const int F7 = 11;  //the number of the LDR pin
const int F6 = 10;  //the number of the LDR pin
const int F5 = 9;  //the number of the LDR pin
const int F4 = 8;  //the number of the LDR pin
const int F3 = 7;  //the number of the LDR pin
const int F2 = 6;  //the number of the LDR pin
const int F1 = 5;  //the number of the LDR pin
const int F0 = 4;  //the number of the LDR pin


//variables para el servomotor entrada
Servo myservo;  // crea el objeto servo
int pos = 0;   // posicion del servo
int pin = 3;   //bit de entrada(0 o 1)
int confirma1=A0; //segundo bit de entrada (0 o 1) para bajar la barra cuando el auto haya pasado por completo


//variables para el servomotor salida
Servo myservo2;  // crea el objeto servo
int pos2 = 0;   // posicion del servo
int pin2 = 22;   //bit de entrada(0 o 1)

//variables para la iluminacion con foco
const int IluminacionINPUT=A1;//pin de entrada, bit digital para la iluminacion
const int IluminacionOUTPUT=32;//pin de salida, bit digital para la iluminacion
const int LEDS_OUTPUT=31;//pin de salida,bit digital para la iluminacion con leds

//Variables para el contador con display
//Se declaran los pines de acuerdo a las letras del diagrama del display
int a = 24;
int b = 25;
int c = 26;
int d = 27;
int e = 28;
int f = 29;
int g = 30;
int contador;//variable para saber el numero de cajones disponibles

int input;//experimento con java

void setup() {
  /**
   *En el metodo void setup() se ajustan y se preparan las funciones
   *que llevara a cabo el microcontrolador
   */

  Serial.begin(9600);//Abre el puerto serie y fija la velocidad en baudios para la transmisión de datos en serie

  //se inicializan los pines que se utilizaran para encender los leds de los semaforos
  pinMode(R8, OUTPUT); 
  pinMode(V8, OUTPUT);  

  pinMode(R7, OUTPUT);  
  pinMode(V7, OUTPUT); 

  pinMode(R6, OUTPUT);  
  pinMode(V6, OUTPUT); 

  pinMode(R5, OUTPUT);  
  pinMode(V5, OUTPUT);  

  pinMode(R4, OUTPUT);  
  pinMode(V4, OUTPUT);  

  pinMode(R3, OUTPUT);  
  pinMode(V3, OUTPUT);  

  pinMode(R2, OUTPUT);  
  pinMode(V2, OUTPUT);  

  pinMode(R1, OUTPUT);  
  pinMode(V1, OUTPUT);  

  pinMode(R0, OUTPUT);  
  pinMode(V0, OUTPUT);  

  //se inicializan las entradas digitales para las fotoresistencias
  pinMode(F8, INPUT);   
  pinMode(F7, INPUT);   
  pinMode(F6, INPUT);   
  pinMode(F5, INPUT);   
  pinMode(F4, INPUT);   
  pinMode(F3, INPUT);   
  pinMode(F2, INPUT);   
  pinMode(F1, INPUT);  
  pinMode(F0, INPUT);      

  //inicializacion del servomotor de entrada
   myservo.attach(2);    // vincula el servo al pin digital (salida al servo)   
   pinMode(pin, INPUT);  //definir pin como entrada (3)
   pinMode(confirma1, INPUT);  //definir pin como entrada (A0)
   myservo.write(160);     //posicion inicial del servo(160 grados)        
   delay(10);

   //inicializacion del servomotor de salida
   myservo2.attach(23);    // vincula el servo al pin digital (salida al servo)   
   pinMode(pin2, INPUT);  //definir pin como entrada (22)
   myservo2.write(165);     //posicion inicial del servo(165 grados)        
   delay(10);

   //definimos las variables para la iluminacion
   pinMode(IluminacionINPUT,INPUT);
   pinMode(IluminacionOUTPUT,OUTPUT);
   pinMode(LEDS_OUTPUT,OUTPUT);

   //mostramos nueve espacios al comenzar el programa
   contador=9;//el numero de cajones disponibles al iniciar el programa
   actualizaContador(contador);

   //se definen los pines de salida que controlaran el display del contador
   pinMode(a, OUTPUT);
   pinMode(b, OUTPUT);
   pinMode(c, OUTPUT);
   pinMode(d, OUTPUT);
   pinMode(e, OUTPUT);
   pinMode(f, OUTPUT);
   pinMode(g, OUTPUT); 

   pinMode(13, OUTPUT); // Experimento con java
}



void loop() {
/**
 * La función loop en Arduino es la que se ejecuta un número infinito de veces. 
 * Al encenderse el Arduino se ejecuta el código del setup y luego se entra al loop, 
 * el cual se repite de forma indefinida hasta que se apague o se reinicie el microcontrolador.
 */


//control del servomotor de entrada:
int value=digitalRead(pin);  //se guarda el valor logico del sensor para la entrada al estacionamiento
int valueConfirma1=digitalRead(confirma1);  //valor logico del segundo sensor para la entrada al estacionamiento
int grados=myservo.read();//se identifica en que angulo de giro esta el servomotor de entrada



/**
 * la puerta de la entrada solo se abrira si hay presencia de un carro
 * y si el contador es mayor o igual a 1
 */

if(contador>=1){ 
    if(grados>=160){ //si la barra si esta abajo
        if(value==HIGH){  //si hay prescencia de carro
          levanta_barra();     
        }
     }
     else if(grados<=70){//si la barra esta arriba
       if(valueConfirma1==HIGH){ //si el carro paso por el segundo sensor de la entrada
        delay(700);
        baja_barra();
        contador--;
        actualizaContador(contador);
       }
     }    
} 
  




//control del servomotor de salida:
int value2=digitalRead(pin2);  //valor logico del sensor de salida del estacionamiento
int grados2=myservo2.read();   //se identifica en que angulo de giro esta el servomotor de salida


if(contador<=9){ 
    if(grados2>=120){ //si la barra si esta abajo
        if(value2==HIGH){  //si hay prescencia de carro       
          levanta_barra2();     
        }
     }
     else if(grados2<=85){//si la barra esta arriba
       if(value2==LOW){ //si no hay prescencia de carro
        delay(6000);
        baja_barra2();
        if(contador<9){contador++;}
         actualizaContador(contador);
       }
     }    
} 

  /**
   * variables para evaluar el estado de los semaforos
   * estas recibirán el bit digital (1 o 0) que arrojan las
   * fotoresistencias de los semaforos
   */

  int  ValueF8= digitalRead(F8 );  //lectura digital de pin
  int  ValueF7= digitalRead(F7);  //lectura digital de pin
  int  ValueF6= digitalRead(F6);  //lectura digital de pin
  int  ValueF5= digitalRead(F5);  //lectura digital de pin
  int  ValueF4= digitalRead(F4);  //lectura digital de pin
  int  ValueF3= digitalRead(F3);  //lectura digital de pin
  int  ValueF2= digitalRead(F2);  //lectura digital de pin
  int  ValueF1= digitalRead(F1);  //lectura digital de pin
  int  ValueF0= digitalRead(F0);  //lectura digital de pin


  //Se evalua el estado de cada uno de los semaforos
  Semaforo(ValueF8,R8,V8);
  Semaforo(ValueF7,R7,V7);
  Semaforo(ValueF6,R6,V6);
  Semaforo(ValueF5,R5,V5);
  Semaforo(ValueF4,R4,V4);
  Semaforo(ValueF3,R3,V3);
  Semaforo(ValueF2,R2,V2);
  Semaforo(ValueF1,R1,V1);
  Semaforo(ValueF0,R0,V0);

  /*int umbral=digitalRead(IluminacionINPUT);
  if(umbral==HIGH){
    digitalWrite(IluminacionOUTPUT,HIGH);
    digitalWrite(LEDS_OUTPUT,LOW);
  }else{
    digitalWrite(IluminacionOUTPUT,LOW);
    digitalWrite(LEDS_OUTPUT,HIGH);   
  }*/


  //Control de la iluminacion del estacionamiento
  int umbral=analogRead(IluminacionINPUT);
  int luz = (5.0 * umbral * 100.0)/1024.0;
  Serial.println(luz);
  if(luz<=200){
    digitalWrite(IluminacionOUTPUT,HIGH);
    digitalWrite(LEDS_OUTPUT,LOW);
    delay(100);
  }else if(luz>=330){
    digitalWrite(IluminacionOUTPUT,LOW);
    digitalWrite(LEDS_OUTPUT,HIGH); 
    delay(100);   
  }



   //experimento con java
   if (Serial.available() > 0) {
    input = Serial.read();
    if (input == '1'){
      digitalWrite(13, HIGH); //Si el valor de input es 1, se enciende el LED
    }
    else
    {
      digitalWrite(13, LOW); //Si el valor de input es diferente de 1, se apaga el LED
    }
  }
  
  
}



//Metodos necesarios para el correcto funcionamiento del programa:


void Semaforo(int BIT,const int ROJO,const int VERDE){
 /**
 * Metodo para evaluar el estado de un semaforo,+
 * si hay prescencia de un carro en el cajon de estacionamiento
 * se encenderá el led rojo y se apagara el led verde. De lo contrario
 * se encenderá el led verde y se apagara el led rojo.
 */
  if (BIT==HIGH) {  
    digitalWrite(ROJO, HIGH);               
    digitalWrite(VERDE, LOW);               
   }
  else {
    digitalWrite(ROJO, LOW);          
    digitalWrite(VERDE, HIGH);          
  }  
}


//servomotor de entrada
void levanta_barra(){
  /**
   * Este metodo levanta la barra de la entrada del estacionamiento
   * con retraso de 5 milisegundos.
   */
   for (pos = 160; pos >=70; pos -= 1) 
   {
      myservo.write(pos);              
      delay(5);                       
   }
  }

   void  baja_barra(){
    /**
   * Este metodo baja la barra de la entrada del estacionamiento
   * con retraso de 5 milisegundos.
   */
   for (pos = 70; pos <= 160; pos += 1) 
   {
      myservo.write(pos);              
      delay(5);                       
   }
  }


//servomotor de salida
  void levanta_barra2(){
   /**
   * Este metodo levanta la barra de la salida del estacionamiento
   * con retraso de 8 milisegundos.
   */
   for (pos = 167; pos >=85; pos -= 1) 
   {
      myservo2.write(pos);              
      delay(8);                       
   }
  }

   void  baja_barra2(){
    /**
   * Este metodo baja la barra de la salida del estacionamiento
   * con retraso de 8 milisegundos.
   */
   for (pos = 85; pos <= 167; pos += 1) 
   {
      myservo2.write(pos);              
      delay(8);                       
   }
  }

   
   void actualizaContador(int Conta){
   /**
   * Este metodo recibe el valor actual del contador para
   * mostrarlo en el display
   */
    if(Conta==9){nueve();}
    else if(Conta==8){ocho();}
    else if(Conta==7){siete();}
    else if(Conta==6){seis();}
    else if(Conta==5){cinco();}
    else if(Conta==4){cuatro();}
    else if(Conta==3){tres();}
    else if(Conta==2){dos();}
    else if(Conta==1){uno();}
    else if(Conta==0){cero();}
   }

   //metodos necesarios para mostrar lel conteo del estacionamiento en el display
   void cero(){
    digitalWrite(a,LOW);
    digitalWrite(b,LOW);
    digitalWrite(c,LOW);
    digitalWrite(d,LOW);
    digitalWrite(e,LOW);
    digitalWrite(f,LOW);
    digitalWrite(g,HIGH); 
   } 

   void uno(){
    digitalWrite(a,HIGH);
    digitalWrite(b,LOW);
    digitalWrite(c,LOW);
    digitalWrite(d,HIGH);
    digitalWrite(e,HIGH);
    digitalWrite(f,HIGH);
    digitalWrite(g,HIGH); 
   }

   void dos(){
    digitalWrite(a,LOW);
    digitalWrite(b,LOW);
    digitalWrite(c,HIGH);
    digitalWrite(d,LOW);
    digitalWrite(e,LOW);
    digitalWrite(f,HIGH);
    digitalWrite(g,LOW);
   }

   void tres(){
    digitalWrite(a,LOW);
    digitalWrite(b,LOW);
    digitalWrite(c,LOW);
    digitalWrite(d,LOW);
    digitalWrite(e,HIGH);
    digitalWrite(f,HIGH);
    digitalWrite(g,LOW);
   }


   void cuatro(){
    digitalWrite(a,HIGH);
    digitalWrite(b,LOW);
    digitalWrite(c,LOW);
    digitalWrite(d,HIGH);
    digitalWrite(e,HIGH);
    digitalWrite(f,LOW);
    digitalWrite(g,LOW);
   }

   void cinco(){
    digitalWrite(a,LOW);
    digitalWrite(b,HIGH);
    digitalWrite(c,LOW);
    digitalWrite(d,LOW);
    digitalWrite(e,HIGH);
    digitalWrite(f,LOW);
    digitalWrite(g,LOW);
   }

   void seis(){
    //Seis
    digitalWrite(a,LOW);
    digitalWrite(b,HIGH);
    digitalWrite(c,LOW);
    digitalWrite(d,LOW);
    digitalWrite(e,LOW);
    digitalWrite(f,LOW);
    digitalWrite(g,LOW);
   }

   void siete(){
    digitalWrite(a,LOW);
    digitalWrite(b,LOW);
    digitalWrite(c,LOW);
    digitalWrite(d,HIGH);
    digitalWrite(e,HIGH);
    digitalWrite(f,HIGH);
    digitalWrite(g,HIGH);
   }

   void ocho(){
    //Ocho
    digitalWrite(a,LOW);
    digitalWrite(b,LOW);
    digitalWrite(c,LOW);
    digitalWrite(d,LOW);
    digitalWrite(e,LOW);
    digitalWrite(f,LOW);
    digitalWrite(g,LOW);
   }

   void nueve(){
    digitalWrite(a,LOW);
    digitalWrite(b,LOW);
    digitalWrite(c,LOW);
    digitalWrite(d,HIGH);
    digitalWrite(e,HIGH);
    digitalWrite(f,LOW);
    digitalWrite(g,LOW);
   }



