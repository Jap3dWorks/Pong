//
// Created by Jordi on 4/23/2022.
//
#include <iostream>
#include "text_template.h"
#include "Pong/logger.h"
#include <string>
#include <regex>


int main() {

    auto replace_text = std::string("hola { name }, que tal?");
    auto text_template = TextTemplate(replace_text);

    text_template.set_template_data(
            {
                    {"name", {"Paco"}}
            });

    LOG_INFO(text_template.format_var("pepe"));

    LOG_INFO(text_template.render());

    return 0;

}