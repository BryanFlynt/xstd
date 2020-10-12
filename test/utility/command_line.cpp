/*
 * test_command_line.cpp
 *
 *  Created on: Apr 27, 2019
 *      Author: bflynt
 */

// Let Catch provide main():
#define CATCH_CONFIG_MAIN
#include "../../test/catch.hpp"

#include "xstd/detail/utility/command_line.hpp"

#include <cassert>
#include <cstring>
#include <string>
#include <iostream>

// POSIX Command ONLY
#include <wordexp.h>

//
// Function to parse string like the command line arguments
// Copied from some bad online code
//
char **split_commandline(const char *cmdline, int *argc) {
	int i;
	char **argv = nullptr;
	assert(argc);

	if (!cmdline) {
		return nullptr;
	}

	wordexp_t p;

	// Note! This expands shell variables.
	if (wordexp(cmdline, &p, 0)) {
		return nullptr;
	}

	*argc = p.we_wordc;

	if (!(argv = (char**)calloc(*argc, sizeof(char *)))) {
		goto fail;
	}

	for (i = 0; i < p.we_wordc; i++){
		if (!(argv[i] = strdup(p.we_wordv[i]))) {
			goto fail;
		}
	}

	wordfree(&p);

	return argv;
	fail:
	wordfree(&p);

	if (argv) {
		for (i = 0; i < *argc; i++) {
			if (argv[i]) {
				free(argv[i]);
			}
		}
		free(argv);
	}

	return nullptr;
}



TEST_CASE("Command Line", "[default]") {

	std::string cmd = "program -i input.txt -o output.txt -abc --def -hj arg1 --klm arg2";

	int narg = 0;
	char** args = split_commandline(cmd.c_str(), &narg);
	xstd::CommandLine cmd_line(narg,args);

	SECTION("Check Parsed Input"){
		REQUIRE( cmd_line.exists("i") );
		REQUIRE( cmd_line.exists("o") );
		REQUIRE( cmd_line.exists("a") );
		REQUIRE( cmd_line.exists("b") );
		REQUIRE( cmd_line.exists("c") );
		REQUIRE( cmd_line.exists("def") );
		REQUIRE( cmd_line.exists("h") );
		REQUIRE( cmd_line.exists("j") );
		REQUIRE( cmd_line.exists("klm") );
		REQUIRE( cmd_line.get("i")   == "input.txt" );
		REQUIRE( cmd_line.get("o")   == "output.txt" );
		REQUIRE( cmd_line.get("h")   == "arg1" );
		REQUIRE( cmd_line.get("j")   == "arg1" );
		REQUIRE( cmd_line.get("klm") == "arg2" );

		REQUIRE( not cmd_line.exists("abc") );
		REQUIRE( not cmd_line.exists("hj") );
		REQUIRE( not cmd_line.exists("d") );
		REQUIRE( not cmd_line.exists("e") );
		REQUIRE( not cmd_line.exists("f") );
		REQUIRE( not cmd_line.exists("k") );
		REQUIRE( not cmd_line.exists("l") );
		REQUIRE( not cmd_line.exists("m") );
	}
}


