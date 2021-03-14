#include <string>

using namespace std;

class User {
    public:

        string name;        //Atributo de nombre
        string email;       //Atributo de correo electronico
        string password;    //Atributo de contrasena
        string token;       //Atributo de token
        
        /**
         * Constructor de la clase User
        **/
        User (string name = "", string email = "", string password = "", string token = "")
        {
            name = name;
            email = email;
            password = password;
            token = token;
        }

        /**
         * Metodo para serializar el objeto
        **/
        template <typename Writer>
        void Serialize(Writer& writer) const 
        {
            writer.StartObject();
            writer.String("name");
            writer.String(name.c_str());
            writer.String("email");
            writer.String(email.c_str());
            writer.String("password");
            writer.String(password.c_str());
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