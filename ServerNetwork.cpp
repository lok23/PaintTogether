#include <unordered_map>
#include "ServerNetwork.hpp"
#include "Draw.hpp"

/* "App" functions */
void ServerNetwork::ExecuteCommand(Command* c){
    // if executed command is same as m_undo's commands, we do not want to change the same pixel to the same color twice in a row
    if (!m_undo.empty() && *(m_undo.top()) == *c) {
        std::cout << "Duplicate detected! Command NOT executed" << std::endl;
        return;
    }

    // once we execute a command, there are no undos to redo, so we can remove every Command in m_redo.
    while(!m_redo.empty()) {
        m_redo.pop();
    }

    // We don't keep track of image in ServerNetwork, so we don't need these following lines of code
    // sf::Image& m_image = this->GetImage();
    //c->execute(&m_image);

    m_undo.push(c);
}

/*! \brief	Removes Command from m_undo, undoes the Command,
*	  	and pushes it to top of m_redo. Does nothing if m_undo is empty.
*/
void ServerNetwork::UndoCommand() {
    if (m_undo.size() <= 0) {
        return;
    }
    // We don't keep track of image in ServerNetwork, so we don't need these following lines of code
    // sf::Image& m_image = this->GetImage();
    // m_undo.top()->undo(&m_image);
    m_redo.push(m_undo.top());
    m_undo.pop();
}

/*! \brief	Removes Command from m_redo, redoes the Command,
*	 	and pushes it to top of m_undo. Does nothing if m_undo is empty.
*/
void ServerNetwork::RedoCommand() {
    if (m_redo.size() <= 0) {
        return;
    }
    // We don't keep track of image in ServerNetwork, so we don't need these following lines of code

    // sf::Image& m_image = this->GetImage();
    // m_redo.top()->redo(&m_image);
    m_undo.push(m_redo.top());
    m_redo.pop();
}

void ServerNetwork::clearStacks(){
    while (!m_undo.empty()){
        m_undo.pop();
    }
    while (!m_redo.empty()){
        m_redo.pop();
    }
}

sf::Color ServerNetwork::getBackgroundColor() {
    return backgroundColor;
}

void ServerNetwork::changeBackgroundColor(sf::Color newBackgroundColor) {
    backgroundColor = newBackgroundColor;
}

/* "ServerNetwork" functions */
ServerNetwork::ServerNetwork(unsigned short port) : listen_port(port){
    logl("SERVERNETWORK: SERVERNETWORK CONSTRUCTOR CALLED");

    logl("Chat Server Started");

    changeBackgroundColor(sf::Color::White);

     if(listener.listen(listen_port) != sf::Socket::Done){
          logl("Could not listen");
     }
}

void ServerNetwork::ConnectClients(std::vector<sf::TcpSocket *> * client_array){
    logl("SERVERNETWORK: CONNECTCLIENTS CALLED");

     while(true){
          sf::TcpSocket * new_client = new sf::TcpSocket();
          if(listener.accept(*new_client) == sf::Socket::Done){
               new_client->setBlocking(false);
               client_array->push_back(new_client);
               logl("Added client " << new_client->getRemoteAddress() << ":" << new_client->getRemotePort() << " on slot " << client_array->size());
          }else{
               logl("Server listener error, restart the server");
               delete(new_client);
               break;
          }
     }
}

void ServerNetwork::DisconnectClient(sf::TcpSocket * socket_pointer, size_t position){
    logl("SERVERNETWORK: DISCONNECTCLIENT CALLED");

     logl("Client " << socket_pointer->getRemoteAddress() << ":" << socket_pointer->getRemotePort() << " disconnected, removing");
     socket_pointer->disconnect();
     delete(socket_pointer);
     client_array.erase(client_array.begin() + position);
}

void ServerNetwork::BroadcastPacket(sf::Packet & packet, sf::IpAddress exclude_address, unsigned short port){
    logl("SERVERNETWORK: BROADCASTPACKET CALLED");

    for(size_t iterator = 0; iterator < client_array.size(); iterator++){
          sf::TcpSocket * client = client_array[iterator];
          if(client->send(packet) != sf::Socket::Done){
              logl("Could not send packet on broadcast");
          }
     }
}

void ServerNetwork::ReceivePacket(sf::TcpSocket * client, size_t iterator){
    // Don't want to log here because this method gets called a lot
    // logl("SERVERNETWORK: RECEIVEPACKET CALLED");

    // We receive ints instead of sf::Color from the packets,
    // so this map helps us convert from ints to the proper sf::Color
    std::unordered_map<int, sf::Color> sample_map {
            {1, sf::Color::Black},
            {2, sf::Color::White},
            {3, sf::Color::Red},
            {4, sf::Color::Green},
            {5, sf::Color::Blue},
            {6, sf::Color::Yellow},
            {7, sf::Color::Magenta},
            {8, sf::Color::Cyan}
    };

    sf::Packet packet;
     if(client->receive(packet) == sf::Socket::Disconnected){
          DisconnectClient(client, iterator);
     }else{
          if(packet.getDataSize() > 0) {
              std::string typeOfData;
              packet >> typeOfData;

              if (typeOfData == "text") {
                  std::string received_message;
                  packet >> received_message;
                  packet.clear();

                  packet << typeOfData << received_message << client->getRemoteAddress().toString()
                         << client->getRemotePort();

                  BroadcastPacket(packet, client->getRemoteAddress(), client->getRemotePort());
                  logl(client->getRemoteAddress().toString() << ":" << client->getRemotePort() << " 'typeOfData: "
                                                             << typeOfData << " " << received_message << "'");
              } else if (typeOfData == "draw") {
                  int x; int y; int prevColorInt; int currColorInt;
                  packet >> x >> y >> prevColorInt >> currColorInt;
                  packet.clear();

                  packet << typeOfData << x << y << prevColorInt << currColorInt << client->getRemoteAddress().toString()
                                                 << client->getRemotePort();

                  BroadcastPacket(packet, client->getRemoteAddress(), client->getRemotePort());
                  logl(client->getRemoteAddress().toString() << ":" << client->getRemotePort() << " 'typeOfData: "
                                                             << typeOfData << " x=" << x << " y=" << y);

                  sf::Color prevColor = sample_map[prevColorInt];
                  sf::Color currColor = sample_map[currColorInt];

                  this->ExecuteCommand(new Draw(x, y, prevColor, currColor));

              } else if (typeOfData == "undo_command") {
                  packet << typeOfData << client->getRemoteAddress().toString()
                         << client->getRemotePort();

                  BroadcastPacket(packet, client->getRemoteAddress(), client->getRemotePort());
                  logl(client->getRemoteAddress().toString() << ":" << client->getRemotePort() << " 'typeOfData: "
                                                             << typeOfData);

                  this->UndoCommand();

              } else if (typeOfData == "redo_command") {
                  packet << typeOfData << client->getRemoteAddress().toString()
                         << client->getRemotePort();

                  BroadcastPacket(packet, client->getRemoteAddress(), client->getRemotePort());
                  logl(client->getRemoteAddress().toString() << ":" << client->getRemotePort() << " 'typeOfData: "
                                                             << typeOfData);

                  this->RedoCommand();

              } else if (typeOfData == "spacebar_command") {
                  int currColorInt;
                  packet >> currColorInt;
                  packet.clear();

                  packet << typeOfData << currColorInt;
                  BroadcastPacket(packet, client->getRemoteAddress(), client->getRemotePort());
                  logl(client->getRemoteAddress().toString() << ":" << client->getRemotePort() << " 'typeOfData: "
                                                             << typeOfData);

                  this->clearStacks();
                  sf::Color newBackgroundColor = sample_map[currColorInt];
                  this->changeBackgroundColor(newBackgroundColor);
              } else if (typeOfData == "request_undo") {

                  // We are converting the entirety of m_undo into a packet.
                  // It will be received by ClientNetwork in backwards order.
                  int numOfCommandsGoingIntoThePacket = m_undo.size();
                  packet.clear();
                  packet << typeOfData << numOfCommandsGoingIntoThePacket;
                  std::stack<Command*> copy_stack(m_undo);
                  while (!copy_stack.empty()) {
                      Command* topFromStack = copy_stack.top();
                      copy_stack.pop();
                      Draw* drawTopFromStack = dynamic_cast<Draw*>(topFromStack);
                      int x = drawTopFromStack->getNewX();
                      int y = drawTopFromStack->getNewY();
                      int prevColor = this->ConvertColorToInt(drawTopFromStack->getPrevColor());
                      int currColor = this->ConvertColorToInt(drawTopFromStack->getCurrColor());
                      packet << x << y << prevColor << currColor;
                  }
                  BroadcastPacket(packet, client->getRemoteAddress(), client->getRemotePort());
                  logl(client->getRemoteAddress().toString() << ":" << client->getRemotePort() << " 'typeOfData: "
                                                             << typeOfData << " number of undo_stack commands sent: " << numOfCommandsGoingIntoThePacket);
              } else if (typeOfData == "request_redo") {

                  // We are converting the entirety of m_redo into a packet.
                  // It will be received by ClientNetwork in backwards order.
                  int numOfCommandsGoingIntoThePacket = m_redo.size();
                  packet.clear();
                  packet << typeOfData << numOfCommandsGoingIntoThePacket;
                  std::stack<Command *> copy_stack(m_redo);
                  while (!copy_stack.empty()) {
                      Command *topFromStack = copy_stack.top();
                      copy_stack.pop();
                      Draw *drawTopFromStack = dynamic_cast<Draw *>(topFromStack);
                      int x = drawTopFromStack->getNewX();
                      int y = drawTopFromStack->getNewY();
                      int prevColor = this->ConvertColorToInt(drawTopFromStack->getPrevColor());
                      int currColor = this->ConvertColorToInt(drawTopFromStack->getCurrColor());
                      packet << x << y << prevColor << currColor;
                  }
                  BroadcastPacket(packet, client->getRemoteAddress(), client->getRemotePort());
                  logl(client->getRemoteAddress().toString() << ":" << client->getRemotePort() << " 'typeOfData: "
                                                             << typeOfData << " number of redo_stack commands sent: "
                                                             << numOfCommandsGoingIntoThePacket);
              } else if (typeOfData == "request_background_color") {
                  int backgroundColorInt = this->ConvertColorToInt(this->getBackgroundColor());
                  packet.clear();

                  packet << typeOfData << backgroundColorInt;

                  BroadcastPacket(packet, client->getRemoteAddress(), client->getRemotePort());
                  logl(client->getRemoteAddress().toString() << ":" << client->getRemotePort() << " 'typeOfData: "
                                                             << typeOfData);
              } else {
                  logl("Unexpected Server ReceivePacket Error!");
              }
          }
     }
}

int ServerNetwork::ConvertColorToInt(sf::Color color) {
    // Packets cannot take sf::Color, so we must convert sf::Color's to ints instead
    if (color == sf::Color::Black) {
        return 1;
    } else if (color == sf::Color::White) {
        return 2;
    } else if (color == sf::Color::Red) {
        return 3;
    } else if (color == sf::Color::Green) {
        return 4;
    } else if (color == sf::Color::Blue) {
        return 5;
    } else if (color == sf::Color::Yellow) {
        return 6;
    } else if (color == sf::Color::Magenta) {
        return 7;
    } else if (color == sf::Color::Cyan) {
        return 8;
    } else {
        logl("Error: ConvertColorToInt received invalid sf::Color");
    }
}

void ServerNetwork::ManagePackets(){
    logl("SERVERNETWORK: MANAGEPACKETS CALLED");


    while(true){
          for(size_t iterator = 0; iterator < client_array.size(); iterator++){
              ReceivePacket(client_array[iterator], iterator);
          }
     }
}

void ServerNetwork::Run(){
    logl("SERVERNETWORK: RUN CALLED");

    std::thread connection_thread(&ServerNetwork::ConnectClients, this, &client_array);

    ManagePackets();
}