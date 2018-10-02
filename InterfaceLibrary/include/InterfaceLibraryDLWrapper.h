/*
 * InterfaceLibraryDLWrapper.h
 *
 *  Created on: Nov 23, 2014
 *      Author: alanmc
 */

#ifndef INTERFACELIBRARYDLWRAPPER_H_
#define INTERFACELIBRARYDLWRAPPER_H_

#include <string>

namespace IfLibrary {

class DLWrapper {
public:
    virtual ~DLWrapper() {}

    virtual void* dlopen(const std::string& file) = 0;
    virtual void* dlsym(void* handle, const std::string& symbol) = 0;
    virtual char* dlerror() = 0;
};

} /* namespace IfLibrary */

#endif /* INTERFACELIBRARYDLWRAPPER_H_ */
