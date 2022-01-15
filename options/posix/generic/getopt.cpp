#include <mlibc/getopt.hpp>
#include <unistd.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <utility>

namespace mlibc {

// optarg and optind are provided to us by the GLIBC part of the mlibc.
int do_short_getopt(int argc, char **argv, const char *optstring) {
	char c;
	char *place;
	static char *scan = NULL;

	// The man page for getopt() states that
	// if optstring[0] == '+' or the POSIXLY_CORRECT env var is set
	// then option scanning is stopped at the first place a non-option
	// is found. If this is not the case, then getopt() should keep
	// scanning, and permute argv such that the nonoptions are
	// moved to the back. This is in line with glibc behaviour, but
	// musl doesn't seem to implement this.
	bool posixly_correct = optstring[0] == '+';
	if (getenv("POSIXLY_CORRECT"))
		posixly_correct = true;

	optarg = NULL;

	if (!scan || *scan == '\0') {
		if (optind == 0)
			optind++;

		if (optind >= argc)
			return EOF;

		if (argv[optind][0] != '-' || argv[optind][1] == '\0') {
			if (posixly_correct)
				return EOF;

			// Scan till we find next option argument.
			// TODO(geert): preserve nonoption argument order.
			for (int i = optind + 1; i < argc; i++) {
				if (argv[i][0] == '-' && argv[i][1] != '\0') {
					std::swap(argv[optind], argv[i]);
					break;
				}
			}

			// Check again if we have an option argument now.
			if (argv[optind][0] != '-' || argv[optind][1] == '\0')
				return EOF;
		}

		if (argv[optind][1] == '-' && argv[optind][2] == '\0') {
			optind++;
			return EOF;
		}

		scan = argv[optind]+1;
		optind++;
	}

	c = *scan++;
	place = strchr(optstring, c);

	if (!place || c == ':') {
		fprintf(stderr, "%s: unknown option -%c\n", argv[0], c);
		return '?';
	}

	place++;
	if (*place == ':') {
		if (*scan != '\0') {
			optarg = scan;
			scan = NULL;
		} else if( optind < argc ) {
			optarg = argv[optind];
			optind++;
		} else {
			fprintf(stderr, "%s: option requires argument -%c\n", argv[0], c);
			return ':';
		}
	}

	return c;
}

} // namespace mlibc
