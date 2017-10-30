#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include "include/ps_client/client.h"

using std::cout;
using std::cin;
using std::endl;

string my_uid_topic;

void usage(int status) {
	cout << "Usage:   chat [MY_USER_ID] [OTHER_USER_ID] [HOST] [PORT]" << endl;
	exit(status);
}

void *chat_generator(void *arg) {
	Client *client = (Client *)arg;
	string message;
	while (getline(cin, message)) {
		client->publish(my_uid_topic.c_str(), message.c_str(), message.size());
		cout << endl;
	}
	return NULL;
}

int main(int argc, char* argv[]) {
	/* Pare command line options */
	if (argc != 5) {
		usage(1);
	}

	string my_uid = argv[1];
	my_uid_topic = my_uid+"-topic";
	const char* other_uid = argv[2];
	const char* host = argv[3];
	const char* port = argv[4];
	char other_uid_topic[BUFSIZ];
	sprintf(other_uid_topic, "%s-topic", other_uid);

	Client c(host, port, my_uid.c_str());

	cout << "Welcome to our completely unnecessary chatting service! ";
	cout << "Type a message and press enter to send." << endl;

	Thread generator;
	EchoCallback e;

	generator.start(chat_generator, (void *)&c);
	generator.detach();

	c.subscribe(other_uid_topic, &e);
	c.run();

	return EXIT_SUCCESS;
}

