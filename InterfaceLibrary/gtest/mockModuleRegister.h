/*
 * mockModuleRegister.h
 *
 *  Created on: Dec 10, 2014
 *      Author: alanmc
 */

#ifndef GTEST_MOCKMODULEREGISTER_H_
#define GTEST_MOCKMODULEREGISTER_H_

#include "../include/InterfaceLibrary.h"
#include "../include/InterfaceVersion.h"

class DummyType : IfLibrary::UnknownInterface {
public:
    static UnknownInterface* create(va_list& va_args) {
        return new DummyType;
    }

    static long hashCode() {
        return 5678;
    }
};

extern "C" void registerInterfaces(IfLibrary::InterfaceLibrary& ifLib) {
    ifLib.registerInterface<DummyType>("1.0.0.0", DummyType::create);
}



#endif /* GTEST_MOCKMODULEREGISTER_H_ */
