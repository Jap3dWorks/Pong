//
// Created by Jordi on 5/30/2022.
//

#ifndef GL_TEST_TEXT_TEMPLATE_H
#define GL_TEST_TEXT_TEMPLATE_H

#include <iostream>
#include <string>
#include <regex>
#include <unordered_map>
#include <utility>
//#include <format>


// Very simple text parser using jinja2 syntax.
// https://jinja.palletsprojects.com/en/3.1.x/templates/

class TemplateValue;
using FormatValues = std::unordered_map<std::string, TemplateValue>;

class TemplateValue {
private:
    std::string _value;
    FormatValues _values;

public:
    TemplateValue() = default;

    TemplateValue(const std::string& value, const FormatValues& values) :
        _value(value), _values(values) {}

    TemplateValue(std::string &&value, FormatValues &&values) :
        _value(std::move(value)), _values(std::move(values)) {}

    explicit TemplateValue(const std::string& value):
            _value(value) {}

    explicit TemplateValue(std::string&& value) :
            _value(std::move(value)) {}

    ~TemplateValue() = default;

    TemplateValue(const TemplateValue& template_value) = default;

    TemplateValue(TemplateValue&& template_value) noexcept :
        _value(std::move(template_value._value)),
        _values(std::move(template_value._values)) {}

    TemplateValue& operator=(const TemplateValue& other) {
        if (this != &other) {
            _value = other._value;
            _values = other._values;
        }
        return *this;
    }

    TemplateValue& operator=(TemplateValue&& other)  noexcept {
        _value = std::move(other._value);
        _values = std::move(other._values);

        return *this;
    }

    TemplateValue &operator[](const std::string &key) {
        return _values[key];
    }

    [[nodiscard]] bool operator==(const TemplateValue &other) {
        return _value == other._value;
    }

public:
    [[nodiscard]] const std::string &to_string() const {
        return _value;
    }

};

template<>
struct std::hash <TemplateValue> {
    std::size_t operator()(const TemplateValue& template_value) const noexcept
    {
        return std::hash<std::string>()(template_value.to_string());
    }
};



template<class _elem, class _rxtraits, typename Lambda>
std::string inline regex_replace(std::string _string,
                                 const std::basic_regex<_elem, _rxtraits> &_regex,
                                 Lambda&& lambda,
                                 std::regex_constants::match_flag_type _flags = std::regex_constants::match_default)
{
    auto match = std::smatch();

    std::uint32_t offset = 0;
    auto itr = _string.cbegin();
    auto end = _string.cend();

    while(std::regex_search(itr, end, match, _regex, _flags)) {
        offset += match.position();
        auto replace_str = lambda(match);
        _string.replace(offset, match.length(), replace_str);
        offset += replace_str.length();

        itr = _string.cbegin() + offset;
        end = _string.cend();
    }

    return _string;
}

class TextTemplate {
private:
    std::string _template;
    FormatValues _data;

    std::regex _var_patt{R"(\{\{ (\w+)((?:\[\"(\w+)\"\])*) \}\})"};
    std::regex _chain_patt{R"(\[\"([^\[\]]+)\"\])"};


private:

    std::string inline _resolve(const std::string& text, FormatValues& data) {
        return regex_replace(
            text, _var_patt,
            [this](std::smatch &_m) -> std::string {
                auto _result = &_data[_m[1].str()];

                auto _bracket_chain = _m[2].str();
                auto _start = _bracket_chain.cbegin();
                auto _match = std::smatch();

                while (std::regex_search(_start,
                                         _bracket_chain.cend(),
                                         _match,
                                         _chain_patt)) {
                    _result = &_result->operator[](_match[1].str());
                    _start = _match.suffix().first;
                }
                return _result->to_string();
                }
            );
    }

public:
    TextTemplate()=default;
    ~TextTemplate()=default;

    TextTemplate(const TextTemplate& other) :
        _template(other._template), _data(other._data) {}

    TextTemplate(TextTemplate&& other) noexcept :
        _template(std::move(other._template)), _data(std::move(other._data)) {}

    explicit TextTemplate(std::string text_template) :
    _template(std::move(text_template)) {}

    TextTemplate(std::string&& text_template, FormatValues&& data) :
            _template(std::move(text_template)), _data(std::move(data)) {}


    TextTemplate& operator=(const TextTemplate& other) {
        if (this != &other) {
            _template = other._template;
            _data = other._data;
        }

        return *this;
    }

    TextTemplate& operator=(TextTemplate&& other) noexcept {
        _template = std::move(other._template);
        _data = std::move(other._data);

        return *this;
    }

public:
    void set_template_data(const FormatValues& data) {
        _data = data;
    }

    void set_template_data(FormatValues&& data) noexcept {
        _data = std::move(data);
    }

    void set_template(const std::string& text_template) {
        _template = text_template;
    }

    std::string render() {
        return _resolve(_template, _data);;
    }
};

#endif //GL_TEST_TEXT_TEMPLATE_H

