#include "Ponte.h"

int StringIsOn(std::string nam1, std::vector<std::string> nam2) {
    for (int i = 0;i < nam2.size();i++) {
        if (nam1._Equal(nam2.at(i))) {
            return  i;
        }
    }
    return -1;
}

template <typename Srz>
void Serialization( Srz& thing, std::string& bufferstr) {
    size_t sz = sizeof(Srz);
    Srz thing2;
    char* buffer = new char[sz];
    std::memcpy(buffer, &thing, sz);
    bufferstr = buffer;
    delete[] buffer;
}

template <typename Srz>
void Deserialization( Srz& thing, char* buffer) {
    size_t sz = sizeof(Srz);
    std::memcpy(&thing , buffer , sz);
}

template <typename Srz>
void Deserialization(Srz& thing, std::string buffer) {
    size_t sz = sizeof(Srz);
    std::memcpy(&thing, buffer.c_str(), sz);
}


Cliente::Cliente() {
        if (WSAStartup(MAKEWORD(2, 2), &wsdata) != 0) {
            std::cout << "deu ruim na lib do windows\n";
        }
        serversk = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (serversk == INVALID_SOCKET) {
            std::cout << "deu ruim na hora de abrir o socket\n";
        }
        printf("nome de usuario: ");
        std::cin >> MyName;
    }
int Cliente::ConnectToP(u_short port,const char* IP) {
        int result;
        struct sockaddr_in serverAddr;
        serverAddr.sin_family = AF_INET;
        serverAddr.sin_port = htons(port); // Porta do servidor
        serverAddr.sin_addr.s_addr = inet_addr(IP); // Endereço IP do servidor
        result = connect(serversk, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
        send(serversk, MyName.c_str(), MyName.length(), 0);
        if (result != -1) {
           
            std::cout << "conectado\n";
            return 1;
        }
        std::cout << "falha ao conectar\n";
        return -1;
    }
int Cliente::SendToP(std::string message) {
        int i = send(serversk, message.c_str(), message.length(), 0);
        if (i != -1) {
            return 0;
        }
        return -1;
    }
int Cliente::ReciveFromP(std::string &buffstr) {
        char* buff = new char[bufflimit];
        int r = recv(serversk, buff, bufflimit, 0);
        if (r != -1) {
            buffstr.assign(buff, r);
            delete[] buff;
            return 0;
        }
        delete[] buff;
        return -1;
        
    }

void Ponte::HandleSituation(int r, int i) {
    if(AutoBan)
    {
        if (r == 0 || r == -1) {
            if (ClientsNumErrors.at(i) > ClientsMaxErrors) {
                KickClient(i);
            }
            else {
                ClientsNumErrors.insert(ClientsNumErrors.begin() + i, ClientsNumErrors.at(i) + 1);
            }
        }
        else {
            ClientsNumErrors.insert(ClientsNumErrors.begin() + i, 0);
        }
    }
}    
Ponte::Ponte(const char* IP, u_short PORT) {

        if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) {
            std::cout << "deu ruim na lib do windows\n";
        }
        listensk = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (listensk == INVALID_SOCKET) {
            std::cout << "deu ruim na hora de abrir o socket\n";
        }

        struct sockaddr_in PonteAddr;
        PonteAddr.sin_family = AF_INET;
        PonteAddr.sin_addr.s_addr = inet_addr(IP);
        PonteAddr.sin_port = htons(8080);

        bind(listensk, (struct sockaddr*)&PonteAddr, sizeof(PonteAddr));

       
        
    }
int Ponte::ReOpenSocket(const char *IP, u_short PORT) {
    if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0) {
        std::cout << "deu ruim na lib do windows\n";
    }
    listensk = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listensk == INVALID_SOCKET) {
        std::cout << "deu ruim na hora de abrir o socket\n";
    }

    struct sockaddr_in PonteAddr;
    PonteAddr.sin_family = AF_INET;
    PonteAddr.sin_addr.s_addr = inet_addr(IP);
    PonteAddr.sin_port = htons(8080);

    return bind(listensk, (struct sockaddr*)&PonteAddr, sizeof(PonteAddr));
}
Ponte::~Ponte() {
    ClientsSK.clear();
    ClientsSADR.clear();
    ClientsSK.~vector();
    ClientsSADR.~vector();
    ClientsNumErrors.clear();
    ClientsNumErrors.~vector();
    closesocket(listensk);
    WSACleanup();
}
int Ponte::PutOnLisen() {
    //std::cout << "serivodr esta ouvindo\n";
    if (listen(listensk, SOMAXCONN) == 0) {
        sockaddr_in ad;
        SOCKET sk;
        int sz = sizeof(ad);

        sk = accept(listensk, (struct sockaddr*)&ad, &sz);
        if (sk == SOCKET_ERROR){
            //std::cout << "erro entre cliente e ponte\n";
        }
        else {
            char tonka[100];
            int r = recv(sk, tonka, 100, 0);
            std::string tonka2;
            tonka2.assign(tonka, r);
            if (StringIsOn(tonka2, ClientsName) == -1) {
                ClientsName.push_back(tonka2);
                ClientsSK.push_back(sk);
                ClientsSADR.push_back(ad);
                ClientsNumErrors.push_back(0);
                std::cout << tonka2 << '\n';
                std::cout << "esta on porra\n";
                //send(sk, MessageConected.c_str(), MessageConected.length(), 0);
                return 0;
            }
            else
            {
                std::cout << "não foi possivel adicionar o cliente pois seu nome ja esta no servidor";
                return -1;
            }
                
        }
    }
    else {
        std::cout << "erro ao escutar\n";
        return -1;
            
    }

}
void Ponte::KickClient(std::string name) {
    int n = StringIsOn(name, ClientsName);
    printf("o cliente %s esta sendo desconectado por inatividade", ClientsName.at(n).c_str());
    ClientsSK.erase(ClientsSK.begin() + n);
    ClientsSADR.erase(ClientsSADR.begin() + n);
    ClientsName.erase(ClientsName.begin() + n);
    ClientsNumErrors.erase(ClientsNumErrors.begin() + n);
}
void Ponte::KickClient(int n) {
    printf("o cliente %s esta sendo desconectado por inatividade", ClientsName.at(n).c_str());
    ClientsSK.erase(ClientsSK.begin() + n);
    ClientsSADR.erase(ClientsSADR.begin() + n);
    ClientsName.erase(ClientsName.begin() + n);
    ClientsNumErrors.erase(ClientsNumErrors.begin() + n);
}
void Ponte::TellAll(std::string message) {
    for (int i = 0; i < ClientsSK.size();i++) {
        SOCKET sk = ClientsSK.at(i);
        int r = send(sk, message.c_str(), message.length(), 0);
        std::cout << r << '\n';
        HandleSituation(r, i);
    }
}
void Ponte::TellAll(std::vector<std::string> message) {
    for (int i = 0; i < ClientsSK.size();i++) {
        for (int x = 0;x < message.size();x++) {
            SOCKET sk = ClientsSK.at(i);
            std::string ms = message.at(i);
            int r = send(sk, ms.c_str(), ms.length(), 0);
            HandleSituation(r, i);
        }
           
    }
}
void Ponte::tellOne(std::string message, std::string name) {
    int indexcl;
    indexcl = StringIsOn(name, ClientsName);
    SOCKET sk = ClientsSK.at(indexcl);
    int r = send(sk, message.c_str(), message.length(), 0);
    HandleSituation(r, indexcl);
        
}
void Ponte::ReciveFrom(std::string name, std::string &buff) {
    int n = StringIsOn(name, ClientsName);
    if(n != -1)
    {
        char* buffc = new char[bufferLimit];
        int r = recv(ClientsSK.at(n), buffc, bufferLimit, 0);
        if (r != -1) {
            buff.assign(buffc, r);
        }
        HandleSituation(r, n);
        delete[] buffc;
    }
        
}
std::vector<std::string> Ponte::ReciveFromAll() {
    std::vector<std::string> messages;
    for (int i = 0;i < ClientsSK.size();i++) {
       
        char* buff = new char[bufferLimit];
        int r = recv(ClientsSK.at(i), buff, bufferLimit, 0);
        if (r != -1)
        {
            std::string h;
            h.assign(buff, r);
            messages.push_back(h);
        }
        HandleSituation(r, i);
        delete[] buff;
    }
       
    return messages;
}
void Ponte::ReciveFromAll(std::vector<std::string> &messages) {
    for (int i = 0;i < ClientsSK.size();i++) {
        char* buff = new char[bufferLimit];
        int r = recv(ClientsSK.at(i), buff, bufferLimit, 0);
        if (r != -1)
        {
            std::string ms;
            ms.assign(buff, r);
            messages.push_back(ms);
        }
        HandleSituation(r, i);
        delete[] buff;
    }
}
void Ponte::NoWaitMode(bool a) {
    u_long mode;
    if (a)
        mode = 1;
    else
        mode = 0;


    if (ioctlsocket(listensk, FIONBIO, &mode) != NO_ERROR) {
        printf("deu ruim ao desbloquear o soket\n");
    }
}