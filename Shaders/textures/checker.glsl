#ifndef CHECKER_GLSL
#define CHEKCER_GLSL

float checkers_grab_box(in vec2 point, in vec2 dpdx, in vec2 dpdy) {
    //    float col_val = pow(mod(floor(p.x), 2) - mod(floor(p.y), 2), 2);
    //    return col_val;

    // filter kernel
    vec2 w = abs(dpdx) + abs(dpdy) + 0.001;
    // analytical integral (box_filter)
    vec2 i = 2.0*(
    // xor pattern
    abs(fract((point - 0.5*w) * 0.5) - 0.5) - abs(fract((point + 0.5 * w) * 0.5) - 0.5)
    ) / w;

    return 0.5 - 0.5 * i.x * i.y;
}

#endif CHECKER_GLSL
