#include "pch.h"


class IPCTestProducer : public ::testing::Test 
{
public:
	IPCTestProducer() {}
	void SetUp(){

	}

	void TearDown() {
	
	}

	~IPCTestProducer() {
		
	}

};


TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}

TEST(TestIPCProducer, producer) {
	
}

TEST(TestIPProducer, producer) {

}
/*
TEST_F(testFixture, producer_fixture) {

}*/