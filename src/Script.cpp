#include "Script.h"

#include "absl/status/status.h"
#include "absl/status/statusor.h"
#include "absl/strings/str_format.h"

#include <filesystem>
#include <fstream>
#include <variant>

namespace Script
{

Token::Token(TokenValue value): value(value)
{
}

bool Token::isBlock() const
{
    return std::holds_alternative<Block>(value);
}

bool Token::isKeyword() const
{
    return std::holds_alternative<Keyword>(value);
}
bool Token::isInt() const
{
    return std::holds_alternative<int>(value);
}

Block Token::blockValue() const
{
    return std::get<Block>(value);
}

Keyword Token::keywordValue() const
{
    return std::get<Keyword>(value);
}

int Token::intValue() const
{
    return std::get<int>(value);
}

const std::regex blockRegex(R"(\w+:)");
const std::regex keywordPattern(R"((\w+)(?:\s+(\d+))?)");

absl::StatusOr<Actions> decode(std::string_view scriptID) noexcept
{
    auto actions = Actions();
    std::string path = absl::StrFormat("data/scripts/%s", scriptID);
    if (!std::filesystem::exists(path))
        return absl::NotFoundError(absl::StrFormat("Script: %s not found", scriptID));

    std::ifstream f(path);

    auto blocks = std::map<std::string, std::vector<std::string>>();

    std::string block;
    auto content = std::vector<std::string>();
    std::string line;
    while (std::getline(f, line))
    {
        // Block end
        if (line.empty())
        {
            blocks[block] = content;
            block.clear();
            content.clear();
        }

        if (!line.empty())
        {
            // Block instruction
            if (line[0] == '\t')
            {
                std::smatch match;
                if (std::regex_match(line, match, keywordPattern))
                {
                    std::string str = match[1].str();
                    int num = match[2].str().empty() ? 0 : std::stoi(match[2]);
                    std::cout << "String: " << str << ", Integer: " << num << std::endl;
                    actions.push_back(Action(str, num));
                }
                content.push_back(line);
            }
            else
            {
                // Block begin
                std::smatch match;
                if (std::regex_search(line, match, blockRegex))
                    block = match[0];
            }
        }
    }

    if (f.eof())
    {
        std::cout << "EOF\n";
        blocks[block] = content;
        block.clear();
        content.clear();
    }

    for (auto& b: blocks)
    {
        std::cout << "Block: " << b.first << "\n";
        for (auto& c: b.second)
        {
            std::cout << "\t " << c << "\n";
        }
    }

    f.close();
    return actions;
}
}; // namespace Script
