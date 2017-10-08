Client::Client(const char* host, const char* port, const char* cid) {
	this->host = host;;
	this->port = port;
	this->cid = cid;

	//TODO: connect to server
   	//TODO: IDENTIFY	
	/*
	fprintf(server_stream, "IDENTIFY %s\n", topic);
	char buffer[BUFSIZ];
	fgets(buffer, BUFSIZ, server_stream);
	puts(buffer);*/
}

void Client::publish(const char* topic, const char* message, size_t length) {
	fprintf(server_stream, "PUBLISH %s %s\n", topic, length);
	char buffer[BUFSIZ];
	fgets(buffer, BUFSIZ, server_stream);
	puts(buffer);
}

void Client::subscribe(const char* topic, Callback *callback) {
	fprintf(server_stream, "SUBSCRIBE %s\n", topic);
	char buffer[BUFSIZ];
	fgets(buffer, BUFSIZ, server_stream);
	puts(buffer);
	callback_map.insert(std::pair<char*, Callback*>(topic ,callback));
}

void Client::unsubscribe(const char* topic) {
	fprintf(server_stream, "UNSUBSCRIBE %s\n", topic);
	char buffer[BUFSIZ];
	fgets(buffer, BUFSIZ, server_stream);
	puts(buffer);
}

void Client::disconnect() {
	//TODO: user_id? 
	fprintf(server_stream, "DISCONNECT %s\n", cid);
	char buffer[BUFSIZ];
	fgets(buffer, BUFSIZ, server_stream);
	puts(buffer);
}

void Client::run() {
	///TODO: userr_id
	fprintf(server_stream, "RETRIEVE %s\n", cid);
	char buffer[BUFSIZ];
	fgets(buffer, BUFSIZ, server_stream);
	// TODO: parse topic
	puts(buffer);
	// TODO: get body w/ no newline
}

void Client::shutdown() {

}
