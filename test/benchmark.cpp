
#include "../ptrlist.h"
#include "vptrlist.h"
#include <chrono>

class ElapsedTimer
{
public:
    typedef std::chrono::nanoseconds TimeT;
    ElapsedTimer() {}
    void start()
    {
        recent = std::chrono::high_resolution_clock::now();
    }
    void restart()
    {
        recent = std::chrono::high_resolution_clock::now();
    }
    int64_t elapsed()
    {
        using std::chrono::nanoseconds;
        using std::chrono::duration_cast;
        return duration_cast<nanoseconds>(std::chrono::high_resolution_clock::now() - recent).count();
    }
    std::chrono::high_resolution_clock::time_point recent;
};

#define BENCHMARK(taskName, expression)\
{\
    ElapsedTimer clock;\
    clock.start();\
    {expression}\
    int64_t done = clock.elapsed();\
    printf("=== [%lli] nanoseconds == %s \n", static_cast<long long>(done), (taskName));\
    fflush(stdout);\
}


int main()
{
    VPtrList<int>   xxx;
    PtrList<int>    yyy;
    int count = 10000000;

    printf("Test with %d entries sizes\n", count);
    fflush(stdout);

    BENCHMARK("SH - adding", {
                for(int i=0; i < count; i++)
                    xxx.push_back(i);
                  });
    BENCHMARK("MY - adding", {
                for(int i=0; i < count; i++)
                    yyy.push_back(i);
                  });

    printf("\n");
    fflush(stdout);

    BENCHMARK("SH - deleting-from-middle", {
                xxx.erase(xxx.begin()+500);
                  });
    BENCHMARK("MY - deleting-from-middle", {
                yyy.erase(yyy.begin()+500);
                  });

    printf("\n");
    fflush(stdout);

    BENCHMARK("SH - contains", {
                unsigned long long sum = 0;
                if(xxx.contains(24562))
                    sum += 1;
                if(xxx.contains(34524562))
                    sum += 1;
                  });

    BENCHMARK("MY - contains", {
                unsigned long long sum = 0;
                if(yyy.contains(24562))
                    sum += 1;
                if(yyy.contains(34524562))
                    sum += 1;
                  });

    printf("\n");
    fflush(stdout);

    BENCHMARK("SH - iteration", {
                  unsigned long long sum = 0;
                    for(VPtrList<int>::iterator i = xxx.begin();
                        i != xxx.end();
                        i++)
                        sum += (**i);
                  });
    BENCHMARK("MY - iteration", {
                  unsigned long long sum = 0;
                    for(PtrList<int>::iterator i = yyy.begin();
                        i != yyy.end();
                        i++)
                        sum += (*i);
                  });

    printf("\n");
    fflush(stdout);

    BENCHMARK("SH - cleanup", {
                    xxx.clear();
                  });
    BENCHMARK("MY - cleanup", {
                    yyy.clear();
                  });
    printf("\n");
    fflush(stdout);

    return 0;
}
