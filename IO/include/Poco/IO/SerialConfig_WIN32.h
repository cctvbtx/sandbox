//
// SerialConfig_WIN32.h
//
// $Id: //poco/Main/template/class.h#8 $
//
// Library: IO
// Package: Serial
// Module:  SerialConfig
//
// Definition of the SerialConfigImpl class for WIN32.
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


#ifndef IO_SerialConfig_WIN32_INCLUDED
#define IO_SerialConfig_WIN32_INCLUDED


#include "Poco/IO/IO.h"
#include <windows.h>


namespace Poco {
namespace IO {


class IO_API SerialConfigImpl
{
public:
	enum ParityImpl
	{
		NONE_IMPL = NOPARITY,
		ODD_IMPL = ODDPARITY,
		EVEN_IMPL = EVENPARITY,
		MARK_IMPL = MARKPARITY,
		SPACE_IMPL = SPACEPARITY
	};

	enum StartBitsImpl
	{
		ONESTART_IMPL = ONESTOPBIT,
		ONE5START_IMPL = ONE5STOPBITS,
		TWOSTART_IMPL = TWOSTOPBITS
	};

	enum StopBitsImpl
	{
		ONESTOP_IMPL = ONESTOPBIT,
		ONE5STOP_IMPL = ONE5STOPBITS,
		TWOSTOP_IMPL = TWOSTOPBITS
	};

protected:
	static const int MSEC;

	SerialConfigImpl(
		int speed,
		int dataBits,
		char parity,
		StartBitsImpl startBits,
		StopBitsImpl stopBits,
		bool useXonXoff,
		unsigned char xOnChar,
		unsigned char xOffChar,
		bool useEOF,
		unsigned char eofChar,
		int bufferSize,
		int timeout);

	void setSpeedImpl(int speed);
	void setDataBitsImpl(int dataBits);
	void setParityImpl(ParityImpl parity);
	void setParityCharImpl(char parityChar);
	void setStartBitsImpl(StartBitsImpl startBits);
	void setStopBitsImpl(StopBitsImpl stopBits);
	void setUseXonXoffImpl(bool use);
	void setXonCharImpl(unsigned char xOn);
	void setXoffCharImpl(unsigned char xOff);
	void setUseEOFImpl(bool useEOF);
	void setEOFCharImpl(unsigned char eof);
	void setBufferSizeImpl(int size);
	void setTimeoutSecondsImpl(int timeout);
	void setTimeoutImpl(int timeout);

	int getSpeedImpl() const;
	int getDataBitsImpl() const;
	ParityImpl getParityImpl() const;
	char getParityCharImpl() const;
	StartBitsImpl getStartBitsImpl() const;
	StopBitsImpl getStopBitsImpl() const;
	bool getUseXonXoffImpl() const;
	unsigned char getXonCharImpl() const;
	unsigned char getXoffCharImpl() const;
	bool getUseEOFImpl() const;
	unsigned char getEOFCharImpl() const;
	int getBufferSizeImpl() const;
	int getTimeoutSecondsImpl() const;
	int getTimeoutImpl() const;

	DCB& dcb();
	COMMTIMEOUTS& commTimeouts();

private:
	SerialConfigImpl();

	DCB _dcb;
	COMMTIMEOUTS _cto;
	bool _useXonXoff;
	bool _useEOF;
	int _bufferSize;

	friend class SerialPortImpl;
	friend class SerialConfig;
};


//
// inlines
//
inline void SerialConfigImpl::setSpeedImpl(int speed)
{
	_dcb.BaudRate = speed;
}


inline void SerialConfigImpl::setDataBitsImpl(int dataBits)
{
	_dcb.ByteSize = dataBits;
}


inline void SerialConfigImpl::setParityImpl(SerialConfigImpl::ParityImpl parity)
{
	_dcb.fParity = (parity != SerialConfigImpl::ParityImpl::NONE_IMPL);
	_dcb.Parity = parity;
}


inline void SerialConfigImpl::setStartBitsImpl(SerialConfigImpl::StartBitsImpl startBits)
{
	_dcb.StopBits = startBits;
}


inline void SerialConfigImpl::setStopBitsImpl(SerialConfigImpl::StopBitsImpl stopBits)
{
	_dcb.StopBits = stopBits;
}


inline void SerialConfigImpl::setUseXonXoffImpl(bool use)
{
	_useXonXoff = use;
}


inline void SerialConfigImpl::setXonCharImpl(unsigned char xOn)
{
	_dcb.XonChar = xOn;
}


inline void SerialConfigImpl::setXoffCharImpl(unsigned char xOff)
{
	_dcb.XoffChar = xOff;
}


inline void SerialConfigImpl::setUseEOFImpl(bool useEOF)
{
	_useEOF = useEOF;
}


inline void SerialConfigImpl::setEOFCharImpl(unsigned char eof)
{
	_dcb.EofChar = eof;
}


inline void SerialConfigImpl::setBufferSizeImpl(int size)
{
	_bufferSize = size;
}


inline void SerialConfigImpl::setTimeoutSecondsImpl(int timeout)
{
	setTimeoutImpl(timeout*MSEC);
}


inline void SerialConfigImpl::setTimeoutImpl(int timeout)
{
	_cto.ReadTotalTimeoutConstant = timeout;
}


inline int SerialConfigImpl::getSpeedImpl() const
{
	return _dcb.BaudRate;
}


inline int SerialConfigImpl::getDataBitsImpl() const
{
	return _dcb.ByteSize;
}


inline SerialConfigImpl::ParityImpl SerialConfigImpl::getParityImpl() const
{
	return (SerialConfigImpl::ParityImpl) _dcb.Parity;
}


inline SerialConfigImpl::StartBitsImpl SerialConfigImpl::getStartBitsImpl() const
{
	return (SerialConfigImpl::StartBitsImpl) _dcb.StopBits;
}


inline SerialConfigImpl::StopBitsImpl SerialConfigImpl::getStopBitsImpl() const
{
	return (SerialConfigImpl::StopBitsImpl) _dcb.StopBits;
}


inline bool SerialConfigImpl::getUseXonXoffImpl() const
{
	return _useXonXoff;
}


inline unsigned char SerialConfigImpl::getXonCharImpl() const
{
	return _dcb.XonChar;
}


inline unsigned char SerialConfigImpl::getXoffCharImpl() const
{
	return _dcb.XoffChar;
}


inline bool SerialConfigImpl::getUseEOFImpl() const
{
	return _useEOF;
}


inline unsigned char SerialConfigImpl::getEOFCharImpl() const
{
	return _dcb.EofChar;
}


inline int SerialConfigImpl::getBufferSizeImpl() const
{
	return _bufferSize;
}


inline int SerialConfigImpl::getTimeoutSecondsImpl() const
{
	return getTimeoutImpl()*MSEC;
}


inline int SerialConfigImpl::getTimeoutImpl() const
{
	return _cto.ReadTotalTimeoutConstant;
}


inline DCB& SerialConfigImpl::dcb()
{
	return _dcb;
}


inline COMMTIMEOUTS& SerialConfigImpl::commTimeouts()
{
	return _cto;
}


} } // namespace Poco::IO


#endif // IO_SerialConfig_WIN32_INCLUDED
