//
// SSHSessionImpl.cpp
//
// $Id: //poco/Main/SSH/src/SSHSessionImpl.cpp#2 $
//
// Library: SSH
// Package: SSHCore
// Module:  SSHSessionImpl
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


#include "Poco/SSH/SSHSessionImpl.h"
#include "Poco/SSH/SSHHostValidator.h"
#include "Poco/SSH/SSHException.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/SocketImpl.h"
#include "Poco/NumberFormatter.h"


namespace Poco {
namespace SSH {


SSHSessionImpl::SSHSessionImpl(SSHHostValidator& validator, const Poco::Net::SocketAddress& addr, const std::string& user, const std::string& pwd):
	_socket(addr),
	_pSession(0)
{
	establish(validator, user, pwd);
}


	
SSHSessionImpl::SSHSessionImpl(SSHHostValidator& validator, const Poco::Net::StreamSocket& sock, const std::string& user, const std::string& pwd):
	_socket(sock),
	_pSession(0)
{
	establish(validator, user, pwd);
}


SSHSessionImpl::~SSHSessionImpl()
{
	close();
}


void SSHSessionImpl::establish(SSHHostValidator& validator, const std::string& user, const std::string& pwd)
{
	poco_assert (_pSession == 0);

	_pSession = libssh2_session_init();
	if (!_pSession)
		throw SSHException("Session init failed");
	int rc = libssh2_session_startup(_pSession, static_cast<int>(_socket.impl()->sockfd()));
	if(rc)
	{
		libssh2_session_free(_pSession); _pSession = 0;
		throw SSHException("Session init failed:" + Poco::NumberFormatter::format(rc));
	}

	const char* pFingerprint = libssh2_hostkey_hash(_pSession, LIBSSH2_HOSTKEY_HASH_MD5);
	if (!validator.trustWorthy(_socket.peerAddress(), pFingerprint))
	{
		libssh2_session_free(_pSession); _pSession = 0;
		throw SSHException("Server address not trusted");
	}

	if (libssh2_userauth_password(_pSession, user.c_str(), pwd.c_str()))
	{
		close();
		throw SSHAuthException("Authentication failed");
	}
}


void SSHSessionImpl::close()
{
	if (_pSession)
	{
		libssh2_session_disconnect(_pSession, NULL);
		libssh2_session_free(_pSession);
		_pSession = 0;
	}
}


void SSHSessionImpl::setBlocking(bool val)
{
	poco_check_ptr (_pSession);
	libssh2_session_set_blocking(_pSession, (val?1:0));
}


bool SSHSessionImpl::isBlocking() const
{
	poco_check_ptr (_pSession);
	return (libssh2_session_get_blocking(_pSession) != 0);
}


} } // namespace Poco::SSH