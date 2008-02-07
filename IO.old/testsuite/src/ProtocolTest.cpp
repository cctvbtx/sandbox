//
// ProtocolTest.cpp
//
// $Id: //poco/Main/template/test.cpp#6 $
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#include "ProtocolTest.h"
#include "TestProtocol.h"
#include "Poco/IO/Protocol.h"
#include "Poco/IO/ProtocolStream.h"
#include "Poco/IO/SerialConfig.h"
#include "Poco/IO/SerialChannel.h"
#include "Poco/AutoPtr.h"
#include "Poco/Exception.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include <sstream>


using Poco::IO::SerialConfig;
using Poco::IO::SerialChannel;
using Poco::IO::ProtocolInputStream;
using Poco::IO::ProtocolOutputStream;
using Poco::IO::Serial;
using Poco::AutoPtr;
using Poco::CircularReferenceException;
using Poco::NullPointerException;
using Poco::NotFoundException;


ProtocolTest::ProtocolTest(const std::string& name): 
	CppUnit::TestCase(name),
	_serialConfig(SerialConfig::BPS_9600, 
	SerialConfig::DATA_BITS_EIGHT, 
	'N', 
	SerialConfig::START_ONE,
	SerialConfig::STOP_ONE,
	SerialConfig::FLOW_CTRL_SOFTWARE,
	0x11,//xOn
	0x13,//xOff
	false,//use EOF
	0,//EOF 
	100,//buffer size
	100)//timeout
{
#if defined(POCO_OS_FAMILY_WINDOWS)
	_serialName1 = "COM1";
	_serialName2 = "COM2";
#elif defined(POCO_OS_FAMILY_UNIX)
	throw NotImplementedException("Not implemented");
#else
	throw NotImplementedException("Not implemented");
#endif
}


ProtocolTest::~ProtocolTest()
{
}


void ProtocolTest::testOne()
{
	Serial* pCom1 = new Serial(_serialName1, _serialConfig);
	Serial* pCom2 = new Serial(_serialName2, _serialConfig);

	TestProtocol tp1(pCom1, 1);
	TestProtocol tp2(pCom2, 1);

	std::string rawData = "<data1>123</data1>";

	tp1.write("123", 3);
	assert (tp1.readRaw() == rawData);
	tp1.send();
	assert (tp1.readRaw() == "");
	std::string str;
	tp2.receive(str);
	assert (tp2.readRaw() == rawData);
	assert ("123" == str);
    tp2.clear();
	assert (tp2.readRaw() == "");

	tp1.write("123");
	assert (tp1.readRaw() == "");
	tp2.receive(str);
	assert (tp2.readRaw() == rawData);
	assert ("123" == str);
}


void ProtocolTest::testTwo()
{
	Serial* pCom1 = new Serial(_serialName1, _serialConfig);
	Serial* pCom2 = new Serial(_serialName2, _serialConfig);

	AutoPtr<TestProtocol> pTp1 = new TestProtocol(pCom1, 1);
	AutoPtr<TestProtocol> pTp2 = new TestProtocol(pCom2, 1);
	TestProtocol* pTp3 = new TestProtocol(2); 
	TestProtocol* pTp4 = new TestProtocol(2); 

	pTp1->add(pTp3);
	pTp2->add(pTp4);

	std::string rawData = "<data2><data1>123</data1></data2>";

	pTp1->write("123", 3);
	assert (pTp1->readRaw() == rawData);
	pTp1->send();
	assert (pTp1->readRaw() == "");
	std::string str;
	pTp2->receive(str);
	assert (pTp2->readRaw() == rawData);
	assert ("123" == str);
    pTp2->clear();
	assert (pTp2->readRaw() == "");

	pTp1->write("123");
	assert (pTp1->readRaw() == "");
	pTp2->receive(str);
	assert (pTp2->readRaw() == rawData);
	assert ("123" == str);

	str.clear();
	pTp2->send();
	pTp1->receive(str);
	assert ("123" == str);

	try
	{
		pTp3->add(pTp1);
		fail ("must fail");
	}
	catch (CircularReferenceException&) {}

	try
	{
		pTp3->add(pTp3);
		fail ("must fail");
	}
	catch (CircularReferenceException&) {}

	try
	{
		pTp3->add(0);
		fail ("must fail");
	}
	catch (NullPointerException&) {}

	try
	{
		pTp3->remove("xyz");
		fail ("must fail");
	}
	catch (NotFoundException&) {}

	rawData = "<data2>321</data2>";

	pTp3->write("321", 3);
	std::string s = pTp3->readRaw();
	assert (pTp3->readRaw() == rawData);
	pTp3->send();
	assert (pTp3->readRaw() == "");
	str.clear();
	pTp4->receive(str);
	assert (pTp4->readRaw() == rawData);
	assert ("321" == str);
    pTp4->clear();
	assert (pTp4->readRaw() == "");

	pTp1->detach();//does nothing for a root protocol
	pTp1->remove("TestProtocol2");
	pTp4->detach();

	rawData = "<data1>123</data1>";

	pTp1->write("123", 3);
	assert (pTp1->readRaw() == rawData);
	pTp1->send();
	assert (pTp1->readRaw() == "");
	str.clear();
	pTp2->receive(str);
	assert (pTp2->readRaw() == rawData);
	assert ("123" == str);
    pTp2->clear();
	assert (pTp2->readRaw() == "");

	pTp1->write("123");
	assert (pTp1->readRaw() == "");
	pTp2->receive(str);
	assert (pTp2->readRaw() == rawData);
	assert ("123" == str);

	rawData = "<data1>321</data1>";

	str.clear();
	pTp1->write("321");
	assert (pTp1->readRaw() == "");
	pTp2->receive(str);
	assert (pTp2->readRaw() == rawData);
	assert ("321" == str);

	pTp1->add(new TestProtocol(2));
	pTp2->add(new TestProtocol(2));

	rawData = "<data2><data1>456</data1></data2>";

	str.clear();
	pTp1->write("456");
	assert (pTp1->readRaw() == "");
	pTp2->receive(str);
	assert (pTp2->readRaw() == rawData);
	assert ("456" == str);
}


void ProtocolTest::testChain()
{
	const int channels = 10;
	TestProtocol tp1(0);
	TestProtocol tp2(0);

	tp1.setChannel(new Serial(_serialName1, _serialConfig));
	tp2.setChannel(new Serial(_serialName2, _serialConfig));
	
	std::ostringstream pre;
	std::ostringstream post;
	post << "</data0>";
	int i = 1;
	for (; i < channels; ++i)
	{
		tp1.add(new TestProtocol(i)); 
		tp2.add(new TestProtocol(i)); 
		pre << "<data" << channels - i << '>';
		post << "</data" << i << '>';
	}
	pre << "<data0>";

	std::string pr = pre.str();;
	std::string po = post.str();
	std::string str;
	tp1.write("1234567890");
	tp2.receive(str);

	assert (pre.str() + str + post.str() == tp2.readRaw());
	assert ("1234567890" == str);
}


void ProtocolTest::testStreams()
{
	Serial* pCom1 = new Serial(_serialName1, _serialConfig);
	Serial* pCom2 = new Serial(_serialName2, _serialConfig);

	AutoPtr<TestProtocol> pTp1 = new TestProtocol(pCom1, 1);
	AutoPtr<TestProtocol> pTp2 = new TestProtocol(pCom2, 1);
	TestProtocol* pTp3 = new TestProtocol(2);
	TestProtocol* pTp4 = new TestProtocol(2);
	pTp1->add(pTp3);
	pTp2->add(pTp4);

	ProtocolOutputStream sos(pTp1);
	ProtocolInputStream sis(pTp2);

	sos << "1234567890" << std::endl;
	std::string str;
	sis >> str;
	assert("1234567890" == str);

	std::string rawData = "<data2><data1>0987654321</data1></data2>";
	pTp1->writeRaw(rawData); pTp3->send();
	sis >> str;
	assert("0987654321" == str);

	str.clear();
	sos << "5432109876" << std::flush;
	pTp2->receive(str);
	assert (pTp2->readRaw() == "<data2><data1>5432109876</data1></data2>");
	assert ("5432109876" == str);
}


void ProtocolTest::setUp()
{
}


void ProtocolTest::tearDown()
{
}


CppUnit::Test* ProtocolTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ProtocolTest");

	CppUnit_addTest(pSuite, ProtocolTest, testOne);
	CppUnit_addTest(pSuite, ProtocolTest, testTwo);
	CppUnit_addTest(pSuite, ProtocolTest, testChain);
	CppUnit_addTest(pSuite, ProtocolTest, testStreams);

	return pSuite;
}
