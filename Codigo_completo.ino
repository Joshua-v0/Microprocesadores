//Set the LED light to pin 8
int LED = 8;
int LED2 = 10;

volatile int frecuencia_flujo;       // Cuenta los pulsos del sensor de flujo
unsigned int litros_por_hora;        // Almacena el cálculo de litros por hora
float volumen_total = 0.0;           // Acumula el volumen total en litros
float volumen_deseado = 4;
const byte pin_sensor_flujo = 2;     // Pin digital donde se conecta el sensor de flujo
unsigned long tiempo_actual;         // Tiempo actual en milisegundos
unsigned long tiempo_anterior;       // Tiempo de la última medición
 char bomba_activada;
//Set the float sensor to pin 3
#define Float_Switch 3
#define Float_Switch2 4
void contar_pulso() {
  frecuencia_flujo++;
}
void setup() {
  // initialize digital pin 8 as an output.
  flowMeterSetup();
  switchSetup();
}

void loop() {
 floatSwitch();
 flowMeter();
}


void switchSetup(){
  pinMode(LED, OUTPUT);
  pinMode(Float_Switch, INPUT_PULLUP);
  pinMode(LED2, OUTPUT);
  pinMode(Float_Switch2, INPUT_PULLUP);
}
void flowMeterSetup(){
  pinMode(pin_sensor_flujo, INPUT);
  digitalWrite(pin_sensor_flujo, HIGH); // Activa resistencia pull-up interna (opcional)

  Serial.begin(9600); // Inicializa la comunicación serial

  // Configura la interrupción externa 0 (pin digital 2 en la mayoría de los Arduinos)
  attachInterrupt(digitalPinToInterrupt(pin_sensor_flujo), contar_pulso, RISING);

  // Habilita interrupciones globales
  sei();

  tiempo_actual = millis();
  tiempo_anterior = tiempo_actual;
}
void flowMeter(){
  tiempo_actual = millis();

  // Ejecutar cada 1000 milisegundos (1 segundo)
  if (tiempo_actual >= (tiempo_anterior + 1000)) {
    tiempo_anterior = tiempo_actual;

    // Fórmula: Frecuencia de pulsos (Hz) = 7.5 * Q, donde Q es el caudal en L/min
    // Caudal (L/h) = (frecuencia * 60) / 7.5
    litros_por_hora = (frecuencia_flujo * 60) / 7.5;

    // Convertimos el flujo a litros por segundo y lo sumamos al volumen total
    float litros_por_segundo = (float)litros_por_hora / 3600.0;
    volumen_total += litros_por_segundo;

    if(volumen_total >= volumen_deseado){
      bomba_activada = 'N';
      delay(5000);
      //Sensor de humedad
      
    }
    else{
      bomba_activada = 'S';
    }

    // Reiniciar el contador de pulsos para el siguiente segundo
    frecuencia_flujo = 0;

    // Mostrar resultados en el monitor serial
    Serial.print("Caudal: ");
    Serial.print(litros_por_hora);
    Serial.print(" L/h\t");

    Serial.print("Volumen total: ");
    Serial.print(volumen_total, 2); // Mostrar 2 decimales
    Serial.println(" L");
    Serial.print("Señal de activacion de la bomba ");
    Serial.println(bomba_activada);
  }
}
void floatSwitch(){

 if(digitalRead(Float_Switch) == HIGH)
  {
    digitalWrite(LED, HIGH); //Turn LED on
  }

 else
  {
    digitalWrite(LED, LOW); //Turn LED off
  }

  /*--------*/

  if(digitalRead(Float_Switch2) == HIGH)
  {
    digitalWrite(LED2, HIGH); //Turn LED on
  }

 else
  {
    digitalWrite(LED2, LOW); //Turn LED off
  }

}