/*
 * Copyright © 2012 Canonical Ltd.
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License version 3,
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authored by: Alan Griffiths <alan@octopull.co.uk>
 */

#ifndef ANBOX_BRIDGE_PENDING_CALL_CACHE_
#define ANBOX_BRIDGE_PENDING_CALL_CACHE_

#include <functional>
#include <mutex>
#include <map>

namespace google {
namespace protobuf {
class Closure;
class MessageLite;
} // namespace protobuf
} // namespace google

namespace anbox {
namespace protobuf {
namespace bridge {
class Invocation;
class Result;
} // namespace bridge
} // namespace protobuf
namespace bridge {
class PendingCallCache {
public:
    PendingCallCache();

    void save_completion_details(anbox::protobuf::bridge::Invocation const &invocation,
                                 google::protobuf::MessageLite *response,
                                 google::protobuf::Closure *complete);
    void populate_message_for_result(anbox::protobuf::bridge::Result &result,
                                     std::function<void(google::protobuf::MessageLite*)> const& populator);
    void complete_response(anbox::protobuf::bridge::Result& result);
    void force_completion();
    bool empty() const;

private:
    struct PendingCall {
        PendingCall(google::protobuf::MessageLite *response,
                    google::protobuf::Closure *target) :
            response(response),
            complete(target) {
        }

        PendingCall() :
            response(0),
            complete() {
        }

        google::protobuf::MessageLite *response;
        google::protobuf::Closure *complete;
    };

    std::mutex mutable mutex_;
    std::map<int, PendingCall> pending_calls_;
};
} // namespace bridge
} // namespace anbox

#endif