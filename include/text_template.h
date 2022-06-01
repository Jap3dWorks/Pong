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
#include <format>


// Very simple text parser using jinja2 syntax.
// https://jinja.palletsprojects.com/en/3.1.x/templates/

class TemplateValue {
private:
    using TemplateValueMap = std::unordered_map<std::string, TemplateValue>;
    std::string _value;
    TemplateValueMap _values;

public:
    TemplateValue()=default;
    explicit TemplateValue(const std::string& value): _value(value) {}
    TemplateValue(std::string&& value): _value(std::move(value)) {}

    ~TemplateValue()=default;

    TemplateValue& operator[](const std::string& key) {
        return _values[key];
    }

    bool operator==(const TemplateValue& other) {
        return _value == other._value;
    }

     [[nodiscard]] const std::string& to_string() const {
        return _value;
    }

};

template<>
struct std::hash <TemplateValue> {
    std::size_t operator()(const TemplateValue& template_value)
    {
        return hash<std::string>()(template_value.to_string());
    }
};


using FormatValues = std::unordered_map<std::string, TemplateValue>;

class TextTemplate {
private:
    std::string _template;
    FormatValues _data;
    std::string _variable_patt_format{R"(\{{ {} \}})"};

    std::regex _bracket_patt{R"(\{ (\w+)\[\"(\w+)\"\]) \})"};
    std::regex _var_patt{R"(\{ (\w+) \})"};

private:

    std::string _resolve(const std::string& key, FormatValues& data) {
        std::smatch base_match;
        std::string result = key;
        while (true) {
            if (std::regex_search(result, base_match, _bracket_patt)) {
                // replace match by
                // TODO: replace result with new data
                // key[base_match.position(), base_match.end()]
                data[base_match[1]][base_match[2]].to_string();
//            _resolve()
            }

            else if (std::regex_search(result, base_match, _var_patt)) {
                // replace result with new data

                data[base_match[1]].to_string();
            }

            else {
                break;
            }
        }


        return result;


//        if ("[]" in key) {
//            std::string result;
//            FormatValues subdata;
//            for (auto& section: sections){
//                subdata = subdata[section];
//            }
//            return subdata.to_string;
//        }
//        else {
//            return data.to_string();
//        }
    }

public:

    explicit TextTemplate(std::string text_template) :
    _template(std::move(text_template)) {}

    void set_template_data(const FormatValues& data) {
        _data = data;
    }

    void set_template(const std::string& text_template) {
        _template = text_template;
    }

    [[nodiscard]] inline std::string format_var(const std::string &var_name) {
        return std::vformat(_variable_patt, std::make_format_args(var_name));
    }

    std::string render() {
        std::string result = _template;
        for (auto &patt_val: _data) {
            result = std::regex_replace(
                    result,
                    std::regex(format_var(patt_val.first)),
                    patt_val.second.to_string());
        }

        return result;
    }
};

#endif //GL_TEST_TEXT_TEMPLATE_H
