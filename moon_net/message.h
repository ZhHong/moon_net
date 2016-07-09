/****************************************************************************
Copyright (c) 2016 moon

http://blog.csdn.net/greatchina01

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#pragma once
#include "actor_define.h"

namespace moon
{
	//actor ��Ϣ
	class message
	{
	public:
		explicit message(const buffer_ptr& data);

		~message();

		message(const message& msg);

		message(message&& msg);

		message& operator=(const message&) = delete;

		/**
		* ��ȡ��Ϣ�ķ�����ID��һ����ϢҪô��actor֮��ͨѶ����Ϣ��Ҫô��������Ϣ
		* �ο� get_socket_id
		*/
		module_id				get_sender() const;

		/**
		* ��ȡ��Ϣ��socketID ,�ο�get_sender
		*/
		socket_id					get_socket_id() const;
	
		/**
		* ������Ϣ�ķ�����ID,���������Ϣ�� actor��ͨѶ����Ϣ
		*/
		void							set_sender(module_id sender);

		/**
		* ������Ϣ�ķ�����ID,���������Ϣ�� ������Ϣ
		*/
		void							set_sender(socket_id sender);

		/**
		* ������Ϣ�Ľ�����ID
		*/
		void							set_receiver(module_id sender);

		module_id				get_receiver() const;

		/**
		* �����û��Զ�������
		*/
		void							set_userdata(uint8_t* data, size_t size) const;
	
		const uint8_t*			get_userdata(size_t size) const;
	
		/**
		* ��ȡ��Ϣ���ݣ���������ֻ���ģ���Ϊ�㲥��Ϣʱ ����Ϣ���ǹ����
		*/
		const buffer_ptr&	msg_data() const;

		/**
		* ��ȡ��Ϣ����
		*/
		const uint8_t*			data() const;

		/**
		* ��ȡ��Ϣ��С
		*/
		size_t						size() const;

		/**
		* ������Ϣ����
		*/
		void							set_type(EMessageType type);

		EMessageType			get_type() const;
	protected:
		class Imp;
		std::shared_ptr<Imp>           _imp;
	};
};


