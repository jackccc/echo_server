#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/asio/yield.hpp>
#include <iostream>

boost::asio::io_service io_service;
boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), 2223);
boost::asio::ip::tcp::acceptor acceptor(io_service, endpoint);

char data[1024];

void handle(boost::asio::coroutine ct,boost::asio::ip::tcp::socket* sock,
	const boost::system::error_code& error,size_t bytes_transferred)
{
	if (error)
	{
		std::cout << "read发生错误：" << error.message() << std::endl;
		delete sock;
		return;
	}

	reenter(ct)
	{
		yield boost::asio::async_write(*sock, boost::asio::buffer(data, bytes_transferred),
			boost::bind(handle,ct, sock,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));

		yield sock->async_read_some(boost::asio::buffer(data),
			boost::bind(handle, boost::asio::coroutine(),sock,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
	}

}

void session(boost::asio::ip::tcp::socket* socket,
	const boost::system::error_code& error)
{
	if (error)
	{
		delete socket;
	}
	else
	{
		std::cout << "有客户端连接" << std::endl;

		socket->async_read_some(boost::asio::buffer(data),
			boost::bind(handle, boost::asio::coroutine(),socket,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
	}

	boost::asio::ip::tcp::socket* s = new boost::asio::ip::tcp::socket(io_service);
	acceptor.async_accept(*s, boost::bind(session, s,
		boost::asio::placeholders::error));
}

int main(int argc, char* argv[])
{
	boost::asio::ip::tcp::socket* socket = new boost::asio::ip::tcp::socket(io_service);
	acceptor.async_accept(*socket, boost::bind(session, socket, boost::asio::placeholders::error));

	io_service.run();
	return 0;
}
