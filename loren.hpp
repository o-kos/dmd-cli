#pragma once

#include <string>

std::wstring loren() {
    static const wchar_t *lorens[] = {
        L"Lorem ipsum dolor sit amet, consectetur adipiscing elit.",
        L"Nunc at metus et dui fringilla elementum.",
        L"Sed pretium sem nec tellus congue porta.",
        L"Aenean sit amet tellus ultricies, consectetur nunc eu, aliquam lectus.",
        L"Praesent euismod urna pharetra sapien porttitor, in malesuada diam ornare.",
        L"Duis eu erat pellentesque, ornare mauris eu, eleifend risus.",
        L"",
        L"Proin nec metus non elit tempor sodales.",
        L"In vel metus aliquet urna aliquam vestibulum.",
        L"Aliquam vitae odio tristique, vehicula nisl et, tempus ex.",
        L"Praesent ornare neque in scelerisque faucibus.",
        L"",
        L"Proin sagittis risus eu odio lacinia, id vestibulum est ultricies.",
        L"Proin vel urna consectetur, rhoncus felis a, sodales felis.",
        L"Quisque feugiat libero et libero ultricies malesuada.",
        L"",
        L"Phasellus laoreet sapien in maximus dignissim.",
        L"Praesent nec odio vehicula, laoreet massa ac, mollis erat.",
        L"",
        L"Vivamus eu neque elementum, sagittis nunc sed, volutpat mi.",
        L"Integer aliquet neque eu vehicula rhoncus.",
        L"Sed in justo sit amet turpis euismod condimentum.",
        L"Pellentesque in ex sit amet lacus laoreet ultricies.",
        L"Ut imperdiet metus eget neque laoreet mattis.",
        L"Donec consectetur nulla eu volutpat facilisis."
    };
    static size_t idx = 0;
    constexpr size_t lcount = sizeof(lorens) / sizeof(*lorens);
    return lorens[idx++ % (lcount)];
}


