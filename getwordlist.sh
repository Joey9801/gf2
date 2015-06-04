# example sh script to generate catalog file
# it searches for strings marked with _() in all .cpp files in directory ./src
# the generated file is called 'logicsim.pot' and is generated in ./po
CPP_FILE_LIST=`find ./src -name '*.cc' -print`
xgettext -d app_name -s --keyword=_ -p ./i18n -o logicsim.pot $CPP_FILE_LIST
