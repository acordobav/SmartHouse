#include <string>

using namespace std;

class Camera {
    public:

        string location;    //Atributo de ubicacion
        string last_photo;  //Atributo de ultima foto
        
        /**
         * Constructor de la clase Camera
        **/
        Camera (string location = "", string last_photo = "")
        {
            this->location = location;
            this->last_photo = last_photo;
        }
        
        /**
         * Metodo para serializar el objeto
        **/
        template <typename Writer>
        void Serialize(Writer& writer) const 
        {
            writer.StartObject();
            writer.String("location");
            writer.String(location.c_str());
            writer.String("last_photo");
            writer.String(last_photo.c_str());
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