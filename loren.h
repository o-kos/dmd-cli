#pragma once

#include <string>

std::string nextLoren() {
    static const char *lorens[] = {
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit.",
        "Nunc at metus et dui fringilla elementum.",
        "Sed pretium sem nec tellus congue porta.",
        "Aenean sit amet tellus ultricies, consectetur nunc eu, aliquam lectus.",
        "Praesent euismod urna pharetra sapien porttitor, in malesuada diam ornare.",
        "Duis eu erat pellentesque, ornare mauris eu, eleifend risus.",
        "",
        "Proin nec metus non elit tempor sodales.",
        "In vel metus aliquet urna aliquam vestibulum.",
        "Aliquam vitae odio tristique, vehicula nisl et, tempus ex.",
        "Praesent ornare neque in scelerisque faucibus.",
        "",
        "Proin sagittis risus eu odio lacinia, id vestibulum est ultricies.",
        "Proin vel urna consectetur, rhoncus felis a, sodales felis.",
        "Quisque feugiat libero et libero ultricies malesuada.",
        "",
        "Phasellus laoreet sapien in maximus dignissim.",
        "Praesent nec odio vehicula, laoreet massa ac, mollis erat.",
        "",
        "Vivamus eu neque elementum, sagittis nunc sed, volutpat mi.",
        "Integer aliquet neque eu vehicula rhoncus.",
        "Sed in justo sit amet turpis euismod condimentum.",
        "Pellentesque in ex sit amet lacus laoreet ultricies.",
        "Ut imperdiet metus eget neque laoreet mattis.",
        "Donec consectetur nulla eu volutpat facilisis."
    };
    static size_t idx = 0;
    constexpr size_t lcount = sizeof(lorens) / sizeof(*lorens);
    return lorens[idx++ % (lcount)];
}


