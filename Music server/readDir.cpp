#include <iostream>
#include <dirent.h>

using namespace std;

void list_dir() {
	DIR * folder;
	struct dirent * file;
	string file_name, dir = "Music";

	if (folder = opendir(dir.c_str())) { 
		while (file = readdir(folder)) {
			file_name = file->d_name;
			if ( file_name.find(".ogg") != string::npos && file_name != "." && file_name != "..") {
				cout << file->d_name << endl;  
			}
		}
	}
	else {
		cout << "No se pudo abrir el directorio " << endl;           
	}
	closedir(folder);
}

int main(int argc, char *argv[]) {
	list_dir();
	return 0;
}
