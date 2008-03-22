//
// CachingChannel.h
//
// $Id: //poco/Main/Logging/Server/include/CachingChannel.h#2 $
//
// Definition of the CachingChannel class.
//
// Copyright (c) 2006-2008, Applied Informatics Software Engineering GmbH.
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


#ifndef LoggingServer_CachingChannel_INCLUDED
#define LoggingServer_CachingChannel_INCLUDED


#include "Poco/Channel.h"
#include "Poco/Message.h"
#include "Poco/Mutex.h"
#include <list>
#include <vector>


class CachingChannel: public Poco::Channel
	/// Caches the last n Messages in memory
{
public:
	static const std::string PROP_SIZE;

	typedef std::list<Poco::Message> Messages;
	
	CachingChannel();
		/// Creates the CachingChannel. Caches 100 messages in memory

	CachingChannel(std::size_t n);
		/// Creates the CachingChannel. Caches n messages in memory

	~CachingChannel();
		/// Destroys the CachingChannel.

	void setProperty(const std::string& name, const std::string& value);
		/// The following properties are allowed:
		///     size: a non-negative integer value, must be at least 1, otherwise ignored

	std::string getProperty(const std::string& name) const;

	void log(const Poco::Message& msg);
		/// Writes the log message to the cache

	void getMessages(std::vector<Poco::Message>& msg, int offset, int numEntries) const;
		/// Retrieves numEntries Messages starting with position offset. Most recent messages are first.

	std::size_t getMaxSize() const;

	std::size_t getCurrentSize() const;

	static void registerChannel();

private:
	CachingChannel(const CachingChannel&);

	Messages   _cache;
	std::size_t _size;
	std::size_t _maxSize;
	mutable Poco::FastMutex _mutex;
};


inline std::size_t CachingChannel::getMaxSize() const
{
	return _maxSize;
}


inline std::size_t CachingChannel::getCurrentSize() const
{
	return _size;
}

#endif // LoggingServer_CachingChannel_INCLUDED