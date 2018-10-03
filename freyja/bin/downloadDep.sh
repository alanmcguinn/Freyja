#!/bin/bash
source ${BUILDSYSTEM_ROOT}/bin/common

CURL="`which curl` -sS"

ARTIFACT=
ARTIFACT_VERSION=
VERBOSE=0
CLEAN_LOCAL=0

function log() {
	if [ ${VERBOSE} -eq 1 ]; then
		info "$1"
	fi 
}

function checkArgs() {
	if [ -z $ARTIFACT ]; then
		warn "You must specify an artifact using the -f flag"
		exit -1
	fi
	if [ -z $ARTIFACT_VERSION ]; then
		warn "You must specify an artifact version using the -v flag"
		exit -1
	fi
}

ARTIFACTS_TO_DOWNLOAD=(${ARTIFACT} ${ARTIFACT}_debug)

while getopts "f:v:cVn" opt; do
  case $opt in
	c)
		CLEAN_LOCAL=1
		;;
	n)
		ARTIFACTS_TO_DOWNLOAD=${ARTIFACT}
		;;
    f)
    	ARTIFACT=${OPTARG}
    	;;
    v)
    	ARTIFACT_VERSION=${OPTARG}
    	;;
    V)
    	VERBOSE=1
    	;;
  esac
done

checkArgs

for item in ${ARTIFACT} ${ARTIFACT}_debug; do
	ARTIFACT_FILENAME=${item}-${ARTIFACT_VERSION}.tar
	LOCAL_ARTIFACT_FILENAME=${ARTIFACTORY_LOCAL_CACHE}/${item}/${ARTIFACT_VERSION}/${ARTIFACT_FILENAME}

	if [ ${CLEAN_LOCAL} -eq 1 ]; then
		warn "Cleaning ${LOCAL_ARTIFACT_FILENAME}"
		rm -f ${LOCAL_ARTIFACT_FILENAME}
	fi

	DOWNLOAD_INFO_URI=${ARTIFACTORY_URL}/api/storage/fw-core/${ARTIFACT}/${ARTIFACT_VERSION}/${ARTIFACT_FILENAME}
	
	ARTIFACT_METADATA=`${CURL} -X GET "${DOWNLOAD_INFO_URI}"`
	
	ART_LASTUPDATED_TIME=`echo ${ARTIFACT_METADATA}  | python -c 'import json,sys;obj=json.load(sys.stdin);print obj["lastModified"]'` 
	ART_LASTUPDATED_EPOCH=`date --date ${ART_LASTUPDATED_TIME} +%s`
	
	log "Artifact ${item}-${ARTIFACT_VERSION} last updated on ${ART_LASTUPDATED_TIME}"
	
	LOCAL_LASTUPDATED_EPOCH=0
	if [ -f ${LOCAL_ARTIFACT_FILENAME} ]; then
		LOCAL_LASTUPDATED_EPOCH=`stat --printf=%Y ${LOCAL_ARTIFACT_FILENAME}`
		log "Local artifact last updated on `stat --printf=%y ${LOCAL_ARTIFACT_FILENAME}`"
	else
		log "No local artifact - forcing download"
	fi
	
	localArtifactPath=${ARTIFACTORY_LOCAL_CACHE}/${item}/${ARTIFACT_VERSION}
	mkdir -p ${localArtifactPath}
	cd ${localArtifactPath}
	if [ ${LOCAL_LASTUPDATED_EPOCH} -ge ${ART_LASTUPDATED_EPOCH} ]; then
		ok "Artifact ${LOCAL_ARTIFACT_FILENAME} up to date"
	else
		DOWNLOAD_URI=`echo ${ARTIFACT_METADATA}  | python -c 'import json,sys;obj=json.load(sys.stdin);print obj["downloadUri"]'`
		artifactPath=${item}/${ARTIFACT_VERSION}
		localArtifactPath=${ARTIFACTORY_LOCAL_CACHE}/${item}/${ARTIFACT_VERSION}
		remoteRepoPath=${ARTIFACTORY_REPO}/${artifactPath}/${ARTIFACT_FILENAME}
		cd ${localArtifactPath}
		
		ok "Downloading ${DOWNLOAD_URI}"
		rm -f ${LOCAL_ARTIFACT_FILENAME}
		wget -q ${DOWNLOAD_URI}
		if [ $? -ne 0 ]; then
			error "Download failure from URI ${DOWNLOAD_URI}"
		fi
	fi
	tar xf ${LOCAL_ARTIFACT_FILENAME}
	cd -
done
