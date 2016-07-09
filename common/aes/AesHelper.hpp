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
#include "aes.h"
#include <vector>
#include <cassert>

/*
char key[] ="1234567890123456";
char iv[]  = "1234567890123456";

aes_helper ah;
ah.set_key(key, iv);

//std::string p1 = string_to_utf8("����������ʾ����ֻ�� ha hah ha");
std::string p1 = ("����������ʾ����ֻ�� ha hah ha");

auto ret = ah.encrypt_cbc((uint8_t*)p1.data(), p1.size() + 1, aes_helper::PaddingMode::Zeros);

auto tmp = ah.decrypt_cbc(ret.data(), ret.size(), aes_helper::PaddingMode::Zeros);

std::string str(tmp.begin(), tmp.end());
//printf("%s", utf8_to_string(str).c_str());
printf("%s",(str).c_str());

*/


class aes_helper
{
public:
	static const uint8_t key_size = 16;
	aes_helper()
	{
		memset(m_key, 0, key_size);
		memset(m_iv, 0, key_size);
	}

	enum class PaddingMode
	{
		Zeros,
		PKCS7Padding
		//PKCS5Padding
	};

	//16�ֽ�(128λ)��Կ,16�ֽ�(128λ)iv, ���ivΪ�գ� ��iv����Կ��ͬ
	void set_key(const char* szkey, const char* sziv = nullptr)
	{
		for (int i = 0; i < key_size; i++)
		{
			m_key[i] = szkey[i];
			if (sziv == nullptr)
			{
				m_iv[i] = m_key[i];
			}
		}

		if (sziv != nullptr)
		{
			for (int i = 0; i < key_size; i++)
			{
				m_iv[i] = sziv[i];
			}
		}
	}
	
	std::vector<uint8_t> encrypt_cbc(uint8_t* inData, size_t inLen,PaddingMode Mode)
	{
		assert(inData != nullptr && inLen > 0);

		size_t len = 0;
		uint8_t padding_value = 0;
		check_padding(inLen, Mode, len, padding_value);
		memcpy(m_buffer.data(), inData, inLen);

		std::vector<uint8_t> ret(len, 0);
		AES128_CBC_encrypt_buffer(ret.data(),m_buffer.data(), len, m_key, m_iv);
		return std::move(ret);
	}

	std::vector<uint8_t> decrypt_cbc(uint8_t* inData, size_t inLen,PaddingMode Mode)
	{
		assert(inData != nullptr && inLen > 0);

		std::vector<uint8_t> ret(inLen, 0);

		AES128_CBC_decrypt_buffer(ret.data(), inData, inLen, m_key, m_iv);

		remove_padding(Mode, ret);
		return ret;
	}

	std::vector<uint8_t> encrypt_ecb(uint8_t* inData, size_t inLen, PaddingMode Mode)
	{
		assert(inData != nullptr && inLen > 0);

		size_t len = 0;
		uint8_t padding_value = 0;
		check_padding(inLen, Mode, len, padding_value);
		memcpy(m_buffer.data(), inData, inLen);

		std::vector<uint8_t> ret(len, 0);
		for (size_t i = 0; i < m_buffer.size(); i += key_size)
		{
			AES128_ECB_encrypt(m_buffer.data() + i, m_key, ret.data() + i);
		}
		return ret;
	}

	std::vector<uint8_t> decrypt_ecb(uint8_t* inData, size_t inLen, PaddingMode Mode)
	{
		assert(inData != nullptr && inLen > 0);

		std::vector<uint8_t> ret(inLen, 0);
		for (size_t i = 0; i < inLen; i += key_size)
		{
			AES128_ECB_decrypt(inData+i, m_key, ret.data()+i);
		}
		
		remove_padding(Mode, ret);
		return ret;
	}

private:
	void remove_padding(PaddingMode Mode, std::vector<uint8_t>& ret)
	{
		if (Mode == PaddingMode::PKCS7Padding)
		{
			uint8_t n = ret.back();
			for (uint8_t i = 0; i < n; i++)
			{
				ret.pop_back();
			}
		}
	}

	void check_padding(size_t inLen, PaddingMode Mode, size_t& len, uint8_t& padding_value)
	{
		len = inLen;
		padding_value = 0;
		switch (Mode)
		{
		case PaddingMode::Zeros:
			if (inLen % 16 == 0)
			{
				len += 16;
			}
			else
			{
				len += (16 - inLen % 16);
			}
			break;
		case PaddingMode::PKCS7Padding:
		{
			if (inLen % 16 == 0)
			{
				len += 16;
				padding_value = 16;
			}
			else
			{
				len += (16 - inLen % 16);
				padding_value = 16 - inLen % 16;
			}
		}
		break;
		default:
			assert(0 && "unknown PaddingMode");
		}

		if (m_buffer.size() < len)
		{
			m_buffer.resize(len);
		}
		memset(m_buffer.data(), padding_value, m_buffer.size());
	}

private:
	uint8_t			m_key[key_size];
	uint8_t			m_iv[key_size];

	std::vector<uint8_t>  m_buffer;
};
