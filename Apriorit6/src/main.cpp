#include "guards/file_handle_guard.hpp"
#include "deleters/socket_deleters.hpp"
#include "shared_ptr.hpp"

#include <fstream>
#include <iostream>
#include <memory>
#include <winsock2.h>

namespace
{
    void DemoTask1()
    {
        std::ofstream out("demo_task1.txt");
        out << "123\n";
        out.close();

        HANDLE handle = CreateFileA(
            "demo_task1.txt",
            GENERIC_READ,
            FILE_SHARE_READ,
            nullptr,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            nullptr);

        apriorit6::task1::guards::FileHandleGuard guard(handle);
        std::cout << "Task1 guard valid: " << std::boolalpha << guard.IsValid() << '\n';
    }

    void DemoTask2()
    {
        WSADATA data{};
        if (WSAStartup(MAKEWORD(2, 2), &data) != 0)
        {
            std::cout << "Task2 WSAStartup failed" << '\n';
            return;
        }

        using apriorit6::task2::deleters::FunctionDeleter;
        using apriorit6::task2::deleters::LambdaDeleter;
        using apriorit6::task2::deleters::SocketFunctor;

        auto* s1 = new SOCKET(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));
        auto* s2 = new SOCKET(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));
        auto* s3 = new SOCKET(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP));

        std::unique_ptr<SOCKET, decltype(LambdaDeleter)> p1(s1, LambdaDeleter);
        std::unique_ptr<SOCKET, SocketFunctor> p2(s2, SocketFunctor{});
        std::unique_ptr<SOCKET, std::function<void(SOCKET*)>> p3(s3, FunctionDeleter);

        const bool any_valid =
            (p1 && *p1 != INVALID_SOCKET) ||
            (p2 && *p2 != INVALID_SOCKET) ||
            (p3 && *p3 != INVALID_SOCKET);

        std::cout << "Task2 sockets created: " << std::boolalpha << any_valid << '\n';
        WSACleanup();
    }

    void DemoTask4()
    {
        apriorit6::task4::SharedPtr<int> first(new int(10));
        apriorit6::task4::SharedPtr<int> second(first);
        std::cout << "Task4 value: " << *second << ", use_count: " << second.UseCount() << '\n';
    }
}

int main()
{
    DemoTask1();
    DemoTask2();
    DemoTask4();
    return 0;
}
