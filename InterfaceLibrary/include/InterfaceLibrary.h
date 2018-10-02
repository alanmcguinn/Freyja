/*
 * InterfaceLibrary.h
 *
 *  Created on: Nov 21, 2014
 *      Author: alanmc
 */

#ifndef INTERFACELIBRARY_H_
#define INTERFACELIBRARY_H_

#include <stdarg.h>
#include <typeinfo>

#include "InterfaceVersion.h"
#include "IfLibContainer.h"

namespace IfLibrary {

class InterfaceLibrary {
public:
    virtual ~InterfaceLibrary() {}

    template <typename InterfaceType> void registerInterface(const std::string& interfaceVersion, IfLibContainer::CreateFn createMethod) {
        registerInterfaceImpl(typeid(InterfaceType), InterfaceVersion(interfaceVersion), createMethod);
    }

    template<typename InterfaceType> InterfaceType* createInterface(const std::string& semanticVersion, ...) {
        InterfaceType* retVal = 0;

        va_list argsList;

        va_start(argsList, semanticVersion);
        retVal = dynamic_cast<InterfaceType*>(createInterfaceImpl(typeid(InterfaceType), semanticVersion, argsList));
        va_end(argsList);

        return retVal;
    }

    typedef void (*RegisterFn)(IfLibrary::InterfaceLibrary& ifLib);

private:

    virtual void registerInterfaceImpl(const std::type_info& type, const InterfaceVersion& interfaceVersion, IfLibContainer::CreateFn createMethod) = 0;
    virtual UnknownInterface* createInterfaceImpl(const std::type_info& type, const std::string& semanticVersion, va_list& argList) = 0;

};

} // namespace IfLibrary

#endif /* INTERFACELIBRARY_H_ */
