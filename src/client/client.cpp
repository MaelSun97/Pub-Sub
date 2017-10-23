#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <netdb.h>

#include "../../lib/client.h"
#include "../../lib/macros.h"

Client::Client(const char* host, const char* port, const char* cid) {
	this->host = host;
	this->port = port;
	this->uid = cid;
	srand(time(NULL));
	this->nonce = rand() % 1000 + 100;
	this->finished = false;

	/* Connect to server */
	server_stream = socket_dial(host, port);
	if (server_stream == NULL) {
		exit(1);
	}

	/* Identify */
	Message m;
	m.type = "IDENTIFY";
	m.sender = uid;
	m.nonce = nonce;
	outgoing.push(m);
}

FILE *Client::socket_dial(const char *host, const char *port) {
	/* Lookup server address info */
	struct addrinfo *results;
	struct addrinfo hints;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	int status;
	if ((status = getaddrinfo(host, port, &hints, &results)) != 0) {
		perror("getaddrinfo");
		return NULL;
	}

	/* Create Socket */
	int sockfd = -1;
	for (struct addrinfo *p = results; p != NULL && sockfd < 0; p = p->ai_next) {
		/*Allocate socket */
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0){
			perror("socket");
			continue;
		}

		/* Connect */
		if (connect(sockfd, p->ai_addr, p->ai_addrlen) < 0) {
			perror("connect");
			continue;
		}
	}

	freeaddrinfo(results);

	if (sockfd < 0) {
		return NULL;
	}

	/* Open file stream from socket file descriptor */
	FILE *client_file = fdopen(sockfd, "w+");
	if (client_file == NULL) {
		perror("fdopen");
		return NULL;
	}

	return client_file;

}

void Client::publish(const char* topic, const char* message, size_t length) {
	Message m;
	m.type = "PUBLISH";
	m.topic = topic;
	m.sender = uid;
	m.nonce = nonce;
	m.length = length;
	m.body = message;
	outgoing.push(m);
}

void Client::subscribe(const char* topic, Callback *callback) {
	Message m;
	m.type = "SUBSCRIBE";
	m.topic = topic;
	m.sender = uid;
	m.nonce = nonce;
	outgoing.push(m);
	callback_map.insert(std::pair<const char*, Callback*>(topic ,callback));
}

void Client::unsubscribe(const char* topic) {
	Message m;
	m.type = "UNSUBSCRIBE";
	m.topic = topic;
	m.sender = uid;
	m.nonce = nonce;
	outgoing.push(m);
}

void Client::disconnect() {
	Message m;
	m.type = "DISCONNECT";
	m.sender = uid;
	m.nonce = nonce;
	outgoing.push(m);
	pthread_mutex_lock(&lock);
	finished = true;
	pthread_mutex_unlock(&lock);
}

void Client::run() {
	thread_pub.start(thread_pub_func, this);
	thread_retr.start(thread_retr_func, this);
	thread_call.start(thread_call_func, this);

	thread_pub.detach();
	thread_retr.detach();
	thread_call.detach();
}

bool Client::shutdown() {
	pthread_mutex_lock(&lock);
	bool result = finished;
	pthread_mutex_unlock(&lock);
	return result;
}

void *thread_pub_func(void *args) {
	Client* client = (Client*)args;

	while (!client->shutdown()) {
		Message m = (client->outgoing).pop();
		fprintf(client->server_stream, "%s %s %lu\n", m.type.c_str(), m.topic.c_str(), m.length);
		char buffer[BUFSIZ];
		fgets(buffer, BUFSIZ, client->server_stream);
		puts(buffer);
	}
	return NULL;
}

void *thread_retr_func(void *args) {
	Client* client = (Client*)args;

	while (!client->shutdown()) {
		fprintf(client->server_stream, "RETRIEVE %s\n", client->uid);
		char buffer[BUFSIZ];
		puts(buffer);
		char topic[BUFSIZ];
		char sender[BUFSIZ];
		long unsigned length;
		fscanf(client->server_stream, "MESSAGE %s FROM %s LENGTH %lu", topic, sender, &length);
		char body[length];
		fgets(body, length, client->server_stream);
		Message m;
		m.type = "MESSAGE";
		m.topic = topic;
		m.sender = sender;
		m.length = length;
		m.body = body;
		(client->incoming).push(m);
	}
	return NULL;
}

void *thread_call_func(void *args) {
	Client* client = (Client*)args;
	while (!client->shutdown()) {
		Message m = (client->incoming).pop();
		(client->callback_map)[m.type.c_str()]->run(m);
	}
	return NULL;
}
