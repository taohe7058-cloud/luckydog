#include "ExecutorImpl.hpp"
#include <new>

namespace adas
{
    Executor* Executor::NewExecutor(const Pose& pose) noexcept {
        return new (std::nothrow) ExecutorImpl(pose);
    }

    ExecutorImpl::ExecutorImpl(const Pose& pose) noexcept : pose(pose) {}

    Pose ExecutorImpl::Query(void) const noexcept {
        return pose;
    }

    void ExecutorImpl::Execute(const std::string& commands) noexcept {
        for (const auto cmd : commands) {
            switch (cmd) {
                case 'M': Move(); break;
                case 'L': TurnLeft(); break;
                case 'R': TurnRight(); break;
                case 'F': Fast(); break; // 新增 F 指令处理
                default: break;
            }
        }
    }

    // --- 状态变更 ---
    void ExecutorImpl::Fast() noexcept {
        isFast = !isFast; // 切换状态
    }

    // --- 核心业务逻辑 (受状态影响) ---

    void ExecutorImpl::Move() noexcept {
        if (isFast) {
            Step(); // 加速状态下，多走一步
        }
        Step();     // 无论是否加速，都要走这一步
    }

    void ExecutorImpl::TurnLeft() noexcept {
        if (isFast) {
            Step(); // 加速状态下，先走一步
        }
        RotateLeft(); // 然后转向
    }

    void ExecutorImpl::TurnRight() noexcept {
        if (isFast) {
            Step(); // 加速状态下，先走一步
        }
        RotateRight(); // 然后转向
    }

    // --- 基础原子操作 (不处理状态，只干活) ---

    void ExecutorImpl::Step() noexcept {
        switch (pose.heading) {
            case 'N': pose.y++; break;
            case 'S': pose.y--; break;
            case 'E': pose.x++; break;
            case 'W': pose.x--; break;
        }
    }

    void ExecutorImpl::RotateLeft() noexcept {
        switch (pose.heading) {
            case 'N': pose.heading = 'W'; break;
            case 'W': pose.heading = 'S'; break;
            case 'S': pose.heading = 'E'; break;
            case 'E': pose.heading = 'N'; break;
        }
    }

    void ExecutorImpl::RotateRight() noexcept {
        switch (pose.heading) {
            case 'N': pose.heading = 'E'; break;
            case 'E': pose.heading = 'S'; break;
            case 'S': pose.heading = 'W'; break;
            case 'W': pose.heading = 'N'; break;
        }
    }
}