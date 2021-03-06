//
// SerialChannel.h
//
// $Id: //poco/Main/DeviceIO/include/Poco/DeviceIO/Serial/SerialChannel.h#1 $
//
// Library: Serial
// Package: Serial
// Module:  SerialChannel
//
// Definition of the SerialChannel class.
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
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


#ifndef Serial_SerialChannel_INCLUDED
#define Serial_SerialChannel_INCLUDED


#include "Poco/DeviceIO/Serial/Serial.h"
#include "Poco/DeviceIO/Channel.h"
#include "Poco/DeviceIO/Serial/SerialConfig.h"


#if defined(POCO_OS_FAMILY_WINDOWS)
	#if defined(POCO_WIN32_UTF8)
		#include "Poco/DeviceIO/Serial/SerialChannel_WIN32U.h"
	#else
		#include "Poco/DeviceIO/Serial/SerialChannel_WIN32.h"
	#endif
#elif defined(POCO_OS_FAMILY_UNIX)
	#include "Poco/DeviceIO/Serial/SerialChannel_POSIX.h"
#endif


namespace Poco {
namespace DeviceIO {
namespace Serial {


class Serial_API SerialChannel: public Channel, private SerialChannelImpl
{
public:
	SerialChannel(SerialConfig* pConfig);
		/// Creates serial channel.

	~SerialChannel();
		/// Destroys serial channel.

	void open();
		/// Opens the serial chanel.

	void close();
		/// Closes the serial channel.

private:
	void init();
	
	int readData(char* pBuffer, int length);
	int readData(char*& pBuffer);
	int writeData(const char* buffer, int length);

	SerialChannel(const SerialChannel&);
	const SerialChannel& operator=(const SerialChannel&);

	SerialConfig* _pConfig;
};


//
// inlines
//


inline void SerialChannel::init()
{
	initImpl();
}


inline void SerialChannel::open()
{
	openImpl();
}


inline void SerialChannel::close()
{
	closeImpl();
}


inline int SerialChannel::readData(char* pBuffer, int length)
{
	return readImpl(pBuffer, length);
}


inline int SerialChannel::readData(char*& pBuffer)
{
	return readImpl(pBuffer);
}


inline int SerialChannel::writeData(const char* pBuffer, int length)
{
	return writeImpl(pBuffer, length);
}


} } } // namespace Poco::DeviceIO::Serial


#endif // Serial_SerialChannel_INCLUDED
