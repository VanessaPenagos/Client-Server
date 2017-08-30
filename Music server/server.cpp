#include <zmqpp/zmqpp.hpp>
#include <iostream>
#include <string>
#include <cassert>
#include <unordered_map>
#include <fstream>
#include <dirent.h>


using namespace std;
using namespace zmqpp;

vector<char> readFileToBytes(const string& fileName) {
  cout << fileName << " File" << endl;
	ifstream ifs(fileName, ios::binary | ios::ate);
	ifstream::pos_type pos = ifs.tellg();

	vector<char> result(pos);
  ifs.seekg(0, ios::beg);
	ifs.read(result.data(), pos);

	return result;
}

void fileToMesage(const string& fileName, message& msg, int part) {
  vector<char> bytes = readFileToBytes("Music/"+fileName+"-"+to_string(part)+".ogg");
	msg.add_raw(bytes.data(), bytes.size());
}


string split(string s, char del){
  string nameSong = "";
  for (int i = 0; i < int(s.size()); i++) {
  	if (s[i] != del)
      nameSong += s[i];
  	else
			break;
  }
	 return nameSong;
}

void readDir(string dir, unordered_map<string,int> &songs) {
  DIR * folder;
  struct dirent * file;
  string file_name, key;
  if ((folder = opendir(dir.c_str()))) {
    while ((file = readdir(folder))) {
      file_name = file->d_name;
      if ( file_name.find(".ogg") != string::npos) {
        key = split(file_name,'-');
        if (songs.count(key)){
          songs[key]++;
        } else {
          songs[key] = 1;
        }
      }
    }
  }
  closedir(folder);
}

int main(int argc, char** argv) {
  context ctx;
  socket s(ctx, socket_type::rep);
  s.bind("tcp://192.168.9.17:5555");

  string path(argv[1]);
  unordered_map<string,int> songs;
  readDir(path, songs);

  cout << "Start serving requests!\n";
  while(true) {
    message m;
    s.receive(m);

    string op;
    m >> op;

    cout << "Action:  " << op << endl;

    if(op == "list") {  // Use case 1: Send the songs
      message n;
      n << songs.size();
      for(const auto& p : songs)
        n << p.first;
      s.send(n);

    }else if(op == "play") {
      string songName;
      m >> songName;
      cout << "sending parts of " << songName
           << " number: " << songs[songName] << endl;
			message n;
			n << songs[songName] << "file";
			fileToMesage(songName, n, 0);
			s.send(n);

    }else if(op == "part") {
      string songName;
      int part;
      m >> songName >> part;
      cout << "sending song " << songName
           << " part " << songs[songName] << endl;
      message n;
      n << songs[songName] << "file";
      fileToMesage(songName, n, part);
      s.send(n);

    }else {
      cout << "Invalid operation requested!!\n";
    }
  }
  cout << "Finished\n";
  return 0;
}
