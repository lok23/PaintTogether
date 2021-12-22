#pragma once
#include <iostream>
#include <thread>
#include <chrono>
#include <SFML/Network.hpp>
#include <SFML/Graphics/Image.hpp>
#include <stack>
#include "Command.hpp"

#define logl(x) std::cout << x << std::endl
#define log(x) std::cout << x

class ClientNetwork{

    /* "App" member variables */
    // Stack that stores the most recently done command.
    std::stack<Command*> m_undo;	// Stack that stores the most recently undone command.
    std::stack<Command*> m_redo;
    // Main image
    sf::Image* m_image;
    // Create a sprite that we overaly
    // on top of the texture.
    sf::Sprite* m_sprite;
    // Texture sent to the GPU for rendering
    sf::Texture* m_texture;
    // Our rendering window
    sf::RenderWindow* m_window;
    // User selected color (num 1-8)
    sf::Color currColor;
    // whether canvas window is at the front of the screen
    bool hasFocus;

    /* "ClientNetwork" member variables */
    sf::TcpSocket socket;
    sf::Packet last_packet;
    bool isConnected = false;
public:

    /* "App" functions */
    void 	ExecuteCommand(Command* c);
    void    UndoCommand();
    void    RedoCommand();

    void setCurrColor(sf::Color color);
    void clearStacks();
    void setImage(sf::Image *image);

    sf::Image& GetImage();
    sf::Texture& GetTexture();
    sf::RenderWindow& GetWindow();
    sf::Color getCurrColor();

    void Destroy();
    void Init();
    void Update();
    void DrawFunc();
    void Loop();


    /* "ClientNetwork" functions */
    ClientNetwork();
    void Connect(const char *, unsigned short);
    void ReceiveTextOrDrawThread(sf::TcpSocket *);

    void RequestUndoStack();
    void RequestRedoStack();
    void RequestBackgroundColor();

    void SendTextThread();
    void SendDrawThread(int x, int y, sf::Color prevColor, sf::Color currColor);

    void SendUndoCommand();
    void SendRedoCommand();
    void SendSpacebarCommand(sf::Color color);
    void SendPacket(sf::Packet &);
    void Run();

};