//
// PathMapping.h
//
//
// Library: ServletEx
// Package: Servlet
// Module:  PathMapping
//
// Definition of the PathMapping class.
//
// Copyright (c) 2006, Aleksandar Fabijanic and Contributors.
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


#ifndef ServletEx_PathMapping_INCLUDED
#define ServletEx_PathMapping_INCLUDED


#include "Poco/Servlet/ServletBase.h"
#include <string>
#include <map>


#if defined(_MSC_VER)
	#pragma warning(disable:4251) // ... needs to have dll-interface warning 
#endif


namespace Poco {
namespace Servlet {
namespace Ex {


class Servlet_API PathMapping
	/// PathMapping class maps paths to servlet names.
	/// Paths do not contain context portion. 
	/// 
	///	Path forming rules are:
	///
	/// - A string beginning with a �/� character and ending with a �/*� suffix is used
	///		for path mapping.
	///	- A string beginning with a �*.� prefix is used as an extension mapping.
	///	- A string containing only the �/� character indicates the "default" servlet of
	///		the application. In this case the servlet path is the request URI minus the context
	///		path and the path info is null.
	///	- All other strings are used for exact matches only.
{
public:

	PathMapping();

	virtual ~PathMapping();

	void addMapping(const std::string& path, const std::string& name);
		/// Add name-path to the mapping.

	void removeName(const std::string& name);
		/// Remove all the the entries with the specified name from the mapping.

	void removePath(const std::string& path);
		/// Remove the entry with specified path from the mapping.

	std::string resolveName(const std::string& path);
		/// Resolve the name of the servlet that handles the given path.
		/// This method assumes that path does not contain context name.
		/// 
		/// Here is an example resolve scenario:
		///
		///	o Mapping:
		///		Path pattern	|	Servlet Name
		///		----------------+-------------
		///		/				|	servlet0
		///		/foo/bar/*		|	servlet1
		///		/baz/*			|	servlet2
		///		/catalog		|	servlet3
		///		*.bop			|	servlet4
		///
		///	o Resolve:
		///		Path 					| Servlet Name
		///		------------------------+-------------
		///		/foo/bar/index.html		| servlet1
		///		/foo/bar/index.bop		| servlet1
		///		/baz					| servlet2
		///		/baz/index.html			| servlet2
		///		/catalog				| servlet3
		///		/catalog/index.html 	| servlet0
		///		/catalog/racecar.bop	| servlet4
		///		/index.bop				| servlet4
		
	std::string resolvePath(const std::string& name) const;
		/// Resolves the path for the given servlet name.
		/// If multiple paths are mapped to the servlet, the first one
		/// one found is returned.

	std::string getDefaultServlet(const std::string& path = "/");
		/// Returns the default servlet (normally the servlet mapped to "/" path).

	static std::string splitPath(std::string& path);
		/// Utility method.
		/// Extracts and returns context name from path string.
		///	It modifies the path argument into path only 
		/// (without preceding context name) portion.
		/// For example: extractName("/context/dir1/") shall
		/// return string "context" and modify path into "/dir1/";
		/// extractName("/context/dir1/file.txt") shall
		/// return string "context" and modify path into "/dir1/file.txt";

protected:

	typedef std::map<std::string, std::string> MappingMap;
		/// [path] - [servlet] map

	MappingMap _mapping;
	static const char PATH_SEPARATOR;
};


} } } // namespace Poco::Servlet::Ex


#endif //ServletEx_PathMapping_INCLUDED
