#pragma once

#include <stdint.h>
#include <bits/size_t.h>

namespace {
	// Set when the cancellation is enabled
	constexpr unsigned int tcbCancelEnableBit = 1 << 0;
	// 1 - cancellation is asynchronous, 0 - cancellation is deferred
	constexpr unsigned int tcbCancelAsyncBit = 1 << 1;
	// Set when the thread has been cancelled
	constexpr unsigned int tcbCancelTriggerBit = 1 << 2;
	// Set when the thread is in the process of being cancelled.
	constexpr unsigned int tcbCancelingBit = 1 << 3;
	// Set when the thread is exiting.
	constexpr unsigned int tcbExitingBit = 1 << 4;
	// Set when the thread has exited.
	constexpr unsigned int tcbExitedBit = 1 << 5;
}

namespace mlibc {
	// Returns true when bitmask indicates thread has been asynchronously
	// cancelled.
	static constexpr bool tcb_async_cancelled(int value) {
		return (value & (tcbCancelEnableBit | tcbCancelAsyncBit
				| tcbCancelTriggerBit)) == (tcbCancelEnableBit
					| tcbCancelAsyncBit | tcbCancelTriggerBit);
	}

	// Returns true when bitmask indicates async cancellation is enabled.
	static constexpr bool tcb_async_cancel(int value) {
		return (value & (tcbCancelEnableBit | tcbCancelAsyncBit))
			== (tcbCancelEnableBit | tcbCancelAsyncBit);
	}
}

// Gcc expects the stack canary to be at fs:0x28,
// at least on x86_64, so this struct has fixed
// ABI until stackCanary.
struct Tcb {
	Tcb *selfPointer;
	size_t dtvSize;
	void **dtvPointers;
	int tid;
	int didExit;
	void *returnValue;
	uintptr_t stackCanary;
	int cancelBits;
};

