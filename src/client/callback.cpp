#include <iostream>

#include "../../include/ps_client/client.h"


void EchoCallback::run(Message &m){
	std::cout << m.body << std::endl;
}
