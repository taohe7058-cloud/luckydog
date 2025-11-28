#pragma once
#include "Executor.hpp"

namespace adas
{
    class ExecutorImpl final : public Executor
    {
    public:
        explicit ExecutorImpl(const Pose& pose) noexcept;
        ~ExecutorImpl() noexcept = default;

        void Execute(const std::string& command) noexcept override;
        Pose Query(void) const noexcept override;

    private:
        // 增加 isFast 状态标记
        bool isFast{false}; 

        // 状态与动作分离
        void Fast() noexcept;      // 处理 F 指令
        void Move() noexcept;      // 处理 M 指令
        void TurnLeft() noexcept;  // 处理 L 指令
        void TurnRight() noexcept; // 处理 R 指令

        // 提取原子操作：仅移动1格，不包含任何状态判断
        void Step() noexcept; 
        // 提取原子操作：仅改变朝向
        void RotateLeft() noexcept;
        void RotateRight() noexcept;

    private:
        Pose pose;
    };
}