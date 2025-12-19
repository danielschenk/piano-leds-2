#ifndef COMMON_PLATFORM_HPP
#define COMMON_PLATFORM_HPP

#ifndef TEST_BUILD
#define NOINIT __attribute__((section(".noinit")))
#else
#define NOINIT
#endif

#endif
