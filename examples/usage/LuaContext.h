#pragma once

#include <filesystem>

struct lua_State;

namespace holgen::usages
{
    class LuaContext
    {
        enum class CreateValue
        {
            True
        };

    public:
        inline static constexpr const CreateValue Create = CreateValue::True;
        explicit LuaContext(lua_State* state);

        explicit LuaContext(CreateValue create);
        ~LuaContext();

        operator lua_State*();

        std::string Execute(const std::string& str);
        [[nodiscard]] std::string ExecuteReturn(const std::string& str);

        std::string ExecuteFile(const std::filesystem::path& path);

    private:
        lua_State* mState;
        const bool mOwn;
    };
} // namespace armonium::core::lua
