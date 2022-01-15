#include <assert.h>
#include <getopt.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

static void test_long_opt() {
	optind = 1;
	const char *shortopts = "f:";

	const struct option longopts[] = {
		{"foo", required_argument, NULL, 'f'},
		{NULL, no_argument, NULL, 0}
	};

	int test_argc = 3;

	char *test_argv[] = {
		"dummy",
		"--foo",
		"abc"
	};

	int c;
	c = getopt_long(test_argc, test_argv, shortopts, longopts, NULL);
	assert(c == 'f');
	c = getopt_long(test_argc, test_argv, shortopts, longopts, NULL);
	assert(c == -1);
}

static void test_short_opt() {
	optind = 1;
	const char *shortopts = "f:";

	const struct option longopts[] = {
		{NULL, no_argument, NULL, 0}
	};

	int test_argc = 3;

	char *test_argv[] = {
		"dummy",
		"-f",
		"abc"
	};

	int c;
	c = getopt_long(test_argc, test_argv, shortopts, longopts, NULL);
	assert(c == 'f');
	assert(!strcmp(optarg, "abc"));
	c = getopt_long(test_argc, test_argv, shortopts, longopts, NULL);
	assert(c == -1);
}

int main() {
	test_long_opt();
	test_short_opt();
}
