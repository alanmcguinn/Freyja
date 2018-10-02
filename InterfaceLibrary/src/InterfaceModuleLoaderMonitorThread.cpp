/*
 * InterfaceModuleLoaderMonitorThread.cpp
 *
 *  Created on: Nov 30, 2014
 *      Author: alanmc
 */

#include "../include/InterfaceModuleLoaderMonitorThread.h"

namespace IfLibrary {

InterfaceModuleLoaderMonitorThread::InterfaceModuleLoaderMonitorThread(InterfaceModuleLoaderMonitor& monitor) :
    m_monitor(monitor){
}

void InterfaceModuleLoaderMonitorThread::start() {
    m_monitorThread = boost::thread(boost::ref(m_monitor));
}

void InterfaceModuleLoaderMonitorThread::stop() {
    m_monitor.stop();
    m_monitorThread.interrupt();
    m_monitorThread.join();
}

} /* namespace IfLibrary */
