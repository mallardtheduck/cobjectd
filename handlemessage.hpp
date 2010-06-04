#ifndef HANDLEMESSAGE_HPP_INCLUDED
#define HANDLEMESSAGE_HPP_INCLUDED

#include "messages.hpp"
#include "tcp_connection.hpp"

const BrokerDetails CurrentBrokerDetails = {0, "CO_BROKER", "CrossObject", "DEV"};

void HandleMessage(MessageID_t msgID, shared_ptr<tcp_connection> conn);

#endif // HANDLEMESSAGE_HPP_INCLUDED
