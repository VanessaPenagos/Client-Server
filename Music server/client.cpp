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

int main() {
  cout << "This is the client\n";

  context ctx;
  socket s(ctx, socket_type::req);
  cout << "Connecting to tcp port 5555\n";
  s.connect("tcp://localhost:5555");
  Music music;
  SafeQueue<string> playList;

  while (true) {
    cout << "Operation ? " << endl;
    string operation;
    cin >> operation;

    message m;
    message answer;
    
    if (operation == "add"){
    	string addSong;
    	cout << "Name song: " << endl;
    	cin >> addSong;
    	playList.enqueue(addSong);
    	cout << "Encolada" << endl;
    }

    if (operation == "playlist"){
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

    if (operation == "play") {
    	m << operation;
    	string file;
    	cin >> file;
    	m << file;
      	s.send(m);
      	s.receive(answer);
    } else if (operation == "exit") {
      return 0;
    }

 //    string result;
 //    answer >> result;
 //    if (result == "list") {
 //    	size_t numSongs;
 //    	answer >> numSongs;
 //    	cout << "Available songs: " << numSongs << endl;
	// 	for (size_t i = 0; i < numSongs; i++) {
	// 		string s;
	// 		answer >> s;
	// 		cout << s << endl;
	// 	}
	// } else if (result == "file") {
	// 	messageToFile(answer, "song.ogg");
	// 	music.openFromFile("song.ogg");
	// 	music.play();
	// 	while (music.getStatus() == SoundSource::Playing) {
	// 		cout << "suena!" << endl;
	// 	}
 //    	} else if (result == "added") {}
 //    	else{
 //      	cout << "Don't know what to do!!!" << endl;
 //    	}
   }
  return 0;
}
