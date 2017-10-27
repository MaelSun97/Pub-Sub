#include <iostream>
#include <string>
#include <cstdlib>

#include "include/ps_client/client.h"

using std::cout;
using std::cin;
using std::endl;

string my_uid;

void usage(int status) {
	cout << "Usage:   chat.cpp [MY_USER_ID] [OTHER_USER_ID] [HOST] [PORT]" << endl;
	exit(status);
}

void *chat_generator(void *arg) {
	cout << "Hello" << endl;
	Client *client = (Client *)arg;

	string message;
	while (getline(cin, message)) {
		client->publish(my_uid.c_str(), message.c_str(), message.size());
		cout << endl;
	}
	return NULL;
}

int main(int argc, char* argv[]) {
	/* Pare command line options */
	if (argc != 5) {
		usage(1);
	}
	my_uid = argv[1];
	const char* other_uid = argv[2];
	const char* host = argv[3];
	const char* port = argv[4];


	cout << "Look what you made me do" << endl;

	Client c(host, port, my_uid.c_str());

	Thread generator;
	EchoCallback e;

	generator.start(chat_generator, (void *)&c);
	generator.detach();

	c.subscribe(other_uid, &e);
	c.run();

	cout << "Goodbye" << endl;
	
	return EXIT_SUCCESS;
}
