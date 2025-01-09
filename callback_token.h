#pragma once

namespace cui {

struct NOVTABLE callback_token {
    virtual ~callback_token() noexcept = default;
};

struct callback_token_lambda final : callback_token {
    callback_token_lambda(std::function<void()> release_func) : m_release_func(std::move(release_func)) {}

    callback_token_lambda(const callback_token_lambda&) = delete;
    callback_token_lambda& operator=(const callback_token_lambda&) = delete;

    callback_token_lambda(callback_token_lambda&&) = default;
    callback_token_lambda& operator=(callback_token_lambda&&) = default;

    ~callback_token_lambda() noexcept override { m_release_func(); }

    std::function<void()> m_release_func;
};

} // namespace cui
