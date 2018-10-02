/*
 * InterfaceModuleLoader.h
 *
 *  Created on: Nov 22, 2014
 *      Author: alanmc
 */

#ifndef INTERFACEMODULELOADER_H_
#define INTERFACEMODULELOADER_H_

#include <string>
#include <vector>
#include <set>

#include <boost/filesystem.hpp>
#include <boost/thread/shared_mutex.hpp>

#include "InterfaceLibrary.h"
#include "InterfaceLibraryDLWrapper.h"

namespace IfLibrary {

class InterfaceModuleLoader {
public:
    InterfaceModuleLoader(InterfaceLibrary& ifLib, DLWrapper& dlWrapper, const std::string path);
    InterfaceModuleLoader(InterfaceLibrary& ifLib, DLWrapper& dlWrapper, const std::vector<std::string>& paths);

    void addPath(const std::string path);

    virtual ~InterfaceModuleLoader();

    void loadModules();

private:

    void loadModule(const boost::filesystem::path& modPath);
    void loadModulesFromPath(const boost::filesystem::path& path);
    bool moduleAlreadyLoaded(const boost::filesystem::path& modPath);

    static const std::string MODULE_EXTENSION;
    static const std::string MODULE_REG_METHOD;

    InterfaceLibrary& m_ifLib;
    DLWrapper& m_dlWrapper;

    typedef std::vector<boost::filesystem::path> Paths;
    Paths m_paths;

    typedef std::set<boost::filesystem::path> LoadedPaths;
    LoadedPaths m_loadedPaths;

    boost::shared_mutex m_loaderLock;
};

}

#endif /* INTERFACEMODULELOADER_H_ */
