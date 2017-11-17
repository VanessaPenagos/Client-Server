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
      cout << "Dijo que si! " << endl;
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
  int succesorID = 100;

  cout << "Mi id es : " << Id << endl;
  cout << "Al iniciar el succesor es : " << succesorID << endl;

  context ctx;
  poller poll;

  socket clientSocket(ctx, socket_type::req);  //Pregunta a los otros nodos
  socket serverSocket(ctx, socket_type::rep);  //Escucha a los otros nodos
  clientSocket.connect("tcp://"+succesorIP+":"+succesorPort);
  serverSocket.bind("tcp://*:"+myPort);
  poll.add(clientSocket);
  poll.add(serverSocket);
  message s, r;
  string answer;
  s << "Puedo entrar?" << Id;
  clientSocket.send(s);

  while(true){
    if (poll.poll()) {
      if (poll.has_input(clientSocket)) {
        clientSocket.receive(r);
        string answer;
        r >> answer >> succesorIP >> succesorPort >> succesorID;
        cout << "Su succesor es : " << succesorID << endl;
        if(answer == "Si") {
          s << myIp << myPort << Id;
          clientSocket.send(s);
          clientSocket.receive(r);
        } else {
          s << "Puedo entrar?" << Id;
          clientSocket.send(s);
        }
        //findSuccesor(Id, myIp, myPort, succesorIP, succesorPort, succesorID, clientSocket);
        //Client :3
        //cout << "El sucesor tiene el ID : " << succesorID << endl;
      }

      if (poll.has_input(serverSocket)) {
        message m, n;
        string question;
        int incommingId;
        serverSocket.receive(m);
        m >> question >> incommingId;

        if (question == "Puedo entrar?"){
          if( (incommingId > Id && incommingId <= succesorID) || (incommingId <= Id && incommingId <= 50)){
            cout << "El id esta en el rango " << endl;
            n << "Si" << succesorIP << succesorPort << succesorID;
            serverSocket.send(n);
            serverSocket.receive(m);
            m >> succesorIP >> succesorPort >> succesorID;
            cout << "Pudo entrar, su succesor es : " << succesorID << endl;
            n << "" ;
            serverSocket.send(n);
          } else {
            cout << "El id no esta en el rango" << endl;
            cout << "Su succesor es : " << succesorID << endl;
            n << "No" << succesorIP << succesorPort << succesorID;
          }
        }
      }
    }
  }

  return 0;
}
