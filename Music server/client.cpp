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
  msg.get(&data, 2);
  size_t size = msg.size(2);

  if(add) {
  	ofstream ofs("song.ogg", ios::binary | ios_base::app);
		ofs.write((char*)data, size);
  } else {
  	ofstream ofs("song.ogg", ios::binary);
  	ofs.write((char*)data, size);
  }
}

void playSong(Music *music, message &answer) {
	string result;
  answer >> result;
  if(result == "file") {
    messageToFile(answer, false);
    music->openFromFile("song.ogg");
    music->play();
  }
}

void startPlaylist(SafeQueue<string> &q, Music *music, bool &next, bool &stop, socket &s) {
	message m, answer;
	string nextSong, result;
	int parts, part;
  float songDuration;

  while(true){

  	nextSong = q.dequeue();
		m << "play" << nextSong;
		s.send(m);
		s.receive(answer);
		answer >> parts;
    songDuration = 0;
    part = 1;

    //Play song :
		playSong(music, answer);
    songDuration = music->getDuration().asSeconds();
    while(music->getStatus() == SoundSource::Playing && !next && !stop) {
      if((songDuration - 10.0) <= music->getPlayingOffset().asSeconds() && part < parts) {
    		m << "part" << nextSong << part;
    		s.send(m);
        part++;
    		s.receive(answer);
    		answer >> result;  // no importa.
    		messageToFile(answer, true);
        songDuration = songDuration + music->getDuration().asSeconds();
      }
    }
	  q.enqueue(nextSong);  //Add song again
  	if(stop){
			music->stop();
			stop = false;
    	break;
    }
    next = false;
  }
}

bool existSong(vector<string> v, string nameSong){
	for(const auto& p : v){
		if (p == nameSong) return true;
  }
  return false;
}

int main() {
  cout << "This is the client\n";

  context ctx;
  socket s(ctx, socket_type::req);
  cout << "Connecting to tcp port 5555\n";
  s.connect("tcp://192.168.9.17:5555");

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

    cout << endl <<  "Operation:  " << endl << endl;
    string operation;
    cout << "list: -> list playlist " << endl;
    cout << "listall: -> list all songs " << endl;
    cout << "add: ->  add song to playlist " << endl;
    cout << "play: -> play playlist " << endl;
    cout << "next: -> play next song in playlist " << endl;
    cout << "stop: -> Pause playlist without restart" << endl;
    cout << "exit: -> Exit player"<< endl << endl;
    cin >> operation;

    // List playlist
    if (operation == "list") {
      string songlist;
      if (playList.isEmpty()){
        cout << "Playlist empty" << endl;
      }else {
        int playListSize = playList.size();
        for (int i = 0; i < playListSize; ++i) {
          songlist = playList.dequeue();
          cout << songlist << endl;
          playList.enqueue(songlist);
        }
      }
    }

    // List all songs in the server
    if (operation == "listall"){
    	for(const auto& p : songs){
    		cout << "-" << p << endl;
    	}
    }

    // Add song to playList
    if (operation == "add"){
    	string addSong;
    	cout << "Name song: " << endl;
    	cin >> addSong;
    	if (existSong(songs, addSong)){
    		playList.enqueue(addSong);
    		cout << "Ready!" << endl;
    	} else cout << "Not Found!" << endl;
    }
    //
     if (operation == "next"){  //Next song in playlist
       next = true;
     } else if(operation == "stop") { // Only pause the playlist, but no resume.
       stop = true;
     } else if(operation == "play") { //Play playlist
       thread (startPlaylist, ref(playList), &music, ref(next), ref(stop), ref(s)).detach();
     } else if(operation == "exit") {
       return 0;
     }
  }
  return 0;
}
