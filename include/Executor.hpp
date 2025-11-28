#pragma once
#include <string>

namespace adas
{
    // 定义位置和朝向
    struct Pose {
        int x;
        int y;
        char heading; // 'N', 'S', 'E', 'W'
    };

    // 重载==运算符，方便测试比较
    inline bool operator==(const Pose& lhs, const Pose& rhs) {
        return lhs.x == rhs.x && lhs.y == rhs.y && lhs.heading == rhs.heading;
    }

    // 抽象基类
    class Executor
    {
    public:
        // 工厂方法：创建执行器实例
        static Executor* NewExecutor(const Pose& pose = {0, 0, 'N'}) noexcept;

    public:
        Executor(void) = default;
        virtual ~Executor(void) = default;
        
        // 禁止拷贝
        Executor(const Executor&) = delete;
        Executor& operator=(const Executor&) = delete;

    public:
        // 核心接口：执行指令
        virtual void Execute(const std::string& command) noexcept = 0;
        
        // 查询当前状态
        virtual Pose Query(void) const noexcept = 0;
    };
}