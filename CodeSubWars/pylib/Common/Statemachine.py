# Copyright (c) 2005-2022 Andreas Rose. All rights reserved.
# Released under the MIT license. (see license.txt)

#the initial empty context of a statemachine. this is accessible for all states.
class StateMachineContext:
    def __init__(self):
        pass



#the base class that encapsulates a state with methods which are called on regarding 
#transition changes. from this a desired state should be derived.
class State:
    def __str__(self): 
        return self.__class__.__name__
    
    #is executed once if this state becomes the current state.
    #@param statemachinecontext The context of the state machine .
    def enter(self, statemachinecontext):
        pass

    #is executed periodically if this state is the current state.
    #@param statemachinecontext The context of the state machine .
    def update(self, statemachinecontext):
        pass

    #is executed once if this state is the current state and another state
    #becomes the current state.
    #@param statemachinecontext The context of the state machine .
    def exit(self, statemachinecontext):
        pass



#this class provides the functionality of a table driven state machine.
class StateMachine:
    #constructs a state machine.
    #@param initialState The state that is initially the current state.
    #@param transitionTable The table of state transitions that describes the behavior of 
    #                       the state machine. the table consits of tuples for each transition.
    #                       if the first element of tuple is None only the condition is check for 
    #                       valid transition.
    #                          [(StateA, ConditionA, NextStateF),
    #                           (StateF, ConditionB, NextStateA),
    #                           ...
    #                          ]
    #@param doOnTransition A user defined method that is executed on each transition.
    def __init__(self, initialState, transitionTable, doOnTransition = None):
        self.currentState = initialState
        self.transitionTable = transitionTable
        self.doOnTransition = doOnTransition
        self.context = StateMachineContext()

        
    #returns the current state
    def getCurrentState(self):
        return self.currentState


    #this method updates the state machine. that means it checks the transition table whether
    #a valid transition exists. if found one the transition will be done.
    def updateStates(self):
        for (state, condition, nextState) in self.transitionTable:
            if state != None and state != self.currentState: continue
            
            if nextState == None: continue
            
            if condition != None:
                if not condition(self.context): continue
                
            self.enforceTransition(nextState)
            return
        
        self.currentState.update(self.context)
        
        
    #enforces a state transition to the given state. it will not checked whether the given state is
    #valid.
    def enforceTransition(self, nextState):
        self.currentState.exit(self.context)
        
        if self.doOnTransition != None: self.doOnTransition(self.context)
        self.currentState = nextState            
        
        self.currentState.enter(self.context)

