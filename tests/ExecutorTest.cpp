#include <gtest/gtest.h>
#include <memory>
#include "Executor.hpp"

namespace adas
{
    // --- 需求1 测试回顾 ---
    TEST(ExecutorTest, should_return_init_pose_when_initialized) {
        std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
        ASSERT_EQ(Pose({0, 0, 'E'}), executor->Query());
    }

    TEST(ExecutorTest, should_return_default_pose_when_without_init_and_command) {
        std::unique_ptr<Executor> executor(Executor::NewExecutor());
        ASSERT_EQ(Pose({0, 0, 'N'}), executor->Query());
    }

    TEST(ExecutorTest, should_return_x_plus_1_given_command_is_M_and_facing_is_E) {
        std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
        executor->Execute("M");
        ASSERT_EQ(Pose({1, 0, 'E'}), executor->Query());
    }

    TEST(ExecutorTest, should_return_facing_N_given_command_is_L_and_facing_is_E) {
        std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
        executor->Execute("L");
        ASSERT_EQ(Pose({0, 0, 'N'}), executor->Query());
    }

    TEST(ExecutorTest, should_return_facing_S_given_command_is_R_and_facing_is_E) {
        std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
        executor->Execute("R");
        ASSERT_EQ(Pose({0, 0, 'S'}), executor->Query());
    }

    // --- 需求2：加速功能 (F) 测试 ---

    // 1. 测试 F 状态下，M 指令移动 2 格
    TEST(ExecutorTest, should_move_2_steps_given_status_is_fast_command_is_M_and_facing_is_E) {
        std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
        executor->Execute("FM"); // 激活加速 -> 移动
        ASSERT_EQ(Pose({2, 0, 'E'}), executor->Query());
    }

    // 2. 测试 F 状态下，L 指令：先前进 1 格，再左转
    // 初始(0,0,E) -> 前进1格(1,0,E) -> 左转(1,0,N)
    TEST(ExecutorTest, should_move_1_step_then_turn_left_given_status_is_fast_command_is_L_and_facing_is_E) {
        std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
        executor->Execute("FL");
        ASSERT_EQ(Pose({1, 0, 'N'}), executor->Query());
    }

    // 3. 测试 F 状态下，R 指令：先前进 1 格，再右转
    // 初始(0,0,E) -> 前进1格(1,0,E) -> 右转(1,0,S)
    TEST(ExecutorTest, should_move_1_step_then_turn_right_given_status_is_fast_command_is_R_and_facing_is_E) {
        std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
        executor->Execute("FR");
        ASSERT_EQ(Pose({1, 0, 'S'}), executor->Query());
    }

    // 4. 测试 F 状态可以被取消 (再次输入 F)
    // F(加速) -> F(取消加速) -> M(移动1格)
    TEST(ExecutorTest, should_return_to_normal_speed_given_command_FFM) {
        std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
        executor->Execute("FFM");
        ASSERT_EQ(Pose({1, 0, 'E'}), executor->Query());
    }
}
// ... (保留之前的测试代码) ...

    // --- 需求3：倒车功能 (B) 测试 ---

    // 1. 测试 B 状态下，M 指令：后退 1 格
    // 初始(0,0,E) -> B状态 -> M -> (-1,0,E)
    TEST(ExecutorTest, should_move_back_1_step_given_status_is_reverse_command_is_M_and_facing_is_E) {
        std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
        executor->Execute("BM");
        ASSERT_EQ(Pose({-1, 0, 'E'}), executor->Query());
    }

    // 2. 测试 B 状态下，L 指令：实际右转
    // 初始(0,0,N) -> B状态 -> L -> 朝东(E)
    TEST(ExecutorTest, should_turn_right_given_status_is_reverse_command_is_L_and_facing_is_N) {
        std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'N'}));
        executor->Execute("BL");
        ASSERT_EQ(Pose({0, 0, 'E'}), executor->Query());
    }

    // 3. 测试 B 状态下，R 指令：实际左转
    // 初始(0,0,N) -> B状态 -> R -> 朝西(W)
    TEST(ExecutorTest, should_turn_left_given_status_is_reverse_command_is_R_and_facing_is_N) {
        std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'N'}));
        executor->Execute("BR");
        ASSERT_EQ(Pose({0, 0, 'W'}), executor->Query());
    }

    // 4. 测试 F + B 叠加状态：M 指令后退 2 格
    // 初始(0,0,E) -> F+B -> M -> (-2,0,E)
    TEST(ExecutorTest, should_move_back_2_steps_given_status_is_fast_and_reverse_command_is_M) {
        std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
        executor->Execute("FBM");
        ASSERT_EQ(Pose({-2, 0, 'E'}), executor->Query());
    }

    // 5. 测试 F + B 叠加状态：L 指令先退 1 格，再右转
    // 初始(0,0,E) -> F+B -> L -> 退1格(-1,0) -> 右转(S) -> (-1,0,S)
    TEST(ExecutorTest, should_move_back_1_step_then_turn_right_given_status_is_fast_and_reverse_command_is_L) {
        std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
        executor->Execute("FBL");
        ASSERT_EQ(Pose({-1, 0, 'S'}), executor->Query());
    }
    
    // 6. 测试再次接收 B 指令取消倒车状态
    TEST(ExecutorTest, should_cancel_reverse_status_when_receive_B_again) {
        std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
        executor->Execute("BBM"); // B(开启) -> B(关闭) -> M(正常前进)
        ASSERT_EQ(Pose({1, 0, 'E'}), executor->Query());
    }
}