//
// DatagramLocalSocketTest.cpp
//
// $Id: //poco/svn/Net/testsuite/src/DatagramLocalSocketTest.cpp#2 $
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
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


#include "DatagramLocalSocketTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "UDPEchoServer.h"
#include "UDPLocalEchoServer.h"
#include "Poco/Sockets/DatagramSocket.h"
#include "Poco/Sockets/SocketAddress.h"
#include "Poco/Sockets/SocketException.h"
#include "Poco/Timespan.h"
#include "Poco/Timestamp.h"
#include "Poco/Stopwatch.h"
#include "Poco/Environment.h"
#include "Poco/File.h"
#include "Poco/FileStream.h"
#include <iostream>
#include <sstream>


using Poco::Sockets::Socket;
using Poco::Sockets::DatagramSocket;
using Poco::Sockets::SocketAddress;
using Poco::Sockets::Address;
using Poco::Timespan;
using Poco::Timestamp;
using Poco::Stopwatch;
using Poco::Environment;
using Poco::File;
using Poco::FileOutputStream;
using Poco::TimeoutException;
using Poco::InvalidArgumentException;
using Poco::IOException;


DatagramLocalSocketTest::DatagramLocalSocketTest(const std::string& name): CppUnit::TestCase(name)
{
}


DatagramLocalSocketTest::~DatagramLocalSocketTest()
{
}


void DatagramLocalSocketTest::testDatagramSocketPerformance()
{
	Timestamp::TimeDiff local, net;
	std::size_t initBufSize = 1;
	std::size_t initReps = 1;
	double locData = 0.0, locTime = 0.0, netData = 0.0, netTime = 0.0;

	std::cout << std::endl << "OS Name:         " << Environment::osName() << std::endl;
	std::cout << "OS Version:      " << Environment::osVersion() << std::endl;
	std::cout << "OS Architecture: " << Environment::osArchitecture() << std::endl;

	std::ostringstream os;
	os << Environment::osName() << '-' 
		<< Environment::osVersion() << '-' 
		<< Environment::osArchitecture()
		<< "-UDP.csv";
	File f(os.str());
	if (f.exists()) f.remove();
	FileOutputStream fos(os.str());

	for (std::size_t repetitions = initReps;
			repetitions <= 100000;
			repetitions *= 10)
	{
		for (std::size_t bufSize = initBufSize; bufSize < 20000; bufSize *= 2)
		{
			char* pBuf = new char[bufSize];
			{
				UDPLocalEchoServer echoServer(bufSize);
				DatagramSocket ss(SocketAddress("/tmp/poco.client.udp.sock"), true);
				
				SocketAddress addr(echoServer.address().toString());
				ss.connect(addr);
				int recv = 0, sent = 0;
				Stopwatch sw; int i = 0;
				for (; i < repetitions; ++i)
				{
					sent = 0; recv = 0; local = 0;

					do
					{
						int s;
						sw.restart();
						s = ss.sendBytes(pBuf + sent, bufSize - sent);
						sw.stop();
						local += sw.elapsed();
						sent += s;
					} while (sent < bufSize);

					do
					{
						int r;
						sw.restart();
						r = ss.receiveBytes(pBuf + recv, bufSize - recv);
						sw.stop();
						local += sw.elapsed();
						recv += r;
					} while (recv < bufSize);

					locData += sent;
					locData += recv;
					locTime += local;

					poco_assert (sent == bufSize && recv == bufSize);
				}

				std::cout << "Local UDP socket, " << i << " repetitions, " << bufSize << " bytes, " 
					<< local << " [us]." << std::endl;
				ss.close();
			}

			{
				UDPEchoServer echoServer(bufSize);
				DatagramSocket ss;
				ss.connect(SocketAddress("localhost", echoServer.port()));
				int recv = 0, sent = 0;
				Stopwatch sw; int i = 0; 
				for (; i < repetitions; ++i)
				{
					sent = 0; recv = 0; net = 0;

					do
					{
						int s;
						sw.restart();
						s = ss.sendBytes(pBuf + sent, bufSize - sent);
						sw.stop();
						net += sw.elapsed();
						sent += s;
					} while (sent < bufSize);

					do
					{
						int r;
						sw.restart();
						r = ss.receiveBytes(pBuf + recv, bufSize - recv);
						sw.stop();
						net += sw.elapsed();
						recv += r;
					} while (recv < bufSize);

					netData += sent;
					netData += recv;
					netTime += net;

					poco_assert (sent == bufSize && recv == bufSize);
				}
				
				std::cout << "Network UDP socket, " << i << " repetitions, " << bufSize << " bytes, " 
					<< net << " [us]." << std::endl;
				fos << i << ',' << bufSize << ',';
				ss.close();
			}
			delete pBuf;

			double ratio = local ? ((double) net) / ((double) local) : (double) net;
			double diff = ((double) net) - ((double) local);
			std::cout << "Ratio: " << ratio << "(" << diff / 1000.0 << "[us/msg]" << ')' << std::endl;

			fos << ratio << std::endl;
		}
	}

	poco_assert (locData == netData);

	double locDTR = ((locData / (locTime / Timestamp::resolution())) * 8) / 1000000;
	double netDTR = ((netData / (netTime / Timestamp::resolution())) * 8) / 1000000;

	std::cout << "=================================" << std::endl
		<< "Local DTR: " << locDTR << " [Mbit/s]" << std::endl
		<< "Network DTR: " << netDTR << " [Mbit/s]" << std::endl
		<< "Local sockets speedup: " << ((locDTR / netDTR) * 100) - 100 << '%' << std::endl
		<< "=================================" << std::endl;

	fos << "=================================" << std::endl
		<< "Local DTR: " << locDTR << " [Mbit/s]" << std::endl
		<< "Network DTR: " << netDTR << " [Mbit/s]" << std::endl
		<< "Local sockets speedup: " << ((locDTR / netDTR) * 100) - 100 << '%' << std::endl
		<< "=================================" << std::endl;

	fos.close();
}


void DatagramLocalSocketTest::testEcho()
{
	UDPLocalEchoServer echoServer;
	DatagramSocket ss(SocketAddress("/tmp/poco.client.udp.sock"), true);
	
	SocketAddress addr(echoServer.address().toString());
	ss.connect(addr);
	int n = ss.sendBytes("hello", 5);
	assert (n == 5);
	char buffer[256] = { 0 };
	n = ss.receiveBytes(buffer, sizeof(buffer));
	assert (n == 5);
	assert (std::string(buffer, n) == "hello");
	ss.close();
}


void DatagramLocalSocketTest::testSendToReceiveFrom()
{
	SocketAddress serverAddr("/tmp/poco.server.udp.sock");
	UDPLocalEchoServer echoServer(serverAddr);
	DatagramSocket ss(Address::LOCAL);
	SocketAddress clientAddr("/tmp/poco.client.udp.sock");
	ss.bind(clientAddr, true);
	int n = ss.sendTo("hello", 5, serverAddr);
	assert (n == 5);
	char buffer[256];
	SocketAddress sa;
	n = ss.receiveFrom(buffer, sizeof(buffer), sa);
	assert (sa.host() == echoServer.address().host());
	assert (n == 5);
	assert (std::string(buffer, n) == "hello");
	ss.close();
}


void DatagramLocalSocketTest::setUp()
{
}


void DatagramLocalSocketTest::tearDown()
{
}


CppUnit::Test* DatagramLocalSocketTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("DatagramLocalSocketTest");

	CppUnit_addTest(pSuite, DatagramLocalSocketTest, testDatagramSocketPerformance);
	CppUnit_addTest(pSuite, DatagramLocalSocketTest, testEcho);
	CppUnit_addTest(pSuite, DatagramLocalSocketTest, testSendToReceiveFrom);

	return pSuite;
}