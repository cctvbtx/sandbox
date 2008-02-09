//
// LuaServer.cpp
//
// $Id: //poco/Main/Net/samples/LuaServer/src/LuaServer.cpp#7 $
//
// This sample demonstrates the SocketReactor and SocketAcceptor classes.
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
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


#include "Poco/Net/SocketReactor.h"
#include "Poco/Net/SocketAcceptor.h"
#include "Poco/Net/SocketNotification.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/ServerSocket.h"
#include "Poco/NObserver.h"
#include "Poco/Exception.h"
#include "Poco/Thread.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/Script/Lua/Environment.h"
#include "Poco/Script/Lua/Persister.h"
#include <iostream>
#include <vector>


using Poco::Net::SocketReactor;
using Poco::Net::SocketAcceptor;
using Poco::Net::ReadableNotification;
using Poco::Net::ShutdownNotification;
using Poco::Net::ServerSocket;
using Poco::Net::StreamSocket;
using Poco::NObserver;
using Poco::AutoPtr;
using Poco::Thread;
using Poco::Exception;
using Poco::Util::ServerApplication;
using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::HelpFormatter;
using Poco::Script::Lua::Environment;
using Poco::Script::Lua::Persister;


class LuaServiceHandler
{
public:
	LuaServiceHandler(StreamSocket& socket, SocketReactor& reactor):
		_socket(socket),
		_reactor(reactor),
		_pBuffer(new char[BUFFER_SIZE])
	{
		Application& app = Application::instance();
		//app.logger().information("Connection from " + socket.peerAddress().toString());

		_reactor.addEventHandler(_socket, NObserver<LuaServiceHandler, ReadableNotification>(*this, &LuaServiceHandler::onReadable));
		_reactor.addEventHandler(_socket, NObserver<LuaServiceHandler, ShutdownNotification>(*this, &LuaServiceHandler::onShutdown));
	}
	
	~LuaServiceHandler()
	{/*
		Application& app = Application::instance();
		try
		{
			app.logger().information("Disconnecting " + _socket.peerAddress().toString());
		}
		catch (...)
		{
		}*/
		_reactor.removeEventHandler(_socket, NObserver<LuaServiceHandler, ReadableNotification>(*this, &LuaServiceHandler::onReadable));
		_reactor.removeEventHandler(_socket, NObserver<LuaServiceHandler, ShutdownNotification>(*this, &LuaServiceHandler::onShutdown));
		delete [] _pBuffer;
	}
	
	void onReadable(const AutoPtr<ReadableNotification>& pNf)
	{
		std::vector<char> buffer;
		static int n = 0;
		int r = 0;
		do 
		{
			try { r = _socket.receiveBytes(_pBuffer, BUFFER_SIZE); }
			catch (Exception&) {	break; }
			n += r;
		}while (r);
		
		buffer.assign(_pBuffer, _pBuffer + n);

		Environment e;
		Persister p(e);
		p.unpersist(&buffer);
		e.getField(-1, "doHello");
		e.pushValue(-2);
		e.call("", 1, 0);
		e.getField(-1, "counter");
		int counter = e.toNumber(-1) + 1;

		std::vector<char>().swap(buffer);//clear the buffer

		e.newTable();
		e.newTable();
		e.pushString("name");
		e.pushString("there");
		e.setTable(-3);
		e.pushString("counter");
		e.pushNumber(counter); //increment the counter
		e.setTable(-3);
		e.pushString("doHello");
		e.call("return function (a) print('hello from ' .. a.name .. ' world #' .. a.counter); end;");
		e.setTable(-3);
		p.persist(&buffer);

		_socket.sendBytes(&buffer[0], buffer.size());
		delete this;
	}
	
	void onShutdown(const AutoPtr<ShutdownNotification>& pNf)
	{
		delete this;
	}
	
private:
	enum
	{
		BUFFER_SIZE = 1024
	};
	
	StreamSocket   _socket;
	SocketReactor& _reactor;
	char*          _pBuffer;
};


class LuaServer: public Poco::Util::ServerApplication
	/// The main application class.
	///
	/// This class handles command-line arguments and
	/// configuration files.
	/// Start the LuaServer executable with the help
	/// option (/help on Windows, --help on Unix) for
	/// the available command line options.
	///
	/// To use the sample configuration file (LuaServer.properties),
	/// copy the file to the directory where the LuaServer executable
	/// resides. If you start the debug version of the LuaServer
	/// (EchoServerd[.exe]), you must also create a copy of the configuration
	/// file named EchoServerd.properties. In the configuration file, you
	/// can specify the port on which the server is listening (default
	/// 9977) and the format of the date/time string sent back to the client.
	///
	/// To test the LuaServer you can use any telnet client (telnet localhost 9977).
{
public:
	LuaServer(): _helpRequested(false)
	{
	}
	
	~LuaServer()
	{
	}

protected:
	void initialize(Application& self)
	{
		loadConfiguration(); // load default configuration files, if present
		ServerApplication::initialize(self);
	}
		
	void uninitialize()
	{
		ServerApplication::uninitialize();
	}

	void defineOptions(OptionSet& options)
	{
		ServerApplication::defineOptions(options);
		
		options.addOption(
			Option("help", "h", "display help information on command line arguments")
				.required(false)
				.repeatable(false));
	}

	void handleOption(const std::string& name, const std::string& value)
	{
		ServerApplication::handleOption(name, value);

		if (name == "help")
			_helpRequested = true;
	}

	void displayHelp()
	{
		HelpFormatter helpFormatter(options());
		helpFormatter.setCommand(commandName());
		helpFormatter.setUsage("OPTIONS");
		helpFormatter.setHeader("An echo server implemented using the Reactor and Acceptor patterns.");
		helpFormatter.format(std::cout);
	}

	int main(const std::vector<std::string>& args)
	{
		if (_helpRequested)
		{
			displayHelp();
		}
		else
		{
			// get parameters from configuration file
			unsigned short port = (unsigned short) config().getInt("LuaServer.port", 9977);
			
			// set-up a server socket
			ServerSocket svs(port);
			// set-up a SocketReactor...
			SocketReactor reactor;
			// ... and a SocketAcceptor
			SocketAcceptor<LuaServiceHandler> acceptor(svs, reactor);
			// run the reactor in its own thread so that we can wait for 
			// a termination request
			Thread thread;
			thread.start(reactor);
			// wait for CTRL-C or kill
			waitForTerminationRequest();
			// Stop the SocketReactor
			reactor.stop();
			thread.join();
		}
		return Application::EXIT_OK;
	}
	
private:
	bool _helpRequested;
};


int main(int argc, char** argv)
{
	LuaServer app;
	return app.run(argc, argv);
}
