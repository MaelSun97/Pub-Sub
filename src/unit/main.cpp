// SYSTEM INCLUDES

#include <gtest/gtest.h>

// C++ PROJECT INCLUDES

namespace {

	// The fixture for testing class Foo.
	class ClientTest : public ::testing::Test {
		protected:
			ClientTest() {
				// You can do set-up work for each test here.
			}

			virtual ~ClientTest() {
				// You can do clean-up work that doesn't throw exceptions here.
			}
			// Objects declared here can be used by all tests in the test case for Foo.
	};

	/*
	// Tests the Client methods which push to outgoing
	TEST_F(ClientTest, ClientMessages) {
		Client c;
		const char* topic = "topic";
		const char* message = "message";
		c.publish(topic, message, length);
		//EXPECT_EQ(0, f.Bar(input_filepath, output_filepath));
	}
	*/

	// Tests that Foo does Xyz.
	//TEST_F(FooTest, DoesXyz) {
		// Exercises the Xyz feature of Foo.
	//}

}  // namespace


int main(int argc, char* argv[]) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
