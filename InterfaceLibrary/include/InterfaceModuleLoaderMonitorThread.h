/*
 * InterfaceModuleLoaderMonitorThread.h
 *
 *  Created on: Nov 30, 2014
 *      Author: alanmc
 */

#ifndef INTERFACEMODULELOADERMONITORTHREAD_H_
#define INTERFACEMODULELOADERMONITORTHREAD_H_

#include <boost/thread.hpp>
#include "InterfaceModuleLoaderMonitor.h"

namespace IfLibrary {

class InterfaceModuleLoaderMonitorThread {
public:
    InterfaceModuleLoaderMonitorThread(InterfaceModuleLoaderMonitor& monitor);
    virtual ~InterfaceModuleLoaderMonitorThread() {}

    void start();
    void stop();

private:
    boost::thread m_monitorThread;
    InterfaceModuleLoaderMonitor& m_monitor;
};

} /* namespace IfLibrary */

#endif /* INTERFACEMODULELOADERMONITORTHREAD_H_ */
