#include <stdio.h>
#include <wiringPi.h>
#include <pthread.h>

#define FRONT_DOOR 1    // GPIO 18
#define BACK_DOOR 4     // GPIO 23
#define BATHROOM_DOOR 5 // GPIO 24
#define ROOM1_DOOR 6    // GPIO 25
#define ROOM2_DOOR 27   // GPIO 16

#define KITCHEN_BULB 0      // GPIO 17
#define LIVINGROOM_BULB 2   // GPIO 27
#define BATHROOM_BULB 3     // GPIO 22
#define ROOM1_BULB 21       // GPIO 5
#define ROOM2_BULB 22       // GPIO 6
#define DINNINGROOM_BULB 25 // GPIO 26

// 0 -> cocina
// 1 -> sala
// 2 -> bano
// 3 -> luz cuarto 1
// 4 -> luz cuarto 2
// 5 -> comedor
#define DOORS_SIZE 5
static volatile int bulbs[DOORS_SIZE] = {0};
pthread_mutex_t bulbs_mutex = PTHREAD_MUTEX_INITIALIZER;

// 0 -> puerta frontal
// 1 -> puerta trasera
// 2 -> puerta bano
// 3 -> puerta cuarto 1
// 4 -> puerta cuarto 2
static volatile int doors[5] = {0};
pthread_mutex_t doors_mutex = PTHREAD_MUTEX_INITIALIZER;

// Condicion indica cambio en el estado de una puerta
pthread_cond_t doors_cond = PTHREAD_COND_INITIALIZER;
int change = 0;
/**
 * Funcion para inicializar la bilbioteca
**/
int smhSetup();

/**
 * Funcion que establece la interrupcion al detectar
 * un cambio en el pin de una puerta
**/
void setDoorISR(int pin);

/**
 * Funcion que se ejecuta cuando se levanta una
 * interrupcion por el cambio de estado de una
 * puerta
**/
void doorInterrupt(void);

/**
 * Funcion que obtiene el estado de las puertas
**/
void getDoorsStatus(int array[DOORS_SIZE]);

/**
 * Funcion para cambiar el estado de un bombibllo
**/
int changeBulbState(int bulbId, int newState);