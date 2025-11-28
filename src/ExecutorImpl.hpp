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
        bool isFast{false};
        bool isReverse{false};

        void Fast() noexcept;
        void Reverse() noexcept;
        
        // 新增：掉头处理
        void TurnRound() noexcept;

        void Move() noexcept;
        void TurnLeft() noexcept;
        void TurnRight() noexcept;

        void Step() noexcept;
        void StepBackward() noexcept;
        void RotateLeft() noexcept;
        void RotateRight() noexcept;
        
        // 辅助函数：解析指令（处理 TR 替换）
        std::string ParseCommand(const std::string& command) const noexcept;

    private:
        Pose pose;
    };
}