module;

// Type your code here, or load an example.
#include <iostream>
#include <memory_resource>
#include <vector>
#include <string>
#include <chrono>
#include <array>
#include <thread>
#include "fmt/core.h"
#include "fmt/color.h"

import Fibo.MemoryTracker;
import Fibo.AddressHelper;

export module Fibo.Pmr;

void printIndent(std::string_view msg)
{
    static int numspc = 0;
    auto indents = (msg.at(0) == '~') ? --numspc : numspc++;
    fmt::print(fmt::fg(fmt::color::white),
        "{:>{}}{}\n",
        ' ',
        indents * 2,
        msg);
}

class Foo 
{
public:
    int mId;
    std::pmr::string mName;
    using allocator_type = decltype(mName)::allocator_type;

public:
    explicit Foo(allocator_type alloc = {}) noexcept(std::is_nothrow_default_constructible_v<std::pmr::string>) :
        mId {0},
        mName{alloc}
    {
    }

    Foo(std::string_view name, allocator_type alloc = {}) :
        mId{ 0 },
        mName(name, alloc)
    {
    }

    Foo(Foo const&) = default;
    Foo(Foo const& o, allocator_type alloc) :
        mId{ o.mId }, 
        mName{ o.mName, alloc }
    {
    }

    Foo(Foo&&) = default;
    Foo(Foo&& other, allocator_type alloc) :
        mId{other.mId},
        mName{std::move(other.mName), alloc}
    {
    }

    // Rule of 6
    ~Foo() = default;
    Foo& operator=(Foo const&) = default;
    Foo& operator=(Foo&&) = default;
};

void createVector(std::pmr::memory_resource* pool, int numElm, long long poolAddr = 0)
{
    printIndent("vector");
    std::pmr::vector<Foo> vec{ pool };
    vec.reserve(numElm);
    for (int i = 0; i < numElm; ++i) {
        printIndent("emplace_back");
        vec.push_back(Foo{ "The string should be big enought to avoid the SSO", pool });
        printIndent("~emplace_back");
    }

    long long lastObj = (long long)(&vec[numElm - 1].mName[0]);
    fmt::print("Last obj address: {} - dist: {}\n",
        fmt::ptr(&vec[numElm-1].mName[0]),
        lastObj - poolAddr);

    printIndent("~vector");
}

export void monotonic_on_stack()
{
    fmt::print(
        "+{0:-^{2}}+\n"
        "|{1: ^{2}}|\n"
        "+{0:-^{2}}+\n", "", "monotoic buffer resource on stack", 40);

    std::array<std::byte, 10 * 1024> buf{};
    fmt::print(fmt::fg(fmt::color::white),
        "pool {} bytes, address: {} - {}\n",
        buf.size(),
        fmt::ptr(buf.data()),
        fmt::ptr(buf.data() + buf.size()));
    {
        std::pmr::monotonic_buffer_resource pool{ buf.data(), buf.size() };
        createVector(&pool, 3, (long long)(buf.data()));
    }
}

export void monotonic_unreusable()
{
    fmt::print("\n\n");
    fmt::print(
        "+{0:-^{2}}+\n"
        "|{1: ^{2}}|\n"
        "+{0:-^{2}}+\n", "", "monotoic buffer resource unreusable", 50); 

    printIndent("monotonic_buffer_resource");
    
    std::pmr::monotonic_buffer_resource pool{};
    createVector(&pool, 3);
    std::cout << "********* Can re-use ???? => NO\n";
    createVector(&pool, 2);
    
    printIndent("~monotonic_buffer_resource");
}


export void synchronized_reusable()
{
    fmt::print("\n\n");
    fmt::print(
        "+{0:-^{2}}+\n"
        "|{1: ^{2}}|\n"
        "+{0:-^{2}}+\n", "", "synchronized pool resource reusable", 50);

    printIndent("synchronized_pool_resource");
    
    std::pmr::synchronized_pool_resource pool{};
    createVector(&pool, 3);
    std::cout << "********* Can re-use ???? => YES\n";
    createVector(&pool, 2);
    
    printIndent("~synchronized_pool_resource");
}