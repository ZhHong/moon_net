// example-network.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "EchoClient.hpp"
#include "EchoServer.hpp"

int main()
{
	EchoServer svr;
	svr.Start();

	EchoClient cl;
	cl.Start();

    return 0;
}

