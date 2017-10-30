#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <netdb.h>
#include <iostream>
#include <unistd.h>

#include "../../include/ps_client/client.h"
#include "../../include/ps_client/macros.h"

Client::Client(const char* host, const char* port, const char* cid) {
	this->host = host;
	this->port = port;
	this->uid = cid;
	srand(time(NULL));
	this->nonce = rand() % 1000 + 100;
	this->finished = false;
	this-> lock = PTHREAD_MUTEX_INITIALIZER;
}

FILE *Client::socket_dial(const char *host, const char *port) {
	/* Lookup server address info */
	struct addrinfo *results;
	struct addrinfo hints = {0};
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	int status;
	if ((status = getaddrinfo(host, port, &hints, &results)) != 0) {
		std::cerr << "getaddrinfo: " << gai_strerror(status) << std::endl;
		return NULL;
	}

	/* Create Socket */
	int sockfd = -1;
	for (struct addrinfo *p = results; p != NULL && sockfd < 0; p = p->ai_next) {
		/*Allocate socket */
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) < 0){
			//perror("socket");
			continue;
		}

		/* Connect */
		if (connect(sockfd, p->ai_addr, p->ai_addrlen) < 0) {
			//perror("connect");
			close(sockfd);
			sockfd = -1;
			continue;
		}
		break;
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
	m.body = "SUB";
	outgoing.push(m);
	callback_map.insert(std::pair<string, Callback*>(m.topic, callback));
}

void Client::unsubscribe(const char* topic) {
	Message m;
	m.type = "UNSUBSCRIBE";
	m.topic = topic;
	m.sender = uid;
	m.nonce = nonce;
	m.body = "UNSUB";
	outgoing.push(m);
	callback_map.erase(m.topic);
}

void Client::disconnect() {
	Message m;
	m.type = "DISCONNECT";
	m.sender = uid;
	m.body = "DISC";
	m.nonce = nonce;
	outgoing.push(m);
}

void Client::run() {
	thread_pub.start(thread_pub_func, this);
	thread_pub.detach();

	thread_retr.start(thread_retr_func, this);
	thread_retr.detach();

	while (!shutdown()) {
		Message m = incoming.pop();
		
		if ( callback_map.find(m.topic) == callback_map.end() ) {
			std::cout << "Key not found: " << m.topic << std::endl;
		} else {
			callback_map[m.topic]->run(m);

		}
		
	}	
}

bool Client::shutdown() {
	pthread_mutex_lock(&lock);
	bool result = finished;
	pthread_mutex_unlock(&lock);
	return result;
}

void *thread_pub_func(void *args) {
	Client* client = (Client*)args;

	/* Connect to server */
	FILE* server_stream_pub = client->socket_dial(client->host, client->port);
	if (server_stream_pub == NULL) {
		exit(1);
	}

	/* Identify */
	fprintf(server_stream_pub, "IDENTIFY %s %lu\n", client->uid, client->nonce);
	char buffer[BUFSIZ];
	fgets(buffer, BUFSIZ, server_stream_pub);
	//puts(buffer); *****uncomment for logging messages*****

	while (!client->shutdown()) {
		Message m = (client->outgoing).pop();
		
			
		if (m.type == "DISCONNECT") {
			fprintf(server_stream_pub, "%s %s %lu\n", m.type.c_str(), m.sender.c_str(), m.nonce);
		}else if (m.type == "SUBSCRIBE" || m.type == "UNSUBSCRIBE") {
			fprintf(server_stream_pub, "%s %s\n", m.type.c_str(), m.topic.c_str());
		}else if (m.type == "PUBLISH") {
			fprintf(server_stream_pub, "%s %s %lu\n", m.type.c_str(), m.topic.c_str(), m.length);
			fwrite(m.body.c_str(), 1, m.body.size(), server_stream_pub);
		}else{
			std::cerr << "Unrecognized message type" << std::endl;
		}
		char buffer[BUFSIZ];
		fgets(buffer, BUFSIZ, server_stream_pub);
		//puts(buffer); *****uncomment for logging messages*****
		if (m.type == "DISCONNECT") {
			pthread_mutex_lock(&(client->lock));
			client->finished = true;
			pthread_mutex_unlock(&(client->lock));
		}
	}
	return NULL;
}

void *thread_retr_func(void *args) {
	Client* client = (Client*)args;

	/* Connect to server */
	FILE* server_stream_retr = client->socket_dial(client->host, client->port);
	if (server_stream_retr == NULL) {
		exit(1);
	}

	/* Identify */
	fprintf(server_stream_retr, "IDENTIFY %s %lu\n", client->uid, client->nonce);
	char buffer[BUFSIZ];
	fgets(buffer, BUFSIZ, server_stream_retr);
	//puts(buffer); *****uncomment for logging messages*****


	/* Retrieve */
	while (!client->shutdown()) {
		fprintf(server_stream_retr, "RETRIEVE %s\n", client->uid);
		char topic[BUFSIZ];
		char sender[BUFSIZ];
		long unsigned length;

		char message[BUFSIZ];
		if (fgets(message, BUFSIZ, server_stream_retr) == NULL){
			exit(0);
		}
		
		 if ((sscanf(message, "MESSAGE %s FROM %s LENGTH %lu\n", topic, sender, &length)) < 3) {
			std::cout << "Unrecognized message retrived" << std::endl;
			std::cout << message << std:: endl;
			continue;
		}
		/*
		std::cout << "topic = " << topic << std::endl;
		std::cout << "sender = " << sender << std::endl;
		std::cout << "length = " << length << std::endl;
		*/
		char buffer[128] = "";
		sprintf(buffer, "%%%luc", length);

		char body[length+1];
		memset(body, 0, length+1);
		fscanf(server_stream_retr, buffer, body);
		/*
		if (strlen(body) != length) {
			std::cout << "body: >" << int(body[0]) << "<" << std::endl;
		}
		*/
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
