# Biblioteca SM-Library
Esta biblioteca tiene el propósito de crear una api que permita a un programa de software acceder al estado actual de las puertas y que permita cambiar el estado de los bombillos. Se utiliza la biblioteca smgpio para poder controlar los GPIO del raspberry.

## Utilización

```c
int smhSetup(int switchType);
```

Esta función se encarga de configurar los pines del raspberry para encender los LEDs, y también de establecer la interrupción que se ejecuta cuando se detecta un cambio en uno de los pines que corresponde a una puerta. El argumento switchType determina si el tipo de switch utilizado para las puertas mantiene su estado (por lo que es necesario reaccionar a pulsos negativos y positivos) o si es un switch pulsador (por lo que es necesario reaccionar solo a pulsos positivos).


```c
getBulbsStatus(int array[BULBS_SIZE]);
```

Esta función lee el estado de los bombillos y utiliza el array que recibe como argumento para guardar su valor. Un 1 significa que el bombillo se encuentra encendido, y un 0 significa que está apagado.

```c
changeBulbState(int bulbId, int newState);
```
Esta función cambia el estado de un bombillo específico, utilizando bulbId como identificador y newState como el nuevo estado.


```c
getDoorsStatus(int array[DOORS_SIZE]);
```
Esta función bloquea la ejecución del hilo hasta que se detecte un cambio en uno de los pines de la puerta. El array que recibe como argumento es donde se coloca el estado de las puertas, donde un 0 significa cerrado y un 1 abierto.


## Distribución de Pines e identificador para las puertas y bombillos
El identificador es igual a la posición en la lista correspondiente.

### Puertas
| Nombre | ID | GPIO | PIN | 
| :---: | :---: | :---: | :---: | 
| Delantera | 0 |  18 | 12 |
| Trasera | 1 | 23 | 16 |
| Baño | 2 | 24 | 18 |
| Cuarto 1 | 3 | 25 | 22 |
| Cuarto 2 | 4 | 16 | 36 |

### Bombillos
| Nombre | ID | GPIO | PIN | 
| :---: | :---: | :---: | :---: | 
| Cocina | 0 | 17 | 11 |
| Sala | 1 | 27 | 13 |
| Baño | 2 | 22 | 15 |
| Cuarto 1 | 3 | 5 | 29 |
| Cuarto 2 | 4 | 6 | 31 |
| Comedor | 5 | 26 | 37 |
