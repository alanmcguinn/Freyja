/*
 * InterfaceVersion.cpp
 *
 *  Created on: Nov 23, 2014
 *      Author: alanmc
 */

#include "../include/InterfaceVersion.h"
#include "../include/InterfaceLibraryException.h"

#include <sstream>
#include <boost/lexical_cast.hpp>

namespace IfLibrary {

const char InterfaceVersion::VERSION_DELIMITER('.');

InterfaceVersion::InterfaceVersion() : m_majorVersion(0), m_minorVersion(0), m_patchVersion(0), m_buildVersion(0) {
}

InterfaceVersion::InterfaceVersion(short majorVersion, short minorVersion, short patchVersion, short buildVersion) :
    m_majorVersion(majorVersion), m_minorVersion(minorVersion), m_patchVersion(patchVersion), m_buildVersion(buildVersion) {
}

InterfaceVersion::InterfaceVersion(const std::string& versionStr) : m_majorVersion(0), m_minorVersion(0), m_patchVersion(0), m_buildVersion(0) {
    std::vector<std::string> versions;
    splitVersionString(versionStr, VERSION_DELIMITER, versions);
    try {
        switch (versions.size()) {
        case 4:
            m_buildVersion = boost::lexical_cast<short>(versions[3]);
            /* no break */
        case 3:
            m_patchVersion = boost::lexical_cast<short>(versions[2]);
            /* no break */
        case 2:
            m_minorVersion = boost::lexical_cast<short>(versions[1]);
            /* no break */
        case 1:
            m_majorVersion = boost::lexical_cast<short>(versions[0]);
            break;
        default:
            throw InterfaceLibraryException("Invalid Version String. Only supports up to dotted quad version string");
        }
        m_versionStr = versionStr;
    } catch (boost::bad_lexical_cast& ex) {
        std::string exceptionStr("Error parsing subVersion component to short: ");
        exceptionStr.append(ex.what());
        throw InterfaceLibraryException(exceptionStr);
    }
}

InterfaceVersion::InterfaceVersion(const InterfaceVersion& copy) : m_majorVersion(copy.m_majorVersion), m_minorVersion(copy.m_minorVersion),
        m_patchVersion(copy.m_patchVersion), m_buildVersion(copy.m_buildVersion) {
}

const std::string& InterfaceVersion::toString() {
    if (m_versionStr.size() == 0) {
        std::stringstream ss;
        ss << m_majorVersion << VERSION_DELIMITER <<
                m_minorVersion << VERSION_DELIMITER <<
                m_patchVersion << VERSION_DELIMITER <<
                m_buildVersion;
        m_versionStr = ss.str();
    }

    return m_versionStr;
}

bool InterfaceVersion::operator==(const InterfaceVersion& rhs) const {
    return (m_majorVersion == rhs.m_majorVersion &&
            m_minorVersion == rhs.m_minorVersion &&
            m_patchVersion == rhs.m_patchVersion &&
            m_buildVersion == rhs.m_buildVersion);
}

bool InterfaceVersion::operator<(const InterfaceVersion& rhs) const {
    return ( (m_majorVersion < rhs.m_majorVersion) ||
            (m_majorVersion == rhs.m_majorVersion && m_minorVersion < rhs.m_minorVersion) ||
            (m_majorVersion == rhs.m_majorVersion && m_minorVersion == rhs.m_minorVersion && m_patchVersion < rhs.m_patchVersion) ||
            (m_majorVersion == rhs.m_majorVersion && m_minorVersion == rhs.m_minorVersion && m_patchVersion == rhs.m_patchVersion && m_buildVersion < rhs.m_buildVersion));
}

bool InterfaceVersion::operator<=(const InterfaceVersion& rhs) const {
    return ((*this == rhs) || (*this < rhs));
}

bool InterfaceVersion::operator>(const InterfaceVersion& rhs) const {
    return ( (m_majorVersion > rhs.m_majorVersion) ||
            (m_majorVersion == rhs.m_majorVersion && m_minorVersion > rhs.m_minorVersion) ||
            (m_majorVersion == rhs.m_majorVersion && m_minorVersion == rhs.m_minorVersion && m_patchVersion > rhs.m_patchVersion) ||
            (m_majorVersion == rhs.m_majorVersion && m_minorVersion == rhs.m_minorVersion && m_patchVersion == rhs.m_patchVersion && m_buildVersion > rhs.m_buildVersion));
}

bool InterfaceVersion::operator>=(const InterfaceVersion& rhs) const {
    return ((*this == rhs) || (*this > rhs));
}

std::vector<std::string>& InterfaceVersion::splitVersionString(const std::string& versionString, char delimiter, std::vector<std::string>& result) {
    std::stringstream ss(versionString);
    std::string subStr;
    while (std::getline(ss, subStr, delimiter)) {
        if (subStr.length() > 0)
            result.push_back(subStr);
    }
    return result;
}

} /* namespace IfLibrary */

