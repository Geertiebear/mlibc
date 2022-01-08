#include <getopt.h>
#include <assert.h>
#include <stdio.h>

const char *optstring = "abcdef";

static void match_order() {
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

int main() {
	match_order();
}
