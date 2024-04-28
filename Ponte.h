#ifndef PON
#define PON

#include <iostream>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#include "LoopList.h"
#include <vector>
#include <string>

int StringIsOn(std::string nam1, std::vector<std::string> nam2);

template <typename Srz>
void Serialization(Srz& thing, std::string& bufferstr);

template <typename Srz>
void Deserialization(Srz& thing, char* buffer);

template <typename Srz>
void Deserialization(Srz& thing, std::string buffer);

class Cliente {
public:
    SOCKET soket;
    SOCKET serversk;
    WSADATA wsdata;
    std::string MyName;
    int bufflimit = 1000;

    Cliente();

    int ConnectToP(u_short port, const char* IP);

    int SendToP(std::string message);

    int ReciveFromP(std::string& buffstr);

};

class Ponte {
    void HandleSituation(int r, int i);
public:
    int ClientsMaxErrors = 20;
    WSADATA wsadata;
    SOCKET listensk;
    std::vector<SOCKET> ClientsSK;
    std::vector<sockaddr_in> ClientsSADR;
    std::vector<std::string> ClientsName;
    std::vector<int> ClientsNumErrors;
    std::string MessageConected = "voce_esta_conectado_ao_servidor_legal";
    int bufferLimit = 1000;

    Ponte(const char* IP);

    ~Ponte();

    int ReOpenSocket(const char* IP);

    int PutOnLisen();

    void KickClient(std::string name);

    void KickClient(int n);

    void TellAll(std::string message);

    void TellAll(std::vector<std::string> message);

    void tellOne(std::string message, std::string name);

    std::string ReciveFrom(std::string name);

    std::vector<std::string> ReciveFromAll();

    void ReciveFromAll(std::vector<std::string>& messages);

    void NoWaitMode(bool a);

};
#endif // !PON

