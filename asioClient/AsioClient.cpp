#include "AsioClient.h"

#include <sstream>

#include <boost/bind.hpp>

using namespace boost::asio;
using boost::asio::deadline_timer;
using boost::asio::ip::tcp;
using namespace boost;
using namespace std;


CAsioClient::CAsioClient()
	: _ioservice(std::make_shared<boost::asio::io_service>())
	, _socket(std::make_shared<tcp::socket>(*_ioservice))
	, m_ioservice(*_ioservice)
	, m_deadline(m_ioservice)
	, m_socket(*_socket)
{

}

CAsioClient::CAsioClient(tcp::socket& socket) 
	:m_ioservice(socket.get_io_service())
	,m_deadline(m_ioservice)
	,m_socket(socket)
{

	
}

CAsioClient::~CAsioClient()
{
	boost::system::error_code ec;
	m_deadline.cancel(ec);
}

bool CAsioClient::Connect(const std::string& strIp, int nPort, boost::posix_time::time_duration timeout, boost::system::error_code& ec)
{
	std::string strPort;
	{
		stringstream ss;	
		ss << nPort;		
		ss >> strPort;
	}
	ec = boost::asio::error::would_block;
	boost::system::error_code io_ec;

	tcp::resolver::query query(strIp, strPort);
	tcp::resolver::iterator iter = tcp::resolver(m_ioservice).resolve(query);
	m_deadline.expires_from_now(timeout);
	check_deadline(ec);

	boost::asio::async_connect(m_socket, iter,
			[&ec](boost::system::error_code _ec,tcp::resolver::iterator)
			{
				ec = _ec;
			});
	do m_ioservice.run_one(io_ec); while (ec == boost::asio::error::would_block);

	if (io_ec || ec || !m_socket.is_open())
	{
		return false;
	}
	return true;
}

boost::system::error_code CAsioClient::Receive(boost::asio::mutable_buffers_1& rcvBuf, boost::posix_time::time_duration timeout)
{
	std::size_t recvLength = 0;
	return Receive(rcvBuf,recvLength,timeout);
}

boost::system::error_code CAsioClient::Receive(boost::asio::mutable_buffers_1& rcvBuf, std::size_t &recvLength,boost::posix_time::time_duration timeout)
{

	boost::system::error_code ec = boost::asio::error::would_block;
	boost::system::error_code io_ec;

	m_deadline.expires_from_now(timeout);	
	check_deadline(ec);
	boost::asio::async_read(m_socket, rcvBuf,
		[&ec,&recvLength](boost::system::error_code _ec,std::size_t length)
		{
			recvLength = length;
			ec = _ec;
		});
	
	do m_ioservice.run_one(io_ec); while (ec == boost::asio::error::would_block);

	if(io_ec) return io_ec;
	return ec;
}

boost::system::error_code CAsioClient::Send(const boost::asio::mutable_buffers_1& sendBuf, boost::posix_time::time_duration timeout)
{
	std::size_t sendLength = 0;
	return Send(sendBuf,sendLength,timeout);
}

boost::system::error_code CAsioClient::Send(const boost::asio::mutable_buffers_1& sendBuf, std::size_t &sendLength, boost::posix_time::time_duration timeout)
{
	boost::system::error_code ec = boost::asio::error::would_block;
	boost::system::error_code io_ec;

	m_deadline.expires_from_now(timeout);	
	check_deadline(ec);
	boost::asio::async_write(m_socket, sendBuf,
		[&ec,&sendLength](boost::system::error_code _ec,std::size_t length)
		{
			sendLength = length;
			ec = _ec;
		});

	do m_ioservice.run_one(io_ec); while (ec == boost::asio::error::would_block);
	if(io_ec) return io_ec;
	return ec;
}

void CAsioClient::check_deadline(const boost::system::error_code& ec)
{
	if(ec.value() != boost::asio::error::operation_aborted)
	{
		if (m_deadline.expires_at() <= deadline_timer::traits_type::now())
		{
			boost::system::error_code ignored_ec;
			m_socket.close(ignored_ec);
			m_deadline.expires_at(boost::posix_time::pos_infin);
		}
		m_deadline.async_wait(boost::bind(&CAsioClient::check_deadline, this,boost::asio::placeholders::error));
	}
}
