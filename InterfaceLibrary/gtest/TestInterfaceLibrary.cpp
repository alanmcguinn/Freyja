/*
 * TestTimerExpiry.cpp
 *
 *  Created on: Oct 13, 2014
 *      Author: alanmc
 */

#include "gtest/gtest.h"

#include "../include/InterfaceVersion.h"
#include "../include/InterfaceModuleLoader.h"
#include "../include/InterfaceLibraryException.h"
#include "../include/InterfaceSemanticVersionChecker.h"

#include "mockDLWrapper.h"
#include "mockInterfaceLibrary.h"
#include "mockModuleRegister.h"

#include <sys/time.h>
#include <stdarg.h>

TEST(InterfaceVersion, DefaultCtor) {
    IfLibrary::InterfaceVersion version;

    ASSERT_EQ(version.toString(), "0.0.0.0");
}

TEST(InterfaceVersion, MajorVersionOnlyCtor) {
    IfLibrary::InterfaceVersion version(9);

    ASSERT_EQ(version.toString(), "9.0.0.0");
}

TEST(InterfaceVersion, MinorVersionCtor) {
    IfLibrary::InterfaceVersion version(9, 4);

    ASSERT_EQ(version.toString(), "9.4.0.0");
}

TEST(InterfaceVersion, PatchVersionCtor) {
    IfLibrary::InterfaceVersion version(9, 4, 235);

    ASSERT_EQ(version.toString(), "9.4.235.0");
}

TEST(InterfaceVersion, BuildVersionCtor) {
    IfLibrary::InterfaceVersion version(9, 4, 235, 1096);

    ASSERT_EQ(version.toString(), "9.4.235.1096");
}

TEST(InterfaceVersion, MajorVersionStringCtor) {
    IfLibrary::InterfaceVersion version("8");

    ASSERT_EQ(version.toString(), "8");

    ASSERT_EQ(version.getMajor(), 8);
    ASSERT_EQ(version.getMinor(), 0);
    ASSERT_EQ(version.getPatch(), 0);
    ASSERT_EQ(version.getBuild(), 0);
}

TEST(InterfaceVersion, MinorVersionStringCtor) {
    IfLibrary::InterfaceVersion version("8.65");

    ASSERT_EQ(version.toString(), "8.65");

    ASSERT_EQ(version.getMajor(), 8);
    ASSERT_EQ(version.getMinor(), 65);
    ASSERT_EQ(version.getPatch(), 0);
    ASSERT_EQ(version.getBuild(), 0);
}

TEST(InterfaceVersion, PatchVersionStringCtor) {
    IfLibrary::InterfaceVersion version("8.65.3");

    ASSERT_EQ(version.toString(), "8.65.3");

    ASSERT_EQ(version.getMajor(), 8);
    ASSERT_EQ(version.getMinor(), 65);
    ASSERT_EQ(version.getPatch(), 3);
    ASSERT_EQ(version.getBuild(), 0);
}

TEST(InterfaceVersion, BuildVersionStringCtor) {
    IfLibrary::InterfaceVersion version("8.65.3.3999");

    ASSERT_EQ(version.toString(), "8.65.3.3999");

    ASSERT_EQ(version.getMajor(), 8);
    ASSERT_EQ(version.getMinor(), 65);
    ASSERT_EQ(version.getPatch(), 3);
    ASSERT_EQ(version.getBuild(), 3999);
}

TEST(InterfaceVersion, Operators1) {
    IfLibrary::InterfaceVersion version1("8.65.3.3999");
    IfLibrary::InterfaceVersion version2("6.79.128.0");

    ASSERT_EQ((version1 > version2), true);
    ASSERT_EQ((version2 > version1), false);
    ASSERT_EQ((version1 >= version2), true);
    ASSERT_EQ((version2 >= version1), false);
    ASSERT_EQ((version2 < version1), true);
    ASSERT_EQ((version1 < version2), false);
    ASSERT_EQ((version2 <= version1), true);
    ASSERT_EQ((version1 <= version2), false);
    ASSERT_EQ((version1 == version2), false);
}

TEST(InterfaceVersion, Operators2) {
    IfLibrary::InterfaceVersion version1("6.102.3.3999");
    IfLibrary::InterfaceVersion version2("6.79.128.0");

    ASSERT_EQ((version1 > version2), true);
    ASSERT_EQ((version2 > version1), false);
    ASSERT_EQ((version1 >= version2), true);
    ASSERT_EQ((version2 >= version1), false);
    ASSERT_EQ((version2 < version1), true);
    ASSERT_EQ((version1 < version2), false);
    ASSERT_EQ((version2 <= version1), true);
    ASSERT_EQ((version1 <= version2), false);
    ASSERT_EQ((version1 == version2), false);
}

TEST(InterfaceVersion, Operators3) {
    IfLibrary::InterfaceVersion version1("6.79.605.3999");
    IfLibrary::InterfaceVersion version2("6.79.128.0");

    ASSERT_EQ((version1 > version2), true);
    ASSERT_EQ((version2 > version1), false);
    ASSERT_EQ((version1 >= version2), true);
    ASSERT_EQ((version2 >= version1), false);
    ASSERT_EQ((version2 < version1), true);
    ASSERT_EQ((version1 < version2), false);
    ASSERT_EQ((version2 <= version1), true);
    ASSERT_EQ((version1 <= version2), false);
    ASSERT_EQ((version1 == version2), false);
}

TEST(InterfaceVersion, Operators4) {
    IfLibrary::InterfaceVersion version1("6.79.128.3999");
    IfLibrary::InterfaceVersion version2("6.79.128.56");

    ASSERT_EQ((version1 > version2), true);
    ASSERT_EQ((version2 > version1), false);
    ASSERT_EQ((version1 >= version2), true);
    ASSERT_EQ((version2 >= version1), false);
    ASSERT_EQ((version2 < version1), true);
    ASSERT_EQ((version1 < version2), false);
    ASSERT_EQ((version2 <= version1), true);
    ASSERT_EQ((version1 <= version2), false);
    ASSERT_EQ((version1 == version2), false);
}

TEST(InterfaceVersion, Operators5) {
    IfLibrary::InterfaceVersion version1("6.79.605.3999");
    IfLibrary::InterfaceVersion version2("6.79.605.3999");

    ASSERT_EQ((version1 > version2), false);
    ASSERT_EQ((version2 > version1), false);
    ASSERT_EQ((version1 >= version2), true);
    ASSERT_EQ((version2 >= version1), true);
    ASSERT_EQ((version2 < version1), false);
    ASSERT_EQ((version1 < version2), false);
    ASSERT_EQ((version2 <= version1), true);
    ASSERT_EQ((version1 <= version2), true);
    ASSERT_EQ((version1 == version2), true);
}

TEST(InterfaceVersion, CopyCtor) {
    IfLibrary::InterfaceVersion version("6.79.605.3999");
    IfLibrary::InterfaceVersion versionCopy(version);

    ASSERT_EQ(versionCopy.toString(), "6.79.605.3999");
    ASSERT_EQ(version.toString(), versionCopy.toString());

    ASSERT_EQ(version.getMajor(), 6);
    ASSERT_EQ(version.getMinor(), 79);
    ASSERT_EQ(version.getPatch(), 605);
    ASSERT_EQ(version.getBuild(), 3999);

    ASSERT_EQ(versionCopy.getMajor(), 6);
    ASSERT_EQ(versionCopy.getMinor(), 79);
    ASSERT_EQ(versionCopy.getPatch(), 605);
    ASSERT_EQ(versionCopy.getBuild(), 3999);
}

TEST(InterfaceVersion, Assignment) {
    IfLibrary::InterfaceVersion version("6.79.605.3999");
    IfLibrary::InterfaceVersion versionCopy;

    ASSERT_EQ(version.getMajor(), 6);
    ASSERT_EQ(version.getMinor(), 79);
    ASSERT_EQ(version.getPatch(), 605);
    ASSERT_EQ(version.getBuild(), 3999);

    ASSERT_EQ(versionCopy.toString(), "0.0.0.0");
    ASSERT_EQ(versionCopy.getMajor(), 0);
    ASSERT_EQ(versionCopy.getMinor(), 0);
    ASSERT_EQ(versionCopy.getPatch(), 0);
    ASSERT_EQ(versionCopy.getBuild(), 0);

    versionCopy = version;

    ASSERT_EQ(versionCopy.toString(), "6.79.605.3999");
    ASSERT_EQ(version.toString(), versionCopy.toString());

    ASSERT_EQ(version.getMajor(), 6);
    ASSERT_EQ(version.getMinor(), 79);
    ASSERT_EQ(version.getPatch(), 605);
    ASSERT_EQ(version.getBuild(), 3999);

    ASSERT_EQ(versionCopy.getMajor(), 6);
    ASSERT_EQ(versionCopy.getMinor(), 79);
    ASSERT_EQ(versionCopy.getPatch(), 605);
    ASSERT_EQ(versionCopy.getBuild(), 3999);
}

TEST(InterfaceVersion, TooManyVersions) {
    ASSERT_THROW(IfLibrary::InterfaceVersion("1.2.3.4.5"), IfLibrary::InterfaceLibraryException);
}

TEST(InterfaceVersion, InvalidString) {
    ASSERT_THROW(IfLibrary::InterfaceVersion("NotAVersionString"), IfLibrary::InterfaceLibraryException);
}

TEST(InterfaceVersion, PartiallyInvalidString) {
    ASSERT_THROW(IfLibrary::InterfaceVersion("1.2.3.NotAVersionString"), IfLibrary::InterfaceLibraryException);
}


TEST(InterfaceModuleLoader, Instantiation) {
    IfLibrary_mock::mock_DLWrapper mockDLWrapper;
    IfLibrary_mock::mock_InterfaceLibrary mockInterfaceLibrary;

    ASSERT_NO_THROW(IfLibrary::InterfaceModuleLoader(mockInterfaceLibrary, mockDLWrapper, ""));
}

TEST(InterfaceModuleLoader, LoadSingleModuleFromSinglePath) {
    IfLibrary_mock::mock_DLWrapper mockDLWrapper;
    IfLibrary_mock::mock_InterfaceLibrary mockInterfaceLibrary;

    using ::testing::Return;
    using ::testing::_;
    //using ::testing::Times;
    EXPECT_CALL(mockDLWrapper, dlopen("resources/modLoadPath1/dummyMod.so")).WillOnce(Return(mockDLWrapper.dummyHandle));
    EXPECT_CALL(mockDLWrapper, dlsym(mockDLWrapper.dummyHandle, "registerInterfaces")).WillOnce(Return((void*)&registerInterfaces));
    EXPECT_CALL(mockInterfaceLibrary, registerInterfaceImpl(_, IfLibrary::InterfaceVersion("1.0.0.0"), &DummyType::create)).Times(1);

    // Create one module path
    IfLibrary::InterfaceModuleLoader modLoader(mockInterfaceLibrary, mockDLWrapper, "resources/modLoadPath1");
    modLoader.loadModules();
}

TEST(InterfaceModuleLoader, LoadTwoModulesFromSinglePath) {
    IfLibrary_mock::mock_DLWrapper mockDLWrapper;
    IfLibrary_mock::mock_InterfaceLibrary mockInterfaceLibrary;

    using ::testing::Return;
    using ::testing::_;
    //using ::testing::Times;
    EXPECT_CALL(mockDLWrapper, dlopen("resources/modLoadPath2/dummyMod1.so")).WillOnce(Return(mockDLWrapper.dummyHandle));
    EXPECT_CALL(mockDLWrapper, dlopen("resources/modLoadPath2/dummyMod2.so")).WillOnce(Return(mockDLWrapper.dummyHandle));
    EXPECT_CALL(mockDLWrapper, dlsym(mockDLWrapper.dummyHandle, "registerInterfaces")).Times(2).WillRepeatedly(Return((void*)&registerInterfaces));
    EXPECT_CALL(mockInterfaceLibrary, registerInterfaceImpl(_, IfLibrary::InterfaceVersion("1.0.0.0"), &DummyType::create)).Times(2);

    // Create one module path
    IfLibrary::InterfaceModuleLoader modLoader(mockInterfaceLibrary, mockDLWrapper, "resources/modLoadPath2");
    modLoader.loadModules();
}

TEST(InterfaceModuleLoader, LoadAllModulesFromTwoPaths) {
    IfLibrary_mock::mock_DLWrapper mockDLWrapper;
    IfLibrary_mock::mock_InterfaceLibrary mockInterfaceLibrary;

    using ::testing::Return;
    using ::testing::_;
    //using ::testing::Times;
    EXPECT_CALL(mockDLWrapper, dlopen("resources/modLoadPath1/dummyMod.so")).WillOnce(Return(mockDLWrapper.dummyHandle));
    EXPECT_CALL(mockDLWrapper, dlopen("resources/modLoadPath2/dummyMod1.so")).WillOnce(Return(mockDLWrapper.dummyHandle));
    EXPECT_CALL(mockDLWrapper, dlopen("resources/modLoadPath2/dummyMod2.so")).WillOnce(Return(mockDLWrapper.dummyHandle));
    EXPECT_CALL(mockDLWrapper, dlsym(mockDLWrapper.dummyHandle, "registerInterfaces")).Times(3).WillRepeatedly(Return((void*)&registerInterfaces));
    EXPECT_CALL(mockInterfaceLibrary, registerInterfaceImpl(_, IfLibrary::InterfaceVersion("1.0.0.0"), &DummyType::create)).Times(3);

    // Create one module path
    std::vector<std::string> modPaths;
    modPaths.push_back("resources/modLoadPath2");
    modPaths.push_back("resources/modLoadPath1");
    IfLibrary::InterfaceModuleLoader modLoader(mockInterfaceLibrary, mockDLWrapper, modPaths);
    modLoader.loadModules();
}

TEST(InterfaceSemanticVersionChecker, MissingBracketLeft1) {
    ASSERT_THROW(IfLibrary::InterfaceSemanticVersionChecker("1.2.3.4,9.0.0.0)"), IfLibrary::InterfaceLibraryException);
}

TEST(InterfaceSemanticVersionChecker, MissingBracketLeft2) {
    ASSERT_THROW(IfLibrary::InterfaceSemanticVersionChecker("*1.2.3.4,9.0.0.0)"), IfLibrary::InterfaceLibraryException);
}

TEST(InterfaceSemanticVersionChecker, MissingBracketRight1) {
    ASSERT_THROW(IfLibrary::InterfaceSemanticVersionChecker("[1.2.3.4,9.0.0.0"), IfLibrary::InterfaceLibraryException);
}

TEST(InterfaceSemanticVersionChecker, MissingBracketRight2) {
    ASSERT_THROW(IfLibrary::InterfaceSemanticVersionChecker("[1.2.3.4,9.0.0.0*"), IfLibrary::InterfaceLibraryException);
}

TEST(InterfaceSemanticVersionChecker, MissingVersionSeparator) {
    ASSERT_THROW(IfLibrary::InterfaceSemanticVersionChecker("[1.2.3.4 9.0.0.0)"), IfLibrary::InterfaceLibraryException);
}

TEST(InterfaceSemanticVersionChecker, Instantiation) {

    ASSERT_NO_THROW(IfLibrary::InterfaceSemanticVersionChecker("[1.2.3.4,9.0.0.0)"));
    ASSERT_NO_THROW(IfLibrary::InterfaceSemanticVersionChecker("[1.2.3.4,9.0.0.0]"));
    ASSERT_NO_THROW(IfLibrary::InterfaceSemanticVersionChecker("(1.2.3.4,9.0.0.0)"));
    ASSERT_NO_THROW(IfLibrary::InterfaceSemanticVersionChecker("(1.2.3.4,9.0.0.0]"));

    ASSERT_NO_THROW(IfLibrary::InterfaceSemanticVersionChecker("[1.2.3,9.0)"));
    ASSERT_NO_THROW(IfLibrary::InterfaceSemanticVersionChecker("[1.2,9.0.0.0]"));
}

TEST(InterfaceSemanticVersionChecker, BothInclusive) {
    IfLibrary::InterfaceSemanticVersionChecker checker("[1.0.0.0,2.0.0.0]");

    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("1.0.0.0")), true);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("1.0")), true);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("2.0.0.0")), true);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("1.3.9.2")), true);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("1.22.765.2")), true);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("2.0.0.1")), false);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("2.1.0.0")), false);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("0.999.543.1")), false);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("0.0.0.1")), false);
}

TEST(InterfaceSemanticVersionChecker, LowInclusiveHighExclusice) {
    IfLibrary::InterfaceSemanticVersionChecker checker("[1.0.0.0,2.0.0.0)");

    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("1.0.0.0")), true);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("1.0")), true);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("2.0.0.0")), false);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("1.3.9.2")), true);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("1.22.765.2")), true);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("2.0.0.1")), false);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("2.1.0.0")), false);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("0.999.543.1")), false);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("0.0.0.1")), false);
}

TEST(InterfaceSemanticVersionChecker, LowExclusiveHighInclusice) {
    IfLibrary::InterfaceSemanticVersionChecker checker("(1.0.0.0,2.0.0.0]");

    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("1.0.0.0")), false);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("1.0.0.1")), true);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("2.0.0.0")), true);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("1.3.9.2")), true);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("1.22.765.2")), true);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("2.0.0.1")), false);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("2.1.0.0")), false);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("0.999.543.1")), false);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("0.0.0.1")), false);
}

TEST(InterfaceSemanticVersionChecker, BothExclusive) {
    IfLibrary::InterfaceSemanticVersionChecker checker("(1.0.0.0,2.0.0.0)");

    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("1.0.0.0")), false);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("1.0.0.1")), true);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("2.0.0.0")), false);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("1.3.9.2")), true);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("1.22.765.2")), true);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("2.0.0.1")), false);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("2.1.0.0")), false);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("0.999.543.1")), false);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("0.0.0.1")), false);
}

TEST(InterfaceSemanticVersionChecker, BothInclusiveSmallLowVersion) {
    IfLibrary::InterfaceSemanticVersionChecker checker("[1.0,2.0.0.0]");

    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("1.0.0.0")), true);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("1.0")), true);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("2.0.0.0")), true);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("1.3.9.2")), true);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("1.22.765.2")), true);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("2.0.0.1")), false);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("2.1.0.0")), false);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("0.999.543.1")), false);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("0.0.0.1")), false);
}

TEST(InterfaceSemanticVersionChecker, LowInclusiveHighExclusiceSmallLowVersion) {
    IfLibrary::InterfaceSemanticVersionChecker checker("[1.0,2.0.0.0)");

    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("1.0.0.0")), true);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("1.0")), true);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("2.0.0.0")), false);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("1.3.9.2")), true);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("1.22.765.2")), true);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("2.0.0.1")), false);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("2.1.0.0")), false);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("0.999.543.1")), false);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("0.0.0.1")), false);
}

TEST(InterfaceSemanticVersionChecker, LowExclusiveHighInclusiceSmallLowVersion) {
    IfLibrary::InterfaceSemanticVersionChecker checker("(1.0,2.0.0.0]");

    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("1.0.0.0")), false);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("1.0.0.1")), true);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("2.0.0.0")), true);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("1.3.9.2")), true);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("1.22.765.2")), true);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("2.0.0.1")), false);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("2.1.0.0")), false);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("0.999.543.1")), false);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("0.0.0.1")), false);
}

TEST(InterfaceSemanticVersionChecker, BothExclusiveSmallLowVersion) {
    IfLibrary::InterfaceSemanticVersionChecker checker("(1.0,2.0.0.0)");

    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("1.0.0.0")), false);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("1.0.0.1")), true);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("2.0.0.0")), false);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("1.3.9.2")), true);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("1.22.765.2")), true);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("2.0.0.1")), false);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("2.1.0.0")), false);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("0.999.543.1")), false);
    ASSERT_EQ(checker.satisfies(IfLibrary::InterfaceVersion("0.0.0.1")), false);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

