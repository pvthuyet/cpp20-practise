module;

// Type your code here, or load an example.
#include <iostream>
#include <memory_resource>
#include <vector>
#include <string>
#include <chrono>
#include <array>

import Fibo.MemoryTracker;

export module Fibo.Pmr;

void print(std::string_view msg)
{
    static int numspc = 0;
    auto loop = (msg.at(0) == '~') ? --numspc : numspc++;
    for (int i = 0; i < loop; ++i) std::cout << "  ";
    std::cout << msg << std::endl << std::flush;
}

struct Foo 
{
public:
    int mId{};
    std::string mName;

public:
    Foo() noexcept 
    {
        //std::cout << "default ctor " << this << std::endl;
    }

    ~Foo() noexcept
    {
        //std::cout << "~Foo " << this << std::endl;
    }

    template<typename T>
    Foo(int id, T&& name) noexcept :
        mId{ id },
        mName(std::forward<T>(name))
    {
        //std::cout << "ctor with argument " << this << std::endl;
    }

    Foo(Foo const& o) : mId{ o.mId }, mName{ o.mName }
    {
        //std::cout << "copy ctor " << this << " <= " << &o << std::endl;
    }

    Foo& operator=(Foo const& o) 
    {
        //std::cout << "copy operator " << this << " <= " << &o << std::endl;
        Foo tmp(o);
        *this = std::move(tmp);
        return *this;
    }

    Foo(Foo&& o) noexcept : 
        mId{ o.mId },
        mName{ std::exchange(o.mName, std::pmr::string{}) }
    {
        //std::cout << "move ctor " << this << " <= " << &o << std::endl;
    }

    Foo& operator=(Foo&& o) noexcept 
    {
        if (this != &o) {
            mId = o.mId;
            mName = std::exchange(o.mName, std::pmr::string{});
            //std::cout << "move operator " << this << " <= " << &o << std::endl;
        }
        return *this;
    }
};

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