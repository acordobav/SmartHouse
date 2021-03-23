# Biblioteca SM-Library
Esta biblioteca tiene el propósito de crear una api que permita a un programa de software acceder al estado actual de las puertas y que permita cambiar el estado de los bombillos. Se utiliza la biblioteca wiringPi para poder controlar los GPIO del raspberry.

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
getDoorsStatus(int array[DOORS_SIZE]);
```
Esta función bloquea la ejecución del hilo hasta que se detecte un cambio en uno de los pines de la puerta. El array que recibe como argumento es donde se coloca el estado de las puertas, donde un 0 significa cerrado y un 1 abierto.


## Distribución de Pines

### Puertas
| Nombre | GPIO | PIN | 
| :---: | :---: | :---: | 
| Cocina | 18 | 12 |
| Trasera | 23 | 16 |
| Baño | 24 | 18 |
| Cuarto 1 | 25 | 22 |
| Cuarto 2 | 16 | 36 |

### Bombillos
| Nombre | GPIO | PIN | 
| :---: | :---: | :---: | 
| Cocina | 17 | 11 |
| Sala | 27 | 13 |
| Baño | 22 | 15 |
| Cuarto 1 | 5 | 29 |
| Cuarto 2 | 6 | 31 |
| Comedor | 26 | 37 |
