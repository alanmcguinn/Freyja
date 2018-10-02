/*
 * InterfaceModuleLoaderMonitor.cpp
 *
 *  Created on: Nov 22, 2014
 *      Author: alanmc
 */

#include "../include/InterfaceModuleLoaderMonitor.h"
#include "../include/InterfaceModuleLoader.h"

#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/chrono/system_clocks.hpp>
#include <boost/thread.hpp>

namespace IfLibrary {

InterfaceModuleLoaderMonitor::InterfaceModuleLoaderMonitor(InterfaceModuleLoader& modLoader, unsigned int intervalInSecs) :
        m_modLoader(modLoader), m_intervalInSecs(intervalInSecs), m_running (true) {
}

InterfaceModuleLoaderMonitor::~InterfaceModuleLoaderMonitor() {
}

void InterfaceModuleLoaderMonitor::operator()() {
    while (m_running) {
        using boost::chrono::system_clock;
        using boost::chrono::seconds;
        system_clock::time_point nextPollTime = system_clock::now() + seconds(m_intervalInSecs);

        m_modLoader.loadModules();

        using boost::this_thread::sleep_until;
        sleep_until(nextPollTime);
    }
}

void InterfaceModuleLoaderMonitor::stop() {
    m_running = false;
}

} /* namespace IfLibrary */
