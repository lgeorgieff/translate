#! /bin/sh

#######################################################################################################################
# Copyright (C) 2015  Lukas Georgieff
# Last modified: 01/27/2015
# Description: Extracts all zip files that represents language resources from http://www.dict.cc/ and have a name
#              corresponding to a certain expression. Finally renames all extracted files with a name corresponding to
#              a certain expression, renames them to the language identifer these files cnotain as a comment in the
#              very first line and moves them to the specified destination. Note: the language resources (zip files)
#              must be downloaded either manually or automatically from http://www.dict.cc/ and the license agreement
#              must be set before this script can be used.
#######################################################################################################################


#######################################################################################################################
# This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public
# License as published by the Free Software Foundation in version 2.
#
# This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
# warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along with this program; if not, write to the Free
# Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
#######################################################################################################################


### Definition of all used variables
SCRIPT_NAME=$(basename ${0})
ZIP_FILE_PATTERN="*.zip"
ZIP_FILE_DIRECTORY="."
FILE_PATTERN="*.txt"
DELETE_OLD_ZIPS="false"
DESTINATION_DIRECTORY="."
DESTINATION_DIRECTORY_SET="false"
DO_CHECK="true"
SCP_DESTINATION=""
SCP_SET="false"
TMP_PATH=$(mktemp -d -t translate_XXXXXXXX)


ALL_RESOURCES=(
    "BG-DE.txt" "BG-EN.txt"
    "BS-DE.txt" "BS-EN.txt"
    "CS-DE.txt" "CS-EN.txt"
    "DA-DE.txt" "DA-EN.txt"
    "DE-BG.txt" "DE-BS.txt" "DE-CS.txt" "DE-DA.txt" "DE-EL.txt" "DE-EN.txt" "DE-EO.txt" "DE-ES.txt" "DE-FI.txt"
    "DE-FR.txt" "DE-HR.txt" "DE-HU.txt" "DE-IS.txt" "DE-IT.txt" "DE-LA.txt" "DE-NL.txt" "DE-NO.txt" "DE-PL.txt"
    "DE-PT.txt" "DE-RO.txt" "DE-RU.txt" "DE-SK.txt" "DE-SQ.txt" "DE-SR.txt" "DE-SV.txt" "DE-TR.txt"
    "EL-DE.txt" "EL-EN.txt"
    "EN-BG.txt" "EN-BS.txt" "EN-CS.txt" "EN-DA.txt" "EN-DE.txt" "EN-EL.txt" "EN-EO.txt" "EN-ES.txt" "EN-FI.txt"
    "EN-FR.txt" "EN-HR.txt" "EN-HU.txt" "EN-IS.txt" "EN-IT.txt" "EN-LA.txt" "EN-NL.txt" "EN-NO.txt" "EN-PL.txt"
    "EN-PT.txt" "EN-RO.txt" "EN-RU.txt" "EN-SK.txt" "EN-SQ.txt" "EN-SR.txt" "EN-SV.txt" "EN-TR.txt"
    "EO-DE.txt" "EO-EN.txt"
    "ES-DE.txt" "ES-EN.txt"
    "FI-DE.txt" "FI-EN.txt"
    "FR-DE.txt" "FR-EN.txt"
    "HR-DE.txt" "HR-EN.txt"
    "HU-DE.txt" "HU-EN.txt"
    "IS-DE.txt" "IS-EN.txt"
    "IT-DE.txt" "IT-EN.txt"
    "LA-DE.txt" "LA-EN.txt"
    "NL-DE.txt" "NL-EN.txt"
    "NO-DE.txt" "NO-EN.txt"
    "PL-DE.txt" "PL-EN.txt"
    "PT-DE.txt" "PT-EN.txt"
    "RO-DE.txt" "RO-EN.txt"
    "RU-DE.txt" "RU-EN.txt"
    "SK-DE.txt" "SK-EN.txt"
    "SQ-DE.txt" "SQ-EN.txt"
    "SR-DE.txt" "SR-EN.txt"
    "SV-DE.txt" "SV-EN.txt"
    "TR-DE.txt" "TR-EN.txt")


### Delete all temporary files and folders.
function clean_up {
    if [ "" != $TMP_PATH ]
    then
	rm -rf $TMP_PATH
    fi
}


### Catch ctr-c signals and run the clean_up function. Finally, the script
### exists with exit code 2.
trap ctrl_c INT

function ctrl_c() {
    clean_up
    exit 2
}



### Print the specified error message ($1) on stderr and exits with the
### specified error code ($2).
function error_exit {
    echo "${1}" 1>&2
    clean_up
    exit ${2}
}


# Check if the temporary folder could be created.
# If not exit with status 1.
if [ "" == $TMP_PATH ]
then
    error_exit "Could not create a temporary folder" 1
fi


### Print the usage of this script.
function print_usage {
    echo "Usage: ${0} [OPTION...]"
    echo ""
    echo "-h | --help                   Shows this help"
    echo "-z | --zip-pattern <pattern>  The pattern of the zip files that"
    echo "                              contain the language resources"
    echo "-Z | --zip-directory <path>   The parent directory of the zip files"
    echo "                              that contain the language resources"
    echo "-D | --delete-zip             If set, all unziped zip files will be"
    echo "                              deleted"
    echo "-d | --destination <path>     The folder for the resulting language"
    echo "                              resource files. If this option is set"
    echo "                              -s|--scp <remote> must not be set"
    echo "-s | --scp <remote>           Copies the resulting files to the set"
    echo "                              location. If this option is set -d|"
    echo "                              --destination <path> must not be set"
    echo "-n | --no-check               If set no checks are performed if all"
    echo "                              available language resources are"
    echo "                              available"
    echo ""
}


### Process all command line arguments and set the corresponding variables.
function process_arguments {
    while [[ $# > 0 ]]
    do
	key="$1"
	case $key in
	    "--zip-pattern"|"-z")
		shift
		ZIP_FILE_PATTERN="$1"
		;;
	    "--zip-directory"|"-Z")
		shift
		ZIP_FILE_DIRECTORY="$1"
		;;
	    "--delete-zip"|"-D")
		DELETE_OLD_ZIPS="true"
		;;
	    "--no-check"|"-n")
		DO_CHECK="false"
		;;
	    "--destination"|"-d")
		shift
		if [ "true" == "${SCP_SET}" ]
		then
		    error_exit "The argument -d|--destination must not be used together with the argument -s|--scp" 1
		fi
		DESTINATION_DIRECTORY="$1"
		DESTINATION_DIRECTORY_SET="true"
		;;
	    "--scp"|"-s")
		shift
		if [ "true" == "${DESTINATION_DIRECTORY_SET}" ]
		then
		    error_exit "The argument -s|--scp must not be used together with the argument -d|--destination" 1
		fi
		SCP_DESTINATION="$1"
		SCP_SET="true"
		;;
	    "--help"|"-h")
		print_usage
		exit 0
		;;
	    *)
		print_usage
		error_exit "Unknown argument ${key}" 1
		;;
	esac
	shift
    done
}


### Return a list of all files included in the passed directory.
### The files are returned as full path and via echo, i.e. stdout.
function get_files {
    directory="${1}"
    file_expression="$2"
    for file in $(find "${directory}" -name "${file_expression}")
    do
	if [ -f "${file}" ]
	then
	    echo $(realpath "${file}")
	fi
    done
}


### Unzip all files that mathces the specified zip file directory and zip file
### pattern.
function unzip_files {
    for file in $(get_files "$ZIP_FILE_DIRECTORY" "$ZIP_FILE_PATTERN")
    do
	if [ $(basename "${file}") != "${SCRIPT_NAME}" ]
	then
	    if ! unzip -d $TMP_PATH "${file}"
	    then
		error_exit "Could not unzip file ${file}" 1
	    elif [ "true" == $DELETE_OLD_ZIPS ]
	    then
		 rm "${file}"
	    fi
	fi
    done
}


### Check if all available files are present. If not an error message is
### printed and the programm is stopped with return code 1.
function do_check {
    for resource in "${ALL_RESOURCES[@]}"
    do
	if [ ! -f "$TMP_PATH/${resource}" ]
	then
	    error_exit "The language resource ${resource} is missing!" 1
	fi
    done
}


### Rename all files unzipped files that match the specified file pattern to
### their langauge identifier.
function process_unziped_files {
    # rename files and throw away all comments and empty lines
    for file in $(get_files "${TMP_PATH}" "*")
    do
	target="$(head -1 "${file}" | egrep -o "# [a-Z]+-[a-Z]+" | sed -r "s/ *#[# ]*//").txt"
	egrep -IU -v "(^ *#)|(^?$)" "${file}" > "${TMP_PATH}/${target}"
	rm "${file}"
	echo "Created: ${target}"
    done

    # check if all language resources are available
    if [ "true" == ${DO_CHECK} ]
    then
	do_check
    fi

    # create the destination folder and move all processed files into it
    if [ "true" != "${SCP_SET}" ]
    then
	if [ ! -d "${DESTINATION_DIRECTORY}" ]
	then
	    mkdir -p "${DESTINATION_DIRECTORY}"
	fi
	mv "${TMP_PATH}"/* "${DESTINATION_DIRECTORY}"
    fi

    # copy all language resources to their scp target
    if [ "true" == "${SCP_SET}" ]
    then
	scp "${TMP_PATH}"/* "${SCP_DESTINATION}"
    fi
}


### The actual calls
process_arguments "$@"
unzip_files
process_unziped_files
clean_up
