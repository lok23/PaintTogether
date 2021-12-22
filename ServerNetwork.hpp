#pragma once
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <string.h>
#include <SFML/Network.hpp>
#include <stack>
#include "Command.hpp"

#define MAX_RAW_DATA 256 //Max bytes supported on Raw Data mode

#define logl(x) std::cout << x << std::endl
#define log(x) std::cout << x

class ServerNetwork {

private:
    /* "App" member variables */
    std::stack<Command*> m_undo;	// Stack that stores the most recently undone command.
    std::stack<Command*> m_redo;
    sf::Color backgroundColor;

    /* "ServerNetwork" variables */
    sf::TcpListener listener;
    std::vector<sf::TcpSocket *> client_array;

    unsigned short listen_port;
public:

    /* "App" functions */
    void 	ExecuteCommand(Command* c);
    void    UndoCommand();
    void    RedoCommand();
    void    clearStacks();
    
    void    changeBackgroundColor(sf::Color);
    sf::Color    getBackgroundColor();
    
    int ConvertColorToInt(sf::Color);

    /* "ServerNetwork" functions */
    ServerNetwork(unsigned short);
    void ConnectClients(std::vector<sf::TcpSocket *> *);
    void DisconnectClient(sf::TcpSocket *, size_t);

    void ReceivePacket(sf::TcpSocket *, size_t);

    void BroadcastPacket(sf::Packet &, sf::IpAddress, unsigned short);

    void ManagePackets();
    void Run();
};