#include "widgets/AutoCQState.hpp"
#include <cstdlib>
#include <iostream>

static void check (bool condition, char const * what)
{
  if (!condition) { std::cerr << what << '\n'; std::exit (1); }
}

int main ()
{
  AutoCQState state;
  check (!state.ready (30000, false, false), "No spontaneous startup TX");
  state.request (0, 15000, true);
  check (!state.ready (30000, false, false), "Must observe actual transmission end");
  state.transmissionEnded (13000, 15000);
  check (!state.ready (14000, false, false), "Must not restart within the final TX slot");
  check (!state.ready (15000, true, false), "PTT tail blocks restart");
  check (!state.ready (30000, false, true), "Manual log dialog blocks restart");
  check (state.ready (30000, false, false), "Restart once TX and logging complete");
  state.cancel ();
  state.transmissionEnded (31000, 15000);
  check (!state.ready (60000, false, false), "Stop cannot be undone by delayed TX end");

  state.request (32000, 15000, false); // Received 73 after our RRR.
  state.request (44000, 15000, false); // Duplicate decode must not postpone CQ.
  check (state.ready (45000, false, false), "RX completion and duplicates");
  state.cancel ();
  state.request (7500, 7500, true); // FT4
  state.transmissionEnded (13000, 7500);
  state.transmissionEnded (14999, 7500);
  check (state.ready (15000, false, false), "FT4 period and duplicate end event");
  state.cancel ();
  state.request (0, 0, false);
  check (!state.pending (), "Invalid period rejected");
  std::cout << "Auto CQ state checks passed\n";
}
