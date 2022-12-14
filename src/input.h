#pragma once
#include<iostream>
#include <string>
#include <chrono>
#include <future>
#include <stdio.h>
#include <stdio.h>
#include <string.h>

#ifdef __linux__ 
//linux code goes here
#include "kbhit.h"
#include <unistd.h>
#elif _WIN32
#include <conio.h>
// windows code goes here
#else

#endif


//int key_press() {
//    system("stty raw");
//    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
//    std::chrono::steady_clock::time_point end = now + std::chrono::milliseconds(200);
//    char c = -1, d = -1;
//
//    while (end > now) {
//        d = getchar();
//        if (d == 'a' || d == 'x')
//            c = d;
//        std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
//    }
//
//    system("stty cooked");
//    return c;
//}


int Input() {
    if (kbhit()) return getchar();
    else return -1;
}