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
  cout << "El cliente envio 1." << endl;

  while(true){
    if (poll.poll()) {
      cout << "En el poll " << endl;
      if (poll.has_input(clientSocket)) {
        cout << "Client working..." << endl;
        clientSocket.receive(r);
        cout << "El cliente recibe 1." << endl;

        string answer;
        r >> answer >> succesorIP >> succesorPort >> succesorID;
        if(answer == "Si") {
          s << myIp << myPort << Id;
          clientSocket.send(s);
          cout << "El cliente envio 2." << endl;
          clientSocket.receive(r);
          r >> answer;
          cout << "answer " << answer << endl;
          cout << "El cliente recibe 2." << endl;
        }

        if(answer == "No") {
          s << "Puedo entrar?" << Id;
          clientSocket.send(s);
          cout << "El cliente envio 3." << endl;
        }
      }

      if (poll.has_input(serverSocket)) {
        cout << "Server socket working..." << endl;
        message m, n;
        string question;
        int incommingId;
        serverSocket.receive(m);
        cout << "El server recibe 1." << endl;
        m >> question >> incommingId;
        cout << " Pregunta : " << question << " de : " << incommingId << endl;
        if (question == "Puedo entrar?"){
          if(succesorID == 100){
            n << "Si" << myIp << myPort << Id;
            serverSocket.send(n);
            cout << "Servidor envia : " << " IP : " << myIp << " Port : "  << myPort << " ID : " << Id << endl;
            serverSocket.receive(m);
            cout << "El server recibe 2." << endl;
            m >> succesorIP >> succesorPort >> succesorID;
            cout << "Servidor recibe : " << " succesorIP : " << succesorIP << " succesorPort : "  << succesorPort << " succesorID : " << succesorID << endl;
            n << "" ;
            serverSocket.send(n);
          } else {
            if((incommingId > Id && incommingId <= succesorID)){
              n << "Si" << succesorIP << succesorPort << succesorID;
              cout << "Servidor envia : " << " IP : " << myIp << " Port : "  << myPort << " ID : " << Id << endl;
              serverSocket.send(n);
              cout << "Servidor recibe : " << " succesorIP : " << succesorIP << " succesorPort : "  << succesorPort << " succesorID : " << succesorID << endl;
              serverSocket.receive(m);
              cout << "El server recibe 3." << endl;

              m >> succesorIP >> succesorPort >> succesorID;
              n << "" ;
              serverSocket.send(n);
            } else {
              if(incommingId > succesorID || incommingId <= Id){
                cout << "No se conecto en los anteriores casos." << endl;
              }
          }
        }
          //     if(incommingId >= succesorID) {
          //       n << "Si" << succesorIP << succesorPort << succesorID;
          //       serverSocket.send(n);
          //       serverSocket.receive(m);
          //       m >> succesorIP >> succesorPort >> succesorID;
          //       n << "" ;
          //       serverSocket.send(n);
          //     } else {
          //       n << "No" << succesorIP << succesorPort << succesorID;
          //       serverSocket.send(n);
          //     }
        }
      }
    }
  }

  return 0;
}
