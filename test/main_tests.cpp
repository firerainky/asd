#include <gtest/gtest.h>

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);

    /**
     * 过滤器设置要跑哪些测试，格式是通配符，不是正则表达式，规则如下：
     * --gtest_filter=POSITIVE_PATTERNS[-NEGATIVE_PATTERNS]
      Run only the tests whose name matches one of the positive patterns but
      none of the negative patterns. '?' matches any single character; '*'
      matches any substring; ':' separates two patterns.
     * 默认是全跑，只跑带 Multiply 的可以写成："*Multiply*"
     * 只不跑 Multiply 的可以写成："*:-*Multiply*"
     * 参考：https://github.com/google/googletest/blob/main/docs/advanced.md#running-a-subset-of-the-tests
     */
    ::testing::GTEST_FLAG(filter) = "*";

    return RUN_ALL_TESTS();
}