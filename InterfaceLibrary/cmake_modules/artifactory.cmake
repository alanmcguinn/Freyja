include(cmake_modules/utils.cmake)

macro(declare_target)
    set(options EXECUTABLE SHARED_LIBRARY PLUGIN)
    set(namedParameters TARGET INSTALL_DEST SOVERSION)
    set(multiNamedParameters SOURCES)

    cmake_parse_arguments(declare_target "${options}" "${namedParameters}" "${multiNamedParameters}" ${ARGN})

    if (!${declare_target_TARGET})
        message(fatal_error " declare_target requires a TARGET parameter")
    endif (!${declare_target_TARGET})

    verifyOneOf(oneSet ${declare_target_EXECUTABLE} ${declare_target_SHARED_LIBRARY} ${declare_target_PLUGIN})
    if (NOT ${oneSet})
        message(FATAL_ERROR "Only one of EXECUTABLE|SHARED_LIBRARY|PLUGIN can be used")
    endif (NOT ${oneSet})

    if (${declare_target_EXECUTABLE})
        if ((${declare_target_VERSION}) OR (${declare_target_SOVERSION}))
            message(warning "VERSION and SOVERSION are not applicable for EXECUTABLE targets")
        endif ((${declare_target_VERSION}) OR (${declare_target_SOVERSION}))
    else (${declare_target_EXECUTABLE})
        if(x${CMAKE_BUILD_TYPE} STREQUAL "xDebug")
            add_library(${_target}_debug SHARED ${srcFiles})
            set(_fqTarget ${_target}_debug)
        else(x${CMAKE_BUILD_TYPE} STREQUAL "xDebug")
            add_library(${_target} SHARED ${srcFiles})
            set(_fqTarget ${_target})
        endif(x${CMAKE_BUILD_TYPE} STREQUAL "xDebug")
        if (NOT ${declare_target_SOVERSION} STREQUAL "")
            set(_version ${declare_target_SOVERSION}.${BUILD_NUM}.${PATCH_NUM})
            set_property(TARGET ${_fqTarget} PROPERTY SOVERSION ${declare_target_SOVERSION})
            set_property(TARGET ${_fqTarget} PROPERTY VERSION ${_version})
        endif (NOT ${declare_target_SOVERSION} STREQUAL "")
        if (${declare_target_PLUGIN})
            set_property(TARGET ${declare_target_TARGET} PROPERTY PREFIX "")
        endif (${declare_target_PLUGIN})
        target_include_directories(${_fqTarget} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
        set(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/lib)
    endif (${declare_target_EXECUTABLE})
    set_property(TARGET ${_fqTarget} PROPERTY CXX_STANDARD 11)
    #add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0)
endmacro(declare_target)

macro(add_publish_step _target repoPath)
    get_target_property(versionStr ${_target} SOVERSION)

    set(remoteRepoPath $ENV{ARTIFACTORY_REPO}/${repoPath}/${versionStr}/)
    set(artifactArchive ${_target}-${versionStr}.tar)

    add_custom_target(publish COMMAND ${CMAKE_SOURCE_DIR}/bin/publishArtifact -f ${artifactArchive} -a $ENV{ARTIFACTORY_URL}/${remoteRepoPath} -u$ENV{ARTIFACTORY_USER}:$ENV{ARTIFACTORY_PASS})
    #add_custom_target(publish COMMAND art upload ${artifactArchive} ${remoteRepoPath} --user $ENV{ARTIFACTORY_USER} --password $ENV{ARTIFACTORY_PASS} --url $ENV{ARTIFACTORY_URL})
endmacro(add_publish_step _target)


macro(deploy_to_local_repo _target repoName)
    get_target_property(versionStr ${_target} SOVERSION)
    set(artifactArchive ${_target}-${versionStr}.tar)

    if(x${CMAKE_BUILD_TYPE} STREQUAL "xDebug")
        set(localRepoPath $ENV{ARTIFACTORY_LOCAL_CACHE}/${repoName}_debug/${versionStr})
    else(x${CMAKE_BUILD_TYPE} STREQUAL "xDebug")
        set(localRepoPath $ENV{ARTIFACTORY_LOCAL_CACHE}/${repoName}/${versionStr})
    endif(x${CMAKE_BUILD_TYPE} STREQUAL "xDebug")
    file(MAKE_DIRECTORY ${localRepoPath})

    install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/includes)
    foreach (incDir ${ARGN})
        install(DIRECTORY ${incDir} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/includes)
    endforeach(incDir ${ARGN})

    # Tar up the artifacts
    add_custom_target(makeArchive COMMAND ${CMAKE_COMMAND} -E tar "cf" "${artifactArchive}" includes ${LIBRARY_OUTPUT_PATH})
    # Deploy them to the local repo
    add_custom_target(deployArchive COMMAND cp ${artifactArchive} ${localRepoPath})
    add_dependencies(deployArchive makeArchive)
endmacro(deploy_to_local_repo _target)


macro(include_remote_dep _target _dep _version)
    set(artifactArchive ${_dep}-${_version}.tar)
    set(artifactDebugArchive ${_dep}_debug-${_version}.tar)
    set(artifactPath ${_dep}/${_version})
    set(artifactDebugPath ${_dep}_debug/${_version})

    add_custom_target(downloadRemoteDep${_target}
            COMMAND ${CMAKE_SOURCE_DIR}/bin/downloadDep.sh -f ${_dep} -v ${_version}
            WORKING_DIRECTORY $ENV{ARTIFACTORY_LOCAL_CACHE})
    add_custom_target(unpackRemoteDep${_target}
            COMMAND ${CMAKE_COMMAND} -E tar "xf" "${artifactArchive}"
            WORKING_DIRECTORY $ENV{ARTIFACTORY_LOCAL_CACHE}/${artifactPath})

    add_custom_target(unpackDebugRemoteDep${_target}
            COMMAND ${CMAKE_COMMAND} -E tar "xf" "${artifactDebugArchive}"
            WORKING_DIRECTORY $ENV{ARTIFACTORY_LOCAL_CACHE}/${artifactDebugPath})

    add_dependencies(unpackRemoteDep${_target} downloadRemoteDep${_target})
    add_dependencies(unpackDebugRemoteDep${_target} downloadRemoteDep${_target})
    add_dependencies(${_target} unpackRemoteDep${_target} unpackDebugRemoteDep${_target})
    include_directories($ENV{ARTIFACTORY_LOCAL_CACHE}/${artifactPath}/includes/include)
    if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        target_link_libraries(${_target} $ENV{ARTIFACTORY_LOCAL_CACHE}/${artifactDebugPath}/lib/lib${_dep}_debug.so)
    else(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
        target_link_libraries(${_target} $ENV{ARTIFACTORY_LOCAL_CACHE}/${artifactPath}/lib/lib${_dep}.so)
    endif(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
endmacro(include_remote_dep _target _dep _version)
