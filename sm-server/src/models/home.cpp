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
            LightBulb light1(1,"Cocina", 0);
            LightBulb light2(2,"Comedor",0);
            LightBulb light3(3,"Sala", 0);
            LightBulb light4(4,"Cuarto1",0);
            LightBulb light5(5,"Cuarto2",0);
            this->lightList = {light1, light2, light3, light4, light5};
            
            Door door1(1,"Delantera",0);
            Door door2(2,"Trasera",0);
            Door door3(3,"Cuarto1",0);
            Door door4(4,"Cuarto2",0);
            this->doorList = {door1, door2, door3, door4};

            this->camera.location = "Jardin";
            this->camera.last_photo = "jardin.png";
        }
        Home(Home& other){ }
        Home& operator=(Home& other){ }
};
Home* const Home::home = new Home();