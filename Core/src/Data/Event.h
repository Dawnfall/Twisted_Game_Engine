#pragma once
#include <functional>
#include <vector>

namespace Twisted
{
    template<typename... Args>
    class Event
    {
    public:
        using EventHandler = std::function<void(Args...)>;

        // Add a listener to the event
        void AddListener(const EventHandler& handler)
        {
            listeners.emplace_back(handler);
        }

        // Remove a listener from the event
        void RemoveListener(const EventHandler& handler) 
        {
            listeners.erase(std::remove(listeners.begin(), listeners.end(), handler), listeners.end());
        }

        // Invoke the event, triggering all registered listeners
        void Invoke(Args... args) 
        {
            for (const auto& listener : listeners)
            {
                listener(args...);
            }
        }

    private:
        std::vector<EventHandler> listeners;
    };
}
