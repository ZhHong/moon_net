#pragma once
#include <common/noncopyable.hpp>
#include <tcp_frame.h>
#include <message.h>
#include <buffer_reader.h>
#include <string>
#include <iostream>
#include <functional>

using namespace moon;

class EchoClient
{
public:

	void Start()
	{
		auto handler = std::bind(&EchoClient::OnNetMessage, this, std::placeholders::_1);
		m_Net = std::make_shared<tcp_frame>(handler, 2);//1�������߳�

		//���ӱ����� 11111 �˿ڣ����ڴ˴�ֻ��������⣬������ģ�飬ģ��IDΪ0 ����
		auto id = m_Net->sync_connect("127.0.0.1", "11111", module_id::create(0));

		if (id.value == 0)
		{
			std::cout << "CLIENT : connect server failed" << std::endl;
			return;
		}

		//���������߳�
		m_Net->run();

		std::string str;

		std::cin >> str;

		while (str != "exit")
		{
			std::cout << "CLIENT:send msg " <<str<< std::endl;
			auto msg = buffer::create(str.size() + 1);
			(*msg) << str;
			m_Net->send(id, msg);
			std::cin >> str;
		}

		m_Net->stop();
	}

private:
	void OnNetMessage(const message& msg)
	{
		switch (msg.get_type())
		{
		case EMessageType::SocketConnect:
		{
			buffer_reader br(msg.data(), msg.size());
			std::string addr;
			br >> addr;
			std::cout << "CLIENT:connect server:" << addr << std::endl;
			break;
		}
		case EMessageType::SocketData:
		{
			//��ȡ��������ID
			auto sockID = msg.get_socket_id();
			buffer_reader br(msg.data(), msg.size());
			//��ȡ��������Ϣ
			std::string  clientMsg;
			br >> clientMsg;
			std::cout << "CLIENT:recv msg " << clientMsg << std::endl;
			break;
			break;
		}
		case EMessageType::SocketClose:
		{
			//�Ͽ�����Ϣ����Ĭ��Ϊ�ͻ��˵�ַ
			buffer_reader br(msg.data(), msg.size());
			std::string addr;
			br >> addr;
			std::cout << "CLIENT:server close " << addr << std::endl;
			break;
		}
		default:
			break;
		}
	}

private:
	std::shared_ptr<tcp_frame>   m_Net;
};