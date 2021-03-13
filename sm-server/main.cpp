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

class HomeEndpoint 
{
  public:
    explicit HomeEndpoint(Address addr)
      : httpEndpoint(std::make_shared<Http::Endpoint>(addr))
    { }

    void init(size_t thr = 2) {
      auto opts = Http::Endpoint::options()
        .threads(static_cast<int>(thr));
      httpEndpoint->init(opts);
      setupRoutes();
    }

    void start() {
      httpEndpoint->setHandler(router.handler());
      httpEndpoint->serve();
    }
  private:

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
    }

    void configReponse(Http::ResponseWriter* response) 
    {
      response->headers().add<Pistache::Http::Header::AccessControlAllowOrigin>("*");
      response->headers().add<Pistache::Http::Header::ContentType>("application/json");
    }

    void getLight(const Rest::Request& request, Http::ResponseWriter response)
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

    void getDoor(const Rest::Request& request, Http::ResponseWriter response)
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

    void getCamera(const Rest::Request& request, Http::ResponseWriter response)
    { 
      string result = Home::home->camera.serialize();
      configReponse(&response);
      response.send(Http::Code::Ok, result);
    }

    void getUser(const Rest::Request& request, Http::ResponseWriter response)
    {  
      string result;
      result = FileHandler::readFile("userjson.txt");

      configReponse(&response);
      response.send(Http::Code::Ok, result);
    }

    void putLight(const Rest::Request& request, Http::ResponseWriter response)
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

    void putDoor(const Rest::Request& request, Http::ResponseWriter response)
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

    void putUser(const Rest::Request& request, Http::ResponseWriter response)
    { 
      string userjson = request.body();
      Document doc;
      doc.Parse(userjson.c_str());

      Home::home->user.name = doc["name"].GetString();
      Home::home->user.email = doc["email"].GetString();
      Home::home->user.password = doc["password"].GetString();

      string result = Home::home->user.serialize();
      cout << result << endl;

      cout << FileHandler::writeFile("userjson.txt", result) << endl;;

      configReponse(&response);
      response.send(Http::Code::Ok);
    }

    using Lock = std::mutex;
    using Guard = std::lock_guard<Lock>;
    Lock homeLock;
    std::shared_ptr<Http::Endpoint> httpEndpoint;
    Rest::Router router;
};

void start_endpoint() 
{
  Port port(9080);

  int thr = 2;

  Address addr(Ipv4::any(), port);

  HomeEndpoint home(addr);

  home.init(thr);
  home.start();
}

int main(int argc, char *argv[]) 
{
  start_endpoint();
}