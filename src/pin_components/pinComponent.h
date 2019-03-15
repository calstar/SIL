#pragma once
#include "common.h"

class PinComponent {
    protected:
        bool isActivated;

    public:
        string name;

        PinComponent(string name);

        virtual void activate(int64_t time) = 0;
        virtual void deactivate(int64_t time);
        bool activated();
};