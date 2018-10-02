/*
 * InterfaceLibraryException.cpp
 *
 *  Created on: Nov 23, 2014
 *      Author: alanmc
 */

#include "../include/InterfaceLibraryException.h"

namespace IfLibrary {

InterfaceLibraryException::InterfaceLibraryException(const std::string& exceptionStr) : std::runtime_error(exceptionStr) {
}

} /* namespace IfLibrary */
