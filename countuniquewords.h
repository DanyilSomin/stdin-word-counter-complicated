#ifndef COUNT_UNIQUE_WORDS_H
#define COUNT_UNIQUE_WORDS_H

#include <unordered_set>
#include <mutex>

#include "concurrentqueue.h"

constexpr const size_t gBufferSize{ 2048 };

size_t countUniqueCinWordsThreads(size_t threads);

#endif