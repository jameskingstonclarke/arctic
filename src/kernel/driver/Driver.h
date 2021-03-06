#pragma once

#include "../utils/HashMap.h"

namespace Driver{
    class Driver{
        public:
        Driver();
        ~Driver();
        virtual void init();
        virtual void close();
        private:
        static Utils::HashMap<const char *> drivers;
    };
}