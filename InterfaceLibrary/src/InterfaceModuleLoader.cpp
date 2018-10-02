/*
 * InterfaceModuleLoader.cpp
 *
 *  Created on: Nov 22, 2014
 *      Author: alanmc
 */

#include "../include/InterfaceModuleLoader.h"
#include <iostream>

namespace IfLibrary {

const std::string InterfaceModuleLoader::MODULE_EXTENSION(".so");
const std::string InterfaceModuleLoader::MODULE_REG_METHOD("registerInterfaces");

InterfaceModuleLoader::InterfaceModuleLoader(IfLibrary::InterfaceLibrary& ifLib, DLWrapper& dlWrapper, const std::string path) :
        m_ifLib(ifLib), m_dlWrapper(dlWrapper) {
    m_paths.push_back(boost::filesystem::path(path));
}

InterfaceModuleLoader::InterfaceModuleLoader(InterfaceLibrary& ifLib, DLWrapper& dlWrapper, const std::vector<std::string>& paths) :
        m_ifLib(ifLib), m_dlWrapper(dlWrapper) {
    for (std::vector<std::string>::const_iterator i = paths.begin(); i != paths.end(); i++) {
        m_paths.push_back(boost::filesystem::path(*i));
    }
}

InterfaceModuleLoader::~InterfaceModuleLoader() {
}

void InterfaceModuleLoader::addPath(const std::string path) {
    m_paths.push_back(boost::filesystem::path(path));
}

void InterfaceModuleLoader::loadModules() {
    using namespace boost::filesystem;

    for (Paths::const_iterator i = m_paths.begin(); i != m_paths.end(); i++) {
        path curPath = *i;
        loadModulesFromPath(curPath);
    }
}

void InterfaceModuleLoader::loadModulesFromPath(const boost::filesystem::path& path) {
    using namespace boost::filesystem;

    if (exists(path) && is_directory(path)) {
        for (directory_iterator dirIter(path); dirIter != directory_iterator(); dirIter++) {
            if (extension(dirIter->path()) == MODULE_EXTENSION) {
                boost::upgrade_lock<boost::shared_mutex> upLock(m_loaderLock);
                if (!moduleAlreadyLoaded(dirIter->path())) {
                    boost::upgrade_to_unique_lock<boost::shared_mutex> wrLock(upLock);
                    loadModule(dirIter->path());
                    std::cout << "Loading " << dirIter->path().string() << std::endl;
                }
            }
        }
    }
}

void InterfaceModuleLoader::loadModule(const boost::filesystem::path& modPath) {
    void* modHandle = m_dlWrapper.dlopen(modPath.string());
    if (modHandle != 0) {
        InterfaceLibrary::RegisterFn regMethod = (InterfaceLibrary::RegisterFn)(m_dlWrapper.dlsym(modHandle, MODULE_REG_METHOD));
        if (regMethod) {
            std::cout << "sym found" << std::endl;
            regMethod(m_ifLib);
            m_loadedPaths.insert(modPath);
        } else {
            std::cout << "sym NOT found" << std::endl;
        }
//        dlclose(modHandle);
    } else {
        std::cout << "LoadModule Error: " << m_dlWrapper.dlerror() << std::endl;
    }
}

bool InterfaceModuleLoader::moduleAlreadyLoaded(const boost::filesystem::path& modPath) {
    bool retVal = (m_loadedPaths.find(modPath) != m_loadedPaths.end());
    return retVal;
}

} /* namespace IfLibrary */

