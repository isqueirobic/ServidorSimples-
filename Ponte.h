#ifndef PON
#define PON

#include <iostream>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#include <vector>
#include <string>

// simple server for simple I/O of informations, or any stuff

int StringIsOn(std::string nam1, std::vector<std::string> nam2); // check if a string is on vector of strings

template <typename Srz>
void Serialization(Srz& thing, std::string& bufferstr); // serialze waters it needs (EVERYTHING HAS TO BE THE TYPE IT WAS SINGTED)

template <typename Srz>
void Deserialization(Srz& thing, char* buffer); // does what it says

template <typename Srz>
void Deserialization(Srz& thing, std::string buffer); // does waht it says but with a string

class Cliente { // our client, i could make the clint the server it self, but idk, it's easier this way
public:
    SOCKET soket; // client soket
    SOCKET serversk; // server socket
    WSADATA wsdata; // idk what this is
    std::string MyName; // we need to know who u are some how in the server, using numbers it's to hard
    int bufflimit = 1000; // cant send 1 terabyte of information just sizeof(int) * 1000

    Cliente(); // biuld

    int ConnectToP(u_short port, const char* IP); // conet to our server/ponte whatever u wana call it

    int SendToP(std::string message); // send to the server

    int ReciveFromP(std::string& buffstr); // recivem form the sëvêr

};

class Ponte { // the server
    void HandleSituation(int r, int i); // if the client it's farting around seding erros, it get kicked
public:
    int ClientsMaxErrors = 20; // self explanatory
    WSADATA wsadata;
    SOCKET listensk;
    std::vector<SOCKET> ClientsSK;
    std::vector<sockaddr_in> ClientsSADR;
    std::vector<std::string> ClientsName;
    std::vector<int> ClientsNumErrors;
    std::string MessageConected = "voce_esta_conectado_ao_servidor_legal"; // hallo i'm server(it dont worck yet)
    int bufferLimit = 1000;
    bool AutoBan = true;

    Ponte(const char* IP, u_short PORT); // put yo ip on it and start our engines

    ~Ponte(); // bye bye server

    int ReOpenSocket(const char* IP, u_short PORT); // start the server again
    // if reopen 0 if not -1

    int PutOnLisen(); // see if there is any clients whelling(wating idk how to type that) to joing our garden(server)
    // if listing goes bad returns -1 if not 0

    void KickClient(std::string name); // bye bye by name

    void KickClient(int n); // bye bye by index

    void TellAll(std::string message); // tell all clints one thing

    void TellAll(std::vector<std::string> message); // tell all clients a buntch of things

    void tellOne(std::string message, std::string name); // tell one mf

    void ReciveFrom(std::string name, std::string &buff); // recive from our client and return a string

    std::vector<std::string> ReciveFromAll(); // recive from all of our clients and return a vector of strings

    void ReciveFromAll(std::vector<std::string>& messages); // recive a bunch of information put on a vector and return that vector

    void NoWaitMode(bool a); // this protocol stop our program if the serve is listing, so if u dont want that activathis
    // by putting true, or disable by false

};
#endif // !PON

