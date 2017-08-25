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

void startPlaylist(SafeQueue<string> &q, Music *music, bool &next, bool &stop) {
  while(true){
    string nextSong;
    nextSong = q.dequeue();
    q.enqueue(nextSong);
    playSong(nextSong, music);
    cout << "Next en la funcion:" << next << endl;
    while (music->getStatus() == SoundSource::Playing && !next && !stop) {
			continue;
    }
    if(stop) {
      music->stop();
      stop = false;
      break;
    }
    next = false;
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
  bool next, stop = false;

  for (size_t i = 0; i < numSongs; i++) {
    answer >> song;
    songs.push_back(song);
  }

  while (true) {
    cout << "Operation ? " << endl;
    string operation;
    cin >> operation;


    if (operation == "list") {
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

    if (operation == "add"){
    	string addSong;
    	cout << "Name song: " << endl;
    	cin >> addSong;
    	playList.enqueue(addSong);
    	cout << "Ready" << endl;
    }

    if (operation == "next") next = true;

    if (operation == "stop") stop = true;

    if(operation == "play") thread (startPlaylist, ref(playList), &music, ref(next), ref(stop)).detach();

    if (operation == "exit") return 0;

    //REPRODUCIR CANCIONES
    // if(operation == "songs") {
    //   cout << "Name song? " << endl;
    //   cin >> nameSong;
    //   playSong(nameSong, &music);
    // }
    //LISTAR CANCIONES
    // cin >> operation;
    // if(operation == "songs") {
    // }
    // if (operation == "playlist") {

    // add to play if (operation == "playlist") {}
    //  add to list if ( operation == "songs") {}
    // if (operation == "delete") {}
  }
  return 0;
}
