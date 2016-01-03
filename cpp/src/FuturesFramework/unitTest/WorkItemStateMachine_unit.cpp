// SYSTEM INCLUDES


// C++ PROJECT INCLUDES
#include "catch/catch.hpp"
#include "FuturesFramework/WorkItemStates.hpp"
#include "FuturesFramework/unitTest/WorkItemStateMachineUnitTestInterface.hpp"

// project namespace
namespace FuturesFramework
{
// component namespace
namespace Tests
{

    TEST_CASE("Construct WorkItemStateMachine", "[WorkItemStateMachine_unit]")
    {
        // save keystrokes for work item states
        States::WorkItemState idle = States::WorkItemState::IDLE;
        States::WorkItemState evalPre =
            States::WorkItemState::EVALUATING_PRECONDITIONS;
        States::WorkItemState execMain =
            States::WorkItemState::EXECUTING_MAIN_FUNCTION;
        States::WorkItemState execPost =
            States::WorkItemState::EXECUTING_POSTERIOR_FUNCTION;
        States::WorkItemState schedule =
            States::WorkItemState::SCHEDULE;
        States::WorkItemState reschedule =
            States::WorkItemState::RESCHEDULE;
        States::WorkItemState done = States::WorkItemState::DONE;

        WorkItemStateMachineUnitTestInterface
            idleMachine(idle), evalPreMachine(evalPre),
            execMainMachine(execMain), execPostMachine(execPost),
            schedMachine(schedule), reschedMachine(reschedule),
            doneMachine(done);

        REQUIRE( idleMachine.GetCurrentState() == idle );
        REQUIRE( evalPreMachine.GetCurrentState() == evalPre );
        REQUIRE( execMainMachine.GetCurrentState() == execMain );
        REQUIRE( execPostMachine.GetCurrentState() == execPost );
        REQUIRE( schedMachine.GetCurrentState() == schedule );
        REQUIRE( reschedMachine.GetCurrentState() == reschedule );
        REQUIRE( doneMachine.GetCurrentState() == done );
    }

    TEST_CASE("Testing SetState", "[WorkItemStateMachine_unit]")
    {
        // save keystrokes for work item states
        States::WorkItemState idle = States::WorkItemState::IDLE;
        States::WorkItemState evalPre =
            States::WorkItemState::EVALUATING_PRECONDITIONS;
        States::WorkItemState execMain =
            States::WorkItemState::EXECUTING_MAIN_FUNCTION;
        States::WorkItemState execPost =
            States::WorkItemState::EXECUTING_POSTERIOR_FUNCTION;
        States::WorkItemState schedule =
            States::WorkItemState::SCHEDULE;
        States::WorkItemState reschedule =
            States::WorkItemState::RESCHEDULE;
        States::WorkItemState done = States::WorkItemState::DONE;

        WorkItemStateMachineUnitTestInterface
            stateMachine(States::WorkItemState::IDLE);

        stateMachine.SetState(evalPre);
        REQUIRE( stateMachine.GetCurrentState() == evalPre );

        stateMachine.SetState(idle);
        REQUIRE( stateMachine.GetCurrentState() == idle );

        stateMachine.SetState(execMain);
        REQUIRE( stateMachine.GetCurrentState() == execMain );

        stateMachine.SetState(execPost);
        REQUIRE( stateMachine.GetCurrentState() == execPost );

        stateMachine.SetState(schedule);
        REQUIRE( stateMachine.GetCurrentState() == schedule );

        stateMachine.SetState(reschedule);
        REQUIRE( stateMachine.GetCurrentState() == reschedule );

        stateMachine.SetState(done);
        REQUIRE( stateMachine.GetCurrentState() == done );
    }

    TEST_CASE("Exercise State Machine", "[WorkItemStateMachine_unit]")
    {
        WorkItemStateMachineUnitTestInterface
            stateMachine(States::WorkItemState::IDLE);

        // save keystrokes for triggers
        Types::Result_t success = Types::Result_t::SUCCESS;
        Types::Result_t failure = Types::Result_t::FAILURE;

        // save keystrokes for work item states
        States::WorkItemState idle = States::WorkItemState::IDLE;
        States::WorkItemState evalPre =
            States::WorkItemState::EVALUATING_PRECONDITIONS;
        States::WorkItemState execMain =
            States::WorkItemState::EXECUTING_MAIN_FUNCTION;
        States::WorkItemState execPost =
            States::WorkItemState::EXECUTING_POSTERIOR_FUNCTION;
        States::WorkItemState schedule =
            States::WorkItemState::SCHEDULE;
        States::WorkItemState reschedule =
            States::WorkItemState::RESCHEDULE;
        States::WorkItemState done = States::WorkItemState::DONE;

        SECTION("Moving to \"Scheduled\" state")
        {
            stateMachine.Trigger(success);

            REQUIRE( stateMachine.GetCurrentState() == schedule );

            SECTION("Moving to \"Evaluating Preconditions\" state")
            {
                stateMachine.Trigger(success);

                REQUIRE( stateMachine.GetCurrentState() == evalPre );

                SECTION("Moving to \"Executing Main\" state")
                {
                    stateMachine.Trigger(success);

                    REQUIRE( stateMachine.GetCurrentState() == execMain );

                    SECTION("Moving to \"Executing Posterior\" state")
                    {
                        stateMachine.Trigger(success);

                        REQUIRE( stateMachine.GetCurrentState() == execPost );

                        SECTION("Moving to \"Done\" state with SUCCESS")
                        {
                            stateMachine.Trigger(success);

                            REQUIRE( stateMachine.GetCurrentState() == done );
                        }

                        SECTION("Moving to \"Done\" state with FAILURE")
                        {
                            stateMachine.Trigger(failure);

                            REQUIRE( stateMachine.GetCurrentState() == done );
                        }
                    }

                    SECTION("Moving to \"Done\" state")
                    {
                        stateMachine.Trigger(failure);

                        REQUIRE( stateMachine.GetCurrentState() == done );

                        SECTION("Staying at \"Done\" state with SUCCESS")
                        {
                            stateMachine.Trigger(success);

                            REQUIRE( stateMachine.GetCurrentState() == done );
                        }

                        SECTION("Staying at \"Done\" state with FAILURE")
                        {
                            stateMachine.Trigger(failure);

                            REQUIRE( stateMachine.GetCurrentState() == done );
                        }
                    }
                }

                SECTION("Moving to \"Reschedule\" state")
                {
                    stateMachine.Trigger(failure);

                    REQUIRE( stateMachine.GetCurrentState() == reschedule );

                    SECTION("Moving to \"Evaluating Preconditions\" state")
                    {
                        stateMachine.Trigger(success);

                        REQUIRE( stateMachine.GetCurrentState() == evalPre );
                    }

                    SECTION("Not changing \"Reschedule\" state")
                    {
                        stateMachine.Trigger(failure);

                        REQUIRE( stateMachine.GetCurrentState() == reschedule );
                    }
                }
            }

            SECTION("Not changing SCHEDULE state")
            {
                stateMachine.Trigger(failure);

                REQUIRE( stateMachine.GetCurrentState() == schedule );
            }
        }
        SECTION("Not changing \"IDLE\" state")
        {
            stateMachine.Trigger(failure);

            REQUIRE( stateMachine.GetCurrentState() == idle );
        }
    }

    TEST_CASE("Cancelling State Machine Operations", "[WorkItemStateMachine_unit]")
    {
        // save keystrokes for work item states
        States::WorkItemState idle = States::WorkItemState::IDLE;
        States::WorkItemState evalPre =
            States::WorkItemState::EVALUATING_PRECONDITIONS;
        States::WorkItemState execMain =
            States::WorkItemState::EXECUTING_MAIN_FUNCTION;
        States::WorkItemState execPost =
            States::WorkItemState::EXECUTING_POSTERIOR_FUNCTION;
        States::WorkItemState schedule =
            States::WorkItemState::SCHEDULE;
        States::WorkItemState reschedule =
            States::WorkItemState::RESCHEDULE;
        States::WorkItemState done = States::WorkItemState::DONE;

        WorkItemStateMachineUnitTestInterface
            idleMachine(idle), evalPreMachine(evalPre),
            execMainMachine(execMain), execPostMachine(execPost),
            schedMachine(schedule), reschedMachine(reschedule),
            doneMachine(done);

        // we only allow non-executing states to be cancelled.
        // Otherwise, there might be memory leaks or something
        // due to partial execution of client code. So,
        // executing states will continue.
        idleMachine.Cancel();       // goto DONE state
        evalPreMachine.Cancel();    // no change
        execMainMachine.Cancel();   // no change
        execPostMachine.Cancel();   // no change
        schedMachine.Cancel();      // goto DONE state
        reschedMachine.Cancel();    // goto DONE state
        doneMachine.Cancel();       // remain at DONE state

        REQUIRE( idleMachine.GetCurrentState() == done );
        REQUIRE( evalPreMachine.GetCurrentState() == evalPre );
        REQUIRE( execMainMachine.GetCurrentState() == execMain );
        REQUIRE( execPostMachine.GetCurrentState() == execPost );
        REQUIRE( schedMachine.GetCurrentState() == done );
        REQUIRE( reschedMachine.GetCurrentState() == done );
        REQUIRE( doneMachine.GetCurrentState() == done );
    }

} // end of namespace Tests
} // end of namespace FuturesFramework