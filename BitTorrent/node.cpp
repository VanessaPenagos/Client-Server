#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <zmqpp/zmqpp.hpp>

using namespace std;
using namespace zmqpp;

int generateId() {  // Genera un id de forma aleatoria.
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<> dist(1,50);
  return dist(gen);
}

void findSuccesor(int Id, string myIP, string myPort, string &succesorIP, string &succesorPort, int &succesorID, socket &clientSocket) {
  bool join = false;
  while(!join){
    clientSocket.connect("tcp://"+succesorIP+":"+succesorPort);
    cout << "Conectado con : " << "tcp://"+succesorIP+":"+succesorPort << endl;
    message s, r;
    string answer;
    s << "Puedo entrar?" << Id;
    clientSocket.send(s);
    clientSocket.receive(r);
    cout << "Hola" << '\n';
    r >> answer >> succesorIP >> succesorPort >> succesorID;

    if(answer == "Si"){
      s << myIP << myPort << Id;
      clientSocket.send(s);
      clientSocket.receive(r);
      join = true;
    }
  }
}

int main(int argc, char** argv) {
  if(argc != 5) {
    cerr << "Faltan argumentos " << endl;
    return -1;
  }

  string myIp = argv[1], myPort = argv[2], succesorIP = argv[3], succesorPort = argv[4];
  int Id = generateId();
  cout << "Mi id es : " << Id << endl;

  context ctx;
  poller poll;
  int succesorID = -100;

  socket clientSocket(ctx, socket_type::req);  //Pregunta a los otros nodos
  socket serverSocket(ctx, socket_type::rep);  //Escucha a los otros nodos
  poll.add(clientSocket);
  poll.add(serverSocket);
  cout << "Agrega los servidor al poll " << endl;

  if (poll.poll()) {
    cout << "Estoy en el poll " << endl;
    if (poll.has_input(clientSocket)) {
      //Client :3
      clientSocket.connect("tcp://"+succesorIP+":"+succesorPort);
      findSuccesor(Id, myIp, myPort, succesorIP, succesorPort, succesorID, clientSocket);
      cout << "El sucesor tiene el ID : " << succesorID << endl;
    }

    if (poll.has_input(serverSocket)) {
      serverSocket.bind("tcp://*:"+myPort);
      message m, n;
      string question;
      int incommingId;
      serverSocket.receive(m);
      m >> question >> incommingId;

      if (question == "Puedo entrar?"){
        cout << "Por aqui paso" << endl;
        if( incommingId > Id && incommingId < succesorID){
          n << "Si" << succesorIP << succesorPort << succesorID;
          cout << "Por aqui paso" << endl;
          serverSocket.send(n);
          serverSocket.receive(m);
          m >> succesorIP >> succesorPort >> succesorID;
        } else {
          n << "No" << succesorIP << succesorPort << succesorID;
          cout << "Por aqui tambien" << endl;
        }
      }
    }
  }
  return 0;
}
