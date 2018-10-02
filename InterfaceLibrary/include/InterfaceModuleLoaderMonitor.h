/*
 * InterfaceModuleLoaderMonitor.h
 *
 *  Created on: Nov 22, 2014
 *      Author: alanmc
 */

#ifndef INTERFACEMODULELOADERMONITOR_H_
#define INTERFACEMODULELOADERMONITOR_H_

#include <atomic>

namespace IfLibrary {

class InterfaceModuleLoader;

class InterfaceModuleLoaderMonitor {
public:
    InterfaceModuleLoaderMonitor(InterfaceModuleLoader& modLoader, unsigned int intervalInSecs);
    virtual ~InterfaceModuleLoaderMonitor();

    void operator()();

    void stop();

private:
    InterfaceModuleLoader& m_modLoader;
    unsigned int m_intervalInSecs;
    std::atomic_bool m_running;
};

} /* namespace IfLibrary */

#endif /* INTERFACEMODULELOADERMONITOR_H_ */
