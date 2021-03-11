#include <string>

using namespace std;

class Camera {
    public:

        string location;
        string last_photo;
        
        Camera (string location = "", string last_photo = "")
        {
            this->location = location;
            this->last_photo = last_photo;
        }
};