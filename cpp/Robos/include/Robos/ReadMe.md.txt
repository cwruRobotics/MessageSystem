MessageFramework is a Framework that acts more like an Operating system.

It is designed to be very similar in look and feel to ROS (Robot Operating System)
while at the same time reducing some of the complexities of ROS. The MessageFramework
provides a "data dependent" model of computation versus a "component dependent" model,
that is to say that components are dependent on Messages existing rather than on the
component that generated them.

The MessageFramework has three main features:
	A Central interface That all client code "registers" itself with upon
	startup. A Component tells the MessageFramework what message types it
	"subscribes" to (is dependent on). When a message of type becomes
	available, the Components subscribing to that message type are passed
	the message and executed asynchronously.

	An asynchonous framework. Clients write code that are wrapped into a
	"Component" class which provides the interface between itself and the
	MessageFramework. Client code is guarenteed to execute asynchronously
	and is free to publish its data into a client defined Message struct (read only).
	This allows for data abstraction and helps create a easily maintainable
	client codebase.

	A type system. Clients can generate their own types to be used in their
	robot while supporting the data abstraction provided with Messaging.
	(For now) Clients are only allowed to pass primitive data (exception
	being std::string) as Message data.