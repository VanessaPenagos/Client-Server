#include <zmqpp/zmqpp.hpp>
#include <iostream>
#include <string>
#include <cassert>
#include <undordered>

using namespace std;
using namespace zmqpp;

int main(int argc, char * argv) {
  context ctx;
  socket s(ctx, socket_type::rep);
  s.bind("tcp://*:5555");

  string path(argv[1])
  undordered_map<string,string> songs;
  songs["s1"] = path + "s1.ogg";

  cout << "Start serving request"
  while(true) {
    message m;
    s.receive(m);

    string op;
    m >> op;

    cout << "Action:  " << op << endl;
    if (op == "List") {
      //Use case 1. Send the songs
      message n;
      n << "List" << songs.size();
      for(const auto& p: songs)
        n << p.first;
      s.send(n);
    } else if(op == "Play") {
      //Send song file
    } else {
      cout << "Invalid operation requested!!\n";
    }

  }

  cout << "Finished\n";
  return 0;
}
