#include <bits/stdc++.h>
#include <benchmark/benchmark.h>

using namespace std;

void update(atomic<int> &a) {
  for (int i = 0; i < 3000000; ++i) {
    ++a;
  }
}

struct alignas(16) padded_atomic_int {
  atomic<int> value;
  padded_atomic_int() { value = 0; }
};

void multi_thread_padding() {
  padded_atomic_int a{};
  padded_atomic_int b{};

  // Assign each thread one aligned atomic int type
  thread t1([&]() { update(a.value); });
  thread t2([&]() { update(b.value); });

  // Join threads
  t1.join();
  t2.join();
}

void single_thread() {
  atomic<int> a, b;
  a = b = 0;

  // Update 2 variables sequentially
  update(a);
  update(b);
}

void false_sharing() {
  // Create a single atomic integer
  atomic<int> a, b;
  a = b = 0;

  // Assign each atomic<int> to each thread
  std::thread t1([&]() { update(a); });
  std::thread t2([&]() { update(b); });

  // Join the 2 threads
  t1.join();
  t2.join();
}

void print() {
  atomic<int> a, b;

  // Return addresses of atomic variables
  cout << "Address of a is: " << &a << '\n';
  cout << "Address of b is: " << &b << '\n';
}

static void single_thread_benchmark(benchmark::State& s) {
  while (s.KeepRunning()) single_thread();
}

static void false_sharing_benchmark(benchmark::State& s) {
  while (s.KeepRunning()) false_sharing();
}
static void multi_thread_padding_benchmark(benchmark::State& s) {
  while (s.KeepRunning()) multi_thread_padding();
}

BENCHMARK(single_thread_benchmark)->UseRealTime()->Unit(benchmark::kMillisecond);
BENCHMARK(false_sharing_benchmark)->UseRealTime()->Unit(benchmark::kMillisecond);
BENCHMARK(multi_thread_padding_benchmark)->UseRealTime()->Unit(benchmark::kMillisecond);

BENCHMARK_MAIN();
