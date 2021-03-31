// Authors: Lorenz Gonsa & Sabrina Loder, MMP2a FHS-MMT
#include "pch.h"
#include "CppUnitTest.h"
#include "GameObjects/GameObject.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(UGEUnitTests)
{
public:
	TEST_METHOD(SetNameTest)
	{
		std::string goName = "testobject";
		auto go = new GameObject(0, goName);
		_ASSERT(go->getName() == goName);
		delete go;
	}
};