//
// Created by thornhill on 21/05/25.
//

#include "blueprint.hpp"

#include <qguiapplication_platform.h>
#include <qopenglcontext_platform.h>

#include "debug.hpp"

static auto token_registry = std::make_unique<TokenRegistry>();

Token TokenRegistry::register_token(std::string name) {
    auto token = _id++;
    this->_tokens[token] = name;
    return token;
}

std::string TokenRegistry::from_token(Token value) {
    return this->_tokens[value];
}

bool TokenRegistry::has_token(Token value) const {
    return this->_tokens.contains(value);
}

std::unordered_map<Token, std::string> TokenRegistry::get_tokens() const {
    return this->_tokens;
}

Blueprint::Blueprint() {}

void Blueprint::declare_registry() {
    auto registry = token_registry->register_token("__registry__");

    qInfo() << "Declared Registry with token" << registry;
}

std::shared_ptr<Blueprint> Blueprint::add_controller(std::string name) {
    this->_controllers[name] = std::make_shared<Blueprint>();
    token_registry->register_token(name);

    return this->_controllers[name];
}

void Blueprint::add_method(std::string name, endpoint cb) {
    token_registry->register_token(name);

    this->_endpoints[name] = cb;
}

void Blueprint::add_alias(std::string name, std::string new_name) {
    token_registry->register_token(name);

    this->_aliases[name] = new_name;
}

void Blueprint::process(client_t &client, req_t &req) {
    Token token;

    req->data >> token;
    if (!token_registry->has_token(token)) {
        qCritical() << "Token doesn't exist in registry" << token;
        client->socket << status(STATUS_INTERNAL_ERROR, req);
        return;
    }

    std::string name = token_registry->from_token(token);

    qDebug() << "reading token:" << name;

    if (name == "__registry__") {
        client->socket << (status(STATUS_OK, req) << token_registry->get_tokens());
        return;
    }

    if (this->_aliases.contains(name)) {
        name = this->_aliases[name];
    }

    if (this->_controllers.contains(name)) {
        return this->_controllers[name]->process(client, req);
    }

    if (this->_endpoints.contains(name)) {
        return this->_endpoints[name](client, req);
    }

    client->socket << status(STATUS_INTERNAL_ERROR, req);
}

std::unordered_map<Token, std::string> Blueprint::get_tokens() const {
    return token_registry->get_tokens();
}

