#define BOOST_PYTHON_STATIC_LIB
#include "Spout.h"
#include <boost/python.hpp>

BOOST_PYTHON_MODULE(SpoutSDK)
{
	using namespace boost::python;

	class_<SpoutSender>("SpoutSender")
		.def("CreateSender", &SpoutSender::CreateSender)
		.def("SayHello", &SpoutSender::SayHello)
		.def("SendTexture", &SpoutSender::SendTexture)
		.def("ReleaseSender", &SpoutSender::ReleaseSender)
		;
	class_<SpoutReceiver>("SpoutReceiver")
		.def("pyCreateReceiver", &SpoutReceiver::pyCreateReceiver)
		.def("pyReceiveTexture", &SpoutReceiver::pyReceiveTexture)
		.def("ReleaseReceiver", &SpoutReceiver::ReleaseReceiver)
		.def("SelectSenderPanel", &SpoutReceiver::SelectSenderPanel)
		.def("GetWidth", &SpoutReceiver::GetWidth)
		.def("GetHeight", &SpoutReceiver::GetHeight)
		;
	class_<SpoutSend>("SpoutSend")
		.def("SetupSender", &SpoutSend::SetupSender)
		.def("SendTextureData", &SpoutSend::SendTextureData)
		.def("CloseSender", &SpoutSend::CloseSender)
		;
	class_<SpoutReceive>("SpoutReceive")
		.def("SetupReceiver", &SpoutReceive::SetupReceiver)
		.def("ReceiveTextureData", &SpoutReceive::ReceiveTextureData)
		.def("IsUpdated", &SpoutReceive::IsUpdated)
		.def("IsInitialized", &SpoutReceive::IsInitialized)
		.def("CloseReceiver", &SpoutReceive::CloseReceiver)
		.def("SetReceiverName", &SpoutReceive::SetReceiverName)
		.def("GetSenderFrame", &SpoutReceive::GetSenderFrame)
		.def("GetSenderFps", &SpoutReceive::GetSenderFps)
		.def("GetSenderWidth", &SpoutReceive::GetSenderWidth)
		.def("GetSenderHeight", &SpoutReceive::GetSenderHeight)
		;

}

