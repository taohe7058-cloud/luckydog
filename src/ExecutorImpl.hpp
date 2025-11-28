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
        // 辅助函数，处理具体的移动和转向
        void Move() noexcept;
        void TurnLeft() noexcept;
        void TurnRight() noexcept;

    private:
        Pose pose; // 存储车辆当前状态
    };
}