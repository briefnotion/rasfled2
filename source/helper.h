// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: helper.h
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2858 Core Dynamics
// ***************************************************************************************

#ifndef HELPER_H
#define HELPER_H

#include <deque>
#include <vector>
#include <string>
#include <chrono>

#include <cmath>

#include "helper.h"
#include "definitions.h"
#include "stringthings.h"
#include "fled_time.h"

using namespace std;

// ***************************************************************************************
// Global Definitions
// ***************************************************************************************

# define float_PI		3.14159265358979323846f   // pi
# define float_EARTH_RADIUS		3963.191f       // Earth Radius


// ***************************************************************************************
// STRUCTURES AND CLASSES
// ***************************************************************************************

// -------------------------------------------------------------------------------------

class FLOAT_XYZ
{
  public:

  float X = 0;
  float Y = 0;
  float Z = 0;
};

// -------------------------------------------------------------------------------------
// Simple 2D Ref Map

class stupid_2d
// does nothing but keep track of x with y value.
// x cant change.  just increases everytime an add of y is put into it.
// this thing is just easier to mananage than a mallaloced x,y dimed array.
{
  private:
  deque<int> X;

  public:

  void add(int y);

  int x_size();

  int y(int x);

  void clear();
};



// -------------------------------------------------------------------------------------
// Countdown Timer

class countdown_timer
{
  private:
  bool ACTIVE = false;
  
  bool TRIGGERED = false;
  bool TRIGGER_REPORTED = false;

  bool CHECKED = false;

  unsigned long TIME_START;
  unsigned long TIME_END;
  unsigned long DURATION;

  public:
  void set_timer(unsigned long Current_Time_millis, int Seconds);
  // Starts a timer with the Current_Time_millis to be triggered in 
  //  Seconds.

  bool is_active();
  // Returns true of timer is rinning.

  void update(unsigned long Current_Time_millis);
  // Updates values based on time.

  bool is_triggered();
  // Returns true if the time passed the countdown timer 
  //  threshold.
  // Returns false countdown still running.
  // Also, returns false if trigger previously reported true.

  void end();
  // Resets the timer.

  unsigned long duration();
  // returns duration of timer set at start.

  long elapsed_time(unsigned long Current_Time_millis);
  // returns amount of time passed from start, in milliseconds.

  float timer_position(unsigned long Current_Time_millis);
  // returns percentage of time passed, from start to finish.
  // e.g.  0  = no passed time.
  //      .5  = half way complete.
  //      .95 = 95% complete, 5% remaining time.
  //      Will not report any value > 1.
};

// -------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------
// Stupid Random Generator Structure
class stupid_random
//  This is a simple solution to random numbers not being randomly generated.
//    Generate a string of random numbers to be used and referenced throughout the
//    entirity of the program run.
{
  private:
  char  stupidnumbers[StuRNDsize];

  public:
  void set();

  // Return random char from list based on seed.
  char getB(unsigned long seed, int size);

  // Return random unsigned long from list based on seed.
  //  Number will evenly distributed from other numbers 
  //    based on list size.  Precision is way off, but should
  //    be random
  // Size is the Upper Limit. Lower Limit is 0.
  unsigned long getUL(unsigned long seed, int size);
};

class VAR_CHANGE_MON
// Check to see if a value of an integer has changed.
// Return true and reset if it has.
// Retrun false if it is the same.
{
  private:

  int PREV_VALUE = 0;

  bool switched(int Value);
};

// ---------------------------------------------------------------------------------------
// Hardware Monitor Class
class switch_map
// A simple switch to pin reference map
{
  // Referenced only by position in list.
  public:

  int     pin = 0;          // Hardware PIN Number
  string  SWITCH_NAME = ""; // Switch Name
  bool    value = false;    // 
};

class hardware_monitor
// Create a hardware switch or button varable type.
{
  public:
  // External
  bool booVALUE = false;
  unsigned long tmeTOGGLEDTIME = 0;
  bool ISHARDWARE = false;

  private:
  // Internal
  unsigned long tmeCHANGEDETECTEDTIME = 0;
  bool booPREVCHANGEDETECTED = false;
  unsigned int tmeLEEWAY = 0;
  bool booFIRSTRUN = true;

  public:
  void set(bool booValue, unsigned long tmeCheckTime, int tmeLeeWay, bool isHardware);

  bool changed(bool booValue, unsigned long tmeCheckTime);

  void read(bool booValue, unsigned long tmeCheckTime);
};
// ---------------------------------------------------------------------------------------

class STAT_DATA_DOUBLE
// Provide minor min max stats over time of resets.
// If real stats ever necessary, then weighted average 
//  routine needed.
{
  private:

  bool set = true;

  double DATA  = 0;
  double MIN   = 0;
  double MAX   = 0;

  public:
  void set_data(double data);

  double get_data();

  double get_min();

  double get_max();

  void reset_minmax();
};
// ---------------------------------------------------------------------------------------

class EFFICIANTCY_TIMER
// Measures time passed between calls. 
{
  private:
  double LAST_ASKED_TIME = 0;
  double TIMER_STARTED   = 0;

  public:

  void start_timer(double dblCurrent_Time);
  // Start the timer (stopwatch) by setting its the stopwatch time.
  //  The timer is a simple and can be considered always active. 

  double elapsed_timer_time(double dblCurrent_Time);
  //  Returns the amount of time passed since the reset. 

  double elapsed_time(double dblCurrent_Time);
  // Measures the amount of time elasped since the previous time the function was 
  //  called, then returns the value, then resets for next time. 

  double simple_elapsed_time(double dblCurrent_Time);
  // Only returns the calc of current time - start timer time.
};
// ---------------------------------------------------------------------------------------

class TRUTH_CATCH
// Simple Variable to track if a TRUE value has entered.
{
  private:

  bool HAS_TRUTH = false;

  public:

  void catch_truth(bool Value);
  // Sets HAS_TRUTH = true if Value is true.

  bool has_truth();
  // Returns true a true value was caught in the catch truth routine.
  //  Resets HAS_TRUTH = false after called.
  // Returns false if all catch_truths were false.
};

class FALSE_CATCH
// Simple Variable to track if a FALSE value has entered.
{
  private:

  bool HAS_FALSE = false;

  public:

  void catch_false(bool Value);
  // Sets HAS_TRUTH = true if Value is true.

  bool has_false();
  // Returns true a false value was caught in the catch false routine.
  //  Resets HAS_FALSE = false after called.
  // Returns false if all catch_truths were true.
};

class BOOL_WITH_OVERRIDE
// Bool variable with method to set from seperate sources, dependant on condition. 
// Value work as normal.
// Overridden reveals source of value.
{
  private:

  bool VALUE = false;
  bool OVERRIDDEN = false;

  public:

  void set(bool Override_Condition, bool Override_Value, bool Redundant_Value);
  bool value();
  bool overridden();
};

class BOOL_BOUNCE
// BOOL variable with bounce (change) indication.
// Set and Value work as normal.
// First call to Bounce will return true if variable changed.
// Subsequent calls to Bounce will return false until variable changes again.
{
  private:

  bool VALUE = false;
  bool CHANGED = false;

  public:

  void set(bool Value);
  bool value();
  bool bounce();
};

// ---------------------------------------------------------------------------------------
// Classes

struct MIN_MAX_SIMPLE
{
  public:

  float MIN_VALUE = 0;
  float MAX_VALUE = 0;
};

struct MIN_MAX_TIME_SLICE_SIMPLE
{
  public:

  float MEAN_VALUE = 0;
  float MIN_VALUE = 0;
  float MAX_VALUE = 0;
  unsigned long TIME_CREATED;
};

// ---------------------------------------------------------------------------------------

class MIN_MAX_TIME_SLICE
// Simple min max data point.
{
  private:

  unsigned long TIME_CREATED;
  bool ACTIVE = false;
  float VALUE = 0;
  int SAMPLES = 0;
  float MIN_VALUE = 0;
  float MAX_VALUE = 0;

  int MIN_MAX_TIME_SPAN = 0;

  public:

  unsigned long time_created();

  void clear(unsigned long tmeFrame_Time);
  // Resets value to be used again as new.

  void store_value(float Value);
  // Store value and updates min max.

  float total();
  // Total Value of all samples

  int samples();
  // Number of samples in slice.

  float mean();
  // Returns mean value of stored data points.

  float min();
  // Returns min value of data point.

  float max();
  // Returns max value of data point.
};

// ---------------------------------------------------------------------------------------

class MIN_MAX_TIME_PROPERTIES
// Min Max list properties.
{
  public:

  int SLICES = 0;                     // Number of slices to retain
  unsigned long TIME_SPAN = 0;                  // TOTAL TIME
                                      // Elaped time from first to last slice.
                                      //  e.g: 10,000ms over 5 slices = 1 slice per 2 sec
                                      // Measured in ms.  1000 = 1 sec.
  float DIRECTION_NUTRAL_RANGE = 0;   // Average value change over number of 
                                      //  slice to register difference.

  bool SAMPLE_LIMITED_SPANS = false;
  int SAMPLE_LIMIT = 50;
};

class MIN_MAX_TIME
// Contains list of min max time slices.
// Note:
//  Values can be imprecise because values aren't
//    measured seperately accross time spans.
//    Only first or most recent value saved.
//  Missing time is not taken into account.
{
  private:

  bool ENABLED = false;
  unsigned long SLICE_TIME = 0;

  deque<MIN_MAX_TIME_SLICE> TIME_SLICES;

  unsigned long TIME_SLICE_CREATED_FRAME_TIME = 0;

  void create();

  unsigned long old_expired_frames_check_time = 0;

  void remove_old_expired_frames(unsigned long tmeFrame_Time);

  public:

  MIN_MAX_TIME_PROPERTIES PROP;

  void put_value(float Value, unsigned long tmeFrame_Time);
  // Stores value in time slice
  //  Creates new slice if slice time has passed.

  int slice_size();
  // Return count of slices in use.

  int slice_size_max();
  // Return count of slices in use.

  float min_float();
  // Returns Min value of variable over time slice
  //  as float value.

  int min();
  // Returns Min value of variable over time slice
  //  as int value.

  float max_float();
  // Returns Max value of variable over time slice
  //  as float value.

  int max();
  // Returns Max value of variable over time slice
  //  as int value.

  float mean_float();
  // Returns Mean value of variable over time slice
  //  as float value.

  int direction();
  // Returns direction (pos, same, neg) of varible over time slice.
  //  period. 
  //  Returns -1 if value is falling.
  //  Returns 0 if value is relatively (DIRECTION_NUTRAL_RANGE) the same. 
  //  Returns 1 if value is increasing.
};

// ---------------------------------------------------------------------------------------
// Impact Resistance

struct IMPACT_RESISTANCE_VALUE
// For each letter, assign a behavior and type for it.
{
  float VALUE;
  unsigned long ENTRY_TIME;
};

class IMPACT_RESISTANCE_FLOAT_PROPERTIES
// Not adjustable after routine starts.
{
  public:

  int SIZE = 10;                    // Max Size of Entries
  unsigned long ALIVE_TIME = 150;   // Measured in ms
  
  // With Standard Deviations
  // 1 - About 68% of the data falls within one standard deviation from the mean.
  // 2 - About 95% falls within two standard deviations.
  // 3 - About 99.7% falls within three standard deviations.
  
  float EMPERICAL_RULE_ENABLE = false;
  int EMPERICAL_RULE_DEVIATIONS = 2;
};

class IMPACT_RESISTANCE_FLOAT
// Floating Point varibale designed to average out the previos X entries into one
//  value.  For fast and sparatic values.
// Problem: CPU INTENSIVE. Does not have a simple routine to get the value.  Value 
//  is calculated via for loop where all values must be read to deteimine answer.
//  Cant change the size without reconstructing the entire variable.
{
  private:

  vector<IMPACT_RESISTANCE_VALUE> VALUE_COLLECTION;

  bool FIRST_RUN = true;

  int LATEST_POSITION = 0;

  float LATEST_VALUE = 0.0f;

  bool CHANGED = true;
  unsigned long OLDEST_ENTRY_TIME = 0;

  public:

  IMPACT_RESISTANCE_FLOAT_PROPERTIES PROPS;

  void set_size(int Size);

  void set_alive_time(unsigned long Alive_Time);

  void set_value(unsigned long Time, float Value);

  float latest();

  float impact(unsigned long Time);

};

// ---------------------------------------------------------------------------------------


// ***************************************************************************************
// FUNCTION AND PROCEDURES
// ***************************************************************************************

// ---------------------------------------------------------------------------------------
// Random Number
int intRandomHD(int intBase);

// Some time display type things
int millis_to_time_minutes(long millis_time);

int millis_to_time_seconds(long millis_time);

int get_frame_interval(int Frames_Per_Second);

int radio_translate_to_frequency_6(float Frequency);

float velocity_translate_kmph_to_mph(float kmph);

float velocity_translate_Miph_to_mps(float Miph);

float velocity_translate_kmph_to_meters_per_second(float kmph);

float velocity_translate_meters_per_second_to_kmph(float mps);

float velocity_translate_knots_to_kmph(float knots);

float pressure_translate_kPa_to_inHg(float kPa);

bool emperical_mean(vector<float> &Number_List, float Deviations, 
                          int Qualifying_Count, float &Ret_Mean, 
                          float &Qualifying_Value);
// Sets Pass_Qualify to false if count of valid data points is 
//  less than Qualifying_Limit.

float emperical_mean(vector<float> &Number_List, float Deviations);
// Imperical Rule Mean of Vector
//
// Deviations:
//  1 - 68% of the data falls within one standard deviation of the mean.
//  2 - 95% of the data falls within two standard deviations of the mean.
//  3 - 99.7% of the data falls within three standard deviations of the mean.

FLOAT_XYZ emperical_mean_zyz(vector<FLOAT_XYZ> &Number_List, float Deviations);
// Simple XYZ Meam
// Does not filter out outliers on x y z passes.

int position_of_scale(int size, int scale, int value);
// Calculate position of value on scale (eg 100%) with size.
//  Will not return anything larger than size.

bool is_within(int Value, int Number_1, int Number_2);
// Comarison opperator returns true if Value is in between  
//  or equal to Num 1 and Num 2, regardless if Number 1 
//  is less than or greater than Number 2.

bool is_within(float Value, float Number_1, float Number_2);
// Comarison opperator returns true if Value is in between  
//  or equal to Num 1 and Num 2, regardless if Number 1 
//  is less than or greater than Number 2.

bool set_bool_with_change_notify(bool Set_Value, bool &Bool_Variable);
// Set or copy Set_Value to Bool_Variable
// Returns true if value changed
// Returns false if value was the same.

int get_1_pos_of_int(int Number);

int get_2_pos_of_int(int Number);

float mod_decimal_remainder(float Value, int Mod_Value);
// Return Mod Decimal Remainder.

bool get_bit_value(int baseline, int bit_set_compare);
// Compares Baseline with bitwise of bit_set_compare.
// Returns true if bit is same else returns false.
// eg: D8, check for bit 0001000 is:
//   if (get_bit_value(216, 8) == true)
// Needs Depreciation

bool bit_value(unsigned char Value, int Bit_Location);
//  Checks if a bit at a given position in a byte is 
//    on or off, returning true or false.

int two_byte_complement_signed(unsigned char byte1, unsigned char byte2);
//  Bytes entwined, cybermorphing defined,
//  Unsigned to signed, in the neon's grind.
//  2's complement, the dark code we bind,
//  Params byte1 (LSB), byte2 (MSB), the data we find.
//  Returns a hacked int, the system undermined.

string file_format_system_time();
// Return format "20230628_22.44.39"

string file_format_system_date();
// Return format "20230628_22.44.39"

string file_format_system_hour_minutes_seconds();
// Return format "20230628_22.44.39"

#endif