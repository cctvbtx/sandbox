//
// SerialTestSW.cpp
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


#include "SerialTestSW.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Types.h"
#include "Poco/ActiveResult.h"
#include "Poco/IO/Channel.h"
#include "Poco/IO/SerialConfig.h"
#include "Poco/IO/SerialChannel.h"
#include "Poco/IO/ActiveChannel.h"
#include "Poco/Void.h"
#include "Poco/Types.h"
#include "Poco/BinaryReader.h"
#include "Poco/BinaryWriter.h"

using Poco::UInt32;
using Poco::Int64;
using Poco::UInt64;
using Poco::ActiveResult;
using Poco::Void;
using Poco::BinaryReader;
using Poco::BinaryWriter;
using Poco::IO::Channel;
using Poco::IO::SerialConfig;
using Poco::IO::SerialChannel;
using Poco::IO::ActiveChannel;
using Poco::IO::Serial;


const unsigned char SerialTestSW::SERIAL_EOF = 0x0D;

SerialTestSW::SerialTestSW(const std::string& name): 
	CppUnit::TestCase(name),
	_serialConfig(SerialConfig::BPS_9600, 
	SerialConfig::DATA_BITS_EIGHT, 
	'N', 
	SerialConfig::START_ONE,
	SerialConfig::STOP_ONE,
	SerialConfig::FLOW_CTRL_SOFTWARE,
	0x11,//xOn
	0x13,//xOff
	true,//use EOF
	SERIAL_EOF,//EOF 
	10,//buffer size
	1000)//timeout
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


SerialTestSW::~SerialTestSW()
{
}


void SerialTestSW::testChannel()
{
	Serial com1(_serialName1, _serialConfig);
	Serial com2(_serialName2, _serialConfig);
	
	com1.write('x');
	assert('x' == com2.read());

	std::string str = "1234567890";
	str += 0x0D;
	str += "0987654321";

	assert(11 == com1.write(str));
	assert(10 == com2.read(str).length());	
	assert("1234567890" == str);

	assert(5 == com1.write(const_cast<char*>(str.c_str()), 5));
	char chr[5] = "";
	assert(3 == com2.read(chr, 3));	
	assert('1' == chr[0]);
	assert('2' == chr[1]);
	assert('3' == chr[2]);
	assert(0 == chr[3]);
	memset(chr, 0, sizeof(chr));
	com2.read(chr, 2);	
	assert('4' == chr[0]);
	assert('5' == chr[1]);
	assert(0 == chr[2]);

	SerialConfig config(_serialConfig);
	config.setBufferSize(1);
	config.setBPSRate(SerialConfig::BPS_2400);
	com1.reconfigure(config);
	com2.reconfigure(config);
	com1.write(str);
	com2.read(str);	
	assert("1234567890" == str);	
}


void SerialTestSW::testActiveChannel()
{
	SerialChannel com1(_serialName1, _serialConfig);
	SerialChannel com2(_serialName2, _serialConfig);
	std::string str1 = "1234567890";
	std::string str2 = "";
	
	ActiveChannel<SerialChannel> activePort1(com1);
	ActiveChannel<SerialChannel> activePort2(com2);
	ActiveResult<int> result1 = activePort1.write(str1);
	result1.wait();
	Void v;
	ActiveResult<std::string> result2 = activePort2.read(v);
	result2.wait();
	assert("1234567890" == result2.data());
}


void SerialTestSW::testStreams()
{
	Serial com1(_serialName1, _serialConfig);
	Serial com2(_serialName2, _serialConfig);
	SerialOutputStream sos(com1);
	SerialInputStream sis(com2);

	sos << "1234567890\n";
	std::string str;
	sis >> str;
	assert("1234567890" == str);

	sos << 1.5;
	sos << "\n";
	sis >> str;
	assert("1.5" == str);
}


void SerialTestSW::testBinary()
{
	Serial com1(_serialName1, _serialConfig);
	Serial com2(_serialName2, _serialConfig);

	SerialOutputStream sos(com1);
	SerialInputStream sis(com2);

	BinaryWriter bw(sos);
	BinaryReader br(sis);

	writeBinary(bw);
	readBinary(br);
}


void SerialTestSW::writeBinary(BinaryWriter& writer)
{
	writer << true;
	writer << false;
	writer << 'a';

	writer << (short) -100;
	writer << (unsigned short) 50000;
	writer << -123456;
	writer << (unsigned) 123456;
	writer << (long) -1234567890;
	writer << (unsigned long) 1234567890;
	
#if defined(POCO_HAVE_INT64)
	writer << (Int64) -1234567890;
	writer << (UInt64) 1234567890;
#endif

	writer << (float) 1.5;
	writer << (double) -1.5;
	
	writer << "foo";
	writer << "";

	writer << std::string("bar");
	writer << std::string();
	
	writer.write7BitEncoded((UInt32) 100);
	writer.write7BitEncoded((UInt32) 1000);
	writer.write7BitEncoded((UInt32) 10000);
	writer.write7BitEncoded((UInt32) 100000);
	writer.write7BitEncoded((UInt32) 1000000);

#if defined(POCO_HAVE_INT64)
	writer.write7BitEncoded((UInt64) 100);
	writer.write7BitEncoded((UInt64) 1000);
	writer.write7BitEncoded((UInt64) 10000);
	writer.write7BitEncoded((UInt64) 100000);
	writer.write7BitEncoded((UInt64) 1000000);
#endif

	writer.writeRaw("RAW");
}


void SerialTestSW::readBinary(BinaryReader& reader)
{
	bool b = false;
	reader >> b;
	assert (b);
	reader >> b;
	assert (!b);
	
	char c = ' ';
	reader >> c;
	assert (c == 'a');

	short shortv = 0;
	reader >> shortv;
	assert (shortv == -100);

	unsigned short ushortv = 0;
	reader >> ushortv;
	assert (ushortv == 50000);

	int intv = 0;
	reader >> intv;
	assert (intv == -123456);

	unsigned uintv = 0;
	reader >> uintv;
	assert (uintv == 123456);

	long longv = 0;
	reader >> longv;
	assert (longv == -1234567890);

	unsigned long ulongv = 0;
	reader >> ulongv;
	assert (ulongv == 1234567890);

#if defined(POCO_HAVE_INT64)
	Int64 int64v = 0;
	reader >> int64v;
	assert (int64v == -1234567890);
	
	UInt64 uint64v = 0;
	reader >> uint64v;
	assert (uint64v == 1234567890);
#endif

	float floatv = 0.0;
	reader >> floatv;
	assert (floatv == 1.5);
	
	double doublev = 0.0;
	reader >> doublev;
	assert (doublev == -1.5);

	std::string str;
	reader >> str;
	assert (str == "foo");
	reader >> str;
	assert (str == "");
	
	reader >> str;
	assert (str == "bar");
	reader >> str;
	assert (str == "");

	UInt32 uint32v;
	reader.read7BitEncoded(uint32v);
	assert (uint32v == 100);
	reader.read7BitEncoded(uint32v);
	assert (uint32v == 1000);
	reader.read7BitEncoded(uint32v);
	assert (uint32v == 10000);
	reader.read7BitEncoded(uint32v);
	assert (uint32v == 100000);
	reader.read7BitEncoded(uint32v);
	assert (uint32v == 1000000);

#if defined(POCO_HAVE_INT64)
	reader.read7BitEncoded(uint64v);
	assert (uint64v == 100);
	reader.read7BitEncoded(uint64v);
	assert (uint64v == 1000);
	reader.read7BitEncoded(uint64v);
	assert (uint64v == 10000);
	reader.read7BitEncoded(uint64v);
	assert (uint64v == 100000);
	reader.read7BitEncoded(uint64v);
	assert (uint64v == 1000000);
#endif

	reader.readRaw(3, str);
	assert (str == "RAW");
}


void SerialTestSW::setUp()
{
}


void SerialTestSW::tearDown()
{
}


CppUnit::Test* SerialTestSW::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("SerialTestSW");

	CppUnit_addTest(pSuite, SerialTestSW, testChannel);
	CppUnit_addTest(pSuite, SerialTestSW, testActiveChannel);
	CppUnit_addTest(pSuite, SerialTestSW, testStreams);
	CppUnit_addTest(pSuite, SerialTestSW, testBinary);

	return pSuite;
}
