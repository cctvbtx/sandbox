//
// SerialConfig_POSIX.h
//
// $Id: //poco/Main/Data/include/Poco/IO/SerialConfig_POSIX.h#1 $
//
// Library: IO
// Package: Serial
// Module:  SerialConfig
//
// Definition of the SerialConfigImpl class for POSIX.
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


#ifndef IO_SerialConfig_POSIX_INCLUDED
#define IO_SerialConfig_POSIX_INCLUDED


#include "Poco/IO/IO.h"
#include "Poco/Exception.h"
#include <termios.h>


namespace Poco {
namespace IO {


class IO_API SerialConfigImpl
{
public:
	static const int TENTH_SEC = 10;
	static const int MILLI_SEC = 1000;
	static const int NOT_SUPPORTED = -1;

	enum DataBitsImpl
	{
		DATA_BITS_FIVE_IMPL = CS5,
		DATA_BITS_SIX_IMPL = CS6,
		DATA_BITS_SEVEN_IMPL = CS7,
		DATA_BITS_EIGHT_IMPL = CS8
	};

	enum ParityImpl
	{
		PARITY_NONE_IMPL = 2,
		PARITY_ODD_IMPL = 1,
		PARITY_EVEN_IMPL = 0,
		PARITY_MARK_IMPL = NOT_SUPPORTED,
		PARITY_SPACE_IMPL = NOT_SUPPORTED
	};

	enum StartBitsImpl
	{
		START_ONE_IMPL = 0,
		START_ONE5_IMPL = NOT_SUPPORTED,
		START_TWO_IMPL = 1
	};

	enum StopBitsImpl
	{
		STOP_ONE_IMPL = 1,
		STOP_ONE5_IMPL = NOT_SUPPORTED,
		STOP_TWO_IMPL = 2
	};

	enum FlowControlImpl
	{
		FLOW_CTRL_HARDWARE_IMPL,
		FLOW_CTRL_SOFTWARE_IMPL
	};

	enum BPSRateImpl
	{
		BPS_110_IMPL = B110,
		BPS_300_IMPL = B300,
		BPS_600_IMPL = B600,
		BPS_1200_IMPL = B1200,
		BPS_2400_IMPL = B2400,
		BPS_4800_IMPL = B4800,
		BPS_9600_IMPL = B9600,
		BPS_14400_IMPL = NOT_SUPPORTED,
		BPS_19200_IMPL = B19200,
		BPS_38400_IMPL = B38400,
		BPS_57600_IMPL = B57600,
		BPS_115200_IMPL = B115200,
		BPS_128000_IMPL = NOT_SUPPORTED,
		BPS_230400_IMPL  = B230400,
		BPS_256000_IMPL = NOT_SUPPORTED,
		BPS_460800_IMPL  = B460800
	};

protected:

	SerialConfigImpl(
		BPSRateImpl bpsRate,
		DataBitsImpl dataBits,
		char parity,
		StartBitsImpl startBits,
		StopBitsImpl stopBits,
		FlowControlImpl flowControl,
		unsigned char xOnChar,
		unsigned char xOffChar,
		bool useEOF,
		unsigned char eofChar,
		int bufferSize,
		int timeout);

	void setBPSRateImpl(BPSRateImpl bpsRate);
	void setDataBitsImpl(DataBitsImpl dataBits);
	void setParityImpl(ParityImpl parity);
	void setParityCharImpl(char parityChar);
	void setStartBitsImpl(StartBitsImpl startBits);
	void setStopBitsImpl(StopBitsImpl stopBits);
	void setFlowControlImpl(FlowControlImpl flowControl,
		unsigned char xOnChar,
		unsigned char xOffChar);
	void setUseXonXoffImpl(unsigned char xOnChar,
		unsigned char xOffChar);
	void setXonCharImpl(unsigned char xOn);
	void setXoffCharImpl(unsigned char xOff);
	void setUseEOFImpl(bool useEOF);
	void setEOFCharImpl(unsigned char eof);
	void setBufferSizeImpl(int size);
	void setTimeoutSecondsImpl(int timeout);
	void setTimeoutImpl(int timeout);

	BPSRateImpl getBPSRateImpl() const;
	DataBitsImpl getDataBitsImpl() const;
	ParityImpl getParityImpl() const;
	char getParityCharImpl() const;
	StartBitsImpl getStartBitsImpl() const;
	StopBitsImpl getStopBitsImpl() const;
	FlowControlImpl getFlowControlImpl() const;
	bool getUseXonXoffImpl() const;
	unsigned char getXonCharImpl() const;
	unsigned char getXoffCharImpl() const;
	bool getUseEOFImpl() const;
	unsigned char getEOFCharImpl() const;
	int getBufferSizeImpl() const;
	int getTimeoutSecondsImpl() const;
	int getTimeoutImpl() const;

private:
	SerialConfigImpl();

	termios _termios;
	FlowControlImpl _flowControl;
	bool _useEOF;

	friend class SerialChannelImpl;
};


//
// inlines
//


inline void SerialConfigImpl::setStartBitsImpl(SerialConfigImpl::StartBitsImpl startBits)
{
	throw Poco::InvalidAccessException("Operation not supported.");
}


inline void SerialConfigImpl::setUseEOFImpl(bool useEOF)
{
	//TODO
}


inline void SerialConfigImpl::setEOFCharImpl(unsigned char eof)
{
	//TODO
}


inline void SerialConfigImpl::setBufferSizeImpl(int size)
{
	_termios.c_cc[VMIN] = size;
}


inline void SerialConfigImpl::setTimeoutSecondsImpl(int timeout)
{
	_termios.c_cc[VTIME] = timeout * TENTH_SEC;
}


inline void SerialConfigImpl::setTimeoutImpl(int timeout)
{
	setTimeoutSecondsImpl(timeout * MILLI_SEC);
}


inline void SerialConfigImpl::setUseXonXoffImpl(unsigned char,
		unsigned char)
{
	setFlowControlImpl(FLOW_CTRL_SOFTWARE_IMPL, 0, 0);
}


inline void SerialConfigImpl::setXonCharImpl(unsigned char xOn)
{
	throw Poco::InvalidAccessException("Operation not supported.");
}


inline void SerialConfigImpl::setXoffCharImpl(unsigned char xOff)
{
	throw Poco::InvalidAccessException("Operation not supported.");
}


inline SerialConfigImpl::BPSRateImpl SerialConfigImpl::getBPSRateImpl() const
{
	return (BPSRateImpl) _termios.c_ospeed;
}


inline SerialConfigImpl::StartBitsImpl SerialConfigImpl::getStartBitsImpl() const
{
	throw Poco::InvalidAccessException("Operation not supported.");
}


inline SerialConfigImpl::FlowControlImpl SerialConfigImpl::getFlowControlImpl() const
{
	return _flowControl;
}


inline bool SerialConfigImpl::getUseXonXoffImpl() const
{
	return FLOW_CTRL_SOFTWARE_IMPL == _flowControl;
}


inline unsigned char SerialConfigImpl::getXonCharImpl() const
{
	throw Poco::InvalidAccessException("Operation not supported.");
}


inline unsigned char SerialConfigImpl::getXoffCharImpl() const
{
	throw Poco::InvalidAccessException("Operation not supported.");
}


inline bool SerialConfigImpl::getUseEOFImpl() const
{
	return _useEOF;
}


inline unsigned char SerialConfigImpl::getEOFCharImpl() const
{
	return 0;//TODO
}


inline int SerialConfigImpl::getBufferSizeImpl() const
{
	return _termios.c_cc[VMIN];
}


inline int SerialConfigImpl::getTimeoutSecondsImpl() const
{
	return getTimeoutImpl() * MILLI_SEC;
}


inline int SerialConfigImpl::getTimeoutImpl() const
{
	return _termios.c_cc[VTIME] * MILLI_SEC / TENTH_SEC;
}


} } // namespace Poco::IO


#endif // IO_SerialConfig_POSIX_INCLUDED
