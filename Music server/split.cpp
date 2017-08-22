#include <fstream>
#include <iostream>
#include <string>

using namespace std;

string split(string s, char del){

    string nameSong = "";
	for (int i = 0; i < int(s.size()); i++) {
        if (s[i] != del)
            nameSong += s[i];
		else
			return nameSong;  
  	}
}


int main() {
	string myString = "Hola.ogg";
	myString = split(myString, '.');
	cout << " My string :: " << myString << endl;
	
	return 0;
}
