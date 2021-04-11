#include <iostream>       
#include <string>
#include <vector>
#include <fstream>
#include <pthread.h>
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

extern "C" {
  #include <smarthouse.h>
}

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <cstdlib>
#include <functional>
#include <thread>

namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace websocket = beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

int update = 0;
pthread_mutex_t updateMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t updateCond = PTHREAD_COND_INITIALIZER;

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
      Routes::Put(router, "/lights/:state", Routes::bind(&HomeEndpoint::putLights, this));
      Routes::Put(router, "/user", Routes::bind(&HomeEndpoint::putUser, this));
      Routes::Post(router, "/user", Routes::bind(&HomeEndpoint::postUser, this));

      // Options requests
      Routes::Options(router, "/light", Routes::bind(&HomeEndpoint::options, this));
      Routes::Options(router, "/door", Routes::bind(&HomeEndpoint::options, this));
      Routes::Options(router, "/camera", Routes::bind(&HomeEndpoint::options, this));
      Routes::Options(router, "/user", Routes::bind(&HomeEndpoint::options, this));
      Routes::Options(router, "/light/:id", Routes::bind(&HomeEndpoint::options, this));
      Routes::Options(router, "/lights/:state", Routes::bind(&HomeEndpoint::options, this));
    }

    /**
     * Metodo para configurar la respuesta del request
     * response: respuesta del metodo
    **/
    void configReponse(Http::ResponseWriter* response) 
    {
      //response->headers().add<Pistache::Http::Header::AccessControlAllowOrigin>("http://" + host);
      response->headers().add<Pistache::Http::Header::AccessControlAllowOrigin>("*");
      response->headers().add<Pistache::Http::Header::ContentType>("application/json, text/plain, */*");
      response->headers().add<Pistache::Http::Header::AccessControlAllowMethods>("GET, PUT, POST, DELETE, HEAD, OPTIONS, PATCH, PROPFIND, PROPPATCH, MKCOL, COPY, MOVE, LOCK");
      response->headers().add<Pistache::Http::Header::AccessControlAllowHeaders>("access-control-allow-methods,authorization,Content-Length,Connection,Access-Control-Allow-Origin,Origin ,Accept, X-Requested-With, Content-Type, Access-Control-Request-Method, Access-Control-Request-Headers,X-Access-Token,XKey,Authorization");
    
    }

    void options(const Rest::Request& request, Http::ResponseWriter response)
    {
      configReponse(&response);
      response.send(Http::Code::Ok);
    }

    /**
     * Metodo para verificar el token del usuario
     * header: valor del header del request
     * return: 0 si el token del request coincide con el token de usuario
     *         1 si el token del request no coincide con el token de usuario
    **/
    int verifyToken(const Rest::Request& request)
    {
      try
      {
        auto header = request.headers().getRaw("Authorization").value();
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
      catch(const std::exception& e)
      {
        return 1;
      }
      
    }

    /**
     * Metodo para obtener la lista de luces de la casa
     * request: solicitud http recibida
     * response: respuesta del metodo que envia el json con la lista de luces
    **/
    void getLight(const Rest::Request& request, Http::ResponseWriter response)
    { 
      int verify = verifyToken(request);

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
      int verify = verifyToken(request);

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
      int verify = verifyToken(request);

      string imageName = Home::home->camera.last_photo;

      if(verify == 0)
      { 
        ifstream imageFile(imageName, ifstream::binary);
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
      int verify = verifyToken(request);

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
     * Metodo para encender o apagar una luz
     * request: solicitud http recibida
     * response: respuesta del metodo de confirmacion
    **/
    void putLight(const Rest::Request& request, Http::ResponseWriter response)
    {
      int verify = verifyToken(request);

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
            changeBulbState(it->id, it->state);
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
     * Metodo para encender o apagar todas las luces
     * request: solicitud http recibida
     * response: respuesta del metodo de confirmacion
    **/
    void putLights(const Rest::Request& request, Http::ResponseWriter response)
    {
      int verify = verifyToken(request);

      if (verify == 0)
      {
        auto state = request.param(":state").as<std::string>();
        int state_int = stoi(state);
        Guard guard(homeLock);
        list<LightBulb>::iterator it;
        for (it = Home::home->lightList.begin(); it != Home::home->lightList.end(); ++it)
        {
          it->state = state_int;
          changeBulbState(it->id, it->state);
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
     * Metodo para modificar los datos del usuario
     * request: solicitud http recibida
     * response: respuesta del metodo de confirmacion
    **/
    void putUser(const Rest::Request& request, Http::ResponseWriter response)
    { 
      int verify = verifyToken(request);

      if (verify == 0)
      {
        string userjson = request.body();
        Document doc;
        doc.Parse(userjson.c_str());

        Home::home->user.name = doc["name"].GetString();
        Home::home->user.email = doc["email"].GetString();
        Home::home->user.password = doc["password"].GetString();

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
      string userName = doc2["name"].GetString();
      
      //Se comparan los datos de autenticacion
      if (userEmail == doc["email"].GetString() && userPassword == doc["password"].GetString())
      {
        string token = userEmail + ":" + userPassword;
        string encode = base64_encode((const unsigned char*)token.c_str(), token.length());
        string strResponse = "{\"token\":\"" + encode + "\",\"name\":\"" + userName + "\"}";
        configReponse(&response);
        response.send(Http::Code::Ok, strResponse);
      }
      else
      {
        configReponse(&response);
        response.send(Http::Code::Unauthorized);
      }
    }

    /**
     * Metodo para notificar al sistema que tome una foto
     * request: solicitud http recibida
     * response: respuesta del metodo con el token del usuario
    **/
    void postCamera(const Rest::Request& request, Http::ResponseWriter response)
    { 
      int verify = verifyToken(request);

      if (verify == 0)
      {
        //Tomar una foto

        configReponse(&response);
        response.send(Http::Code::Ok);
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

//
void *start_smartHouse(void *input)
{
  smhSetup(0);
  while(1)
  {
    int doors [5] = {0};
    getDoorsStatus(doors);
    list<Door>::iterator it;
    int i = 0;
    for (it = Home::home->doorList.begin(); it != Home::home->doorList.end(); ++it)
    {
      it->state = doors[i];
      i++;
    }
    pthread_mutex_lock(&updateMutex);
    update = 1;
    pthread_cond_broadcast(&updateCond);
    pthread_mutex_unlock(&updateMutex);
    /**
    for (it = Home::home->doorList.begin(); it != Home::home->doorList.end(); ++it)
    {
      cout << "Id Puerta: " << it->id << " Estado: " << it->state << endl;
    }
    **/
  }
}

void *start_restEndpoint(void *input) 
{
  Port port(9082);
  int thr = 2;
  Address addr(Ipv4::any(), port);
  HomeEndpoint home(addr);
  home.init(thr);
  home.start();

  return NULL;
}

void websocketServer(tcp::socket socket)
{
  // Construct the stream by moving in the socket
  websocket::stream<tcp::socket> ws{std::move(socket)};
  try
  {
    // Accept the websocket handshake
    ws.accept();

    std::string message = "{\"update\":1}";
    for(;;)
    {
      
      pthread_mutex_lock(&updateMutex);
      while(!update)
      {
        pthread_cond_wait(&updateCond, &updateMutex);
      }
      update = 0;
      pthread_mutex_unlock(&updateMutex);
      
      ws.write(net::buffer(message));
    }
    /* code */
  }
  catch(const std::exception& e)
  {
    cout << "Se desconecto el cliente" << endl;
  }
}

void *start_websocketEndpoint(void *input)
{
  using namespace boost::asio;

  // The io_context is required for all I/O
  net::io_context ioc{1};

  while(1)
  {
    try
    {
      // The acceptor receives incoming connections
      tcp::acceptor acceptor(ioc, ip::tcp::endpoint(ip::address(), 9081));
      for(;;)
      {
          // This will receive the new connection
          tcp::socket socket{ioc};

          // Block until we get a connection
          acceptor.accept(socket);

          // Launch the session, transferring ownership of the socket
          std::thread(&websocketServer, std::move(socket)).detach();
      }
    }
    catch(const std::exception& e)
    {
      cout << e.what() << endl;
      cout << "Se desconecto el cliente" << endl;
    }
  }
    
}

/**
 * Metodo que se encarga de crear un file con el json de user si no
 * existe uno previo. Si ya existe no crea ningun file.
 * filename: nombre del archivo a crear.
**/
void createFile(string filename)
{
  ifstream fileRead;
  fileRead.open(filename);
  if(fileRead)
  {
    //Existe el file
    fileRead.close();
  }
  else
  {
    //No existe el file
    Home::home->user.name = "admin";
    Home::home->user.email = "admin@admin";
    Home::home->user.password = "password";
    string content = Home::home->user.serialize();
    ofstream fileWrite;
    fileWrite.open(filename);
    if (fileWrite.is_open())
    {
      fileWrite << content;
      fileWrite.close();
    }
  }
  
}

int main(int argc, char *argv[])
{
  createFile("userjson.txt");
  pthread_t thread1, thread2, thread3;

  pthread_create(&thread1, NULL, start_restEndpoint, NULL);
  pthread_create(&thread2, NULL, start_websocketEndpoint, NULL);
  pthread_create(&thread3, NULL, start_smartHouse, NULL);

  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);
  pthread_join(thread3, NULL);
}