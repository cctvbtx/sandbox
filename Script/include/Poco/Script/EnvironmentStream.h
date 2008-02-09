//
// EnvironmentStream.h
//
// $Id: //poco/Main/Script/include/Poco/EnvironmentStream.h#6 $
//
// Library: Script
// Package: Streams
// Module:  EnvironmentStream
//
// Definition of the EnvironmentStreamBuf, EnvironmentInputStream and EnvironmentOutputStream classes.
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
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


#ifndef Script_EnvironmentStream_INCLUDED
#define Script_EnvironmentStream_INCLUDED


#include "Poco/Script/Script.h"
#include "Poco/Script/Environment.h"
#include "Poco/Script/EnvironmentStreamBuf.h"
#include <istream>
#include <ostream>


namespace Poco {
namespace Script {


class Script_API EnvironmentIOS: public virtual std::ios
	/// The base class for EnvironmentInputStream and EnvironmentOutputStream.
{
public:
	EnvironmentIOS(Environment& env, std::ios::openmode defaultMode);
		/// Creates the basic stream.
		
	~EnvironmentIOS();
		/// Destroys the stream.

	void open(Environment& env, std::ios::openmode mode);
		/// Opens the environment specified by env, using the given mode.

	void close();
		/// Closes the environment stream.

	EnvironmentStreamBuf* rdbuf();
		/// Returns a pointer to the underlying streambuf.

protected:
	EnvironmentStreamBuf _buf;
	std::ios::openmode _defaultMode;
};


class Script_API EnvironmentInputStream: public EnvironmentIOS, public std::istream
	/// An input stream for reading from an environment.
{
public:

	EnvironmentInputStream(Environment& env, std::ios::openmode mode = std::ios::in);
		/// Creates the EnvironmentInputStream for the environment given by env, using
		/// the given mode.

	~EnvironmentInputStream();
		/// Destroys the stream.
};


class Script_API EnvironmentOutputStream: public EnvironmentIOS, public std::ostream
	/// An output stream for writing to a environment.
{
public:
	EnvironmentOutputStream(Environment& env, std::ios::openmode mode = std::ios::out | std::ios::trunc);
		/// Creates the EnvironmentOutputStream for the environment given by env, using
		/// the given mode.

	~EnvironmentOutputStream();
		/// Destroys the EnvironmentOutputStream.
};


class Script_API EnvironmentStream: public EnvironmentIOS, public std::iostream
	/// A stream for reading from and writing to a environment.
{
public:
	EnvironmentStream(Environment& env, std::ios::openmode mode = std::ios::out | std::ios::in);
		/// Creates the EnvironmentStream for the environment given by env, using
		/// the given mode.

	~EnvironmentStream();
		/// Destroys the EnvironmentOutputStream.
};


} } // namespace Poco::Script


#endif // Script_EnvironmentStream_INCLUDED
