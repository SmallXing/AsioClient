#include "AsioClient.h"

#include <iostream>

#include <boost/progress.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread.hpp>

int main(int argc,char** argv)
{
	boost::asio::io_service io;


	if(0)
	{
		boost::asio::ip::tcp::socket tcpSocket(io);

		{
			std::cout << "---------------------------------------------------------------------------------------" << std::endl;
			CAsioClient client(tcpSocket);
			boost::system::error_code ec;

			boost::progress_timer t;
			bool ret = client.Connect("127.0.0.1",8899,boost::posix_time::seconds(20),ec);
			if(ec)
			{
				std::cout << "---- 连接失败" << ec.message() <<std::endl;
			}
			if(!ret)
			{

				std::cout << "---- 连接失败" << ec.message() <<std::endl;
			}


		}

		CAsioClient client(tcpSocket);
		boost::system::error_code ec;

		char buf[1];
		buf[0] = 'A';


		{
			boost::progress_timer t;
			ec = client.Send(boost::asio::buffer(buf,1),boost::posix_time::seconds(20));
			if(ec)
			{
				std::cout << "---- 发送失败" << ec.message() <<std::endl;
			}

		}

		std::vector<char> vec;
		vec.resize(10);
		{
			boost::progress_timer t;
			ec = client.Receive(boost::asio::buffer(vec),boost::posix_time::seconds(10));
			if(ec)
			{
				//std::cout << vec.size() << std::endl;
				std::cout << "---- 接收失败" << ec.message() <<std::endl;
			}
		}



		ec = client.Receive(boost::asio::buffer(vec),boost::posix_time::seconds(10));
		if(ec)
		{
			//std::cout << vec.size() << std::endl;
			std::cout << "---- 接收失败" << ec.message() <<std::endl;
		}

	}else
	{
	
		std::cout << "---------------------------------------------------------------------------------------" << std::endl;
		CAsioClient client;
		boost::system::error_code ec;

		bool ret = client.Connect("127.0.0.1",8899,boost::posix_time::seconds(20),ec);
		if(ec)
		{
			std::cout << "---- 连接失败" << ec.message() <<std::endl;
		}
		if(!ret)
		{

			std::cout << "---- 连接失败" << ec.message() <<std::endl;
		}


		char buf[1];
		buf[0] = 'A';
		ec = client.Send(boost::asio::buffer(buf,1),boost::posix_time::seconds(20));
		if(ec)
		{
			std::cout << "---- 发送失败" << ec.message() <<std::endl;
		}


		std::vector<char> vec;
		vec.resize(10);

		{
			boost::progress_timer t;
			ec = client.Receive(boost::asio::buffer(vec),boost::posix_time::seconds(10));
			if(ec)
			{
				//std::cout << vec.size() << std::endl;
				std::cout << "---- 接收失败" << ec.message() <<std::endl;
			}
		}



		ec = client.Receive(boost::asio::buffer(vec),boost::posix_time::seconds(10));
		if(ec)
		{
			//std::cout << vec.size() << std::endl;
			std::cout << "---- 接收失败" << ec.message() <<std::endl;
		}

	}



	return 0;
}