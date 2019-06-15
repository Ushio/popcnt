#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <iostream>
#include <chrono>
#include <intrin.h>

uint32_t popcnt32_naive(uint32_t x) {
	uint32_t n = 0;
	for (int i = 0; i < 32; ++i) {
		if (x & 0x1) {
			n++;
		}
		x = x >> 1;
	}
	return n;
}
uint32_t popcnt32_bitop(uint32_t x) {
	x = (x & 0x55555555) + ((x & 0xAAAAAAAA) >> 1);
	x = (x & 0x33333333) + ((x & 0xCCCCCCCC) >> 2);
	x = (x & 0x0F0F0F0F) + ((x & 0xF0F0F0F0) >> 4);
	x = (x & 0x00FF00FF) + ((x & 0xFF00FF00) >> 8);
	x = (x & 0x0000FFFF) + ((x & 0xFFFF0000) >> 16);
	return x;
}
uint32_t popcnt32_intrin(uint32_t x) {
	return __popcnt(x);
}

class Stopwatch {
public:
	Stopwatch() :_beginAt(std::chrono::high_resolution_clock::now()) {

	}
	// seconds
	double elapsed() const {
		auto n = std::chrono::high_resolution_clock::now();
		return (double)std::chrono::duration_cast<std::chrono::microseconds>(n - _beginAt).count() * 0.001 * 0.001;
	}
private:
	std::chrono::high_resolution_clock::time_point _beginAt;
};

template <class F>
uint32_t popcnt32_run(F f) {
	uint32_t c = 0;
	for (uint32_t x = 0; x < 0xFFFFFF; ++x) {
		c = f(x);
	}
	return c;
}

TEST_CASE("bench") {
	{
		Stopwatch sw;
		auto c = popcnt32_run(popcnt32_naive);
		printf("popcnt32_naive %f ms\n", sw.elapsed() * 1000.0);
		printf("    dummy print : %d\n", c);
	}
	{
		Stopwatch sw;
		auto c = popcnt32_run(popcnt32_bitop);
		printf("popcnt32_bitop %f ms\n", sw.elapsed() * 1000.0);
		printf("    dummy print : %d\n", c);
	}
	{
		Stopwatch sw;
		auto c = popcnt32_run(popcnt32_intrin);
		printf("popcnt32_intrin %f ms\n", sw.elapsed() * 1000.0);
		printf("    dummy print : %d\n", c);
	}
}

TEST_CASE("popcnt") {
	REQUIRE(popcnt32_naive(0xFFFFFFFF) == 32);
	REQUIRE(popcnt32_naive(0x0FFFFFFF) == 28);
	REQUIRE(popcnt32_naive(0x00FFFFFF) == 24);
	REQUIRE(popcnt32_naive(0x000FFFFF) == 20);
	REQUIRE(popcnt32_naive(0x0000FFFF) == 16);
	REQUIRE(popcnt32_naive(0x00000FFF) == 12);
	REQUIRE(popcnt32_naive(0x000000FF) == 8);
	REQUIRE(popcnt32_naive(0x0000000F) == 4);
	REQUIRE(popcnt32_naive(0x00000000) == 0);

	REQUIRE(popcnt32_bitop(0xFFFFFFFF) == 32);
	REQUIRE(popcnt32_bitop(0x0FFFFFFF) == 28);
	REQUIRE(popcnt32_bitop(0x00FFFFFF) == 24);
	REQUIRE(popcnt32_bitop(0x000FFFFF) == 20);
	REQUIRE(popcnt32_bitop(0x0000FFFF) == 16);
	REQUIRE(popcnt32_bitop(0x00000FFF) == 12);
	REQUIRE(popcnt32_bitop(0x000000FF) == 8);
	REQUIRE(popcnt32_bitop(0x0000000F) == 4);
	REQUIRE(popcnt32_bitop(0x00000000) == 0);

	REQUIRE(popcnt32_intrin(0xFFFFFFFF) == 32);
	REQUIRE(popcnt32_intrin(0x0FFFFFFF) == 28);
	REQUIRE(popcnt32_intrin(0x00FFFFFF) == 24);
	REQUIRE(popcnt32_intrin(0x000FFFFF) == 20);
	REQUIRE(popcnt32_intrin(0x0000FFFF) == 16);
	REQUIRE(popcnt32_intrin(0x00000FFF) == 12);
	REQUIRE(popcnt32_intrin(0x000000FF) == 8);
	REQUIRE(popcnt32_intrin(0x0000000F) == 4);
	REQUIRE(popcnt32_intrin(0x00000000) == 0);

	for (uint32_t x = 0; x < 0xFFFF; ++x) {
		REQUIRE(popcnt32_intrin(x) == popcnt32_naive(x));
	}
	for (uint32_t x = 0; x < 0xFFFF; ++x) {
		REQUIRE(popcnt32_intrin(x << 16) == popcnt32_naive(x << 16));
	}

	for (uint32_t x = 0; x < 0xFFFF; ++x) {
		REQUIRE(popcnt32_intrin(x) == popcnt32_bitop(x));
	}
	for (uint32_t x = 0; x < 0xFFFF; ++x) {
		REQUIRE(popcnt32_intrin(x << 16) == popcnt32_bitop(x << 16));
	}
}
