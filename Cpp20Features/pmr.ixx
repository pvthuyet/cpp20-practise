module;

// Type your code here, or load an example.
#include <iostream>
#include <memory_resource>
#include <vector>

export module Fibo.Pmr;

struct Foo {
    int x{ 0 };
    int y{ 0 };

    Foo() noexcept {
        std::cout << "default ctor\n";
    }

    explicit Foo(int v) noexcept : x{ v }, y{ v } {
        std::cout << "ctor with argument: " << v << std::endl;
    }

    Foo(Foo const& o) : x{ o.x }, y{ o.y } {
        std::cout << "copy ctor\n";
    }
    Foo& operator=(Foo const& o) {
        std::cout << "copy operator\n";
        Foo tmp(o);
        *this = std::move(tmp);
        return *this;
    }

    Foo(Foo&& o) noexcept : 
        x{ std::exchange(o.x, 0) }, 
        y{ std::exchange(o.y,0) } {
        std::cout << "move ctor: " << x << std::endl;
    }

    Foo& operator=(Foo&& o) noexcept {
        if (this != &o) {
            x = std::exchange(o.x, 0);
            y = std::exchange(o.y, 0);
            std::cout << "move operator\n";
        }
        return *this;
    }
};

export void testMonotoicBuffer()
{
    constexpr size_t sz = 128;
    constexpr size_t numElm = 14;
    std::byte mem[sz] = {};
    std::pmr::monotonic_buffer_resource alloc(&mem, sz);

    std::pmr::vector<Foo> vec(&alloc);
    vec.reserve(numElm); // IMPORTANT for performance

    for (int i = 1; i < numElm + 1; ++i) {
        vec.emplace_back(i);
    }

    std::cout << "\nvalue from statck mem:\n";
    for (int i = 0; i < sz; i += sizeof(Foo)) {
        Foo* p = static_cast<Foo*>((void*)&mem[i]);
        std::cout << &p->x << " #" << p->x << std::endl;
    }

    std::cout << "\nvalue from vector:\n";
    for (auto&& e : vec)
        std::cout << &e.x << " #" << e.x << std::endl;
}

#include <chrono>
export void testStringOnStack()
{
    constexpr size_t sz = 128 * 1024;
    std::string strOnHeap;
    //strOnHeap.reserve(sz);
    auto loops = sz - 16;

    auto start = std::chrono::steady_clock::now();
    for (size_t i = 0; i < loops; ++i) {
        strOnHeap += "a";
    }
    auto end = std::chrono::steady_clock::now();
    std::cout << "String on Heap: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " �s\n";

    std::byte mem[sz] = {};
    std::pmr::monotonic_buffer_resource rsrc(&mem, sz);
    std::pmr::string strOnStack(&rsrc);
    //strOnStack.reserve(sz);

    auto start2 = std::chrono::steady_clock::now();
    for (size_t i = 0; i < loops; ++i) {
        strOnStack += "a";
    }
    auto end2 = std::chrono::steady_clock::now();
    std::cout << "String on stack: " << std::chrono::duration_cast<std::chrono::microseconds>(end2 - start2).count() << " �s\n";

}