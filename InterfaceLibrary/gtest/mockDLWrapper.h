/*
 * mockDLWrapper.h
 *
 *  Created on: Nov 23, 2014
 *      Author: alanmc
 */

#ifndef GTEST_MOCKDLWRAPPER_H_
#define GTEST_MOCKDLWRAPPER_H_

#include "../include/InterfaceLibraryDLWrapper.h"
#include "gmock/gmock.h"

namespace IfLibrary_mock {

class mock_DLWrapper : public IfLibrary::DLWrapper {
public:
    mock_DLWrapper() { dummyHandle = (void*)1234;}
    virtual ~mock_DLWrapper() {}

    MOCK_METHOD1(dlopen, void*(const std::string&));
    MOCK_METHOD2(dlsym, void*(void*, const std::string&));
    MOCK_METHOD0(dlerror, char*());

    void* dummyHandle;

};

} /* namespace IfLibrary_mock */

#endif /* GTEST_MOCKDLWRAPPER_H_ */
