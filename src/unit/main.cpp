// SYSTEM INCLUDES

#include <gtest/gtest.h>
// C++ PROJECT INCLUDES
#include <ps_client/client.h>

namespace {

	// The fixture for testing class Foo.
	class ClientTest : public ::testing::Test {
		protected:
			ClientTest() {
			}
	};

	
	// Tests the Client methods which push to outgoing
	TEST_F(ClientTest, ClientMessages) {
		const char* host = "localhost";
		const char* port = "9411";	
		const char*	cid = "pbui";
		Client c(host, port, cid);
		string topic = "topic";
		string message = "message";
		size_t length = 7;
		EchoCallback e;
		
		c.publish(topic.c_str(), message.c_str(), length);
		Message m1 = c.outgoing.pop();
		EXPECT_EQ(m1.topic, topic);
		EXPECT_EQ(m1.body, message);
		EXPECT_EQ(m1.length, length);
		EXPECT_EQ(m1.type, "PUBLISH");
	
		c.subscribe(topic.c_str(), &e);
		m1 = c.outgoing.pop();
		EXPECT_EQ(m1.topic, topic);
		EXPECT_NE(c.callback_map.find(topic), c.callback_map.end());
		EXPECT_EQ(c.callback_map[topic], &e);
		EXPECT_EQ(m1.type, "SUBSCRIBE");
	
		c.unsubscribe(topic.c_str());
		m1 = c.outgoing.pop();
		EXPECT_EQ(m1.topic, topic);
		EXPECT_EQ(m1.type, "UNSUBSCRIBE");
		EXPECT_EQ(c.callback_map.find(topic), c.callback_map.end());
	
		c.disconnect();	
		m1 = c.outgoing.pop();
		EXPECT_EQ(m1.type, "DISCONNECT");
		EXPECT_EQ(m1.sender, "pbui");

	}


	

	// Tests that Foo does Xyz.
	//TEST_F(FooTest, DoesXyz) {
		// Exercises the Xyz feature of Foo.
	//}
	class QueueTest:  public ::testing::Test {
		protected:
			QueueTest(){
			}

	};
	

	TEST_F(QueueTest, QueuePushPop) {
		Queue q;
		Message m, m1;
		m.topic = "topic";
		m.body = "body";
		m.length = 4;
		q.push(m);
		m1 = q.pop();
		EXPECT_EQ(m.topic, m1.topic);
		EXPECT_EQ(m.body, m1.body);
		EXPECT_EQ(m.length, m1.length);
	}
	class EchoCallbackTest: public ::testing::Test {
		protected: 
			EchoCallbackTest(){			
			}
		};

	TEST_F(EchoCallbackTest, CallbackRun){
		Message m;
		m.body = "body";
		EchoCallback e;
		e.run(m);	
	}
	
	class ThreadTest: public ::testing:Test{
		protected:
			ThreadTest(){}
		public:
			void* func(void *arg){int n = 3; return (void*) &n;}
		};
	TEST_F(ThreadTest, thread){
		Thread t;
	//	t.start(func, NULL);
		
		}

}  // namespace


int main(int argc, char* argv[]) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}


