/*
 * DLWrapperImpl.h
 *
 *  Created on: Nov 23, 2014
 *      Author: alanmc
 */

#ifndef IMPL_DLWRAPPERIMPL_H_
#define IMPL_DLWRAPPERIMPL_H_

#include "../InterfaceLibraryDLWrapper.h"

namespace IfLibrary {

class DLWrapperImpl: public DLWrapper {
public:
    DLWrapperImpl();
    virtual ~DLWrapperImpl() {}

    virtual void* dlopen(const std::string& file);
    virtual void* dlsym(void* handle, const std::string& symbol);
    virtual char* dlerror();
};

} /* namespace IfLibrary */

#endif /* IMPL_DLWRAPPERIMPL_H_ */
