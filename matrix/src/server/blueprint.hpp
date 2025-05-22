//
// Created by thornhill on 21/05/25.
//

#pragma once

#include <memory>

#include "display_server.hpp"
#include "blueprint.hpp"

class WSClient;

typedef uint16_t Token;


typedef void (*endpoint)(client_t &, req_t &);


class TokenRegistry {
public:

    virtual ~TokenRegistry() = default;

    Token register_token(std::string);
    std::string from_token(Token);
    bool has_token(Token) const;

    std::unordered_map<Token, std::string> get_tokens() const;

private:
    uint16_t _id = 0;

    std::unordered_map<Token, std::string> _tokens;
};

class Blueprint {
public:

    Blueprint();

    virtual ~Blueprint() = default;

    void declare_registry();

    std::shared_ptr<Blueprint> add_controller(std::string);
    void add_method(std::string, endpoint);

    void add_alias(std::string, std::string);

    void process(client_t &, req_t &);

    std::unordered_map<Token, std::string> get_tokens() const;

private:

    std::shared_ptr<Blueprint> _parent;

    std::unordered_map<std::string, std::string> _aliases;

    std::unordered_map<std::string, std::shared_ptr<Blueprint>> _controllers;
    std::unordered_map<std::string, endpoint> _endpoints;

};
