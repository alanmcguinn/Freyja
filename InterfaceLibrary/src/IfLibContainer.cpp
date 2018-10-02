/*
 * IfLibContainer.cpp
 *
 *  Created on: Apr 19, 2015
 *      Author: alanmc
 */

#include "../include/IfLibContainer.h"

namespace IfLibrary {

IfLibContainer::IfLibContainer(const std::type_info& type, const InterfaceVersion& version, CreateFn creatFunction) : m_type(type), m_version(version), m_createFunction(creatFunction) {
}

const std::type_index& IfLibContainer::getType() const {
    return m_type;
}

const InterfaceVersion& IfLibContainer::getVersion() const {
    return m_version;
}

IfLibContainer::CreateFn IfLibContainer::getCreateFunction() const {
    return m_createFunction;
}

} /* namespace IfLibrary */
