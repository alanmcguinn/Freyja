/*
 * InterfaceVersion.h
 *
 *  Created on: Nov 23, 2014
 *      Author: alanmc
 */

#ifndef INTERFACEVERSION_H_
#define INTERFACEVERSION_H_

#include <string>
#include <vector>

namespace IfLibrary {

class InterfaceVersion {
public:
    InterfaceVersion();
    explicit InterfaceVersion(short majorVersion, short minorVersion = 0, short patchVersion = 0, short buildVersion = 0);
    explicit InterfaceVersion(const std::string& versionStr);
    InterfaceVersion(const InterfaceVersion& copy);
    virtual ~InterfaceVersion() {}

    const std::string& toString();

    bool operator==(const InterfaceVersion& rhs) const;
    bool operator<(const InterfaceVersion& rhs) const;
    bool operator<=(const InterfaceVersion& rhs) const;
    bool operator>(const InterfaceVersion& rhs) const;
    bool operator>=(const InterfaceVersion& rhs) const;

    InterfaceVersion& operator++(int);
    InterfaceVersion& operator--(int);

#ifdef GTEST
    short getMajor() const { return m_majorVersion; }
    short getMinor() const { return m_minorVersion; }
    short getPatch() const { return m_patchVersion; }
    short getBuild() const { return m_buildVersion; }

#endif

private:

    std::vector<std::string>& splitVersionString(const std::string& versionString, char delimiter, std::vector<std::string>& result);

    short m_majorVersion;
    short m_minorVersion;
    short m_patchVersion;
    short m_buildVersion;

    std::string m_versionStr;

    static const char VERSION_DELIMITER;
};

} /* namespace IfLibrary */

#endif /* INTERFACEVERSION_H_ */
