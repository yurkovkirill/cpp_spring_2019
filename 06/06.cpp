#include <iostream>
#include <fstream>
#include <condition_variable>
#include <thread>
#include <mutex>
#include <atomic>
#include <queue>
#include <string>

constexpr auto data = "file.bin";
constexpr size_t memLim = 8 * 1024 * 1024;
constexpr int threadNum = 2;
constexpr size_t threadMemLim = memLim / threadNum;
constexpr size_t threadMemLimCount = threadMemLim / sizeof(uint64_t);

std::condition_variable cond;

std::mutex finishM;
std::mutex queueM;
std::mutex readM;
std::mutex splitM;

std::atomic<int> loadC;
std::atomic<int> sortC;

void merge(std::string& str1, std::string& str2, uint64_t* buf, int id, int it, int fileC, std::queue<std::string>& outputFiles)
{
    std::ifstream f1(str1, std::ios::binary);
    std::ifstream f2(str2, std::ios::binary);
    std::string name = std::to_string(it) + "_" + std::to_string(id) + "_" + std::to_string(fileC) + ".bin";
    std::ofstream out(name, std::ios::binary);

    size_t lim = threadMemLimCount / 4;
    uint64_t* bufL = buf;
    uint64_t* bufR = buf + lim;
    uint64_t* bufM = buf + 2 * lim;

    f1.read(reinterpret_cast<char*>(bufL), lim * sizeof(uint64_t));
    size_t redL = f1.gcount() / sizeof(uint64_t);
    f2.read(reinterpret_cast<char*>(bufR), lim * sizeof(uint64_t));
    size_t redR = f2.gcount() / sizeof(uint64_t);

    size_t limM = 2 * lim;
    size_t l = 0;
    size_t m = 0;
    size_t r = 0;

    while (!f1.eof() || !f2.eof() || l < redL || r < redR) {
        if (l == redL && !f1.eof()) {
            f1.read(reinterpret_cast<char*>(bufL), lim * sizeof(uint64_t));
            redL = f1.gcount() / sizeof(uint64_t);
            l = 0;
        }
        if (r == redR && !f2.eof()) {
            f2.read(reinterpret_cast<char*>(bufR), lim * sizeof(uint64_t));
            redR = f2.gcount() / sizeof(uint64_t);
            r = 0;
        }
        if (m == limM) {
            out.write(reinterpret_cast<char*>(bufM), m * sizeof(uint64_t));
            m = 0;
        }

        if (l < redL && r < redR) {
            if (bufR[r] < bufL[l]) {
                bufM[m] = bufR[r];
                ++m;
                ++r;
            }
            else {
                bufM[m] = bufL[l];
                ++l;
                ++m;
            }
        }
        else {
            if (l == redL && r < redR) {
                bufM[m] = bufR[r];
                ++m;
                ++r;
            }
            else {
                if (r == redR && l < redL) {
                    bufM[m] = bufL[l];
                    ++l;
                    ++m;
                }
            }
        }
    }

    out.write(reinterpret_cast<char*>(bufM), m * sizeof(uint64_t));
    std::unique_lock<std::mutex> lock(queueM);
    outputFiles.push(name);
}

void split(uint64_t* locBuf, std::ifstream& dataStream, int id, std::queue<std::string>& outputFiles)
{
    int it = 0;
    int fileC = 0;
    while (!dataStream.eof()) {
        std::unique_lock<std::mutex> localLock(readM);
        dataStream.read(reinterpret_cast<char*>(locBuf), threadMemLimCount * sizeof(uint64_t));
        std::streamsize curSize = dataStream.gcount() / sizeof(uint64_t);
        localLock.unlock();
        if (curSize != 0) {
            std::string name = std::to_string(it) + "_" + std::to_string(id) + "_" + std::to_string(fileC) + ".bin";
            std::ofstream out(name, std::ios::binary);
            out.write(reinterpret_cast<char*>(locBuf), curSize * sizeof(uint64_t));
            outputFiles.push(name);
            ++fileC;
        }
    }
}

void MTMSort(uint64_t* buf, std::ifstream& dataStream, int id, std::queue<std::string>& outputFiles)
{
    uint64_t* locBuf = buf + id * threadMemLimCount;
    int it = 0;
    int fileC = 0;
    {
        std::unique_lock<std::mutex> lockS(splitM);

        split(locBuf, dataStream, id, outputFiles);

        loadC++;

        cond.notify_all();
        while (loadC != threadNum) { //wake up protect
            cond.wait(lockS);
        }
    }

    while (outputFiles.size() >= 2) {
        queueM.lock();
        if (outputFiles.size() >= 2) {
            it++;
            std::string tmp1 = outputFiles.front();
            outputFiles.pop();
            std::string tmp2 = outputFiles.front();
            outputFiles.pop();
            merge(tmp1, tmp2, locBuf, id, it, fileC, outputFiles);
            fileC++;
        }
        queueM.unlock();
    }
    sortC++;
}

int main()
{
    uint64_t* buf = new uint64_t[memLim / sizeof(uint64_t)];
    std::ifstream dataStream(data, std::ios::binary);
    std::queue<std::string> outputFiles;
    std::vector<std::thread> threads;
    loadC = 0;
	sortC = 0;

    for (int i = 0; i < threadNum; ++i) {
        threads.emplace_back(MTMSort, std::ref(buf), std::ref(dataStream), i, std::ref(outputFiles));
    }
    for (int i = 0; i < threadNum; ++i) {
        threads[i].join();
    }

    if (sortC == threadNum) {
        if (outputFiles.empty()) {
            std::cerr << "error, no output files" << std::endl;
        }
        else
            std::cout << "finished, result in file: " << outputFiles.front() << std::endl;
    }

    delete[] buf;
    return 0;
}