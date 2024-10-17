/*
  Comedero para mascotas, by ISET 57, 2024.

  Primera version:
  Tiene un timer que activa el motor cada ciertas horas.
  La idea es que la mascota pueda comer digamos a las 12 hs y luego a las 21 hs.
  Seria un primer evento en el minuto 0 y luego en el minuto 540.
  Luego espera durante 900 minutos para pasar al minuto 0, y comienza de nuevo el ciclo.

  - Usa un motor de pasos para mover el mecanismo.
  - Se puede usar un ESP8266, un ESP32, o un Arduino UNO/Nano.
  - Tiene un boton de reset, para marcar el comienzo del minuto 0.
  - Se enciende un LED parpadeante para indicar que está vivo.
  - Cuando funciona el motor, el LED permanece encendido.

  - Podrian agregarse mas LEDS o displays.
  - Tambien podría ser un parlante para generar sonido para acostumbrar a la mascota a comer.


  El motor usa un POLOLU A4988 como driver.
  https://www.pololu.com/docs/0J40/0J40.pdf

*/
#include <Arduino.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

// bobinas
int motorA = 1;
int motorB = 2;
int motorC = 3;
int motorD = 4;

int Minutos             = 0;
int TiempoPara2daComida = 540;   // desde las 12hs a las 21 hs = 540 minutos
int Tiempo1Dia          = 1440;  // 1 dia = 1440 minutos
int TiempoMotorON       = 10;    // en segundos

// gira el motor durante unos segundos, el tiempo va a depender de la cantidad de alimento necesario.
void EncenderMotor()
{
    int cont = 0;
    // motor enable
    unsigned long T = millis() + TiempoMotorON * 1000;
    while (T > millis())
    {
        // mover el motor unos pasos...y espero que se cumpla el tiempo de fin.
        digitalWrite(motorA, 1);
        digitalWrite(motorB, 0);
        digitalWrite(motorC, 0);
        digitalWrite(motorD, 0);
        delay(222);
        digitalWrite(motorA, 0);
        digitalWrite(motorB, 1);
        digitalWrite(motorC, 0);
        digitalWrite(motorD, 0);
        delay(222);
        digitalWrite(motorA, 0);
        digitalWrite(motorB, 0);
        digitalWrite(motorC, 1);
        digitalWrite(motorD, 0);
        delay(222);
        digitalWrite(motorA, 0);
        digitalWrite(motorB, 0);
        digitalWrite(motorC, 0);
        digitalWrite(motorD, 1);
        delay(222);
        cont++;
        if (cont > 100)
        {
            break;
        }
        pinMode(LED_BUILTIN, cont % 2);
    }
    // motor disable
    digitalWrite(motorA, 0);
    digitalWrite(motorB, 0);
    digitalWrite(motorC, 0);
    digitalWrite(motorD, 0);
}

void setup()
{
    Serial.begin(115200);
    Serial.println("Inicio del control de comida para  mascotas");

    pinMode(LED_BUILTIN, OUTPUT);  // led de estoy vivo
    pinMode(motorA, OUTPUT);
    pinMode(motorB, OUTPUT);
    pinMode(motorC, OUTPUT);
    pinMode(motorD, OUTPUT);

    digitalWrite(motorA, 0);
    digitalWrite(motorB, 0);
    digitalWrite(motorC, 0);
    digitalWrite(motorD, 0);

    // Inicio el ciclo en hora 0, sería el almuerzo.
    EncenderMotor();
}

void loop()
{
    static unsigned long previousMillis = 0;
    unsigned long currentMillis         = millis();
    if (currentMillis - previousMillis >= 1000)
    {
        previousMillis = currentMillis;

        // estoy vivo!
        digitalWrite(LED_BUILTIN, 1);
        delay(66);
        digitalWrite(LED_BUILTIN, 0);

        // 1 segundo cumplido, incremento los minutos:
        Minutos++;

        if (Minutos == TiempoPara2daComida)
        {
            // esta sería la cena, y espero hasta el dia siguiente...
            EncenderMotor();
        }

        if (Minutos == Tiempo1Dia)
        {
            Minutos = 0;
            EncenderMotor();
        }
    }
}
