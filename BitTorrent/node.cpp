#include <iostream>
#include <random>
#include <sstream>
#include <string>
#include <zmqpp/zmqpp.hpp>

using namespace std;
using namespace zmqpp;

int generateId() {
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

  context ctx;
  int succesorID = -100;

  socket clientSocket(ctx, socket_type::req);  //Pregunta a los otros nodos
  socket serverSocket(ctx, socket_type::rep);  //Escucha a los otros nodos

  if((stoi(succesorIP) == 0) && (stoi(succesorPort) == 0)) {
      succesorID = 0;
  } else {
    cout << "Preguntar a succesorIP + succesorPort por su ID " << endl;
  }

  cout << "El sucesor tiene el ID : " << succesorID << endl;
  return 0;
}
