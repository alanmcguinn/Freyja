/*
 * InterfaceLibrary.h
 *
 *  Created on: Jan 30, 2014
 *      Author: alanmc
 */

#ifndef INTERFACELIBRARYIMPL_H_
#define INTERFACELIBRARYIMPL_H_

#include "../InterfaceLibrary.h"
#include <typeindex>
#include <map>

#include <boost/thread/shared_mutex.hpp>
#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <boost/multi_index/mem_fun.hpp>

namespace IfLibrary {

class InterfaceLibraryImpl : public InterfaceLibrary {
public:
    InterfaceLibraryImpl() {}

private:

    typedef boost::multi_index_container<IfLibContainer,
            boost::multi_index::indexed_by<
                // sort by composite key timerName:DocumentSetId
                boost::multi_index::ordered_unique<
                    boost::multi_index::composite_key<IfLibContainer,
                        boost::multi_index::const_mem_fun<IfLibContainer, const std::type_index&, &IfLibContainer::getType> ,
                        boost::multi_index::const_mem_fun<IfLibContainer, const InterfaceVersion&, &IfLibContainer::getVersion> >,
                        boost::multi_index::composite_key_compare<std::less<std::type_index>, std::greater<InterfaceVersion> > > > >
    InterfaceLibraryRepo;

    virtual void registerInterfaceImpl(const std::type_info& type, const InterfaceVersion& interfaceVersion, IfLibContainer::CreateFn createMethod);
    UnknownInterface* createInterfaceImpl(const std::type_info& type, const std::string& semanticVersion, va_list& argsList);

    InterfaceLibraryRepo m_libraryMap;

    boost::shared_mutex m_libraryLock;
};

} /* namespace InterfaceLibrary */

#endif /* INTERFACELIBRARYIMPL_H_ */
