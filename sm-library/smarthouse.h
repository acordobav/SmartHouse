#include <stdio.h>
#include <wiringPi.h>
#include <pthread.h>

#define FRONT_DOOR 1    // GPIO 18 - PIN 12
#define BACK_DOOR 4     // GPIO 23 - PIN 16
#define BATHROOM_DOOR 5 // GPIO 24 - PIN 18
#define ROOM1_DOOR 6    // GPIO 25 - PIN 22
#define ROOM2_DOOR 27   // GPIO 16 - PIN 36

#define KITCHEN_BULB 0      // GPIO 17 - PIN 11
#define LIVINGROOM_BULB 2   // GPIO 27 - PIN 13
#define BATHROOM_BULB 3     // GPIO 22 - PIN 15
#define ROOM1_BULB 21       // GPIO 5  - PIN 29
#define ROOM2_BULB 22       // GPIO 6  - PIN 31
#define DINNINGROOM_BULB 25 // GPIO 26 - PIN 37

// 0 -> cocina
// 1 -> sala
// 2 -> bano
// 3 -> luz cuarto 1
// 4 -> luz cuarto 2
// 5 -> comedor
#define BULBS_SIZE 6
static volatile int bulbs[BULBS_SIZE] = {0};
pthread_mutex_t bulbs_mutex = PTHREAD_MUTEX_INITIALIZER;

// 0 -> puerta frontal
// 1 -> puerta trasera
// 2 -> puerta bano
// 3 -> puerta cuarto 1
// 4 -> puerta cuarto 2
#define DOORS_SIZE 5
static volatile int doors[DOORS_SIZE] = {0};
pthread_mutex_t doors_mutex = PTHREAD_MUTEX_INITIALIZER;

// Condicion indica cambio en el estado de una puerta
pthread_cond_t doors_cond = PTHREAD_COND_INITIALIZER;
int change = 0;

/**
 * Funcion para inicializar la bilbioteca
 * switchType: 0 -> Este tipo de switch levanta durante un momento
 *                  un voltaje positivo, por lo que es necesario que
 *                  el sistema solo reaccione a pulsos positivos.
 *             1 -> Este tipo de switch mantiene su estado, por lo que es
 *                  necesario que el sistema reaccione a pulsos negativos
 *                  y positivos.
**/
int smhSetup(int switchType);

/**
 * Funcion que establece la interrupcion al detectar
 * un cambio en el pin de una puerta
 * pin: numero de wPi que corresponde al pin 
 *      de una puerta
 * switchType: 0 -> switch pulsador
 *             1 -> switch que mantiene su estado
 * interrupt: puntero a la funcion de la interrupcion
 *            que se debe ejecutar al detectar un cambio
**/
void setDoorISR(int pin, int switchType, void *interrupt);

/**
 * Funcion que se ejecuta cuando se levanta una
 * interrupcion por el cambio de estado de una
 * puerta
**/
void frontdoorInterrupt(void);
void backdoorInterrupt(void);
void bathroomdoorInterrupt(void);
void room1doorInterrupt(void);
void room2doorInterrupt(void);
void doorInterrupt(int id, int pin);

/**
 * Funcion lee el valor de los pines de las puertas para
 * establecer su valor inicial en la lista de puertas
**/
void setDoorInitialState();

/**
 * Funcion que obtiene el estado de las puertas
 * array: lista donde escribir el resultado
**/
void getDoorsStatus(int array[DOORS_SIZE]);

/**
 * Funcion que obtiene el estado de los bombillos
 * array: lista donde escribir el resultado
**/
void getBulbsStatus(int array[BULBS_SIZE]);

/**
 * Funcion para cambiar el estado de un bombibllo
 * bulbId: identificador del bombillo, que corresponde
 *         con su posicion en la lista de bombillos
 * newState: 0 para apagar, 1 para encender
**/
void changeBulbState(int bulbId, int newState);