/*
 * InterfaceSemanticVersionChecker.cpp
 *
 *  Created on: Apr 18, 2015
 *      Author: alanmc
 */

#include "../include/InterfaceSemanticVersionChecker.h"
#include "../include/InterfaceLibraryException.h"

#include <sstream>

namespace IfLibrary {

InterfaceSemanticVersionChecker::InterfaceSemanticVersionChecker(const std::string& semanticVersionString): m_lowInclusive(true), m_highInclusive(true) {
    parseSemanticVersionString(semanticVersionString);
}

bool InterfaceSemanticVersionChecker::satisfies(const InterfaceVersion& versionToCheck) {
    bool retVal = true;

    retVal = retVal && (m_lowInclusive ? (m_lowVersion <= versionToCheck) : (m_lowVersion < versionToCheck));
    retVal = retVal && (m_highInclusive ? (m_highVersion >= versionToCheck) : (m_highVersion > versionToCheck));

    return retVal;
}

void InterfaceSemanticVersionChecker::parseSemanticVersionString(const std::string& semanticVersionString) {
    if (semanticVersionString.empty())
        throw InterfaceLibraryException("Semantic VersionString is empty");
    switch  (semanticVersionString[0]) {
    case LEFT_SQUARE:
        m_lowInclusive = true;
        break;
    case LEFT_ROUND:
        m_lowInclusive = false;
        break;
    default:
        std::stringstream ss;
        ss << "Unexpected character @ 0 = \'" << semanticVersionString[0] << "\'. Should be either \'" << LEFT_SQUARE << "\' or \'" << LEFT_ROUND << "\'.";
        throw InterfaceLibraryException(ss.str());
    }

    switch  (semanticVersionString[semanticVersionString.length() - 1]) {
    case RIGHT_SQUARE:
        m_highInclusive = true;
        break;
    case RIGHT_ROUND:
        m_highInclusive = false;
        break;
    default:
        std::stringstream ss;
        ss << "Unexpected character @ " << (semanticVersionString.length() - 1) <<" = \'" << semanticVersionString[0] << "\'. Should be either \'" << RIGHT_SQUARE << "\' or \'" << RIGHT_ROUND << "\'.";
        throw InterfaceLibraryException(ss.str());
    }

    std::string::size_type sepPos = semanticVersionString.find(VERSION_SEPARATOR);
    if (sepPos == std::string::npos) {
        throw InterfaceLibraryException("Cannot find Version Separator \',\'. Version Check format looks like [1.2.3.4,9.0.0.0)");
    }
    std::string lowVer = semanticVersionString.substr(1, sepPos - 1);
    std::string highVer = semanticVersionString.substr(sepPos + 1, semanticVersionString.length() - sepPos - 2);

    m_lowVersion = InterfaceVersion(lowVer);
    m_highVersion = InterfaceVersion(highVer);
}

} /* namespace IfLibrary */
