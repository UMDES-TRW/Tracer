# Tracer
Finds paths to deadlocked states

Use Tracer.cpp with blocking-events-full.txt from Stephen Lanham's countBlockingStates to find deadlock states and paths to those states.
Tracer will output two files: Tracer_deadlock_states.txt, which represents the deadlock states, and deadlock_paths.txt, which contains paths to the deadlock states.

If you only want to find deadlock states (and not paths to them), instead use BlockFind.cpp. This will output deadlock_states.txt.

It is not necessary to run BlockFind before running Tracer; BlockFind has been integrated into Tracer.

CompareEvents.cpp is used to find automata that have events in common with a target automaton.

BuildFSM.cpp is used to build an automaton representing a certain path to a deadlock state.

Verify.cpp is used to verify that a deadlock state is blocking.
