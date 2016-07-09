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
#include <common/noncopyable.hpp>
namespace moon
{
	class actor_manager;

	/**
	* actor�Ļ���.
	*/
	class actor:noncopyable
	{
	public:
		friend class worker;
		friend class actor_manager;
		/**
		* ���캯��
		*
		* @id һ��Ψһ��ID,Ĭ���Ǹ��ۼ�ֵ.
		*/
		actor(module_id id);

		virtual							~actor();

		/**
		* ������actor(���˵�ǰactor)�㲥��Ϣ
		*
		* @msg ��Ϣ����
		*/
		void								broadcast_message(message& msg);

		/**
		* ��ĳ��actor������Ϣ
		*
		* @msg ��Ϣ����
		*/
		void								send_message(module_id receiver,message& msg);

		/**
		* ��ȡ���actor�� ID
		*
		* @return actorID
		*/
		const module_id			get_id() const;

		/**
		* ��ȡ���actor��name
		*
		* @return actor name
		*/
		const std::string&		get_name() const;

		/**
		* ��ȡ��ʱ��������
		*
		* @return ��ʱ��������
		*/
		timer_pool&				get_timer_pool();
	protected:
		/**
		* �������ó�ʼ�����actor,Ӧ����д�������,��ʼ��������actor
		*
		* @config key-value ����
		*/
		virtual void					init(const std::unordered_map<std::string, std::string>& config);

		/**
		* ����actor��name
		*
		* @name 
		*/
		void								set_name(const std::string& name);

		/**
		* ��actor��ӽ�worker thread ʱ����ô˺���
		*
		*/
		virtual void					on_enter();

		/**
		* ��actor��worker thread�Ƴ�ʱ����ô˺���
		*
		*/
		virtual void					on_exit();

		/**
		* ��Ϣ��������Ӧ����д�˺��� ��������Ϣ
		*
		*	@msg  ��Ϣ
		*/
		virtual void					handle_message(const message& msg) = 0;

		/**
		* ����Ϣ��ӽ� actor����Ϣ����
		*
		*	@msg  ��Ϣ
		*/
		void								add_message(const message& msg);
		
		/**
		* ��ȡ��ǰactor��Ϣ���еĴ�С
		*
		*	@return  ��Ϣ���еĴ�С
		*/
		size_t							get_mq_size();

		/**
		* ��Ϣ����������������worker thread����
		*
		*	@interval ѭ�����
		*/
		virtual void					update(uint32_t interval);
	private:
		void								set_manager(actor_manager* mgr);

		void								set_timer_pool(timer_pool* tmgr);
		//check message queue,pop a message,if message queue size >0,return true else return false

		/**
		* ���actor����Ϣ���г��ȣ��������0 ����true ���򷵻�false
		*
		*	@return 
		*/
		bool								peek_message();
	private:
		struct Imp;
		std::shared_ptr<Imp>											_Imp;
	};
};

