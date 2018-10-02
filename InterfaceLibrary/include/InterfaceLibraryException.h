/*
 * InterfaceLibraryException.h
 *
 *  Created on: Nov 23, 2014
 *      Author: alanmc
 */

#ifndef INTERFACELIBRARYEXCEPTION_H_
#define INTERFACELIBRARYEXCEPTION_H_

#include <stdexcept>

namespace IfLibrary {

class InterfaceLibraryException: public std::runtime_error {
public:
    explicit InterfaceLibraryException(const std::string& exceptionStr);
    virtual ~InterfaceLibraryException() {}
};

} /* namespace IfLibrary */

#endif /* INTERFACELIBRARYEXCEPTION_H_ */
