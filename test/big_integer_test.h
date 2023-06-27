#pragma once

#include "big_integer.h"
#include <gtest/gtest.h>

/**
 * 使用 Typed Tests 同时测试不同 limb type 的 big integer
 * reference: https://github.com/google/googletest/blob/main/docs/advanced.md#typed-tests
 */

// 需要测试的 limb_type 类型列表，目前仅需要 uint32 和 uint64
using LimbTypes = ::testing::Types<limbtype>;