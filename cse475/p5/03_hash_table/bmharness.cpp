#include <sys/time.h>
#include <cstdlib>
#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <thread>
#include <atomic>
#include "bmconfig.hpp"

using std::string;
using std::cout;
using std::thread;

/**
 * Recall that the Config object is a static member, but since we declared it
 * in a header, we had to mark it extern.  This is where we provide "backing"
 * for it, so that it appears in a .o file and doesn't lead to link errors.
 */
Config Config::CFG;

/**
 * Print benchmark configuration and results (throughput)
 */
void dump_csv()
{
    // csv output
    std::cout << "csv"
              << ", R=" << Config::CFG.lookpct
              << ", d=" << Config::CFG.duration   << ", p=" << Config::CFG.threads
              << ", m=" << Config::CFG.elements
              << ", ops=" << Config::CFG.opcount << ", time=" << Config::CFG.totaltime
              << ", throughput="
              << (1000000000LL * Config::CFG.opcount) / (Config::CFG.totaltime)
              << std::endl;
}

/**
 *  Print usage
 */
void usage()
{
    std::cerr << "Usage: project5a [flags]\n";
    std::cerr << "    -d: number of seconds to time (default 1)\n";
    std::cerr << "    -p: number of threads (default 1)\n";
    std::cerr << "    -r: % lookup operations (remainder split ins/rmv)\n";
    std::cerr << "    -m: range of keys in data set\n";
    std::cerr << "    -h: print help (this message)\n\n";
    exit(1);
}

/**
 *  Parse command line arguments
 */
void parseargs(int argc, char** argv)
{
    // parse the command-line options
    int opt;
    while ((opt = getopt(argc, argv, "d:p:hm:r:")) != -1) {
        switch(opt) {
          case 'd': Config::CFG.duration      = strtol(optarg, NULL, 10); break;
          case 'p': Config::CFG.threads       = strtol(optarg, NULL, 10); break;
          case 'm': Config::CFG.elements      = strtol(optarg, NULL, 10); break;
          case 'r':
            Config::CFG.lookpct = strtol(optarg, NULL, 10);
            Config::CFG.inspct = (100 - Config::CFG.lookpct)/2 + strtol(optarg, NULL, 10);
            break;
          case 'h':
            usage();
        }
    }
}

/**
 * Signal handler to end a test
 */
extern "C" void catch_SIGALRM(int)
{
    Config::CFG.running = false;
}

/**
 *  Helper function for computing time in a cross-platform way
 */
long long getElapsedTime()
{
    struct timespec t;
    clock_gettime(CLOCK_REALTIME, &t);
    uint64_t tt = (((long long)t.tv_sec) * 1000000000L) + ((long long)t.tv_nsec);
    return tt;
}

// some integers that we'll use as barriers
std::atomic<uint32_t> b0(0);
std::atomic<uint32_t> b1(0);
std::atomic<uint32_t> b2(0);

/**
 * Run a timed experiment
 */
void run(uintptr_t id)
{
    // wait until all threads created, then set alarm and read timer
    b0.fetch_add(1); while (b0 != Config::CFG.threads) { std::this_thread::yield(); }

    if (id == 0) {
        signal(SIGALRM, catch_SIGALRM);
        alarm(Config::CFG.duration);
        Config::CFG.totaltime = getElapsedTime();
    }

    // wait until read of start timer finishes, then start transactions
    b1.fetch_add(1); while (b1 != Config::CFG.threads) { std::this_thread::yield(); }
    uint32_t count = 0;
    uint32_t seed = id;

    // run until alarm fires
    while (Config::CFG.running) {
        bench_test(id, &seed);
        ++count;
    }

    // wait until all operations finish, then get time
    b2.fetch_add(1); while (b2 != Config::CFG.threads) { std::this_thread::yield(); }
    if (id == 0)
        Config::CFG.totaltime = getElapsedTime() - Config::CFG.totaltime;

    // add this thread's count to an accumulator
    __sync_fetch_and_add(&Config::CFG.opcount, count);
}

/**
 *  Main routine... it's a bit ugly to hide main() inside this function, but
 *  it means we could easily switch to a different data structure by
 *  replacing ListBench.cpp, and main() wouldn't change at all.
 */
int main(int argc, char** argv)
{
    // parse the command line
    parseargs(argc, argv);

    // initialize the benchmark
    bench_init();

    // for tracking our threads
    std::thread threads[256];

    // launch the threads
    for (uint32_t j = 1; j < Config::CFG.threads; j++)
        threads[j] = thread(run, j);

    // all of the other threads should be waiting to run the benchmark, but
    // they can't until this thread starts the benchmark too...
    run(0);

    // everyone should be done.  Join all threads so we don't leave anything
    // hanging around
    for (uint32_t k = 1; k < Config::CFG.threads; k++)
        threads[k].join();

    // make sure the hash table is in a valid state
    bool v = bench_verify();
    std::cout << "Verification: " << (v ? "Passed" : "Failed") << "\n";

    // dump the results
    dump_csv();
}
