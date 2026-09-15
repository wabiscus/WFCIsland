#pragma once

#include <functional>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <utility>

/**
 * @brief A finite state machine (FSM) implementation
 *
 * @tparam State The type representing the states of the FSM
 * @tparam Event The type representing the events that trigger transitions
 */
template <typename State, typename Event>
class FSM
{
public:
    /**
     * @brief Type alias for a callback function
     */
    using Callback = std::function<void()>;

    /**
     * @brief Struct representing a transition in the FSM
     */
    struct Transition
    {
        State from;
        Event event;
        State to;
    };

public:
    /**
     * @brief Construct a new FSM object
     * @param initialState The initial state of the FSM
     */
    explicit FSM(State initialState)
        : m_currentState(initialState)
    {
    }

    /**
     * @brief Get the current state of the FSM
     * @return The current state
     */
    State getState() const
    {
        return m_currentState;
    }

    /**
     * @brief Check if the FSM is in a specific state
     * @param state The state to check
     * @return True if the FSM is in the specified state, false otherwise
     */
    bool is(State state) const
    {
        return m_currentState == state;
    }

    /**
     * @brief Add a transition to the FSM
     * @param from The source state
     * @param event The event that triggers the transition
     * @param to The target state
     */
    void addTransition(
        State from,
        Event event,
        State to)
    {
        m_transitions[from].push_back({from, event, to});
    }

    /**
     * @brief Check if the FSM can handle a specific event
     * @param event The event to check
     * @return True if the FSM can handle the event, false otherwise
     */
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

    /**
     * @brief Handle an event and transition to a new state if applicable
     * @param event The event to handle
     * @return True if the event was handled and a transition occurred, false otherwise
     */
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

    /**
     * @brief Transition to a new state
     * @param newState The new state to transition to
     */
    void transitionTo(State newState)
    {
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

    /**
     * @brief Set a callback function to be called when entering a specific state
     * @param state The state to set the callback for
     * @param callback The callback function
     */
    void onEnter(
        State state,
        Callback callback)
    {
        m_onEnter[state] = std::move(callback);
    }

    /**
     * @brief Set a callback function to be called when exiting a specific state
     * @param state The state to set the callback for
     * @param callback The callback function
     */
    void onExit(
        State state,
        Callback callback)
    {
        m_onExit[state] = std::move(callback);
    }

    /**
     * @brief Get the transitions of the FSM
     * @return A const reference to the transitions map
     */
    const std::unordered_map<State, std::vector<Transition>>&
    getTransitions() const
    {
        return m_transitions;
    }

private:
    /**
     * @brief The current state of the FSM
     */
    State m_currentState;

    /**
     * @brief The transitions of the FSM
     */
    std::unordered_map<
        State,
        std::vector<Transition>>
        m_transitions;

    /**
     * @brief The callback functions for entering states
     */
    std::unordered_map<State, Callback> m_onEnter;
    /**
     * @brief The callback functions for exiting states
     */
    std::unordered_map<State, Callback> m_onExit;
};