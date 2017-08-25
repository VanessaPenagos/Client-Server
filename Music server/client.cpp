#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <zmqpp/zmqpp.hpp>
#include <fstream>
#include <thread>
#include "safeQueue.h"

using namespace std;
using namespace sf;
using namespace zmqpp;

void messageToFile(const message &msg, const string &fileName) {
  const void *data;
  msg.get(&data, 1);
  size_t size = msg.size(1);

  ofstream ofs(fileName, ios::binary);
  ofs.write((char *)data, size);
}

void playSong(string nameSong, Music *music) {
  context ctx;
  socket s(ctx, socket_type::req);
  s.connect("tcp://localhost:5555");
  message m;
  message answer;
  string result;

  m << "play";
  m << nameSong;
  s.send(m);
  s.receive(answer);
  answer >> result;
  if(result == "file") {
    messageToFile(answer, "song.ogg");
    music->openFromFile("song.ogg");
    music->play();
  }
}

void playPlaylist(SafeQueue<string> &q, Music *music) {
  int sizeQueue = q.size();
  while(sizeQueue--){
    string nextSong;
    nextSong = q.dequeue();
    playSong(nextSong, music);
    while (music->getStatus() == SoundSource::Playing) {
			continue;
    }
  }
}

int main() {
  cout << "This is the client\n";

  context ctx;
  socket s(ctx, socket_type::req);
  cout << "Connecting to tcp port 5555\n";
  s.connect("tcp://localhost:5555");

  Music music;
  SafeQueue<string> playList;
  vector<string> songs;
  message m;
  message answer;
  size_t numSongs;
  string song, nameSong;

  m << "list";
  s.send(m);
  s.receive(answer);
  answer >> numSongs;

  for (size_t i = 0; i < numSongs; i++) {
    answer >> song;
    songs.push_back(song);
  }

  while (true) {
    cout << "Operation ? " << endl;
    string operation;
    cin >> operation;


    if (operation == "add"){
    	string addSong;
    	cout << "Name song: " << endl;
    	cin >> addSong;
    	playList.enqueue(addSong);
    	cout << "Ready" << endl;
    }

    if (operation == "list") {
      cin >> operation;
      if(operation == "songs") {
      }
      if (operation == "playlist") {
        string songlist;
        if (playList.isEmpty()){
          cout << "Playlist empty" << endl;
        } else {
          int playListSize = playList.size();
          for (int i = 0; i < playListSize; ++i) {
            songlist = playList.dequeue();
            cout << songlist << endl;
            playList.enqueue(songlist);
          }
        }
      }
    }
    if(operation == "play") {
      if(operation == "songs") {
        cout << "Name song? " << endl;
        cin >> nameSong;
        playSong(nameSong, &music);
      }

      if(operation == "play") { // Cambiar por playlist
        cout << "Si la llame " << endl;
        playPlaylist(ref(playList), &music);
      }
    }
    if (operation == "exit") {
      return 0;
    }


    // add to play if (operation == "playlist") {}
    //  add to list if ( operation == "songs") {}
    // if (operation == "delete") {}
    //
    // if (operation == "next") {}
    //
    // if (operation == "stop") {}
}
  return 0;
}
