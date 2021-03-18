#include "smarthouse.h"

int smhSetup()
{
  if (wiringPiSetup() < 0)
  {
    printf("Unable to setup wiringPi\n");
    return 1;
  }

  // Se establecen los pines de los bombillos como
  // pines de salida
  /*pinMode(KITCHEN_BULB, OUTPUT);
  pinMode(LIVINGROOM_BULB, OUTPUT);
  pinMode(BATHROOM_BULB, OUTPUT);
  pinMode(ROOM1_BULB, OUTPUT);
  pinMode(ROOM2_BULB, OUTPUT);
  pinMode(DINNINGROOM_BULB, OUTPUT);*/

  setDoorISR(FRONT_DOOR);
  setDoorISR(BACK_DOOR);
  setDoorISR(BATHROOM_DOOR);
  setDoorISR(ROOM1_DOOR);
  setDoorISR(ROOM2_DOOR);
}

void setDoorISR(int pin)
{
  if (wiringPiISR(pin, INT_EDGE_BOTH, &doorInterrupt) < 0)
  {
    printf("Unable to setup ISR on Wpi: %d\n", pin);
  }
}

void doorInterrupt(void)
{
  // Se bloquea el recurso (lista puertas)
  pthread_mutex_lock(&doors_mutex);

  // Se obtiene el estado de las puertas
  doors[0] = digitalRead(FRONT_DOOR);
  doors[1] = digitalRead(BACK_DOOR);
  doors[2] = digitalRead(BATHROOM_DOOR);
  doors[3] = digitalRead(ROOM1_DOOR);
  doors[4] = digitalRead(ROOM2_DOOR);
  
  // Se emite la senal de cambio en una puerta
  pthread_cond_broadcast(&doors_cond);

  change = 1;
  // Se desbloquea el recurso (lista puertas)
  pthread_mutex_unlock(&doors_mutex);
}

void getDoorsStatus(int array[DOORS_SIZE])
{
  // Se bloquea el recurso (lista puertas)
  pthread_mutex_lock(&doors_mutex);

  while(!change) {
    // Se espera por la senal de cambio de estado
    // en una puerta
    pthread_cond_wait(&doors_cond, &doors_mutex);
  }
  change = 0;
  // Se copia el array que indica el estado
  for (int i = 0; i < DOORS_SIZE; i++)
  {
    array[i] = doors[i];
  }

  // Se desbloquea el recurso (lista puertas)
  pthread_mutex_unlock(&doors_mutex);
}

int main(void)
{
  smhSetup();

  
  int test[DOORS_SIZE] = {0};
  while (1)
  {
    getDoorsStatus(test);
    for (int i = 0; i < DOORS_SIZE; i++)
    {
      printf("%d ", test[i]);
    }
    printf("\n");
  }

  return 0;
}