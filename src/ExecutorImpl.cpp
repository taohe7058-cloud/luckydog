#include "ExecutorImpl.hpp"
#include <new>
#include <algorithm> // for replace

namespace adas
{
    Executor* Executor::NewExecutor(const Pose& pose) noexcept {
        return new (std::nothrow) ExecutorImpl(pose);
    }

    ExecutorImpl::ExecutorImpl(const Pose& pose) noexcept : pose(pose) {}

    Pose ExecutorImpl::Query(void) const noexcept {
        return pose;
    }

    // 解析指令：将 TR 替换为 Z
    std::string ExecutorImpl::ParseCommand(const std::string& command) const noexcept {
        std::string result = command;
        std::string target = "TR";
        std::string replacement = "Z";
        
        size_t pos = 0;
        while ((pos = result.find(target, pos)) != std::string::npos) {
            result.replace(pos, target.length(), replacement);
            pos += replacement.length();
        }
        return result;
    }

    void ExecutorImpl::Execute(const std::string& commands) noexcept {
        // 先解析，处理双字符指令
        const std::string parsedCommands = ParseCommand(commands);

        for (const auto cmd : parsedCommands) {
            switch (cmd) {
                case 'M': Move(); break;
                case 'L': TurnLeft(); break;
                case 'R': TurnRight(); break;
                case 'F': Fast(); break;
                case 'B': Reverse(); break;
                case 'Z': TurnRound(); break; // 处理 TR (Z)
                default: break;
            }
        }
    }

    void ExecutorImpl::Fast() noexcept { isFast = !isFast; }
    void ExecutorImpl::Reverse() noexcept { isReverse = !isReverse; }

    // --- 掉头逻辑 (需求4) ---
    void ExecutorImpl::TurnRound() noexcept {
        // B 状态下忽略 TR [cite: 3082]
        if (isReverse) {
            return;
        }

        // F 状态下 TR: Move -> Left -> Move -> Left 
        if (isFast) {
            Step();       // 注意：这里使用原子操作 Step()，不受 isFast/isReverse 影响
            RotateLeft();
            Step();
            RotateLeft();
            return;
        }

        // 普通 TR: Left -> Move -> Left 
        RotateLeft();
        Step();
        RotateLeft();
    }

    void ExecutorImpl::Move() noexcept {
        if (isFast) {
            if (isReverse) StepBackward(); else Step();
        }
        if (isReverse) StepBackward(); else Step();
    }

    void ExecutorImpl::TurnLeft() noexcept {
        if (isFast) {
            if (isReverse) StepBackward(); else Step();
        }
        if (isReverse) RotateRight(); else RotateLeft();
    }

    void ExecutorImpl::TurnRight() noexcept {
        if (isFast) {
            if (isReverse) StepBackward(); else Step();
        }
        if (isReverse) RotateLeft(); else RotateRight();
    }

    void ExecutorImpl::Step() noexcept {
        switch (pose.heading) {
            case 'N': pose.y++; break;
            case 'S': pose.y--; break;
            case 'E': pose.x++; break;
            case 'W': pose.x--; break;
        }
    }

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