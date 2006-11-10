//
// SerialConfig.h
//
// $Id: //poco/Main/template/class.h#8 $
//
// Library: IO
// Package: Serial
// Module:  SerialConfig
//
// Definition of the SerialConfig class.
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


#ifndef IO_SerialConfig_INCLUDED
#define IO_SerialConfig_INCLUDED


#include "Poco/IO/IO.h"


#if defined(POCO_OS_FAMILY_WINDOWS)
#include "Poco/IO/SerialConfig_WIN32.h"
#endif


namespace Poco {
namespace IO {


class IO_API SerialConfig: private SerialConfigImpl
{
public:
	enum Parity
	{
		//on Windows, PARITY_* conflicts with
		//system macros, hence PAR_*
		PAR_NONE = PARITY_NONE_IMPL,
		PAR_ODD = PARITY_ODD_IMPL,
		PAR_EVEN = PARITY_EVEN_IMPL,
		PAR_MARK = PARITY_MARK_IMPL,
		PAR_SPACE = PARITY_SPACE_IMPL
	};

	enum StartBits
	{
		START_ONE = START_ONE_IMPL,
		START_ONE5 = START_ONE5_IMPL,
		START_TWO = START_TWO_IMPL
	};

	enum StopBits
	{
		STOP_ONE = STOP_ONE_IMPL,
		STOP_ONE5 = STOP_ONE5_IMPL,
		STOP_TWO = STOP_TWO_IMPL
	};

	enum FlowControl
	{
		FLOW_CTRL_HARDWARE = FLOW_CTRL_HARDWARE_IMPL,
		FLOW_CTRL_SOFTWARE = FLOW_CTRL_SOFTWARE_IMPL
	};

	enum BaudRate
	{
		BAUD_RATE_110 = BAUD_RATE_110_IMPL,
		BAUD_RATE_300 = BAUD_RATE_300_IMPL,
		BAUD_RATE_600 = BAUD_RATE_600_IMPL,
		BAUD_RATE_1200 = BAUD_RATE_1200_IMPL,
		BAUD_RATE_2400 = BAUD_RATE_2400_IMPL,
		BAUD_RATE_4800 = BAUD_RATE_4800_IMPL,
		BAUD_RATE_9600 = BAUD_RATE_9600_IMPL,
		BAUD_RATE_14400 = BAUD_RATE_14400_IMPL,
		BAUD_RATE_19200 = BAUD_RATE_19200_IMPL,
		BAUD_RATE_38400 = BAUD_RATE_38400_IMPL,
		BAUD_RATE_57600 = BAUD_RATE_57600_IMPL,
		BAUD_RATE_115200 = BAUD_RATE_115200_IMPL,
		BAUD_RATE_128000 = BAUD_RATE_128000_IMPL,
		BAUD_RATE_256000 = BAUD_RATE_256000_IMPL
	};

	SerialConfig();
	SerialConfig(BaudRate baudRate,
		int dataBits=8,
		char parity='N',
		StartBits startBits=START_ONE,
		StopBits stopBits=STOP_ONE,
		FlowControl flowControl=FLOW_CTRL_HARDWARE,
		unsigned char xOnChar=0,
		unsigned char xOffChar=0,
		bool useEOF=true,
		unsigned char eof=DEFAULT_EOF,
		int bufferSize=1,
		int timeout=5000);

	void setBaudRate(BaudRate baudRate);
	void setDataBits(int dataBits);
	void setParity(ParityImpl parity);
	void setParityChar(char parityChar);
	void setStartBits(StartBits startBits);
	void setStopBits(StopBits stopBits);
	void setFlowControl(FlowControl flowControl,
		unsigned char xOnChar,
		unsigned char xOffChar);
	void setUseXonXoff(unsigned char xOnChar,
		unsigned char xOffChar);
	void setXonChar(unsigned char xOn);
	void setXoffChar(unsigned char xOff);
	void setUseEOF(bool useEOF);
	void setEOFChar(unsigned char eof);
	void setBufferSize(int size);
	void setTimeoutSeconds(int timeout);
	void setTimeout(int timeout);

	BaudRate getBaudRate() const;
	int getDataBits() const;
	ParityImpl getParity() const;
	char getParityChar() const;
	StartBits getStartBits() const;
	StopBits getStopBits() const;
	FlowControl getFlowControl() const;
	bool getUseXonXoff() const;
	unsigned char getXonChar() const;
	unsigned char getXoffChar() const;
	bool getUseEOF() const;
	unsigned char getEOFChar() const;
	int getBufferSize() const;
	int getTimeoutSeconds() const;
	int getTimeout() const;

	static const unsigned char DEFAULT_EOF;
};


//
// inlines
//
inline void SerialConfig::setBaudRate(SerialConfig::BaudRate baudRate)
{
	setBaudRateImpl((SerialConfigImpl::BaudRateImpl) baudRate);
}


inline void SerialConfig::setDataBits(int dataBits)
{
	setDataBitsImpl(dataBits);
}


inline void SerialConfig::setParity(SerialConfigImpl::ParityImpl parity)
{
	setParityImpl(parity);
}


inline void SerialConfig::setParityChar(char parityChar)
{
	setParityCharImpl(parityChar);
}


inline void SerialConfig::setStartBits(SerialConfig::StartBits startBits)
{
	setStartBitsImpl((SerialConfigImpl::StartBitsImpl) startBits);
}


inline void SerialConfig::setStopBits(SerialConfig::StopBits stopBits)
{
	setStopBitsImpl((SerialConfigImpl::StopBitsImpl) stopBits);
}


inline void SerialConfig::setFlowControl(SerialConfig::FlowControl flowControl,
	unsigned char xOnChar,
	unsigned char xOffChar)
{
	setFlowControlImpl((SerialConfigImpl::FlowControlImpl) flowControl, 
		xOnChar, 
		xOffChar);
}


inline void SerialConfig::setUseXonXoff(unsigned char xOnChar,
		unsigned char xOffChar)
{
	setUseXonXoffImpl(xOnChar, xOffChar);
}


inline void SerialConfig::setUseEOF(bool useEOF)
{
	setUseEOFImpl(useEOF);
}


inline void SerialConfig::setEOFChar(unsigned char eof)
{
	setEOFCharImpl(eof);
}


inline void SerialConfig::setBufferSize(int size)
{
	setBufferSizeImpl(size);
}


inline void SerialConfig::setTimeoutSeconds(int timeout)
{
	setTimeoutSecondsImpl(timeout);
}


inline void SerialConfig::setTimeout(int timeout)
{
	setTimeoutImpl(timeout);
}


inline SerialConfig::BaudRate SerialConfig::getBaudRate() const
{
	return (SerialConfig::BaudRate) getBaudRateImpl();
}


inline int SerialConfig::getDataBits() const
{
	return getDataBitsImpl();
}


inline SerialConfigImpl::ParityImpl SerialConfig::getParity() const
{
	return getParityImpl();
}


inline char SerialConfig::getParityChar() const
{
	return getParityCharImpl();
}

inline SerialConfig::StartBits SerialConfig::getStartBits() const
{
	return (SerialConfig::StartBits) getStartBitsImpl();
}


inline SerialConfig::StopBits SerialConfig::getStopBits() const
{
	return (SerialConfig::StopBits) getStopBitsImpl();
}


inline SerialConfig::FlowControl SerialConfig::getFlowControl() const
{
	return (SerialConfig::FlowControl) getFlowControlImpl();
}


inline bool SerialConfig::getUseXonXoff() const
{
	return getUseXonXoffImpl();
}


inline unsigned char SerialConfig::getXonChar() const
{
	return getXonCharImpl();
}


inline unsigned char SerialConfig::getXoffChar() const
{
	return getXoffCharImpl();
}


inline bool SerialConfig::getUseEOF() const
{
	return getUseEOFImpl();
}


inline unsigned char SerialConfig::getEOFChar() const
{
	return getEOFCharImpl();
}


inline int SerialConfig::getBufferSize() const
{
	return getBufferSizeImpl();
}


inline int SerialConfig::getTimeoutSeconds() const
{
	return getTimeoutSecondsImpl();
}

inline int SerialConfig::getTimeout() const
{
	return getTimeoutImpl();
}


} } // namespace Poco::IO


#endif // IO_SerialConfig_INCLUDED
