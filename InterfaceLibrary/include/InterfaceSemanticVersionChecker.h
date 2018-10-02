/*
 * InterfaceSemanticVersionChecker.h
 *
 *  Created on: Apr 18, 2015
 *      Author: alanmc
 */

#ifndef INTERFACESEMANTICVERSIONCHECKER_H_
#define INTERFACESEMANTICVERSIONCHECKER_H_

#include "InterfaceVersion.h"

namespace IfLibrary {

class InterfaceSemanticVersionChecker {
public:
    InterfaceSemanticVersionChecker(const std::string& semanticVersionString);
    virtual ~InterfaceSemanticVersionChecker() {}

    bool satisfies(const InterfaceVersion& versionToCheck);

    static const char LEFT_SQUARE = '[';
    static const char LEFT_ROUND = '(';
    static const char RIGHT_SQUARE = ']';
    static const char RIGHT_ROUND = ')';

    static const char VERSION_SEPARATOR = ',';

private:

    void parseSemanticVersionString(const std::string& semanticVersionString);

    bool m_lowInclusive;
    bool m_highInclusive;

    InterfaceVersion m_lowVersion;
    InterfaceVersion m_highVersion;
};

} /* namespace IfLibrary */

#endif /* INTERFACESEMANTICVERSIONCHECKER_H_ */
