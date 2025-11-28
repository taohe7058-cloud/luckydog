#include <gtest/gtest.h>
#include <memory>
#include "Executor.hpp"

namespace adas
{
    // 1. 测试初始化：默认位置 (0,0,N)
    TEST(ExecutorTest, should_return_default_pose_when_without_init_and_command) {
        std::unique_ptr<Executor> executor(Executor::NewExecutor());
        ASSERT_EQ(Pose({0, 0, 'N'}), executor->Query());
    }

    // 2. 测试初始化：指定位置
    TEST(ExecutorTest, should_return_init_pose_when_initialized) {
        std::unique_ptr<Executor> executor(Executor::NewExecutor({1, 5, 'E'}));
        ASSERT_EQ(Pose({1, 5, 'E'}), executor->Query());
    }

    // 3. 测试 M 指令 (4个方向)
    TEST(ExecutorTest, should_return_x_plus_1_given_command_is_M_and_facing_is_E) {
        std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
        executor->Execute("M");
        ASSERT_EQ(Pose({1, 0, 'E'}), executor->Query());
    }

    TEST(ExecutorTest, should_return_y_plus_1_given_command_is_M_and_facing_is_N) {
        std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'N'}));
        executor->Execute("M");
        ASSERT_EQ(Pose({0, 1, 'N'}), executor->Query());
    }

    // 4. 测试 L 指令
    TEST(ExecutorTest, should_return_facing_W_given_command_is_L_and_facing_is_N) {
        std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'N'}));
        executor->Execute("L");
        ASSERT_EQ(Pose({0, 0, 'W'}), executor->Query());
    }

    TEST(ExecutorTest, should_return_facing_N_given_command_is_L_and_facing_is_E) {
        std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
        executor->Execute("L");
        ASSERT_EQ(Pose({0, 0, 'N'}), executor->Query());
    }

    // 5. 测试 R 指令
    TEST(ExecutorTest, should_return_facing_E_given_command_is_R_and_facing_is_N) {
        std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'N'}));
        executor->Execute("R");
        ASSERT_EQ(Pose({0, 0, 'E'}), executor->Query());
    }

    // 6. 测试批量指令 (M L R 组合)
    TEST(ExecutorTest, should_execute_batch_commands) {
        std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'N'}));
        // N -> M(0,1,N) -> L(0,1,W) -> M(-1,1,W) -> R(-1,1,N)
        executor->Execute("MLMR");
        ASSERT_EQ(Pose({-1, 1, 'N'}), executor->Query());
    }
}