//codigo pantalla i2c FALTA
 /*
 #include <Wire.h> 
#include <LiquidCrystal_I2C.h>

//Crear el objeto lcd  dirección  0x3F y 16 columnas x 2 filas
LiquidCrystal_I2C lcd(0x3F,16,2);  //

void setup() {
  // Inicializar el LCD
  lcd.init();
  
  //Encender la luz de fondo.
  lcd.backlight();
  
  // Escribimos el Mensaje en el LCD.
  lcd.print("Hola Mundo");
}

void loop() {
   // Ubicamos el cursor en la primera posición(columna:0) de la segunda línea(fila:1)
  lcd.setCursor(0, 1);
   // Escribimos el número de segundos trascurridos
  lcd.print(millis()/1000);
  lcd.print(" Segundos");
}


*/



//AHT10 FALTA

/*
//Se declaran las librerías 
#include <Wire.h> // Librería para establecer comunicación I2C
#include <AHT10.h> // Librería para utilizar el sensor AHT10

AHT10 myAHT10(0x38);

void setup() {
Wire.begin(); // Función que inicializa la librería Wire
Serial.begin(9600); //Se inicia la comunicación serial 
Serial.println("AHT10"); // Se imprime el nombre de sensor
if (!myAHT10.begin()) { // Si la comunicación con el sensor falla se imprime el un mensaje de error 
Serial.println("Error no se el sensor!");
while (1);
}
}

void loop() {
float temp = myAHT10.readTemperature(); //Se lee la temperatura y se asigna "tem"
float hum = myAHT10.readHumidity(); //Se lee humedad y se asigna "hum" 
Serial.print("Temp: "); Serial.print(temp); Serial.print(" °C"); //Se imprime el valor de tempertura 
Serial.print("tt"); // Imprime dos pestañas para acomodar los valores de temperatura y humedad
Serial.print("Humidity: "); Serial.print(hum); Serial.println(" %"); //Se imprime el valor de humedad
delay(1000);
}
*/



// sensor magnetico FALTA



/*
//declaramos las variables
int Led=13;       //pin del led
int pin_digital = 7//pin de señal de sensor
;int  val;//variable para almacenar un valor
void  setup()
{
pinMode(Led,OUTPUT);//define LED como salida
pinMode(pin_digital = 7,INPUT);//define señal sensor como entrada
}
void  loop()

{ val=digitalRead(pin_digital = 7);//lee el valor de la entrada digital
if(val==HIGH)//Cuando el sensor detecta campo
{
Serial.println("ENCENDIDO");
}
else        
{
Serial.println("APAGADO");
}
}
*/


//LED guiño
/*

#define ledPIN  9
 
void setup() {
  Serial.begin(9600);    //iniciar puerto serie
  pinMode(ledPIN , OUTPUT);  //definir pin como salida
}
 
void loop(){
  digitalWrite(ledPIN , HIGH);   // poner el Pin en HIGH
  delay(1000);                   // esperar un segundo
  digitalWrite(ledPIN , LOW);    // poner el Pin en LOW
  delay(1000);                   // esperar un segundo
}


*/


// LED
/*


#define LED 22
void setup()
{
pinMode(LED,OUTPUT);
}
void loop()
{
digitalWrite(LED,HIGH);
delay(1000);
digitalWrite(LED,LOW);
delay(1000);
}


*/




//buzzer


 /*

#define buzzPin  18 // Connect Buzzer on Digital Pin2
void setup() 
{ 
pinMode(buzzPin, OUTPUT); 
} 
void loop() 
{ 
digitalWrite(buzzPin, HIGH); 
delayMicroseconds(50); 
digitalWrite(buzzPin, LOW); 
delayMicroseconds(50); 
}


*/




//corriente


/*

#include <Wire.h>
#include <Adafruit_INA219.h>
Adafruit_INA219 ina219;
 	float voltage_V = 0,shuntVoltage_mV,busVoltage_V;
 	float current_mA = 0;
 	float power_mW = 0;
 	float energy_Wh=0;
 	long time_s=0;
void setup(void)
{
 	Serial.begin(9600);
 	uint32_t currentFrequency;
 	ina219.begin();
 	Serial.println("Measuring voltage and current with INA219");
}
void loop(void)
{
 	getData();
 	delay(2000);
}
void getData(){
 	
 time_s=millis()/(1000); // convert time to sec
 busVoltage_V = ina219.getBusVoltage_V();
 shuntVoltage_mV = ina219.getShuntVoltage_mV();
 voltage_V = busVoltage_V + (shuntVoltage_mV / 1000);
 current_mA = ina219.getCurrent_mA();
 //power_mW = ina219.getPower_mW(); 
 power_mW=current_mA*voltage_V;
 energy_Wh=(power_mW*time_s)/3600; 		//energy in watt hour
 		
 	
 	Serial.print("Bus Voltage: 		"); Serial.print(busVoltage_V); Serial.println(" V");
 	Serial.print("Shunt Voltage: "); Serial.print(shuntVoltage_mV); Serial.println(" mV");
 	Serial.print("Load Voltage: 	"); Serial.print(voltage_V); Serial.println(" V");
 	Serial.print("Current: 						"); Serial.print(current_mA); Serial.println(" mA");
 	Serial.print("Power: 								"); Serial.print(power_mW); Serial.println(" mW"); 	
 	Serial.print("Energy: 							"); Serial.print(energy_Wh); Serial.println(" mWh");
 	Serial.println("----------------------------------------------------------------------------");
}
*/







 //LDR


/*
 //Código Divisor de tensión con LDR.

void setup(){
  pinMode(35,INPUT_PULLUP);                               
  Serial.begin(9600);               
} 

void loop(){
  int valorLDR = analogRead(35);                
     
  
      Serial.println(valorLDR);
   
  } 

  

*/

  //boton

 /*
  #define boton  2 // Botón asignado en el pin 2.

void setup() {
  // Vamos a usar el puerto serie para mostrar el estado del botón.
  Serial.begin(9600);
  // Ponemos el pin como una entrada, puesto que vamos a leer
  // un botón. Habilito la resistencia de PULLUP.
  pinMode(boton,INPUT_PULLUP);
}

void loop() {
  // Cuando la entrada se ponga a 0, el botón "debería" estar
  // pulsado.
  if ( digitalRead(boton)==LOW ) {
    Serial.println("Botón pulsado");
    delay(100);
      }
}

*/