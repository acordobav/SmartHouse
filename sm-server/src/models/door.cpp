#include <iostream>       
#include <string>
using namespace std;

#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
using namespace rapidjson;

class Door 
{
    public:

        int id;           //Atributte of ID
        string location;  //Atributte of location
        int state;        //Atributte of state: 0 = Closed, 1 = Open
        
        Door (int id, string location, int state)
        {
            this->id = id;
            this->location = location;
            this->state = state;
        }

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

        std::string serialize()
        {
            StringBuffer s;
            Writer<StringBuffer> writer(s);
            Serialize(writer);
            return s.GetString();
        }
};