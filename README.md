# Tracer
Finds paths to deadlocked states

Use BlockFind.cpp to find the deadlock states, using blocking-events-full.txt from Stephen Lanham's blocking search as input.
This will output deadlock_states.txt, a file containing a list of deadlock states.
Use Tracer.cpp with blocking-events-full.txt and deadlock_states.txt as inputs to find paths to the deadlock states.
This will output deadlock_paths.txt, a file containing a list of paths to deadlock states.
