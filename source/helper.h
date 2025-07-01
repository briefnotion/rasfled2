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

#include <iostream>
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
  // Sets HAS_FALSE = true if Value is false.

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

  unsigned long TIME_CREATED = 0;
  unsigned long TIME_ENDED   = 0;
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

  void store_value(float Value, unsigned long tmeFrame_Time);
  void store_value(float Value);
  // Store value and updates min max.

  //void merge(MIN_MAX_TIME_SLICE &Other_Time_Slice);
  // Merges another MIN_MAX_TIME_SLICE into this one.

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

class MIN_MAX_TIME_SLICE_DOUBLE
// Simple min max data point.
{
  private:

  double TIME_CREATED = 0;
  double TIME_ENDED   = 0;
  float VALUE = 0;
  int SAMPLES = 0;
  float MIN_VALUE = 0;
  float MAX_VALUE = 0;

  bool PLACEHOLDER = false;

  public:

  void set_as_placeholder();
  // Retains the data if true but will clear data if updated
  //  Allows time slice to still exist briefly if all its data 
  //  is merged out.  Only set to true by external merge.

  double time_created();
  double time_ended();

  void clear(double tmeFrame_Time);
  // Resets value to be used again as new.

  void store_value(float Value, double tmeFrame_Time);
  void store_value(float Value);
  // Store value and updates min max.

  void merge_into(MIN_MAX_TIME_SLICE_DOUBLE &Other_Time_Slice);
  // Merges another MIN_MAX_TIME_SLICE into this one.

  float total();
  // Total Value of all samples

  int samples();
  // Number of samples in slice.

  bool is_placeholder();
  // Returns true if placeholder flag is set.

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
// Average value is calculated at read time via impact(Time).
// Should not need to recalculate answer if New Time of impact read is not significant 
//  or data hasnt changed.
//
// Problem: If SIZE = 3 and ALIVE_TIME = 150 and entries are very 10 ms, then 
//  impact calls will only return average over previous 30 ms.
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

  int collection_size();
  // for debugging purposes, returns size of IMPACT_RESISTANCE_VALUE queue.

  void set_size(int Size);
  // sets size of IMPACT_RESISTANCE_VALUE queue

  void set_alive_time(unsigned long Alive_Time);
  // When calculating, discards items older than time allowed
  // dont forget to corelate size with time.

  void set_value(unsigned long Time, float Value);
  // enteres a new value

  float latest();
  // returns last value entered into the queue.

  float impact(unsigned long Time);
  // Runs through all calculations.

};

// ---------------------------------------------------------------------------------------


// ***************************************************************************************
// FUNCTION AND PROCEDURES
// ***************************************************************************************

// ---------------------------------------------------------------------------------------

// Debug Walker
float walker_float(unsigned long Time, float One_Second_Value, float Max_value);
// Time based value walker for debugging. 
// Max_value not yet implemented.

// Random Number
int intRandomHD(int intBase);

// Some time display type things
int millis_to_time_minutes(long millis_time);

int millis_to_time_seconds(long millis_time);

int get_frame_interval(int Every_X_ms);

int radio_translate_to_frequency_6(float Frequency);

float temperture_translate_celcius_to_fahrenheit(float celcius);

float velocity_translate_kmph_to_mph(float kmph);

float velocity_translate_Miph_to_mps(float Miph);

float velocity_translate_kmph_to_meters_per_second(float kmph);

float velocity_translate_meters_per_second_to_kmph(float mps);

float velocity_translate_knots_to_kmph(float knots);

float pressure_translate_kPa_to_inHg(float kPa);

bool emperical_mean(vector<float> &Number_List, float Deviations, 
                    int Required_Qualifying_Dataset_Count, float &Ret_Mean, 
                    float &Ret_Mean_Difference);
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

float no_roll_difference(float low_value, float high_value, float roll_value);
// Gets the difference between two values in scenerios where the low value
//  can be below the rollover value and the high value can be above the 
//  rollover value.
//  Example in a compass, low value can be 359 and high value can be 01. 
//    Results should be 2.
//  Will not decern true low and high values.  Currently not necessary.

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

// ---------------------------------------------------------------------------------------


// ***************************************************************************************
// More Classes
// ***************************************************************************************

class EMPERICAL_PROPERTIES
{
  public:

  int VALUE_SIZE = 50;

  float DEVIATIONS = 2.0f;
  //  1 - 68% of the data falls within one standard deviation of the mean.
  //  2 - 95% of the data falls within two standard deviations of the mean.
  //  3 - 99.7% of the data falls within three standard deviations of the mean.
};

class EMPERICAL
{
  private:

  vector<float> VALUES;

  float LATEST_MEAN = -1.0f;
  float LATEST_MEAN_DIFFERENCE;

  bool FIRST_RUN = true;
  bool CHANGED = true;

  public:

  EMPERICAL_PROPERTIES PROPS;

  void add_value(float Value);
  float mean();
};


// ---------------------------------------------------------------------------------------
// VECTOR DEQUE

// Untested Idea for later.
class VECTOR_DEQUE_INT
{
  vector<int> Data;

  int FRONT = 0;
  int BACK = 0;

  int FULL_SIZE = 0;

  int get_vector_position(int Position);

  public:

  void set_size(int Size);

  //void pop_front();

  int size();

  void push_back(int Value);

  int value(int Position);

};

// ---------------------------------------------------------------------------------------
// VECTOR DEQUE 

template <typename T>
class VECTOR_DEQUE
{
  vector<T> DATA;
  int FRONT = 0;
  int BACK = 0;
  int COUNT = 0;
  int FULL_SIZE = 10;

  int get_vector_position(int Position)
  {
    return (FRONT + Position) % FULL_SIZE;
  }

public:
  void set_size(int New_Capacity)
  {
    vector<T> new_data(New_Capacity);
    int num_elements_to_copy = min(COUNT, New_Capacity);

    for (int pos = 0; pos < num_elements_to_copy; pos++)
    {
      //new_data = value(pos);
      new_data.push_back(DATA[pos]);
    }

    DATA = move(new_data);
    FULL_SIZE = New_Capacity;
    FRONT = 0;
    BACK = num_elements_to_copy % FULL_SIZE;
    COUNT = num_elements_to_copy;
  }

  int size()
  {
    return COUNT;
  }

  void push_back(const T& Value)
  {
    if (COUNT == FULL_SIZE)  // Full, overwrite oldest
    {
      FRONT = (FRONT + 1) % FULL_SIZE;
    }
    else
    {
      COUNT++;
    }

    DATA[BACK] = Value;
    BACK = (BACK + 1) % FULL_SIZE;
  }

  T& operator[](int pos) 
  {
    if (pos < 0 || pos >= COUNT)
    {
      throw out_of_range("Index out of range in VECTOR_DEQUE");
    }
    return DATA[get_vector_position(pos)];
  }
};

// ---------------------------------------------------------------------------------------
// VECTOR DEQUE NON SEQUENTIAL

class VECTOR_DEQUE_NON_SEQUENTIAL_FLAGS
{
  public:
  bool HAS_DATA = false;
  bool DO_NOT_OVERWRITE = false;
};

template <typename T>
class VECTOR_DEQUE_NON_SEQUENTIAL
{
  vector<T>     DATA;
  int BACK = 0;
  int COUNT = 0;
  int FULL_SIZE = 10;

  public:
  vector<VECTOR_DEQUE_NON_SEQUENTIAL_FLAGS>  FLAGS;

  void set_size(int New_Capacity)
  {
    vector<T>     new_data(New_Capacity);
    vector<VECTOR_DEQUE_NON_SEQUENTIAL_FLAGS>  new_flags(New_Capacity);

    FULL_SIZE = New_Capacity;
    BACK = 0;
    COUNT = 0;
    
    DATA  = move(new_data);
    FLAGS = move(new_flags);

    /*
    int new_count = 0;

    for (int pos = 0; pos < New_Capacity; pos++)
    {
      if (pos < (int)DATA.size())
      {
        new_data[pos] = DATA[pos];
        new_flags[pos] = FLAGS[pos];
        if (FLAGS[pos].HAS_DATA)
        {
          new_count++;
        }
      }
      else
      {
        new_flags[pos].HAS_DATA = false;
        new_flags[pos].DO_NOT_OVERWRITE = false;
      }
    }

    DATA  = move(new_data);
    FLAGS = move(new_flags);

    FULL_SIZE = New_Capacity;
    BACK = 0;
    COUNT = new_count;
    */
  } 

  int count()
  {
    return COUNT;
  }

  size_t size()
  {
    return DATA.size();
  }

  void push_back(const T& Value)
  {
    if (COUNT == FULL_SIZE)  // Full, overwrite oldest
    {
      int position_found = false;

      for (size_t loc = 0; loc < DATA.size() && position_found == false; loc++)
      {
        if (FLAGS[(BACK + loc) %FULL_SIZE].DO_NOT_OVERWRITE == false)
        {
          position_found = true;

          BACK = (BACK + loc) %FULL_SIZE;

          DATA[BACK] = Value;
          FLAGS[BACK].HAS_DATA = true;
          FLAGS[BACK].DO_NOT_OVERWRITE = false;
        }
      }

      if (position_found == false)
      {
        DATA[BACK] = Value;
        FLAGS[BACK].HAS_DATA = true;
        FLAGS[BACK].DO_NOT_OVERWRITE = false;
      }
      
      BACK = (BACK + 1) % FULL_SIZE;

    }
    else
    {
      // Find first Deleted
      bool replaced = false;
      for (size_t pos = 0; (pos < DATA.size()) && (replaced == false); pos++)
      {
        if (FLAGS[pos].HAS_DATA == false)
        {
          replaced = true;
          DATA[pos] = Value;
          FLAGS[pos].HAS_DATA = true;
          FLAGS[pos].DO_NOT_OVERWRITE = false;
          COUNT++;
        }
      }
    }
  }

  void erase_p(int pos)
  {  
    /*
    if (pos < 0 || pos >= (int)FLAGS.size())
    {
      cout << "Index out of range in erase_p" << endl;
      throw out_of_range("Index out of range in erase_p");
    }
    */
    
    if (FLAGS[pos].HAS_DATA)
    {
      FLAGS[pos].HAS_DATA = false;
      FLAGS[pos].DO_NOT_OVERWRITE = false;
      COUNT--;
    }
  }

  T& operator[](int pos) 
  {
    /*
    if (pos < 0 || pos >= COUNT)
    {
      cout << "Index out of range in VECTOR_DEQUE_NON_SEQUENTIAL" << endl;
      throw out_of_range("Index out of range in VECTOR_DEQUE_NON_SEQUENTIAL");
    }
    */
    return DATA[pos];
  }
};

#endif