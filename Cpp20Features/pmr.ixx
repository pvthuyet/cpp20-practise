module;

// Type your code here, or load an example.
#include <iostream>
#include <memory_resource>
#include <vector>
#include <string>
#include <chrono>
#include <array>
#include "fmt/core.h"
#include "fmt/color.h"

import Fibo.MemoryTracker;
import Fibo.AddressHelper;

export module Fibo.Pmr;

void print(std::string_view msg)
{
    static int numspc = 0;
    auto loop = (msg.at(0) == '~') ? --numspc : numspc++;
    for (int i = 0; i < loop; ++i) std::cout << "  ";
    std::cout << msg << std::endl << std::flush;
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

export void testFoo()
{
    std::pmr::string s1{ "abcdadfadafadadfadfadfdfadfadf" };
    std::pmr::string s2{ "abcdadfadafadadfadfadfdfadfadf" };
    std::pmr::string s3{ "abcdadfadafadadfadfadfdfadfadf" };
   // char ch[] = "hello world";
    //fmt::print("{:>{}} {}", " ", 10, "hello");
    return;

    std::array<std::byte, 10 * 1024> buf{};
    std::cout << "address range: " << buf.data() << " - " << (buf.data() + buf.size()) << std::endl;
    std::pmr::monotonic_buffer_resource pool{ buf.data(), buf.size() };
    fmt::print(fmt::fg(fmt::color::white),
        "pool address: {} - {}\n", 
        fmt::ptr(buf.data()),
        fmt::ptr(buf.data() + buf.size()));

    Foo fo1("abcxxxxxxxxxxxxxxxxxxxxxxx", &pool);
    fmt::print("Obj address: {} - dist: {}\n", 
        fmt::ptr(&fo1.mName[0]), 
        fibo::AddressHelper::diff(fo1.mName[0], buf[0]));

    Foo fo2 = fo1;
    fmt::print("Obj address: {} - dist: {}\n",
        fmt::ptr(&fo2.mName[0]),
        fibo::AddressHelper::diff(fo2.mName[0], buf[0]));

    Foo fo3(fo1, &pool);
    fmt::print("Obj address: {} - dist: {}\n",
        fmt::ptr(&fo3.mName[0]),
        fibo::AddressHelper::diff(fo3.mName[0], buf[0]));
   
}

/*
void createVector(std::pmr::memory_resource* pool, int numElm)
{
    print("vector");
    std::pmr::vector<Foo> vec{ pool };
    vec.reserve(numElm);
    for (int i = 0; i < numElm; ++i) {
        print("emplace_back");
        vec.emplace_back(i + 1, "The string should be big enought to avoid the SSO");
        print("~emplace_back");
    }
    print("~vector");
}

export void monotonic_on_stack()
{
    std::cout << "\n\n*********************************************\n";
    std::cout << "* monotoic buffer resource on stack         *\n";
    std::cout << "*********************************************\n";
    std::array<std::byte, 10 * 1024> buf{};
    std::cout << "address range: " << buf.data() << " - " << buf.data() + buf.size() << std::endl;
    {
        std::pmr::monotonic_buffer_resource pool{ buf.data(), buf.size() };
        createVector(&pool, 3);
    }
}

export void monotonic_unreusable()
{
    std::cout << "\n\n*********************************************\n";
    std::cout << "* monotoic buffer resource                  *\n";
    std::cout << "*********************************************\n";
 
    print("monotonic_buffer_resource");
    
    std::pmr::monotonic_buffer_resource pool{};
    createVector(&pool, 3);
    std::cout << "********* Can re-use ???? => NO\n";
    createVector(&pool, 2);
    
    print("~monotonic_buffer_resource");
}


export void synchronized_reusable()
{
    std::cout << "\n\n*********************************************\n";
    std::cout << "* synchronized pool resource                *\n";
    std::cout << "*********************************************\n";

    print("synchronized_pool_resource");
    
    std::pmr::synchronized_pool_resource pool{};
    createVector(&pool, 3);
    std::cout << "********* Can re-use ???? => YES\n";
    createVector(&pool, 2);
    
    print("~synchronized_pool_resource");
}
*/