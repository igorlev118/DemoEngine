#pragma once

#include "Common/Debug.hpp"
#include "Events/Dispatcher.hpp"
#include "Events/Delegate.hpp"

// Forward declarations.
namespace Common
{
    template<typename Type>
    class DispatcherBase;

    template<typename Type>
    class ReceiverInvoker;
}

/*
    Receiver

    Invokes a delegate after receiving a signal from a dispatcher.
    Single receiver instance can be subscribed to only one dispatcher.

    See Dispatcher template class for more information.
*/

namespace Common
{
    template<typename Type>
    class Receiver;

    template<typename ReturnType, typename... Arguments>
    class Receiver<ReturnType(Arguments...)> : public Delegate<ReturnType(Arguments...)>
    {
    public:
        // Friend declarations.
        friend DispatcherBase<ReturnType(Arguments...)>;
        friend ReceiverInvoker<ReturnType(Arguments...)>;

    public:
        // Constructor.
        Receiver() :
            m_dispatcher(nullptr),
            m_previous(nullptr),
            m_next(nullptr)
        {
        }

        // Destructor.
        virtual ~Receiver()
        {
            // Unsubscribe from the dispatcher.
            this->Unsubscribe();
        }

        // Disallow copying.
        Receiver(const Receiver& other) = delete;
        Receiver& operator=(const Receiver& other) = delete;

        // Move operations.
        Receiver(Receiver&& other) :
            Receiver()
        {
            // Call the assignment operator.
            *this = std::move(other);
        }

        Receiver& operator=(Receiver&& other)
        {
            // Swap class members.
            std::swap(m_dispatcher, other.m_dispatcher);
            std::swap(m_previous, other.m_previous);
            std::swap(m_next, other.m_next);

            // Do not swap the underlying delegate.
            // We only want to swap the dispatcher subscription.
            
            return *this;
        }

        // Subscribes to a dispatcher.
        bool Subscribe(DispatcherBase<ReturnType(Arguments...)>& dispatcher, bool unsubscribeReceiver = true)
        {
            return dispatcher.Subscribe(*this, unsubscribeReceiver);
        }

        // Unsubscribes from the current dispatcher.
        void Unsubscribe()
        {
            if(m_dispatcher != nullptr)
            {
                m_dispatcher->Unsubscribe(*this);

                ASSERT(m_dispatcher == nullptr, "Dispatcher did not unsubscribe this receiver properly!");
                ASSERT(m_previous == nullptr, "Dispatcher did not unsubscribe this receiver properly!");
                ASSERT(m_next == nullptr, "Dispatcher did not unsubscribe this receiver properly!");
            }
        }

    private:
        // Receives an event and invokes a bound function.
        ReturnType Receive(Arguments... arguments)
        {
            ASSERT(m_dispatcher, "Invoked a receiver without it being subscribed!");
            return this->Invoke(std::forward<Arguments>(arguments)...);
        }

        // Make derived invoke method private.
        // We do not want other classes calling this.
        ReturnType Invoke(Arguments... arguments)
        {
            return Delegate<ReturnType(Arguments...)>::Invoke(std::forward<Arguments>(arguments)...);
        }

    private:
        // Intrusive double linked list element.
        DispatcherBase<ReturnType(Arguments...)>* m_dispatcher;
        Receiver<ReturnType(Arguments...)>*       m_previous;
        Receiver<ReturnType(Arguments...)>*       m_next;
    };
}
