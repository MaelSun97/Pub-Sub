#pragma once
#include <string> 
#include <map>

using std::string

#define QUEUE_MAX 64;

typedef void *(*thread_func)(void *);

class Client {
	public:
		Client(const char*, const char*, const char*);
		void publish(const char*, const char*, size_t);
		void subscribe(const char*, Callback*);
		void unsubsrcibe(const char*);
		void disconnect();
		void run();
		bool shutdown();
	private:
		const char* host;
		const char* port;
		const char* cid;
		FILE* server_stream;
		std::map<char*, Callback*> callback_map;
};

class Callback {
	public:
		void virtual run(Message&);
};

struct Message {
	string type;
	string topic;
	string sender;
	size_t nonce;
	string body;	
};

class Thread {
	public:
		void start(thread_func, void*);
		void join(void**);
		void detach();
	private:
		pthread_t thread;
};

class Queue {
	public:
		Queue(Message);
		void push(const Message&);
		Message pop();

	private:
    	std::queue<Message> data;
	    Message sentinel;
		pthread_mutex_t lock;
		pthread_cond_t fill;
		pthread_cond_t empty;
};
