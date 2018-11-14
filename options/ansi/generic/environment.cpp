
#include <stdlib.h>
#include <stdio.h>

#include <bits/ensure.h>
#include <mlibc/allocator.hpp>
#include <mlibc/debug.hpp>

#include <frg/string.hpp>
#include <frg/vector.hpp>

static char *emptyEnvironment[] = { nullptr };

char **environ = emptyEnvironment;

namespace {

// Environment vector that is mutated by putenv() and setenv().
// Cannot be global as it is accessed during library initialization.
frg::vector<char *, MemoryAllocator> *get_env_vector() {
	static frg::vector<char *, MemoryAllocator> env_vector{getAllocator()};
	return &env_vector;
}

void update_env_copy() {
	if(environ == get_env_vector()->data()) {
		return;
	}
	
	// If the environ variable was changed, we copy the environment.
	// Note that we must only copy the pointers but not the strings themselves!
	__ensure(!*environ); // TODO: Actually copy the entries.
	get_env_vector()->push(nullptr);
	
	environ = get_env_vector()->data();
}

void fix_env_pointer() {
	environ = get_env_vector()->data();
}

size_t find_env_index(frg::string_view name) {
	__ensure(environ == get_env_vector()->data());
	__ensure(!get_env_vector()->empty());

	for(size_t i = 0; (*get_env_vector())[i]; i++) {
		frg::string_view view{(*get_env_vector())[i]};
		size_t s = view.find_first('=');
		__ensure(s != size_t(-1));
		if(view.sub_string(0, s) == name)
			return i;
	}

	return -1;
}

} // anonymous namespace

char *getenv(const char *name) {
	// TODO: We do not necessarily need this.
	update_env_copy();

	auto k = find_env_index(name);
	if(k == size_t(-1))
		return nullptr;
	
	frg::string_view view{(*get_env_vector())[k]};
	size_t s = view.find_first('=');
	__ensure(s != size_t(-1));
	return const_cast<char *>(view.data() + s + 1);
}

int putenv(const char *string) {
	update_env_copy();

	frg::string_view view{string};
	size_t s = view.find_first('=');
	if(s == size_t(-1))
		__ensure(!"Environment strings need to contain an equals sign");
	
	auto k = find_env_index(view.sub_string(0, s));
	if(k != size_t(-1)) {
		__ensure(!"Implement enviornment variable replacement");
	}else{
		__ensure(!get_env_vector()->back()); // Last pointer must always be a null delimiter.
		get_env_vector()->back() = const_cast<char *>(string);
		get_env_vector()->push(nullptr);
		fix_env_pointer();
	}

	return 0;
}

int setenv(const char *name, const char *value, int overwrite) {
	// We never free strings here.
	// TODO: Reuse them?
	__ensure(overwrite);
	char *string;
	__ensure(asprintf(&string, "%s=%s", name, value) > 0);
	__ensure(string);

	return putenv(string);
}

int unsetenv(const char *name) {
	update_env_copy();

	auto k = find_env_index(name);
	FRG_ASSERT(k != size_t(-1));

	// Last pointer is always null.
	__ensure(get_env_vector()->size() >= 2 && !get_env_vector()->back());
	std::swap((*get_env_vector())[k], (*get_env_vector())[get_env_vector()->size() - 2]);
	get_env_vector()->pop();
	get_env_vector()->back() = nullptr;

	return 0;
}

