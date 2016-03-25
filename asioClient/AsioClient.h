#pragma once

#include <string>
#include <memory>

#include <boost/asio.hpp>
#include <boost/noncopyable.hpp>

class CAsioClient: boost::noncopyable
{
public:
	CAsioClient();
	CAsioClient(boost::asio::ip::tcp::socket& socket);
	~CAsioClient();
	bool Connect(const std::string& strIp, int nPort, boost::posix_time::time_duration timeout, boost::system::error_code& ec);

	boost::system::error_code Receive(boost::asio::mutable_buffers_1& rcvBuf, boost::posix_time::time_duration timeout);
	boost::system::error_code Receive(boost::asio::mutable_buffers_1& rcvBuf, std::size_t &recvLength, boost::posix_time::time_duration timeout);

	boost::system::error_code Send(const boost::asio::mutable_buffers_1& sendBuf, boost::posix_time::time_duration timeout);
	boost::system::error_code Send(const boost::asio::mutable_buffers_1& sendBuf,  std::size_t &sendLength, boost::posix_time::time_duration timeout);

protected:
	void check_deadline(const boost::system::error_code& ec);

private:
	std::shared_ptr<boost::asio::io_service> _ioservice;
	std::shared_ptr<boost::asio::ip::tcp::socket> _socket;

	boost::asio::io_service& m_ioservice;
	boost::asio::ip::tcp::socket& m_socket;
	boost::asio::deadline_timer m_deadline;
};