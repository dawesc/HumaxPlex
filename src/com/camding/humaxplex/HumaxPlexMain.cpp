/*
 * HumaxPlexMain.cpp
 *
 *  Created on: 1 Apr 2017
 *      Author: chrisd
 */

#include "Boolean.h"
#include "DBAccess.h"
#include "HumaxPlexMain.h"
#include "Scanner.h"

#include <boost/filesystem.hpp>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/tokenizer.hpp>
#include <boost/token_functions.hpp>

#include <iostream>
#include <fstream>

// Include std::exception so we can handling any argument errors
// emitted by the command line parser
#include <exception>

namespace po = boost::program_options;
using boost::program_options::options_description;
using boost::program_options::value;
using namespace std;

using com::camding::humaxplex::Boolean;
using com::camding::humaxplex::DBAccess;
using com::camding::humaxplex::HumaxPlexMain;
using com::camding::humaxplex::Scanner;

HumaxPlexMain::HumaxPlexMain() {
  /* Not used */
}

HumaxPlexMain::~HumaxPlexMain() {
  /* Not used */
}

boost::filesystem::path GetExePath(char* argv[]) {
  boost::filesystem::path full_path( boost::filesystem::initial_path<boost::filesystem::path>() );

  full_path = boost::filesystem::system_complete( boost::filesystem::path(argv[0]));

  return full_path.parent_path();
}

int HumaxPlexMain::ProcessCommandLineArgs(int argc, char* argv[]) {
  options_description desc(
          "\nThe Processes humax shares and transcodes for plex.\n\nAllowed arguments");

  try {
    // Add descriptive text for display when help argument is
    // supplied
    auto dbDefaultPath = GetExePath(argv);
    dbDefaultPath /= ".humaxplex.db";
    std::string defaultDb = dbDefaultPath.native();
    auto defaultTempFolderPath = boost::filesystem::temp_directory_path();
    std::string defaultTempFolder = defaultTempFolderPath.native();

    desc.add_options()
        ("help,h", "Produce this help message.")
        ("verbose,v", "Output more debug")
        ("version,V", "Show version information")
        ("db-file,d", value<string>(&(options_.dbFile))->default_value(defaultDb), "The name of the database file to use.")
        ("source-host-ip,i", value<string>(&(options_.sourceHostIp))->default_value(defaultDb), "The ip of the host to pull from.")
        ("temp,t", value<string>(&(options_.tempFolder))->default_value(defaultTempFolder), "The temporary folder to store files in.")
        ("dest,o", value<string>(&(options_.destFolder))->default_value("/mnt/Humax"), "The destination folder to store files in.");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
    options_.help=vm.count("help");
    options_.verbose=vm.count("verbose");
    options_.version=vm.count("version");

    if (options_.help) {
      cout << desc << std::endl;
      return EXIT_FAILURE;
    }
  } catch (const std::exception& err) {
    std::cerr << err.what() << std::endl;
    std::cout << desc << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

int HumaxPlexMain::Run(int argc, char* argv[]) {
  ProcessCommandLineArgs(argc, argv);
  DBAccess database(options_);
  return 0;
}
