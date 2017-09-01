// Utilities for TBB tutorial

// Perform CPU burn maths calculations for a number of iterations
double burn(unsigned long iterations);

// Loop over the CPU burner until interval has expired (argument is in *milliseconds*)
void burn_for(float ms_interval);
