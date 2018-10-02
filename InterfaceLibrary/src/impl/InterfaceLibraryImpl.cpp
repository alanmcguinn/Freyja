/*
 * InterfaceLibrary.cpp
 *
 *  Created on: Jan 30, 2014
 *      Author: alanmc
 */

#include "impl/InterfaceLibraryImpl.h"
#include "InterfaceSemanticVersionChecker.h"

#include <iostream>
#include <boost/range/iterator_range.hpp>

namespace IfLibrary {

void InterfaceLibraryImpl::registerInterfaceImpl(const std::type_info& type, const InterfaceVersion& interfaceVersion, IfLibContainer::CreateFn createMethod) {
    boost::unique_lock<boost::shared_mutex> lock(m_libraryLock);

    m_libraryMap.insert(IfLibContainer(type, interfaceVersion, createMethod));

}

UnknownInterface* InterfaceLibraryImpl::createInterfaceImpl(const std::type_info& type, const std::string& semanticVersion, va_list& argsList) {
    UnknownInterface* retVal = 0;
    std::cout << "Looking for interface for type " << type.name() << std::endl;

    auto& index = m_libraryMap.get<0>();
    auto range = index.equal_range(std::type_index(type));

    /*
     * We're guaranteed that we will get the results in descending Version order here.
     * So, the first entry we get that matches our semantic version will be the highest
     * registered interface that matches
     */
    InterfaceSemanticVersionChecker versionChecker(semanticVersion);
    for (const auto& ifLibContainer : boost::make_iterator_range(range.first, range.second)) {
        if (versionChecker.satisfies(ifLibContainer.getVersion())) {
            retVal = ifLibContainer.getCreateFunction()(argsList);
            break;
        }
    }

    return retVal;
}

} /* namespace InterfaceLibrary */

