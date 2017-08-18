#include <SFML/Audio.hpp>
#include <fstream>
#include <iostream>
#include <string>
#include <zmqpp/zmqpp.hpp>

using namespace std;
using namespace zmqpp;
using namespace sf;

void messageToFile(const message &msg, const string &fileName) {
  const void *data;
  msg.get(&data, 1);
  size_t size = msg.size(1);

  ofstream ofs(fileName, ios::binary);
  ofs.write((char *)data, size);
}

int main() {
  cout << "This is the client\n";

  context ctx;
  socket s(ctx, socket_type::req);
  cout << "Connecting to tcp port 5555\n";
  s.connect("tcp://localhost:5555");
  Music music;

  while (true) {
    cout << "Operacion? " << endl;
    string operation;
    cin >> operation;

    message m;
    m << operation;

    if (operation == "play") {
      string file;
      cin >> file;
      m << file;
    } else if (operation == "exit") {
      return 0;
    }

    s.send(m);

    message answer;
    s.receive(answer);

    string result;
    answer >> result;
    if (result == "list") {
			cout << "Hola" << endl;
     	size_t numSongs;
      answer >> numSongs;
      cout << "Available songs: " << numSongs << endl;
      for (size_t i = 0; i < numSongs; i++) {
        string s;
        answer >> s;
        cout << s << endl;
      }

     } else if (result == "file") {
      messageToFile(answer, "song.ogg");
      music.openFromFile("song.ogg");
      music.play();
      while (music.getStatus() == SoundSource::Playing) {
        cout << "suena!" << endl;
      }
    } else {
      cout << "Don't know what to do!!!" << endl;
    }
   }
  return 0;
}
