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
};