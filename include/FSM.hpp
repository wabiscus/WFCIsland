#pragma once

#include <functional>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <utility>

template <typename State, typename Event>
class FSM
{
public:
    using Callback = std::function<void()>;

    struct Transition
    {
        State from;
        Event event;
        State to;
    };

public:
    explicit FSM(State initialState)
        : m_currentState(initialState)
    {
    }

    State getState() const
    {
        return m_currentState;
    }

    bool is(State state) const
    {
        return m_currentState == state;
    }

    void addTransition(
        State from,
        Event event,
        State to)
    {
        m_transitions[from].push_back({from, event, to});
    }

    bool canHandle(Event event) const
    {
        auto it = m_transitions.find(m_currentState);

        if (it == m_transitions.end())
        {
            return false;
        }

        for (const Transition& transition : it->second)
        {
            if (transition.event == event)
            {
                return true;
            }
        }

        return false;
    }

    bool handleEvent(Event event)
    {
        auto it = m_transitions.find(m_currentState);

        if (it == m_transitions.end())
        {
            return false;
        }

        for (const Transition& transition : it->second)
        {
            if (transition.event == event)
            {
                transitionTo(transition.to);
                return true;
            }
        }

        return false;
    }

    void transitionTo(State newState)
    {
        // if (newState == m_currentState)
        // {
        //     return;
        // }

        const State previousState = m_currentState;

        auto exitIt = m_onExit.find(previousState);

        if (exitIt != m_onExit.end())
        {
            exitIt->second();
        }

        m_currentState = newState;

        auto enterIt = m_onEnter.find(m_currentState);

        if (enterIt != m_onEnter.end())
        {
            enterIt->second();
        }
    }

    void onEnter(
        State state,
        Callback callback)
    {
        m_onEnter[state] = std::move(callback);
    }

    void onExit(
        State state,
        Callback callback)
    {
        m_onExit[state] = std::move(callback);
    }

    const std::unordered_map<State, std::vector<Transition>>&
    getTransitions() const
    {
        return m_transitions;
    }

private:
    State m_currentState;

    std::unordered_map<
        State,
        std::vector<Transition>>
        m_transitions;

    std::unordered_map<State, Callback> m_onEnter;
    std::unordered_map<State, Callback> m_onExit;
};