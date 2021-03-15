#include <iostream>       
#include <string>
#include <vector>
#include <fstream> 
using namespace std;

#include <pistache/endpoint.h>
#include <pistache/http.h>
#include <pistache/router.h>
using namespace Pistache;

#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/document.h"
using namespace rapidjson;

#include "./src/models/home.cpp"
#include "./src/fileHandler.cpp"
#include "./src/encrypting.cpp"

class HomeEndpoint 
{
  public:
    /**
     * Constructor de la clase HomeEndpoint
     * addr: direccion ip del host
    **/
    explicit HomeEndpoint(Address addr)
      : httpEndpoint(std::make_shared<Http::Endpoint>(addr))
    { }

    /**
     * Metodo para declarar el punto final de comunicacion
     * thr: cantidad de hilos para el servidor
    **/
    void init(size_t thr = 2) {
      auto opts = Http::Endpoint::options()
        .threads(static_cast<int>(thr));
      httpEndpoint->init(opts);
      setupRoutes();
    }

    /**
     * Metodo para inicializar el punto final de comunicacion
    **/
    void start() {
      httpEndpoint->setHandler(router.handler());
      httpEndpoint->serve();
    }
  private:

    /**
     * Metodo para asignar las rutas de los metodos de requests
    **/
    void setupRoutes()
    {
      using namespace Rest;

      Routes::Get(router, "/light", Routes::bind(&HomeEndpoint::getLight, this));
      Routes::Get(router, "/door", Routes::bind(&HomeEndpoint::getDoor, this));
      Routes::Get(router, "/camera", Routes::bind(&HomeEndpoint::getCamera, this));
      Routes::Get(router, "/user", Routes::bind(&HomeEndpoint::getUser, this));
      Routes::Put(router, "/light/:id", Routes::bind(&HomeEndpoint::putLight, this));
      Routes::Put(router, "/door/:id", Routes::bind(&HomeEndpoint::putDoor, this));
      Routes::Put(router, "/user", Routes::bind(&HomeEndpoint::putUser, this));
      Routes::Post(router, "/user", Routes::bind(&HomeEndpoint::postUser, this));
    }

    /**
     * Metodo para configurar la respuesta del request
     * response: respuesta del metodo
    **/
    void configReponse(Http::ResponseWriter* response) 
    {
      response->headers().add<Pistache::Http::Header::AccessControlAllowOrigin>("*");
      response->headers().add<Pistache::Http::Header::ContentType>("application/json");
    }

    /**
     * Metodo para verificar el token del usuario
     * header: valor del header del request
     * return: 0 si el token del request coincide con el token de usuario
     *         1 si el token del request no coincide con el token de usuario
    **/
    int verifyToken(string &header)
    {
      //Se obtiene el token del header del request
      string word = "Basic ";
      auto n = header.find(word);
      if (n != std::string::npos)
      {
          header.erase(n, word.length());
      }
      
      //Se obtiene el token del json del file
      string userjson = FileHandler::readFile("userjson.txt");
      Document doc;
      doc.Parse(userjson.c_str());
      string userEmail = doc["email"].GetString();
      string userPassword = doc["password"].GetString();
      string token = userEmail + ":" + userPassword;
      string encode = base64_encode((const unsigned char*)token.c_str(), token.length());

      //Se verifica si los tokens son iguales
      if (encode == header)
        return 0;
      else
        return 1;
    }

    /**
     * Metodo para obtener la lista de luces de la casa
     * request: solicitud http recibida
     * response: respuesta del metodo que envia el json con la lista de luces
    **/
    void getLight(const Rest::Request& request, Http::ResponseWriter response)
    { 
      auto enc_str = request.headers().getRaw("Authorization").value();
      int verify = verifyToken(enc_str);

      if (verify == 0)
      {
        string result = "[";
        list<LightBulb>::iterator it;
        for (it = Home::home->lightList.begin(); it != Home::home->lightList.end(); ++it)
        { 
          result = result + it->serialize();
          result = result + ",";
        }
        result = result.substr(0, result.size()-1);
        result = result + "]";
      
        configReponse(&response);
        response.send(Http::Code::Ok, result);
      }
      else
      {
        configReponse(&response);
        response.send(Http::Code::Unauthorized);
      }
    }

    /**
     * Metodo para obtener la lista de puertas de la casa
     * request: solicitud http recibida
     * response: respuesta del metodo que envia el json con la lista de puertas
    **/
    void getDoor(const Rest::Request& request, Http::ResponseWriter response)
    { 
      auto enc_str = request.headers().getRaw("Authorization").value();
      int verify = verifyToken(enc_str);

      if (verify == 0)
      {
        string result = "[";
        list<Door>::iterator it;
        for (it = Home::home->doorList.begin(); it != Home::home->doorList.end(); ++it)
        { 
          result = result + it->serialize();
          result = result + ",";
        }
        result = result.substr(0, result.size()-1);
        result = result + "]";
      
        configReponse(&response);
        response.send(Http::Code::Ok, result);
      }
      else
      {
        configReponse(&response);
        response.send(Http::Code::Unauthorized);
      }
    }

    /**
     * Metodo para obtener los datos de la camara
     * request: solicitud http recibida
     * response: respuesta del metodo que envia el json con los datos de camara
    **/
    void getCamera(const Rest::Request& request, Http::ResponseWriter response)
    { 
      auto enc_str = request.headers().getRaw("Authorization").value();
      int verify = verifyToken(enc_str);

      if(verify == 0)
      { 
        ifstream imageFile("jardin.jpg", ifstream::binary);
        if (imageFile)
        { 
          imageFile.seekg(0, imageFile.end);
          int length = imageFile.tellg();
          imageFile.seekg(0, imageFile.beg);
          char * buffer = new char[length];

          imageFile.read(buffer, length);
          string encode = base64_encode((const unsigned char*)buffer, length);
          Home::home->camera.last_photo = encode;
          imageFile.close();
          delete[] buffer;
        }
        string result = Home::home->camera.serialize();
        configReponse(&response);
        response.send(Http::Code::Ok, result);
      }
      else
      {
        configReponse(&response);
        response.send(Http::Code::Unauthorized);
      }
    }

    /**
     * Metodo para obtener los datos del usuario
     * request: solicitud http recibida
     * response: respuesta del metodo que envia el json con los datos del usuario
    **/
    void getUser(const Rest::Request& request, Http::ResponseWriter response)
    { 
      auto enc_str = request.headers().getRaw("Authorization").value();
      int verify = verifyToken(enc_str);

      if (verify == 0)
      {
        string result = FileHandler::readFile("userjson.txt");
        configReponse(&response);
        response.send(Http::Code::Ok, result);
      }
      else
      {
        configReponse(&response);
        response.send(Http::Code::Unauthorized);
      }
    }

    /**
     * Metodo para modificar los datos de la luces
     * request: solicitud http recibida
     * response: respuesta del metodo de confirmacion
    **/
    void putLight(const Rest::Request& request, Http::ResponseWriter response)
    {
      auto enc_str = request.headers().getRaw("Authorization").value();
      int verify = verifyToken(enc_str);

      if (verify == 0)
      {
        auto id = request.param(":id").as<std::string>();
        int id_int = stoi(id);
        Guard guard(homeLock);
        list<LightBulb>::iterator it;
        for (it = Home::home->lightList.begin(); it != Home::home->lightList.end(); ++it)
        {
          if(it->id == id_int)
          {
            it->state = !it->state;
          }
        }
        for (it = Home::home->lightList.begin(); it != Home::home->lightList.end(); ++it)
        {
          cout << "Id Luz: " << it->id << " Estado: " << it->state << endl;
        }
        configReponse(&response);
        response.send(Http::Code::Ok);
      }
      else
      {
        configReponse(&response);
        response.send(Http::Code::Unauthorized);
      }
      
    }

    /**
     * Metodo para modificar los datos de la puertas
     * request: solicitud http recibida
     * response: respuesta del metodo de confirmacion
    **/
    void putDoor(const Rest::Request& request, Http::ResponseWriter response)
    { 
      auto enc_str = request.headers().getRaw("Authorization").value();
      int verify = verifyToken(enc_str);

      if (verify == 0)
      {
        auto id = request.param(":id").as<std::string>();
        int id_int = stoi(id);
        Guard guard(homeLock);
        list<Door>::iterator it;
        for (it = Home::home->doorList.begin(); it != Home::home->doorList.end(); ++it)
        {
          if(it->id == id_int)
          {
            it->state = !it->state;
          }
        }
        for (it = Home::home->doorList.begin(); it != Home::home->doorList.end(); ++it)
        {
          cout << "Id Puerta: " << it->id << " Estado: " << it->state << endl;
        }
        configReponse(&response);
        response.send(Http::Code::Ok);
      }
      else
      {
        configReponse(&response);
        response.send(Http::Code::Unauthorized);
      }
    }

    /**
     * Metodo para modificar los datos del usuario
     * request: solicitud http recibida
     * response: respuesta del metodo de confirmacion
    **/
    void putUser(const Rest::Request& request, Http::ResponseWriter response)
    { 
      auto enc_str = request.headers().getRaw("Authorization").value();
      int verify = verifyToken(enc_str);

      if (verify == 0)
      {
        string userjson = request.body();
        Document doc;
        doc.Parse(userjson.c_str());

        Home::home->user.name = doc["name"].GetString();
        Home::home->user.email = doc["email"].GetString();
        Home::home->user.password = doc["password"].GetString();
        Home::home->user.token = enc_str;

        string result = Home::home->user.serialize();
        cout << result << endl;
        FileHandler::writeFile("userjson.txt", result);

        configReponse(&response);
        response.send(Http::Code::Ok);
      }
      else
      {
        configReponse(&response);
        response.send(Http::Code::Unauthorized);
      }
    }

    /**
     * Metodo para verificar y obtener el token del usuario
     * request: solicitud http recibida
     * response: respuesta del metodo con el token del usuario
    **/
    void postUser(const Rest::Request& request, Http::ResponseWriter response)
    { 
      //Se obtiene el json del body del request
      string userjson = request.body();
      Document doc;
      doc.Parse(userjson.c_str());
      
      //Se obtiene el json del file
      string user = FileHandler::readFile("userjson.txt");
      Document doc2;
      doc2.Parse(user.c_str());
      string userEmail = doc2["email"].GetString();
      string userPassword = doc2["password"].GetString();
      
      //Se comparan los datos de autenticacion
      if (userEmail == doc["email"].GetString() && userPassword == doc["password"].GetString())
      {
        string token = userEmail + ":" + userPassword;
        string encode = base64_encode((const unsigned char*)token.c_str(), token.length());
        configReponse(&response);
        response.send(Http::Code::Ok, encode);
      }
      else
      {
        configReponse(&response);
        response.send(Http::Code::Unauthorized);
      }
    }

    using Lock = std::mutex;
    using Guard = std::lock_guard<Lock>;
    Lock homeLock;
    std::shared_ptr<Http::Endpoint> httpEndpoint;
    Rest::Router router;
};

/**
 * Metodo asignar el puerto y direccion al servidor rest
**/
void start_endpoint() 
{
  Port port(9082);

  int thr = 2;

  Address addr(Ipv4::any(), port);

  HomeEndpoint home(addr);

  home.init(thr);
  home.start();
}

/**
 * Metodo principal de ejecucion
**/
int main(int argc, char *argv[]) 
{
  start_endpoint();
}