#include <regex>

#include "RUtils/Link.hpp"
#include "RUtils/Error.hpp"



RUtils::ErrorOr<Link> Link::parse(const std::string& str) {
    // i have sinned
    // FIXME: This doesnt support both # and ? in one link, but i dont care i dont want to touch this mess any more.
    // original un-esacaped ^(?:(.+?):\/\/)?(?:(.+?)(?::(.+?))?@)?([^./]+\..+?)?(?:\/(?:(.+?)(?:$|#(.*)|\?(.*)))|$)
    // group 1 - protocol, 2 - login, 3 - password, 4 - host, 5 - resource path, 6 - document fragment, 7 - query
    std::regex main_regex("^(?:(.+?):\\/\\/)?(?:(.+?)(?::(.+?))?@)?([^./]+\\..+?)?(?:\\/(?:(.+?)(?:$|#(.*)|\\?(.*)))|$)");
    std::regex file_with_fragment_regex("^(.*?)(?:#(.+))?$");

    if(str.empty()) {
        return RUtils::Error("String is empty!", RUtils::ErrorType::invalid_argument);
    }

    // if the url doesnt match the regex, assume its just a path to local resource.
    std::smatch match;
    if(std::regex_match(str, match, main_regex)) {
        Link ret;
        ret.protocol       = match[1];
        ret.login          = match[2];
        ret.password       = match[3];
        ret.host           = match[4];
        ret.resource_path  = match[5];
        ret.fragment       = match[6];
        ret.query          = match[7];
        return ret;
    } else if(std::regex_match(str, match, file_with_fragment_regex)) {
        Link ret;
        ret.resource_path = match[1];
        ret.fragment = match[2];
        return ret;
    } else {
        Link ret;
        ret.resource_path = str;
        return ret;
    }
}

std::string Link::to_string() {
    std::string temp;
    if(!protocol.empty()) {
        temp += protocol;
        temp += "://";
    }
    if(!login.empty()) {
        temp += login;
        if(!password.empty()) {
            temp += ":";
            temp += password;
        }
        temp += "@";
    }
    if(!host.empty()) {
        temp += host;
    }
    if(!port.empty()) {
        temp += ":";
        temp += port;
    }
    if(!resource_path.empty()) {
        temp += "/";
        temp += resource_path;
    }
    if(!query.empty()) {
        temp += "?";
        temp += query;
    }
    if(!fragment.empty()) {
        temp += "#";
        temp += fragment;
    }

    return temp;
}