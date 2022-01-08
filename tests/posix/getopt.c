#include <getopt.h>
#include <assert.h>
#include <stdio.h>

const char *optstring = "abcdef";

static void match_order() {
	optind = 1;
	char *argv[] = {
		"",
		"-b",
		"-a",
		"-f",
		"-e",
		"-c",
	};
	int argc = sizeof(argv) / sizeof(char *);

	assert(getopt(argc, argv, optstring) == 'b');
	assert(getopt(argc, argv, optstring) == 'a');
	assert(getopt(argc, argv, optstring) == 'f');
	assert(getopt(argc, argv, optstring) == 'e');
	assert(getopt(argc, argv, optstring) == 'c');
	assert(getopt(argc, argv, optstring) == EOF);
}

static void dont_continue_after_nonoption() {
	const char *plus_optstring = "+abcdef";
	optind = 1;
	char *argv[] = {
		"",
		"-b",
		"-a",
		"-f",
		"foo",
		"-e",
		"-c",
	};
	int argc = sizeof(argv) / sizeof(char *);

	assert(getopt(argc, argv, plus_optstring) == 'b');
	assert(getopt(argc, argv, plus_optstring) == 'a');
	assert(getopt(argc, argv, plus_optstring) == 'f');
	assert(getopt(argc, argv, plus_optstring) == EOF);
}

static void continue_after_nonoption() {
	optind = 1;
	char *argv[] = {
		"",
		"-b",
		"-a",
		"-f",
		"foo",
		"-e",
		"-c",
	};
	int argc = sizeof(argv) / sizeof(char *);

	assert(getopt(argc, argv, optstring) == 'b');
	assert(getopt(argc, argv, optstring) == 'a');
	assert(getopt(argc, argv, optstring) == 'f');
	assert(getopt(argc, argv, optstring) == 'e');
	assert(getopt(argc, argv, optstring) == 'c');
	assert(getopt(argc, argv, optstring) == EOF);
}

int main() {
	match_order();
	dont_continue_after_nonoption();
	continue_after_nonoption();
}
