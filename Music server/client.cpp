#include <SFML/Audio.hpp>
#include <SFML/System/Time.hpp>
#include <iostream>
#include <string>
#include <zmqpp/zmqpp.hpp>
#include <fstream>
#include <thread>
#include <unistd.h>
#include "safeQueue.h"

using namespace std;
using namespace sf;
using namespace zmqpp;

void messageToFile(const message &msg, bool add) {
  const void *data;
  string fileName = "song.ogg";
  msg.get(&data, 1);
  size_t size = msg.size(1);

  if(add) {
  	ofstream ofs("song.ogg", ios::binary | ios_base::app);
		ofs.write((char*)data, size);
  } else {
  	ofstream ofs(fileName, ios::binary);
  	ofs.write((char *)data, size);
  }
}

void playSong(Music *music, message &answer) {
  string result;
  answer >> result;
  cout << "Aqui estoy !" << endl;
  if(result == "file") {
  	cout << "Si file hurra!" << endl;
    messageToFile(answer, false);
    music->openFromFile("song.ogg");
    music->play();
  }
}

void startPlaylist(SafeQueue<string> &q, Music *music, bool &next, bool &stop) {
  while(true){
    string nextSong, result;
	  int parts, part = 1;
    nextSong = q.dequeue();
    q.enqueue(nextSong);
	  context ctx;
	  socket s(ctx, socket_type::req);
	  s.connect("tcp://localhost:5555");
	  message m;
	  message answer;
	  float x = 10.0;

	  m << "play";
	  m << nextSong;
	  s.send(m);		//Se envia al servidor la solicitud de reproducir junto al nombre de la cancion
	  s.receive(answer);	//Se recibe el numero de partes que tiene la cancion
	  answer >> parts;
    playSong(music, answer);
    while (music->getStatus() == SoundSource::Playing && !next && !stop && part < parts) {
    	if ( (music->getPlayingOffset().asSeconds() - x )== music->getDuration().asSeconds()){
    		m << "part";
    		m << nextSong;
    		m << part;
    		s.send(m);
    		s.receive(answer);
    		answer >> result;
    		messageToFile(answer, true);
    		part++;
    	} else {
				continue;
    	}
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
