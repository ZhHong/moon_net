#pragma once
#include <common/noncopyable.hpp>
#include <tcp_frame.h>
#include <message.h>
#include <buffer_reader.h>
#include <string>
#include <iostream>
#include <functional>

using namespace moon;

class EchoServer :noncopyable
{
public:
	EchoServer()
	{

	}

	~EchoServer()
	{
		if (m_Net != nullptr)
		{
			//�ر������߳�
			m_Net->stop();
		}
	}

	void Start()
	{
		auto handler = std::bind(&EchoServer::OnNetMessage, this, std::placeholders::_1);
		m_Net = std::make_shared<tcp_frame>(handler,1);//2�������߳�

		//���������� 11111 �˿ڣ����ڴ˴�ֻ��������⣬������ģ�飬ģ��IDΪ0 ����
		m_Net->listen("127.0.0.1", "11111", module_id::create(0));

		//���������߳�
		m_Net->run();

		//��������� Ӧ���������̣߳���ֹ���߳��˳��������Ժ�ͻ��˻����������߳��ڣ������������߳�
	}

private:
	void OnNetMessage(const message& msg)
	{
		switch (msg.get_type())
		{
		case EMessageType::SocketConnect:
		{
			//���ӣ���Ϣ����Ĭ��ΪԶ��������ַ
			buffer_reader br(msg.data(), msg.size());
			std::string addr;
			br >> addr;
			std::cout <<"SERVER:client connect "<<addr<< std::endl;
			break;
		}
		case EMessageType::SocketData:
		{
			//�յ����ݣ�message �����з����ߺͽ����ߵ�ID.
			//�����������Ϣ����ô������Ϊ��������ID

			//��ȡ��������ID
			auto sockID = msg.get_socket_id();


			buffer_reader br(msg.data(), msg.size());
			//��ȡ��������Ϣ
			std::string  clientMsg;
			br >> clientMsg;

			//����Ϣ���ͻ�ȥ ��echoMsg������ָ��,������߳�֮���ڴ����
			auto echoMsg = buffer::create(clientMsg.size() + 1);
			(*echoMsg) << clientMsg;
			m_Net->send(sockID, echoMsg);


			std::cout << "SERVER:echo msg " << clientMsg << std::endl;
			break;
		}
		case EMessageType::SocketClose:
		{
			//�Ͽ�
			buffer_reader br(msg.data(), msg.size());
			std::string addr;
			br >> addr;
			std::cout << "SERVER:client close " << addr << std::endl;
			break;
		}
		default:
			break;
		}
	}

private:
	std::shared_ptr<tcp_frame>   m_Net;
};

