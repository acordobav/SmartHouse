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
  pinMode(KITCHEN_BULB, OUTPUT);
  pinMode(LIVINGROOM_BULB, OUTPUT);
  pinMode(BATHROOM_BULB, OUTPUT);
  pinMode(ROOM1_BULB, OUTPUT);
  pinMode(ROOM2_BULB, OUTPUT);
  pinMode(DINNINGROOM_BULB, OUTPUT);

  // Se establece un valor inicial de apagado
  digitalWrite(KITCHEN_BULB, 0);
  digitalWrite(LIVINGROOM_BULB, 0);
  digitalWrite(BATHROOM_BULB, 0);
  digitalWrite(ROOM1_BULB, 0);
  digitalWrite(ROOM2_BULB, 0);
  digitalWrite(DINNINGROOM_BULB, 0);

  // Se configura la interrupcion en los pines de puertas
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

  while (!change)
  {
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

void getBulbsStatus(int array[BULBS_SIZE])
{
  // Se bloquea el recurso (lista bombillos)
  pthread_mutex_lock(&doors_mutex);

  // Se obtiene el estado de los bombillos
  doors[0] = digitalRead(KITCHEN_BULB);
  doors[1] = digitalRead(LIVINGROOM_BULB);
  doors[2] = digitalRead(BATHROOM_BULB);
  doors[3] = digitalRead(ROOM1_BULB);
  doors[4] = digitalRead(ROOM2_BULB);

  // Se copia el array que indica el estado
  for (int i = 0; i < BULBS_SIZE; i++)
  {
    array[i] = bulbs[i];
  }

  // Se desbloquea el recurso (lista bombillos)
  pthread_mutex_unlock(&doors_mutex);
}

void changeBulbState(int bulbId, int newState)
{
  if (!(newState == 1 || newState == 0))
  {
    printf("El nuevo estado de un bombillo debe ser 0 o 1\n");
    return;
  }

  if (bulbId >= BULBS_SIZE || bulbId < 0)
  {
    printf("Id de bombillo invalido\n");
    return;
  }

  // Se bloquea el recurso (lista bombillos)
  pthread_mutex_lock(&doors_mutex);

  // Se cambia el valor en la lista de bombillos
  bulbs[bulbId] = newState;

  // Se escribe el nuevo valor en el pin correspondiente
  switch (bulbId)
  {
  case 0:
    digitalWrite(KITCHEN_BULB, newState);
    break;

  case 1:
    digitalWrite(LIVINGROOM_BULB, newState);
    break;

  case 2:
    digitalWrite(BATHROOM_BULB, newState);
    break;

  case 3:
    digitalWrite(ROOM1_BULB, newState);
    break;

  case 4:
    digitalWrite(ROOM2_BULB, newState);
    break;

  case 5:
    digitalWrite(DINNINGROOM_BULB, newState);
    break;

  default:
    break;
  }

  // Se desbloquea el recurso (lista bombillos)
  pthread_mutex_unlock(&doors_mutex);
}