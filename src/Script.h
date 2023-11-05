#pragma once

#include "absl/status/statusor.h"

#include <regex>
#include <variant>

namespace Script
{

enum struct Block
{
    loop,
};

enum struct Keyword
{
    go_north,
    go_south,
    go_east,
    go_west,

    rotate_north,
    rotate_south,
    rotate_east,
    rotate_west,

    idle
};

using TokenValue = std::variant<Block, Keyword, int>;

class Token
{
  public:
    Token(TokenValue value);

    bool isBlock() const;
    bool isKeyword() const;
    bool isInt() const;

    Block blockValue() const;
    Keyword keywordValue() const;
    int intValue() const;

  private:
    TokenValue value;
};

struct Action
{
    Keyword keyword;
    int times;
    Action(Keyword keyword, int times): keyword(keyword), times(times) {}
};

using Actions = std::vector<Action>;

absl::StatusOr<Actions> decode(std::string_view fileName) noexcept;

} // namespace Script
