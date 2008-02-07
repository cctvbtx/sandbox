//
// ChannelStream.h
//
// $Id: //poco/Main/Data/include/Poco/IO/ChannelStream.h#1 $
//
// Library: IO
// Package: IO
// Module:  ChannelStream
//
// Definition of the ChannelStream class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
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


#ifndef IO_ChannelStream_INCLUDED
#define IO_ChannelStream_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/UnbufferedStreamBuf.h"
#include "Poco/IO/AbstractChannel.h"
#include <istream>
#include <ostream>


namespace Poco {
namespace IO {


class IO_API ChannelStreamBuf: public UnbufferedStreamBuf
	/// This is the streambuf class used for reading from and writing to a Channel.
{
public:	
	ChannelStreamBuf(AbstractChannel& channel);
		/// Creates a ChannelStreamBuf with the given Channel.

	~ChannelStreamBuf();
		/// Destroys the ChannelStreamBuf.
		
	void close();
		/// Closes the channel.

protected:
	int_type readFromDevice();
	int_type writeToDevice(char c);

private:
	Poco::IO::AbstractChannel& _channel;
};


class IO_API ChannelIOS: public virtual std::ios
	/// The base class for ChannelInputStream and
	/// ChannelOutputStream.
	///
	/// This class is needed to ensure the correct initialization
	/// order of the stream buffer and base classes.
{
public:
	ChannelIOS(AbstractChannel& channel, openmode mode);
		/// Creates the ChannelIOS with the given Channel.
		
	~ChannelIOS();
		/// Destroys the ChannelIOS.
		
	ChannelStreamBuf* rdbuf();
		/// Returns a pointer to the internal ChannelStreamBuf.
		
protected:
	ChannelStreamBuf _buf;
};


class IO_API ChannelOutputStream: public ChannelIOS, public std::ostream
	/// An output stream for writing to a Channel.
{
public:
	ChannelOutputStream(AbstractChannel& channel);
		/// Creates the ChannelOutputStream with the given Channel.

	~ChannelOutputStream();
		/// Destroys the ChannelOutputStream.
		///
		/// Flushes the buffer, but does not close the channel.
};


class IO_API ChannelInputStream: public ChannelIOS, public std::istream
	/// An input stream for reading from a Channel.
	///
	/// Using formatted input from a ChannelInputStream
	/// is not recommended, due to the read-ahead behavior of
	/// istream with formatted reads.
{
public:
	ChannelInputStream(AbstractChannel& channel);
		/// Creates the ChannelInputStream with the given Channel.

	~ChannelInputStream();
		/// Destroys the ChannelInputStream.
};


} } // namespace Poco::IO


#endif // IO_ChannelStream_INCLUDED
