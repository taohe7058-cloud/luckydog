#include "ExecutorImpl.hpp"
#include <new>

namespace adas
{
    // 实现工厂方法
    Executor* Executor::NewExecutor(const Pose& pose) noexcept {
        return new (std::nothrow) ExecutorImpl(pose);
    }

    // 构造函数初始化 Pose
    ExecutorImpl::ExecutorImpl(const Pose& pose) noexcept : pose(pose) {}

    // 查询接口实现
    Pose ExecutorImpl::Query(void) const noexcept {
        return pose;
    }

    // 执行接口实现
    void ExecutorImpl::Execute(const std::string& commands) noexcept {
        for (const auto cmd : commands) {
            switch (cmd) {
                case 'M': Move(); break;
                case 'L': TurnLeft(); break;
                case 'R': TurnRight(); break;
                default: break; // 忽略未知指令
            }
        }
    }

    // M: 前进逻辑
    void ExecutorImpl::Move() noexcept {
        switch (pose.heading) {
            case 'N': pose.y++; break;
            case 'S': pose.y--; break;
            case 'E': pose.x++; break;
            case 'W': pose.x--; break;
        }
    }

    // L: 左转逻辑
    void ExecutorImpl::TurnLeft() noexcept {
        switch (pose.heading) {
            case 'N': pose.heading = 'W'; break;
            case 'W': pose.heading = 'S'; break;
            case 'S': pose.heading = 'E'; break;
            case 'E': pose.heading = 'N'; break;
        }
    }

    // R: 右转逻辑
    void ExecutorImpl::TurnRight() noexcept {
        switch (pose.heading) {
            case 'N': pose.heading = 'E'; break;
            case 'E': pose.heading = 'S'; break;
            case 'S': pose.heading = 'W'; break;
            case 'W': pose.heading = 'N'; break;
        }
    }
}