#include <string>

using namespace std;

class User {
    public:

        string name;
        string email;
        string password;
        string token;
        
        User (string name = "", string email = "", string password = "", string token = "")
        {
            name = name;
            email = email;
            password = password;
            token = token;
        }

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
            writer.String("token");
            writer.String(token.c_str());
            writer.EndObject();
        }

        std::string serialize()
        {
            StringBuffer s;
            Writer<StringBuffer> writer(s);
            Serialize(writer);
            return s.GetString();
        }
};