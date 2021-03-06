#pragma once

#include "common/threads/mutex.h"

namespace love::thread
{
    class Lock
    {
        public:
            Lock(Mutex & mutex);
            Lock(Mutex * mutex);

            Lock(Lock && other);

            ~Lock();

        private:
            Mutex * mutex;
    };
}
