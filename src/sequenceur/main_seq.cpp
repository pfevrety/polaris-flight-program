#include <Arduino.h>
#include "Radio.h"
// #include "Imu.h"
#include <array>
#include "commun/LED.h"
#include "PinSeq.h"
#include "SeqVar.h"
#include "Tridge.h"
#include "LedController.h"
#include "Servo_ST3215.h"
#include "SdLogger.h"

SDLogger logger;
RADIO Radio;
TRIDGE Tridge;
LedController ledSystem(LED_COUNT, LED_INTERFACE);

Servo_ST3215 servoPara(Serial4, 1, 532, 2048, 0); 
Servo_ST3215 servoCanSat(Serial3, 1, 1154, -230, 0);

// IMU imu1(SPI_IMU_1, IMU1_PIN);
// IMU imu2(SPI_IMU_2, IMU2_PIN);

AccelData accelHistory1[AccelBufferLen];
AccelData accelHistory2[AccelBufferLen];

int altiData[AccelBufferLen * 2];
int lastcommandtime = 0;

int nextBurstTime = 0;

AccelData getAverage(const AccelData *history, int len)
{
  float invLen = 1.0f / (float)len;
  AccelData avg = {millis(), {0, 0, 0}, {0, 0, 0}};

  for (int i = 0; i < len; i++)
  {
    avg.acc.x += history[i].acc.x;
    avg.acc.y += history[i].acc.y;
    avg.acc.z += history[i].acc.z;

    avg.gyro.x += history[i].gyro.x;
    avg.gyro.y += history[i].gyro.y;
    avg.gyro.z += history[i].gyro.z;
  }

  avg.acc.x *= invLen;
  avg.acc.y *= invLen;
  avg.acc.z *= invLen;
  avg.gyro.x *= invLen;
  avg.gyro.y *= invLen;
  avg.gyro.z *= invLen;

  return avg;
}

void addMeasure(AccelData measure, AccelData *history, int len)
{
  for (int i = 0; i < len - 1; i++)
  {
    history[i] = history[i + 1];
  }
  history[len - 1] = measure;
}

bool parachuteDeployed{false};
bool atterie{false};

float i = 0.0f;
int8_t FLIGHT_STATE = 0; // 0 = au sol, 1 = en vol, 2 = descente, 3 = atterri
// Si descente alors parachuteDeployed = true

long int flightStartTime;

int altitude_initiale;

void setup()
{
  Serial.begin(9600);
  Tridge.begin();
  Serial.println("Démarage du système");
  delay(50);
  // A faire tester si la radio est bien connectée

  // if (!imu1.init())
  // {
  //   Serial.println("IMU non connectée !");
  // }
  // else
  // {
  //   Serial.println("IMU OK");
  //   imu1.calibrate(); // refaire le code de la calibration
  // }
  
  servoPara.begin();
  servoCanSat.begin();

  pinMode(18, OUTPUT);
  ledSystem.begin();
  ledSystem.setBrightness(80);
  ledSystem.setGlobalColor(LedController::BLUE);


  ledSystem.setColor(LED_PORTE_INDEX, LedController::ORANGE);
  // servoPara.open();
  // servoCanSat.open();

  delay(2000);

  servoCanSat.close();
  servoPara.close();

  // while(!servoCanSat.isTargetReached(0)) {
  //   delay(10); 
  // }
  Serial.println("ca aurait du fonctionner"); 

    if (!logger.begin()) {
        ledSystem.setColor(LED_CANSAT_INDEX, LedController::RED);
    } else {
        ledSystem.setColor(LED_CANSAT_INDEX, LedController::GREEN);
    }

  // servoCanSat.close();
  // while(!servoCanSat.isTargetReached(0)) {
  //   delay(10);
  // }
  ledSystem.setColor(LED_CANSAT_INDEX, LedController::GREEN);
  
  digitalWrite(18, HIGH); // Test buzzer mais ca marche pas
  delay(1000);
  digitalWrite(18, LOW); // Test buzzer mais ca marche pas

  ledSystem.setGlobalColor(LedController::GREEN);
  Serial.println("Envoi des trames de synchronisation à l'Expérience...");
  for (int i = 0; i < 20; i++)
  {
    Tridge.sendSync();
    delay(50);
  }
  Serial.println("Système prêt et synchronisé");
}

void loop()
{
  AccelData raw1;
  AccelData raw2;

  Serial.println("Debut");
  addMeasure(raw1, accelHistory1, AccelBufferLen);
  AccelData acc1 = getAverage(accelHistory1, AccelBufferLen);

  addMeasure(raw2, accelHistory2, AccelBufferLen);
  AccelData acc2 = getAverage(accelHistory2, AccelBufferLen);
  Serial.println("fin");

  if (FLIGHT_STATE == 0)
  {
    //  on a une sécu sur l'accélération pour éviter de se faire avoir par un faux décollage, à voir si on garde
    // acc1.acc.getMagnitude() > 1.5 j'ai enelvé pour les tests à l'arret

    // --- Fonctionnel --- //
    if (digitalRead(PIN_JACK))
    {
      flightStartTime = millis();
      FLIGHT_STATE = 1;
      ledSystem.setGlobalColor(LedController::RED);
      Serial.println("Décollage détecté !");
    }
  }

  else if (FLIGHT_STATE == 1)
  {
    // mettre la condition d'altitude
    if (millis() - flightStartTime > maxFlightDuration)
    {
      FLIGHT_STATE = 2;
    }
  }

  else if (FLIGHT_STATE == 2)
  {
    if (!parachuteDeployed) {
      parachuteDeployed = true;
      ledSystem.setGlobalColor(LedController::ORANGE);
      Serial.println("Déployage du parachute");
      servoCanSat.open();
      delay(2000); // temps d'ouverture du parachute, à ajuster selon les tests
      servoPara.open(); 
    }
    if (acc1.acc.getMagnitude() < 1.0f)
    {
      Serial.println("La fusee a atterri");
      FLIGHT_STATE = 3;
      digitalWrite(18, HIGH); // activer la résistance de pull-up interne du jack
      atterie = true;
      ledSystem.setGlobalColor(LedController::GREEN);
    }
  }

  FlightData flightData = {acc1, parachuteDeployed, atterie, FLIGHT_STATE};

  // La j'ai utilisé un fakeacc sinon ca fait freeze le programme faudra que tu le remplaces par les imu
  AccelData fakeAcc = {
      (uint32_t)millis(),
      Accel(1.23 + i, -4.56, 9.81),
      Accel(0.12, 0.34, -0.56)};
  i++;

  FlightData fakeData = {
      fakeAcc,
      true,
      false,
      FLIGHT_STATE};

    
  Tridge.sendData(fakeData);
  Serial.println("fin envoie");
  logger.logFlightData(fakeData, FLIGHT_STATE);

    if (FLIGHT_STATE == 3 && atterie) {
        logger.close();
    }

  delay(10);
}