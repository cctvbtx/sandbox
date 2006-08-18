//
// SerialConfig_WIN32.cpp
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


#include "Poco/IO/SerialConfig_WIN32.h"


namespace Poco {
namespace IO {


const int SerialConfigImpl::MSEC = 1000;

SerialConfigImpl::SerialConfigImpl(
	int speed,
	int dataBits,
	ParityImpl parity,
	StartBitsImpl startBits,
	StopBitsImpl stopBits,
	bool useXonXoff,
	unsigned char xOn,
	unsigned char xOff,
	bool useEOF,
	unsigned char eof,
	int bufferSize,
	int timeout):
	_speed(speed),
	_dataBits(dataBits),
	_parity(parity),
	_startBits(startBits),
	_stopBits(stopBits),
	_useXonXoff(useXonXoff),
	_xOnChar(xOn),
	_xOffChar(xOff),
	_eof(eof),
	_bufferSize(bufferSize),
	_timeout(timeout)
{
}


} } // namespace Poco::IO
