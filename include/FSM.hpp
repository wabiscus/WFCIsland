#pragma once

#include <functional>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include <vector>

enum class GenerationState
{
    Empty,
    Shape,
    BoundariesDefined,
    Generating,
    Generated
};

template <typename State>
class FSM
{
public:
    using StateType = State;
    using Callback = std::function<void()>;

    struct Transition
    {
        State from;
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

    void addTransition(State from, State to)
    {
        m_transitions[from].push_back(to);
    }

    bool canTransitionTo(State state) const
    {
        auto it = m_transitions.find(m_currentState);

        if (it == m_transitions.end())
        {
            return false;
        }

        for (State target : it->second)
        {
            if (target == state)
            {
                return true;
            }
        }

        return false;
    }

    bool transitionTo(State state)
    {
        if (!canTransitionTo(state))
        {
            return false;
        }

        State previousState = m_currentState;

        auto callbackIt = m_exitCallbacks.find(previousState);

        if (callbackIt != m_exitCallbacks.end())
        {
            callbackIt->second();
        }

        m_currentState = state;

        callbackIt = m_enterCallbacks.find(m_currentState);

        if (callbackIt != m_enterCallbacks.end())
        {
            callbackIt->second();
        }

        return true;
    }

    void onEnter(State state, Callback callback)
    {
        m_enterCallbacks[state] = std::move(callback);
    }

    void onExit(State state, Callback callback)
    {
        m_exitCallbacks[state] = std::move(callback);
    }

private:
    State m_currentState;

    std::unordered_map<State, std::vector<State>> m_transitions;

    std::unordered_map<State, Callback> m_enterCallbacks;
    std::unordered_map<State, Callback> m_exitCallbacks;
};