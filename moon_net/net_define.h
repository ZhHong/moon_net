#pragma once
#include <cstdint>
#include <asio/asio.hpp>
#include <common/moon_define.h>

namespace moon
{
	DECLARE_SHARED_PTR(socket);
	DECLARE_SHARED_PTR(message);
	DECLARE_SHARED_PTR(buffer);

	using net_message_hander = std::function<void(const message_ptr&)>;
}

//Ĭ�ϳ�ʱ���(����)
#define DEFAULT_TIMEOUT_INTERVAL     1000
//Ĭ�ϳ�ʱ����� �����룩
#define DEFAULT_TIMEOUTCHECK_INTERVAL 2000

enum  class ESocketState
{
	Ok,
	Create,
	Accepted,
	Connected,
	Timeout,
	ClientClose,
	Closed,
	Force_Closing
};



typedef uint16_t msg_size_t;
