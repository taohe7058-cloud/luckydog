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
                case 'F': Fast(); break;
                case 'B': Reverse(); break; // 新增 B 指令
                default: break;
            }
        }
    }

    void ExecutorImpl::Fast() noexcept {
        isFast = !isFast;
    }

    // 新增：切换倒车状态
    void ExecutorImpl::Reverse() noexcept {
        isReverse = !isReverse;
    }

    // --- 核心业务逻辑 (同时受 F 和 B 影响) ---

    void ExecutorImpl::Move() noexcept {
        // 如果是加速状态，先移动一次
        if (isFast) {
            if (isReverse) StepBackward(); else Step();
        }
        // 无论如何都要移动一次
        if (isReverse) StepBackward(); else Step();
    }

    void ExecutorImpl::TurnLeft() noexcept {
        // 加速状态下的移动
        if (isFast) {
            if (isReverse) StepBackward(); else Step();
        }
        // 倒车时 L 变右转，正常 L 左转
        if (isReverse) RotateRight(); else RotateLeft();
    }

    void ExecutorImpl::TurnRight() noexcept {
        // 加速状态下的移动
        if (isFast) {
            if (isReverse) StepBackward(); else Step();
        }
        // 倒车时 R 变左转，正常 R 右转
        if (isReverse) RotateLeft(); else RotateRight();
    }

    // --- 原子操作 ---

    void ExecutorImpl::Step() noexcept { // 前进
        switch (pose.heading) {
            case 'N': pose.y++; break;
            case 'S': pose.y--; break;
            case 'E': pose.x++; break;
            case 'W': pose.x--; break;
        }
    }

    // 新增：后退实现 (坐标变化与前进相反)
    void ExecutorImpl::StepBackward() noexcept {
        switch (pose.heading) {
            case 'N': pose.y--; break;
            case 'S': pose.y++; break;
            case 'E': pose.x--; break;
            case 'W': pose.x++; break;
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