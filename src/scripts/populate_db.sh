#! /bin/sh

#######################################################################################################################
# Copyright (C) 2015  Lukas Georgieff
# Last modified: 03/15/2015
# Description: Calls the programme dict2sql for all language resources to dump them into SQL files. Finally, these
#              files are written into the specified data base.
#              Be carefull when runnig this script, since your data base will be set to an initial (= empty) state.
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
LANGUAGE_RESOURCE_PATTERN="[A-Z][A-Z]-[A-Z][A-Z].txt"
LANGUAGE_RESOURCE_DIRECTORY="."
TMP_PATH=$(mktemp -d -t translate_XXXXXXXX)
PGSQL_OPTIONS="--dbname translate --username translate"
STRICT_MODE="false"
DUMPER_SCRIPT=$(realpath ./dict2sql)
SQL_INIT_SCRIPT=$(realpath ./create_schema.sql)
SQL_CLEANUP_SCRIPT=$(realpath ./drop_schema.sql)

### Delete all temporary files and folders.
function clean_up {
    if [ "" != $TMP_PATH ]
    then
	#rm -rf $TMP_PATH
	echo ""
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
    echo "This script populates your data base with values from dict.cc resources."
    echo "WARNING: Be carefull when runnig this script, since your data base will be"
    echo "set to an initial state, i.e. empty state."
    echo ""
    echo "Usage: ${0} [OPTION...]"
    echo ""
    echo "-h | --help                        Shows this help"
    echo "-l | --resource-pattern <pattern>  The file name pattern of the language"
    echo "                                   resources in the specified resource"
    echo "                                   directory that will be processed by this"
    echo "                                   script. The deault value is"
    echo "                                   \"${LANGUAGE_RESOURCE_PATTERN}\""
    echo "-d | --resource-directrory <dir>   The directory that is searched for the"
    echo "                                   language resource files. The default"
    echo "                                   value is \"${LANGUAGE_RESOURCE_DIRECTORY}\""
    echo "-p | --pgsql-options <options>     Options for the data base in which the"
    echo "                                   language resources are written to. The"
    echo "                                   default value is"
    echo "                                   --dbname translate --username translate"
    echo "-s | --strict-mode                 If set, each error in the language"
    echo "                                   resource files aborts this script from"
    echo "                                   further processing"
    echo ""
}


### Process all command line arguments and set the corresponding variables.
function process_arguments {
    while [[ $# > 0 ]]
    do
	key="$1"
	case "${key}" in
	    "--resource-directory"|"-d")
		shift
		LANGUAGE_RESOURCE_DIRECTORY="$1"
		;;
	    "--resource-pattern"|"-d")
		shift
		LANGUAGE_RESOURCE_PATTERN="$1"
		;;
	    "--pgsql-options"|"-p")
		shift
		PGSQL_OPTIONS="$1"
		;;
	    "--strict-mode"|"-s")
		STRICT_MODE="true"
		;;
	    "--help"|"-h")
		print_usage
		exit 0
		;;
	    *)
		echo "Unknown argument ${key}"
		echo "For more information run \"${SCRIPT_NAME} --help\""
		error_exit "" 1
		;;
	esac
	shift
    done
}

### Check if all dependencies of this script are fulfilled.
### If not, an error message is printed and an "exit 1" is performed.
function check_dependencies {
    if [ ! -f "${DUMPER_SCRIPT}" ]
    then
	echo "The executable file ${DUMPER_SCRIPT} does not exist!" 1>&2
	echo "To continue run cmake and make to build this project." 1>&2
	echo "Finally, run this script again." 1>&2
	clean_up
	exit 1
    fi
    if [ ! -x "${DUMPER_SCRIPT}" ]
    then
	echo "The file ${DUMPER_SCRIPT} is not set as executable!" 1>&2
	echo "To continue run \"chmod +x ${DUMPER_SCRIPT}\"." 1>&2
	echo "Finally, run this script again." 1>&2
	clean_up
	exit 1       
    fi
    if [ ! -f "${SQL_INIT_SCRIPT}" ]
    then
	echo "The file ${SQL_INIT_SCRIPT} does not exist!" 1>&2
	echo "To continue checkout this file from the git repository\"." 1>&2
	echo "Finally, run this script again." 1>&2
	clean_up
	exit 1       
    fi
    if [ ! -f "${SQL_CLEANUP_SCRIPT}" ]
    then
	echo "The file ${SQL_CLEANUP_SCRIPT} does not exist!" 1>&2
	echo "To continue checkout this file from the git repository\"." 1>&2
	echo "Finally, run this script again." 1>&2
	clean_up
	exit 1       
    fi
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

### Dump the files in the spcified directory into SQL files in a temporary directory.
function dump_files_to_sql {
    strict_mode=""
    if [ "true" == $STRICT_MODE ]
    then
	strict_mode="--strict-mode"
    fi
    for file in $(get_files "${LANGUAGE_RESOURCE_DIRECTORY}" "${LANGUAGE_RESOURCE_PATTERN}")
    do
	in_lang_id=$(echo $(basename "${file}") | grep -o "^..")
	out_lang_id=$(echo $(basename "${file}") | sed "s/\(^..-\)\(..\)\(.\+\)/\\2/")
	sql_file="${TMP_PATH}/$(basename ${file}).sql"
	echo "Dumping $(basename ${file}) into $(basename ${sql_file})"
	if [ ! $(${DUMPER_SCRIPT} ${strict_mode} --in ${in_lang_id} --out ${out_lang_id} < ${file} > ${sql_file}) ] && [ "true" == ${STRICT_MODE} ]
	then
	    error_exit "Failed to process \"${file}\" in strict mode!" 1
	fi
    done
}

### Write the created SQL files into the specified data base.
function write_to_db {
    echo "Cleaning data base ..."
    psql $PGSQL_OPTIONS --file $SQL_CLEANUP_SCRIPT &> /dev/null
    echo "Initializing data base ..."
    error_message="$(psql $PGSQL_OPTIONS --file $SQL_INIT_SCRIPT -q 2>&1)"
    if [ "0" != "$(echo $?)" ] || [ "" != "${error_message}" ]
    then
	error_exit "Could not initialize data base" 1
    fi
    for file in $(get_files "${TMP_PATH}" "*.sql")
    do
	echo "Populating data base $(basename ${file})..."
	error_message="$(psql $PGSQL_OPTIONS --file ${file} -q 2>&1)"
	if [ "0" != "$(echo $?)" ] || [ "" != "${error_message}" ]
	then
	    echo "${error_message}"
	    error_exit "Could not populate data base with ${file}" 1
	fi
    done
}

### The actual calls
process_arguments "$@"
check_dependencies
dump_files_to_sql
write_to_db
clean_up
