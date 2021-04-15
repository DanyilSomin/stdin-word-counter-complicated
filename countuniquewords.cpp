#include "countuniquewords.h"

#include <iostream>
#include <cctype>
#include <thread>

#include "readhelpers.h"

void countUniqueWordsTask(std::unordered_set<std::string>& uniqueWords,
    std::mutex& uniqueWordsMutex,
    ConcurrentQueue<char*>& readQueue,
    ConcurrentQueue<char*>& processedQueue);

size_t countUniqueCinWordsThreads(size_t threadsAmount)
{
    ConcurrentQueue<char*> readyBuffers;
    ConcurrentQueue<char*> emptyBuffers;

    std::unordered_set<std::string> uniqueWords;
    std::mutex uniqueWordsMutex;

    std::vector<std::thread> threads;

    for (size_t i = 0; i < threadsAmount; ++i)
    {
        emptyBuffers.push(new char[gBufferSize]);

        threads.push_back(std::thread {
            countUniqueWordsTask,
            std::ref(uniqueWords),
            std::ref(uniqueWordsMutex),
            std::ref(readyBuffers),
            std::ref(emptyBuffers) });
    }

    emptyBuffers.push(new char[gBufferSize]);

    do
    {
        char* buffer;
        emptyBuffers.wpop(buffer);

        readWordsFromCin(buffer, gBufferSize);

        readyBuffers.push(buffer);
    } 
    while (canReadMore());

    // stop wait new buffers
    for (auto& t : threads) readyBuffers.push(nullptr); 

    for (auto& t : threads) t.join();

    for (size_t i = 0; i < threadsAmount; ++i)
    {
        char* ch;
        emptyBuffers.wpop(ch);
        delete[] ch;
    }

    return uniqueWords.size();
}


void countUniqueWordsTask(std::unordered_set<std::string>& uniqueWords,
    std::mutex& uniqueWordsMutex,
    ConcurrentQueue<char*>& readQueue,
    ConcurrentQueue<char*>& processedQueue)
{    
    std::unordered_set<std::string> tmpUniqueWords;

    char* buffer;

    while (true)
    {
        readQueue.wpop(buffer);

        if (!buffer) break;
       
        std::string_view str{ buffer };

        char* curCh = buffer;

        std::string word;

        while (*curCh != '\0')
        {
            while (islower(*curCh))
            {
                word.push_back(*curCh);
                ++curCh;
            }

            if (word.size())
            {
                tmpUniqueWords.insert(word);
            }

            word.clear();

            ++curCh;
        }

        uniqueWordsMutex.lock();
        uniqueWords.insert(tmpUniqueWords.begin(), tmpUniqueWords.end());
        uniqueWordsMutex.unlock();

        processedQueue.push(buffer);
    }
}