#pragma once
#include <inttypes.h>
#include <myReadKey/myReadKey.h>

int decode_and_print (int address);
void decode_and_display_bc_ (int lastaddress, int address);
void decode_and_display_bc (int address);
void operation_event (int address);
void register_event (int address);
void counter_event ();
void sat_event ();
void move_event (enum keys *k);
void saveload_event (enum keys *k);
void accumulator_event ();
void reset_event ();
void all_events ();
void quit_event ();
void run_event ();
void step_event ();
void enter_event ();
void event_listener (enum keys *k);
