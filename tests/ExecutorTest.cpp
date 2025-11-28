#include <gtest/gtest.h>
#include <memory>
#include "Executor.hpp"

namespace adas
{
    // ==========================================
    // 需求1：基本控制指令 (M, L, R) 测试
    // ==========================================

    // 测试初始化：指定位置初始化
    TEST(ExecutorTest, should_return_init_pose_when_initialized) {
        std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
        ASSERT_EQ(Pose({0, 0, 'E'}), executor->Query());
    }

    // 测试初始化：默认位置 (0,0,N)
    TEST(ExecutorTest, should_return_default_pose_when_without_init_and_command) {
        std::unique_ptr<Executor> executor(Executor::NewExecutor());
        ASSERT_EQ(Pose({0, 0, 'N'}), executor->Query());
    }

    // M: 移动指令测试 (以朝东为例)
    TEST(ExecutorTest, should_return_x_plus_1_given_command_is_M_and_facing_is_E) {
        std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
        executor->Execute("M");
        ASSERT_EQ(Pose({1, 0, 'E'}), executor->Query());
    }

    // L: 左转指令测试 (N -> W)
    TEST(ExecutorTest, should_return_facing_W_given_command_is_L_and_facing_is_N) {
        std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'N'}));
        executor->Execute("L");
        ASSERT_EQ(Pose({0, 0, 'W'}), executor->Query());
    }

    // R: 右转指令测试 (N -> E)
    TEST(ExecutorTest, should_return_facing_E_given_command_is_R_and_facing_is_N) {
        std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'N'}));
        executor->Execute("R");
        ASSERT_EQ(Pose({0, 0, 'E'}), executor->Query());
    }

    // ==========================================
    // 需求2：加速功能 (F) 测试
    // ==========================================

    // F+M: 加速状态下移动 2 格
    TEST(ExecutorTest, should_move_2_steps_given_status_is_fast_command_is_M_and_facing_is_E) {
        std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
        executor->Execute("FM");
        ASSERT_EQ(Pose({2, 0, 'E'}), executor->Query());
    }

    // F+L: 加速状态下左转 (先前进1格，再左转)
    // (0,0,E) -> 前进(1,0,E) -> 左转(N) -> (1,0,N)
    TEST(ExecutorTest, should_move_1_step_then_turn_left_given_status_is_fast_command_is_L_and_facing_is_E) {
        std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
        executor->Execute("FL");
        ASSERT_EQ(Pose({1, 0, 'N'}), executor->Query());
    }

    // F+R: 加速状态下右转 (先前进1格，再右转)
    // (0,0,E) -> 前进(1,0,E) -> 右转(S) -> (1,0,S)
    TEST(ExecutorTest, should_move_1_step_then_turn_right_given_status_is_fast_command_is_R_and_facing_is_E) {
        std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
        executor->Execute("FR");
        ASSERT_EQ(Pose({1, 0, 'S'}), executor->Query());
    }

    // F取消: 再次输入F取消加速
    TEST(ExecutorTest, should_return_to_normal_speed_given_command_FFM) {
        std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
        executor->Execute("FFM"); // F开启 -> F关闭 -> M走1格
        ASSERT_EQ(Pose({1, 0, 'E'}), executor->Query());
    }

    // ==========================================
    // 需求3：倒车功能 (B) 测试
    // ==========================================

    // B+M: 倒车状态下 M 后退 1 格
    TEST(ExecutorTest, should_move_back_1_step_given_status_is_reverse_command_is_M_and_facing_is_E) {
        std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
        executor->Execute("BM");
        ASSERT_EQ(Pose({-1, 0, 'E'}), executor->Query());
    }

    // B+L: 倒车状态下 L 实际为右转
    // (0,0,N) -> B -> L(右转) -> (0,0,E)
    TEST(ExecutorTest, should_turn_right_given_status_is_reverse_command_is_L_and_facing_is_N) {
        std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'N'}));
        executor->Execute("BL");
        ASSERT_EQ(Pose({0, 0, 'E'}), executor->Query());
    }

    // B+R: 倒车状态下 R 实际为左转
    // (0,0,N) -> B -> R(左转) -> (0,0,W)
    TEST(ExecutorTest, should_turn_left_given_status_is_reverse_command_is_R_and_facing_is_N) {
        std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'N'}));
        executor->Execute("BR");
        ASSERT_EQ(Pose({0, 0, 'W'}), executor->Query());
    }

    // F+B+M: 加速倒车 (后退2格)
    TEST(ExecutorTest, should_move_back_2_steps_given_status_is_fast_and_reverse_command_is_M) {
        std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
        executor->Execute("FBM");
        ASSERT_EQ(Pose({-2, 0, 'E'}), executor->Query());
    }

    // F+B+L: 加速倒车左转 (先退1格，再反向转向即右转)
    // (0,0,E) -> 退1格(-1,0,E) -> L变右转(S) -> (-1,0,S)
    TEST(ExecutorTest, should_move_back_1_step_then_turn_right_given_status_is_fast_and_reverse_command_is_L) {
        std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
        executor->Execute("FBL");
        ASSERT_EQ(Pose({-1, 0, 'S'}), executor->Query());
    }

    // ==========================================
    // 需求4：掉头功能 (TR) 测试
    // ==========================================

    // TR (正常): 左转 -> 前进1格 -> 左转
    // (0,0,E) -> L(N) -> M(0,1,N) -> L(W) -> (0,1,W)
    TEST(ExecutorTest, should_turn_round_given_command_is_TR_and_facing_is_E) {
        std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
        executor->Execute("TR");
        ASSERT_EQ(Pose({0, 1, 'W'}), executor->Query());
    }

    // F+TR (加速): 前进1格 -> 左转 -> 前进1格 -> 左转
    // (0,0,E) -> M(1,0,E) -> L(N) -> M(1,1,N) -> L(W) -> (1,1,W)
    TEST(ExecutorTest, should_execute_fast_turn_round_given_command_is_FTR_and_facing_is_E) {
        std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
        executor->Execute("FTR");
        ASSERT_EQ(Pose({1, 1, 'W'}), executor->Query());
    }

    // B+TR (倒车): TR指令被忽略
    // (0,0,E) -> B开启 -> TR忽略 -> (0,0,E)
    TEST(ExecutorTest, should_ignore_TR_given_status_is_reverse) {
        std::unique_ptr<Executor> executor(Executor::NewExecutor({0, 0, 'E'}));
        executor->Execute("BTR");
        ASSERT_EQ(Pose({0, 0, 'E'}), executor->Query());
    }
}