//
// Created by Jordi on 4/23/2022.
//
#include <iostream>
#include "text_template.h"
#include "Pong/logger.h"
#include <string>
#include <regex>


//template<typename T>
std::string replace(const std::smatch& t) {
    return "adios";
}

int main() {
    auto _string = std::string(R"({ pepe } { pepe["hola"]["que"]["pasa"] }  )");

    auto tmp = TextTemplate(_string);

    FormatValues val = {
            {"pepe", {"paco",
                  {{"hola", {"winchirl",
                         {{"que", {"jeje",
                               {{"pasa", {"aquirl!", {}}}
                               }}
                         }}
                  }}}
                }
            }
    };

    tmp.set_template_data(val);
    LOG_INFO(tmp.render());

    return 0;

}