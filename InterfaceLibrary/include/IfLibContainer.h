/*
 * IfLibContainer.h
 *
 *  Created on: Apr 19, 2015
 *      Author: alanmc
 */

#ifndef IFLIBCONTAINER_H_
#define IFLIBCONTAINER_H_

#include "InterfaceVersion.h"
#include "UnknownInterface.h"
#include <typeindex>

namespace IfLibrary {

class IfLibContainer {
public:
    typedef UnknownInterface* (*CreateFn)(va_list& argList);

    IfLibContainer(const std::type_info& type, const InterfaceVersion& version, CreateFn creatFunction);
    virtual ~IfLibContainer() {}

    const std::type_index& getType() const;
    const InterfaceVersion& getVersion() const;

    CreateFn getCreateFunction() const;

private:
    std::type_index m_type;
    InterfaceVersion m_version;
    CreateFn m_createFunction;
};

} /* namespace IfLibrary */

#endif /* IFLIBCONTAINER_H_ */
