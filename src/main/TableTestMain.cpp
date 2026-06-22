#include "Containers/Table.h"
#include "MM/Mem.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <string>

struct TestConfig
{
    std::string what;
    int count;
    int buckets;
    int speedMs;

    TestConfig()
        : what("all"), count(1000000), buckets(200003), speedMs(1000)
    {}
};

struct TestResult
{
    std::string name;
    bool ok;
    long ms;
    long checks;

    explicit TestResult(const std::string& n)
        : name(n), ok(true), ms(0), checks(0)
    {}
};

static TestConfig readConfig(int argc, char** argv)
{
    TestConfig cfg;

    if (argc > 1) cfg.what     = argv[1];
    if (argc > 2) cfg.count    = std::atoi(argv[2]);
    if (argc > 3) cfg.buckets  = std::atoi(argv[3]);
    if (argc > 4) cfg.speedMs  = std::atoi(argv[4]);

    if (cfg.count    <= 0) cfg.count    = 1;
    if (cfg.buckets  <= 0) cfg.buckets  = 1;
    if (cfg.speedMs  <= 0) cfg.speedMs  = 1;

    return cfg;
}

class TableTester
{
public:
    explicit TableTester(const TestConfig& cfg)
        : config(cfg), passed(0), total(0)
    {}

    int run()
    {
        std::printf("Table tests\n");
        std::printf("test: %s\n",      config.what.c_str());
        std::printf("count: %d\n",     config.count);
        std::printf("buckets: %d\n",   config.buckets);
        std::printf("speed time: %d ms\n\n", config.speedMs);

        if (isTest("all") || isTest("basic"))    runBasic();
        if (isTest("all") || isTest("strings"))  runStrings();
        if (isTest("all") || isTest("iterator")) runIterator();
        if (isTest("all") || isTest("remove"))   runRemoveByIterator();
        if (isTest("all") || isTest("stress"))   runStress();
        if (isTest("all") || isTest("speed"))    runSpeed();
        if (isTest("million"))                   runMillionBuckets();

        std::printf("\nReview: %d of %d passed\n", passed, total);
        return (passed == total) ? 0 : 1;
    }

private:
    TestConfig config;
    int passed;
    int total;

    bool isTest(const char* name) const
    {
        return config.what == name;
    }

    long elapsedMs(clock_t start) const
    {
        return static_cast<long>((clock() - start) * 1000 / CLOCKS_PER_SEC);
    }

    void addResult(const TestResult& res)
    {
        total++;
        if (res.ok)
            passed++;

        std::printf("[%s] %s, time: %ld ms, checks: %ld\n",
            res.name.c_str(),
            res.ok ? "OK" : "FAIL",
            res.ms,
            res.checks);
    }

    void runBasic()
    {
        Mem mem(64 * 1024 * 1024);
        Table table(mem, 17);

        TestResult res("basic");
        clock_t start = clock();

        if (!table.empty() || table.size() != 0)
            res.ok = false;
        res.checks++;

        KV item{ 10, 100 };
        if (table.insert_by_key(&item, sizeof(item)) != 0)
            res.ok = false;
        res.checks++;

        KV sameKey{ 10, 200 };
        if (table.insert_by_key(&sameKey, sizeof(sameKey)) == 0)
            res.ok = false;
        res.checks++;

        size_t size = 0;
        auto* got = static_cast<KV*>(table.at(&item.key, sizeof(item.key), size));
        if (!got || size != sizeof(KV) || got->key != 10 || got->value != 100)
            res.ok = false;
        res.checks++;

        auto* it = table.find(&item, sizeof(item));
        if (!it)
            res.ok = false;
        delete it;
        res.checks++;

        KV missing{ 77, 0 };
        it = table.findByKey(&missing.key, sizeof(missing.key));
        if (it)
            res.ok = false;
        delete it;
        res.checks++;

        table.clear();
        if (!table.empty() || table.size() != 0)
            res.ok = false;
        res.checks++;

        res.ms = elapsedMs(start);
        addResult(res);
    }

    void runStrings()
    {
        Mem mem(64 * 1024 * 1024);
        Table table(mem, 17);

        TestResult res("strings");
        clock_t start = clock();

        KV_str one{ "alpha", "first" };
        KV_str duplicate{ "alpha", "second" };

        if (table.insert_string(&one, sizeof(one)) != 0)
            res.ok = false;
        res.checks++;

        if (table.insert_string(&duplicate, sizeof(duplicate)) == 0)
            res.ok = false;
        res.checks++;

        size_t size = 0;
        auto* got = static_cast<KV_str*>(
            table.at(const_cast<char*>(one.key), std::strlen(one.key) + 1, size));

        if (!got || size != sizeof(KV_str))
            res.ok = false;
        else if (std::strcmp(got->key, "alpha") != 0 || std::strcmp(got->value, "first") != 0)
            res.ok = false;
        res.checks++;

        res.ms = elapsedMs(start);
        addResult(res);
    }

    void runIterator()
    {
        Mem mem(64 * 1024 * 1024);
        Table table(mem, 97);

        TestResult res("iterator");
        clock_t start = clock();

        int n = (config.count > 50000) ? 50000 : config.count;

        for (int i = 0; i < n; i++)
        {
            KV item{ i, i * 10 };
            if (table.insert_by_key(&item, sizeof(item)) != 0)
                res.ok = false;
        }

        int found = 0;
        auto* it = table.newIterator();
        while (it)
        {
            size_t size = 0;
            auto* got = static_cast<KV*>(it->getElement(size));
            if (!got || size != sizeof(KV))
                res.ok = false;

            found++;
            if (!it->hasNext())
                break;

            it->goToNext();
        }
        delete it;

        if (found != n)
            res.ok = false;

        res.checks = found;
        res.ms = elapsedMs(start);
        addResult(res);
    }

    void runRemoveByIterator()
    {
        Mem mem(64 * 1024 * 1024);
        Table table(mem, 31);

        TestResult res("remove iterator");
        clock_t start = clock();

        const int n = 1000;
        for (int i = 0; i < n; i++)
        {
            KV item{ i, i * 2 };
            if (table.insert_by_key(&item, sizeof(item)) != 0)
                res.ok = false;
        }

        int removed = 0;
        auto* it = table.newIterator();
        while (it && removed < n / 2)
        {
            table.remove(it);
            removed++;

            size_t size = 0;
            if (!it->getElement(size) && !it->hasNext())
                break;
        }
        delete it;

        if (table.size() != n - removed)
            res.ok = false;

        res.checks = removed;
        res.ms = elapsedMs(start);
        addResult(res);
    }

    void runStress()
    {
        Mem mem(512 * 1024 * 1024);
        Table table(mem, config.buckets);

        TestResult res("stress");
        clock_t start = clock();

        int* keys   = new int[config.count];
        int* values = new int[config.count];

        for (int i = 0; i < config.count; i++)
        {
            keys[i]   = i;
            values[i] = i * 3 + 17;
        }

        std::srand(1);
        for (int i = config.count - 1; i > 0; i--)
        {
            int j    = std::rand() % (i + 1);
            int tmp  = keys[i];
            keys[i]  = keys[j];
            keys[j]  = tmp;
        }

        for (int i = 0; i < config.count; i++)
        {
            KV item{ keys[i], values[keys[i]] };
            if (table.insert_by_key(&item, sizeof(item)) != 0)
            {
                res.ok = false;
                break;
            }
        }
        res.checks += config.count;

        if (table.size() != config.count)
            res.ok = false;

        int sample = (config.count < 1000) ? config.count : 1000;
        for (int i = 0; i < sample; i++)
        {
            int pos = (i * 7919) % config.count;
            auto* it = table.findByKey(&keys[pos], sizeof(keys[pos]));
            if (!it)
            {
                res.ok = false;
                break;
            }

            size_t size = 0;
            auto* got = static_cast<KV*>(it->getElement(size));
            if (!got || size != sizeof(KV) || got->key != keys[pos])
                res.ok = false;

            delete it;
            res.checks++;
        }

        int removed = 0;
        for (int i = 0; i < config.count; i += 4)
        {
            table.removeByKey(&keys[i], sizeof(keys[i]));
            removed++;
        }

        if (table.size() != config.count - removed)
            res.ok = false;

        int removedSample = (removed < 1000) ? removed : 1000;
        for (int i = 0; i < removedSample; i++)
        {
            int pos  = i * 4;
            auto* it = table.findByKey(&keys[pos], sizeof(keys[pos]));
            if (it)
                res.ok = false;
            delete it;
            res.checks++;
        }

        table.clear();
        if (!table.empty() || table.size() != 0)
            res.ok = false;

        delete[] keys;
        delete[] values;

        res.ms = elapsedMs(start);
        addResult(res);
    }

    void runSpeed()
    {
        TestResult res("speed");
        clock_t start = clock();

        int loops = 0;
        int n = (config.count > 2000) ? 2000 : config.count;

        while (elapsedMs(start) < config.speedMs)
        {
            Mem mem(64 * 1024 * 1024);
            Table table(mem, 4099);

            for (int i = 0; i < n; i++)
            {
                KV item{ i, i + 1 };
                if (table.insert_by_key(&item, sizeof(item)) != 0)
                    res.ok = false;
            }

            for (int i = 0; i < n; i += 7)
            {
                auto* it = table.findByKey(&i, sizeof(i));
                if (!it)
                    res.ok = false;
                delete it;
            }

            for (int i = 0; i < n; i += 5)
                table.removeByKey(&i, sizeof(i));

            loops++;
        }

        res.checks = loops;
        res.ms = elapsedMs(start);
        addResult(res);
    }

    void runMillionBuckets()
    {
        const int n = config.count;
        const size_t initialBuckets = static_cast<size_t>(config.buckets);

        Mem mem(1024 * 1024 * 1024);
        Table table(mem, initialBuckets);

        TestResult res("million buckets");
        clock_t start = clock();

        std::printf("  initial buckets: %zu\n", initialBuckets);
        std::printf("  elements: %d\n", n);
        std::printf("  load factor limit: 0.75 (auto rehash)\n\n");

        int* keys = new int[n];
        int* values = new int[n];

        for (int i = 0; i < n; i++)
        {
            keys[i] = i;
            values[i] = i * 3 + 17;
        }

        std::srand(42);
        for (int i = n - 1; i > 0; i--)
        {
            int j = std::rand() % (i + 1);
            int tmp = keys[i];
            keys[i] = keys[j];
            keys[j] = tmp;
        }

        clock_t insertStart = clock();
        for (int i = 0; i < n; i++)
        {
            KV item{ keys[i], values[keys[i]] };
            if (table.insert_by_key(&item, sizeof(item)) != 0)
            {
                res.ok = false;
                break;
            }
        }
        long insertMs = elapsedMs(insertStart);
        res.checks += n;

        const size_t finalBuckets = table.bucketCount();
        const double loadFactor = static_cast<double>(table.size()) / static_cast<double>(finalBuckets);

        std::printf("  after insert: size=%d, buckets=%zu, load=%.3f, insert time=%ld ms\n",
            table.size(), finalBuckets, loadFactor, insertMs);

        if (table.size() != n)
            res.ok = false;

        if (n > static_cast<int>(initialBuckets) && finalBuckets <= initialBuckets)
            res.ok = false;

        clock_t searchStart = clock();
        for (int i = 0; i < n; i++)
        {
            auto* it = table.findByKey(&keys[i], sizeof(keys[i]));
            if (!it)
            {
                res.ok = false;
                break;
            }

            size_t size = 0;
            auto* got = static_cast<KV*>(it->getElement(size));
            if (!got || size != sizeof(KV) || got->key != keys[i] || got->value != values[keys[i]])
                res.ok = false;

            delete it;
            res.checks++;
        }
        long searchMs = elapsedMs(searchStart);

        std::printf("  search: %d keys, time=%ld ms\n", n, searchMs);

        delete[] keys;
        delete[] values;

        res.ms = elapsedMs(start);
        addResult(res);
    }
};

int main(int argc, char** argv)
{
    TableTester tester(readConfig(argc, argv));
    return tester.run();
}