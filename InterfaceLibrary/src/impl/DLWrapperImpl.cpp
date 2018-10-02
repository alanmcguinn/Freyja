/*
 * DLWrapperImpl.cpp
 *
 *  Created on: Nov 23, 2014
 *      Author: alanmc
 */

#include "impl/DLWrapperImpl.h"
#include <dlfcn.h>

namespace IfLibrary {

DLWrapperImpl::DLWrapperImpl() {
}

void* DLWrapperImpl::dlopen(const std::string& file) {
    void* modHandle = ::dlopen(file.c_str(), RTLD_NOW);

    return modHandle;
}

void* DLWrapperImpl::dlsym(void* handle, const std::string& symbol) {
    void* regMethod = ::dlsym(handle, symbol.c_str());

    return regMethod;
}

char* DLWrapperImpl::dlerror() {
    return ::dlerror();
}

} /* namespace IfLibrary */

