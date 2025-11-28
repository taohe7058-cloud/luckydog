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
        bool isFast{false};    // 加速状态
        bool isReverse{false}; // 新增：倒车状态

        void Fast() noexcept;
        void Reverse() noexcept; // 新增：处理 B 指令

        void Move() noexcept;
        void TurnLeft() noexcept;
        void TurnRight() noexcept;

        void Step() noexcept;         // 前进 1 步
        void StepBackward() noexcept; // 新增：后退 1 步
        void RotateLeft() noexcept;
        void RotateRight() noexcept;

    private:
        Pose pose;
    };
}