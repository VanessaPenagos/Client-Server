#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <thread>
#include <zmqpp/zmqpp.hpp>

using namespace std;
using namespace zmqpp;

int generateId() {  // Genera un id de forma aleatoria.
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<> dist(1,50);
  return dist(gen);
}

void Server( socket &serverSocket, int my_id, string my_ip, string my_port, int &succesor_id, string &succesor_ip, string &succesor_port) {
  serverSocket.bind("tcp://*:" + my_port);
  cout << "Iniciando servidor" << '\n';

  while (true) {
    message s,r;
    int input_id;
    string question;
    serverSocket.receive(r);
    r >> question >> input_id;

    if (question == "Puedo ser tu sucesor?") {
      if (input_id > my_id && input_id <= succesor_id) { // Borrar para cuando se use el sha1
        s << "Si";
        serverSocket.send(s);
        serverSocket.receive(r);
        s << succesor_id << succesor_ip << succesor_port;
        serverSocket.send(s);
        r >> succesor_id >> succesor_ip >> succesor_port;
        cout << "Servidor:: actualiza sucesor " << succesor_id << endl;
      } else {
        if ( input_id > my_id && input_id > succesor_id) {
          s << "Si";
          serverSocket.send(s);
          serverSocket.receive(r);
          s << succesor_id << succesor_ip << succesor_port;
          serverSocket.send(s);
          r >> succesor_id >> succesor_ip >> succesor_port;
          cout << "Servidor:: actualiza sucesor " << succesor_id << endl;
        } else {
          s << "No";
          string request;
          serverSocket.send(s);
          serverSocket.receive(r);
          cout << "AQUI ES" << endl;
          r >> request;
          if (request == "Cual es tu sucesor?") {
            r >> succesor_ip >> succesor_port;
            serverSocket.send(r);
          }
        }
      }
    }
  }
}

int main(int argc, char** argv) {

  string my_ip, my_port, succesor_ip, succesor_port;
  int succesor_id, my_id;

  if (argc == 3){
    my_id = 0;
    my_ip = argv[1];
    my_port = argv[2];
    succesor_id = 0;
    succesor_ip = my_ip;
    succesor_port = my_port;
    cout << "Me he conectado" << endl;
  }

  if (argc == 5) {
    my_id = generateId();
    my_ip = argv[1];
    my_port = argv[2];
    succesor_id = -1;
    succesor_ip = argv[3];
    succesor_port = argv[4];
  }

  context ctx;
  socket serverSocket(ctx,socket_type::rep);
  socket clientSocket(ctx,socket_type::req);
  thread thread_server;
  thread_server = thread(Server, ref(serverSocket), my_id, my_ip, my_port, ref(succesor_id), ref(succesor_ip), ref(succesor_port));
  bool flag = false;

  while (true) {
    if (argc == 5) {
      context ctx;
      message s,r;
      string answer;
      while (!flag) {
        clientSocket.connect("tcp://" + succesor_ip + ":" + succesor_port);
        cout << "Mi ID " << my_id <<endl;

        s << "Puedo ser tu sucesor?" << my_id;
        clientSocket.send(s);
        clientSocket.receive(r);
        r >> answer;

        if (answer == "Si"){
          s << my_id << my_ip << my_port;
          clientSocket.send(s);
          clientSocket.receive(r);
          clientSocket.disconnect("tcp://" + succesor_ip + ":" + succesor_port);
          r >> succesor_id >> succesor_ip >> succesor_port;
          cout << "Mi nuevo sucesor " << succesor_id << endl;
          flag = true;
          cout << "Pude entrar al anillo" << endl;
        } else {
          if (answer == "No") {
            s << "Cual es tu sucesor?";
            clientSocket.send(s);
            clientSocket.receive(r);
            clientSocket.disconnect("tcp://" + succesor_ip + ":" + succesor_port);
            r >> succesor_ip >> succesor_port;
            cout << "Continuo el en siguiente nodo " << succesor_port << endl;
          }
        }
      }
    }
  }
  return 0;
}
