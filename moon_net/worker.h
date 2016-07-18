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
#include <common/loop_thread.hpp>
#include "actor_define.h"
#include "async_event.h"
#include "message.h"

namespace moon
{
	class actor_manager;

	//�����߳�
	class worker:public loop_thread,public async_event, noncopyable
	{
	public:
		worker(actor_manager* mgr);

		~worker();

		/**
		* ���������߳�
		*
		*/
		void run() override;

		/**
		* ֹͣ�����߳�
		*
		*/
		void stop() override;

		/**
		* �����߳����actor
		*
		*/
		void add_actor(const actor_ptr& act);

		/**
		* �Ƴ�actor
		*
		*/
		void remove_actor(module_id id);

		/**
		* ��Ϣ�ַ�
		*/
		void dispatch_message(const message&msg);

		/**
		* ���worker�е�����actor�㲥��Ϣ
		*/
		void broadcast_message(const message& msg);

		int	get_msg_fps();

		uint8_t	get_id();

		void set_id(uint8_t id);
	private:
		void update(uint32_t interval);

	private:
		uint8_t																_worker_id;
		std::unordered_map<module_id, actor_ptr>	_actors;
		//��Ϣ�������
		std::deque<actor*>											_actors_queue;
		actor_manager*												_mgr;

		std::atomic<int>												_fps;
		uint32_t															_msg_counter;
		uint32_t															_timer;

		timer_pool														m_timerpool;
	};
};


