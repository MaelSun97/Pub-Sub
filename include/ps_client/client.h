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
		void virtual run(Message&) = 0;
};

class EchoCallback : public Callback {
	public:
		void run(Message&);
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
		pthread_cond_t cond;
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

		FILE *socket_dial(const char*, const char*);
		const char* host;
		const char* port;
		const char* uid;
		size_t nonce;
		bool finished;
		std::map<string, Callback*> callback_map;
		Queue outgoing;
		Queue incoming;
		pthread_mutex_t lock;
	private:
		Thread thread_pub;
		Thread thread_retr;
		Thread thread_call;
};
