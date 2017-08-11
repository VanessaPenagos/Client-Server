#include <iostream>
#include <string>
#include <zmqpp/zmqpp.hpp>

using namespace std;
using namespace zmqpp;

int main(int argc, char** argv) {
	cout << "This is the client\n";

	if (argc != 4) {
		cerr << "Should be called: " << argv[0] << " <op> operand1 operand2\n";
		return 1;
	}

	context ctx;
	socket s(ctx, socket_type::req);
	s.connect("tcp://localhost:5555");
	cout << "Sending  some work!\n";
	message answer;
	s.receive(answer);


	string result;
	answer >> result;
	if(result == "List") {
		int numsongs;
		answer >> numsongs;
		cout << "number of songs : " << numsongs;
		for(int i = 0; i < numsongs; i++) {
			string s;
			answer >> s;
		}
	}

	int a;
	answer >> a;
	cout << "Answer is " << a << endl;

    cout << "Finished\n";
	return 0;
}
