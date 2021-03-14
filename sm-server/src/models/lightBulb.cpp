#include <iostream>       
#include <string>

using namespace std;

#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
using namespace rapidjson;


class LightBulb {
    public:

        int id;           //Atributo identificador
        string location;  //Atributo de ubicacion
        int state;        //Atributo de estado: 0 = OFF, 1 = ON
        
        /**
         * Constructor de la clase Lightbulb
        **/
        LightBulb (int id, string location, int state)
        {
            this->id = id;
            this->location = location;
            this->state = state;
        }
        
        /**
         * Metodo para serializar el objeto
        **/
        template <typename Writer>
        void Serialize(Writer& writer) const 
        {
            writer.StartObject();
            writer.String("id");
            writer.Uint(id);
            writer.String("location");
            writer.String(location.c_str());
            writer.String("state");
            writer.Uint(state);
            writer.EndObject();
        }

        /**
         * Metodo para convertir el objeto json a string
        **/
        std::string serialize()
        {
            StringBuffer s;
            Writer<StringBuffer> writer(s);
            Serialize(writer);
            return s.GetString();
        }
};