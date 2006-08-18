//
// SerialConfig.cpp
//
// $Id: //poco/Main/template/class.cpp#8 $
//
// Library: IO
// Package: Serial
// Module:  SerialConfig
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


#include "Poco/IO/SerialConfig.h"


#if defined(POCO_OS_FAMILY_WINDOWS)
#include "SerialConfig_WIN32.cpp"
#endif


namespace Poco {
namespace IO {


SerialConfig::SerialConfig(): 
	SerialConfigImpl(2400, 
		8, 
		SerialConfigImpl::NONE_IMPL, 
		SerialConfigImpl::ONESTART_IMPL, 
		SerialConfigImpl::ONESTOP_IMPL,
		false,
		0, 
		-1,
		true,
		0,
		1,
		5000)
{
}


SerialConfig::SerialConfig(
	int speed,
	int dataBits,
	SerialConfig::Parity parity,
	SerialConfig::StartBits startBits,
	SerialConfig::StopBits stopBits,
	bool useXonXoff,
	unsigned char xOnChar,
	unsigned char xOffChar,
	bool useEOF,
	unsigned char eof,
	int bufferSize,
	int timeout):
SerialConfigImpl(
	speed,
	dataBits,
	(SerialConfigImpl::ParityImpl) parity,
	(SerialConfigImpl::StartBitsImpl)startBits,
	(SerialConfigImpl::StopBitsImpl) stopBits,
	useXonXoff,
	xOnChar,
	xOffChar,
	useEOF,
	eof,
	bufferSize,
	timeout)
{
}


} } // namespace Poco::IO
