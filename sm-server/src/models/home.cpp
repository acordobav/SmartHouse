#include <string>
#include <list>
using namespace std;

#include "lightBulb.cpp"
#include "door.cpp"
#include "camera.cpp"
#include "user.cpp"


class Home {
    public:
        static Home* const home;
        list<LightBulb> lightList;  //Atributo de lista de luces
        list<Door> doorList;        //Atributo de lista de puertas
        Camera camera;              //Atributo de camara
        User user;                  //Atributo de usuario
    private:

        /**
         * Constructor de la clase Home
        **/
        Home()
        {
            LightBulb light1(0,"Luz Cocina", 0);
            LightBulb light2(1,"Luz Sala",0);
            LightBulb light3(2,"Luz Baño", 0);
            LightBulb light4(3,"Luz Cuarto 1",0);
            LightBulb light5(4,"Luz Cuarto 2",0);
            LightBulb light6(5,"Luz Comedor", 0);
            this->lightList = {light1, light2, light3, light4, light5, light6};
            
            Door door1(0,"Puerta Frontal",0);
            Door door2(1,"Puerta Trasera",0);
            Door door3(2,"Puerta Baño",0);
            Door door4(3,"Puerta Cuarto 1",0);
            Door door5(4,"Puerta Cuarto 2", 0);
            this->doorList = {door1, door2, door3, door4, door5};

            this->camera.location = "Jardin";
        }
        Home(Home& other){ }
        Home& operator=(Home& other){ }
};
Home* const Home::home = new Home();