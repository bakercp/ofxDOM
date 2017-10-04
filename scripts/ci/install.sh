#!/usr/bin/env bash
set -e


echo "------------------------- not in #TARGET "

TRAVIS_REPO_OWNER=${TRAVIS_REPO_SLUG%/*}
TRAVIS_REPO_NAME=${TRAVIS_REPO_SLUG#*/}

echo "Owner ${TRAVIS_REPO_OWNER}"
echo "Repo ${TRAVIS_REPO_NAME}"


#
# OF_ROOT=~/openFrameworks
# OF_ADDON_NAME=${TRAVIS_REPO_SLUG#*/}
# OF_ADDON_FOLDER=${TRAVIS_BUILD_DIR}
#
#
# # Default addon github info.
# GH_USERNAME='bakercp'
# GH_BRANCH='master'
# GH_DEPTH=1
#
# # An array of required addons that will be gathered below.
# REQUIRED_ADDONS=()
#
# echo "The addon folder is ${OF_ADDON_FOLDER}"
#
#
# echo "The target is ${TARGET}"
# echo ${OF_ROOT}/addons/${OF_ADDON_NAME}/
# ls -lah ${OF_ROOT}/addons/
# ls -lah ${OF_ROOT}/addons/${OF_ADDON_NAME}/
#
# # Extract ADDON_DEPENDENCIES from addon_config.mk file.
# if [ -f ${OF_ADDON_FOLDER}/addon_config.mk ]; then
#   while read line; do
#     if [[ $line == ADDON_DEPENDENCIES* ]] ;
#     then
#       line=${line#*=}
#       IFS=' ' read -ra ADDR <<< "$line"
#       for i in "${ADDR[@]}"; do
#           REQUIRED_ADDONS+=($i)
#       done
#     fi
#   done < ${OF_ADDON_FOLDER}/addon_config.mk
# fi
#
# # Gather addons from all examples.
# for addons_make in ${OF_ADDON_FOLDER}/example*/addons.make; do
#   while read addon; do
#     if [ ${addon} != ${OF_ADDON_NAME} ] ;
#     then
#       REQUIRED_ADDONS+=($addon)
#     fi
#   done < $addons_make
# done
#
# # We aren't de-duplicating array to keep it pure bash.
# for addon in "${REQUIRED_ADDONS[@]}"
# do
#   if [ ! -d ${OF_ROOT}/addons/${addon} ]; then
#     git clone --depth=$GH_DEPTH https://github.com/$GH_USERNAME/$addon.git ${OF_ROOT}/addons/${addon}
#   fi
# done
