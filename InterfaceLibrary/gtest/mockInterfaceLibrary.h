/*
 * mockInterfaceLibrary.h
 *
 *  Created on: Nov 23, 2014
 *      Author: alanmc
 */

#ifndef GTEST_MOCKINTERFACELIBRARY_H_
#define GTEST_MOCKINTERFACELIBRARY_H_

#include "../include/InterfaceLibrary.h"
#include "../include/UnknownInterface.h"
#include "../include/IfLibContainer.h"

namespace IfLibrary_mock {

class mock_InterfaceLibrary : public IfLibrary::InterfaceLibrary {
public:
    mock_InterfaceLibrary() {};
    ~mock_InterfaceLibrary() {};

    MOCK_METHOD3(registerInterfaceImpl, void(const std::type_info& type, const IfLibrary::InterfaceVersion& version, IfLibrary::IfLibContainer::CreateFn createMethod));
    MOCK_METHOD3(createInterfaceImpl, IfLibrary::UnknownInterface*(const std::type_info& type, const std::string& semanticVersion, va_list& argList));
};

} /* namespace IfLibrary_mock */

#endif /* GTEST_MOCKINTERFACELIBRARY_H_ */
