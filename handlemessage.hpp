#ifndef HANDLEMESSAGE_HPP_INCLUDED
#define HANDLEMESSAGE_HPP_INCLUDED

#include <cobject-client/serialize.hpp>
#include <cobject-client/messages.hpp>

#include "tcp_connection.hpp"

namespace cobject
{
    //! The broker version details
    const BrokerDetails CurrentBrokerDetails = {0, "CO_BROKER", "CrossObject", "DEV"};

    /*!
        Handle an incoming protocol message
        \param msgID    The protocol MessageID
        \param conn     The connection that it comes from
    */
    void HandleMessage(MessageID_t msgID, boost::shared_ptr<tcp_connection> conn);
}

#endif // HANDLEMESSAGE_HPP_INCLUDED
