#pragma once
#include <string> 
#include <map>
#include <queue>

using std::string;

#define QUEUE_MAX 64

typedef void *(*thread_func)(void *);

struct Message {
	string type;
	string topic;
	string sender;
	size_t nonce;
	size_t length;
	string body;	
};

class Callback {
	public:
		void virtual run(Message&);
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
		Queue();
		void push(const Message&);
		Message pop();

	private:
    	std::queue<Message> data;
		pthread_mutex_t lock;
		pthread_cond_t fill;
		pthread_cond_t empty;
};

void *thread_pub_func(void *);
void *thread_retr_func(void *);
void *thread_call_func(void *);

class Client {
	public:
		Client(const char*, const char*, const char*);
		void publish(const char*, const char*, size_t);
		void subscribe(const char*, Callback*);
		void unsubscribe(const char*);
		void disconnect();
		void run();
		bool shutdown();
	private:
		FILE *socket_dial(const char*, const char*);
		const char* host;
		const char* port;
		const char* uid;
		size_t nonce;
		FILE* server_stream;
		bool finished;
		std::map<const char*, Callback*> callback_map;
		Queue outgoing;
		Queue incoming;
		Thread thread_pub;
		Thread thread_retr;
		Thread thread_call;
		pthread_mutex_t lock;
};

struct Thread_func_args {
	Queue* queue;
	const char* uid;
	std::map<const char*, Callback*>* map;
	FILE* stream;
};
