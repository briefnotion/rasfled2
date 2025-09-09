// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: json_interface.h
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2858 Core Dynamics
// ***************************************************************************************

#ifndef JSON_INTERFACE_H
#define JSON_INTERFACE_H

// Standard Header Files
#include <stdio.h>
#include <string>
#include <deque>

#include "stringthings.h"
#include "rasapi.h"
#include "LEDstuff.h"

using namespace std;

// Ledgend Scale Types:
#define LEDGEND_OFF     0
#define LEDGEND_RANGE   1
#define LEDGEND_SCALER  2

// -------------------------------------------------------------------------------------
// Notes:
//  Needs tree search entry and read.

// -------------------------------------------------------------------------------------
//  Supporting Functions

bool remove_opens_and_closes(string &Entry, char Open, char Close);

string parse_label(string Text, int Pos_Colon);
// Sub out the Label

string parse_value(string Text, int Pos_Colon);
// Sub out the Value

int pos_of_first_non_space(int Start, string Text);
// Find first non space character starting from the start position.

// -------------------------------------------------------------------------------------
//  JSON Class

class JSON_ENTRY
{
  private:
  
  //int errcountcap = 10000;  // Hardcode Limit

  int find_closing(string Text, int Start_Pos, char Opening, char Closing);
  // Find matching position of closing character matching opening character.
  //  starting from start position.  e.g. "{{{}}}" "{" "}" returns 5th pos.

  bool check_entry(string &Entry, int &Size_Of_Entry, int &Size_Of_Label, 
                              bool &Is_A_Set, bool &Is_A_List, bool &Is_A_Value);
  // Get characteristics of entry.  

  bool parse_item_list(string Entry, bool Is_Set, string Set_Name);
  // Parse list or set.

  string LABEL = "";
  string VALUE = "";

  public:

  deque<JSON_ENTRY> DATA;

  bool IS_VALUE = false;
  bool IS_SET = false;
  bool IS_LIST = false;

  void clear_data();
  // Clear all entries JSON 

  int find_label_pos(string Label_In_List);
  // return pos in JSON entry data list.
  // return -1 if not found.

  int size_of_set(string Set_Label);
  // returns the size set list with with Label, within a set.

  bool set_list(string Entry);
  // Creates a list within a JSON Entry

  bool set_set(string Entry, string Set_Name);
  // Creates a set within a JSON Entry

  string label();
  // Returns label of JSON Entry.

  string value();
  // Returns value of JSON Entry.

  string value_from_list(string Label_In_List);
  // Returns the value of label within a JSON list.

  bool get_if_is(string Label, int &Value);
  // Returns false if label within a json does not match.
  // Returns true  if label within a json does match and changes.
  //  Value to JSON value.

  bool get_if_is(string Label, float &Value);
  // Returns false if label within a json does not match.
  // Returns true  if label within a json does match and changes.
  //  Value to JSON value.

  bool get_if_is(string Label, char &Value);
  // Returns false if label within a json does not match.
  // Returns true  if label within a json does match and changes.
  //  Value to JSON value.

  bool get_if_is(string Label, bool &Value);
  // Returns false if label within a json does not match.
  // Returns true  if label within a json does match and changes.
  //  Value to JSON value.

  bool get_if_is(string Label, CRGB &Value);
  // Returns false if label within a json does not match.
  // Returns true  if label within a json does match and changes.
  //  Value to JSON value.

  bool get_if_is(string Label, string &Value);
  // Returns false if label within a json does not match.
  // Returns true  if label within a json does match and changes.
  //  Value to JSON value.

  bool get_if_is(string Label, STRING_STRING &Value);
  // Returns false if label within a json does not match.
  // Returns true  if label within a json does match and changes.
  //  Value to JSON value.

  bool get_if_is(string Label, STRING_INT &Value);
  // Returns false if label within a json does not match.
  // Returns true  if label within a json does match and changes.
  //  Value to JSON value.

  bool get_if_is(string Label, STRING_FLOAT &Value);
  // Returns false if label within a json does not match.
  // Returns true  if label within a json does match and changes.
  //  Value to JSON value.

  bool get_if_is(string Label, STRING_DOUBLE &Value);
  // Returns false if label within a json does not match.
  // Returns true  if label within a json does match and changes.
  //  Value to JSON value.

  void create_label_value(string Label, string Value);
  // Creates a simple Label Value JSON Entry.
  //    mount_offset.create_label_value("mount offset", "-180");
  //      [mount offset]  [-180]

  void put_json_in_list(JSON_ENTRY Entry);
  // Moves a JSON Entry into JSON Entry list.

  void put_json_in_set(string Set_Name, JSON_ENTRY Entry);
  // Moves a JSON Entry into JSON Entry set with Set Name.
  //    configuration_json.ROOT.put_json_in_set("configuration", entry);
  //      [configuration]
};

class JSON_INTERFACE
{
  /*
  USAGE

  --------
  To create and save:

  JSON_INTERFACE state_json;
  deque<string> state_dq_string;

  bool ret_success = false;

  CRGB color = sdSysData.get_running_color();

  state_json.ROOT.create_label_value(quotify("red"), quotify(to_string((int)(color.r))));
  state_json.ROOT.create_label_value(quotify("green"), quotify(to_string((int)(color.g))));
  state_json.ROOT.create_label_value(quotify("blue"), quotify(to_string((int)(color.b))));
  state_json.ROOT.create_label_value(quotify("description"), quotify(sdSysData.get_running_color_str()));

  state_json.json_print_build_to_string_deque(state_dq_string);
  ret_success = deque_string_to_file(strFilename, state_dq_string, false);

  Results:
    {
      "red" : "0" ,
      "green" : "0" ,
      "blue" : "63" ,
      "description" : "Blue"
    }

  
  --------

  To create a more complex list

    JSON_ENTRY compass_points;
    for (size_t pos = 0; pos < COMPASS_HISTORY.size(); pos++)
    {
      if (COMPASS_HISTORY.FLAGS[pos].HAS_DATA && COMPASS_HISTORY.FLAGS[pos].DO_NOT_OVERWRITE)
      {
        JSON_ENTRY single_point;
        single_point.create_label_value(quotify("X"), quotify(to_string(COMPASS_HISTORY[pos].POINT.X)));
        single_point.create_label_value(quotify("Y"), quotify(to_string(COMPASS_HISTORY[pos].POINT.Y)));
        single_point.create_label_value(quotify("Z"), quotify(to_string(COMPASS_HISTORY[pos].POINT.Z)));
        compass_points.put_json_in_list(single_point);
      }
    }
    configuration_json.ROOT.put_json_in_set(quotify("calibration points"), compass_points);

  Results:

    "calibration points" :
    [
      {
        "X" : "450.000000" ,
        "Y" : "232.000000" ,
        "Z" : "1595.000000"
      } ,
      {
        "X" : "449.000000" ,
        "Y" : "240.000000" ,
        "Z" : "1607.000000"
      } ,
      {
        "X" : "458.000000" ,
        "Y" : "229.000000" ,
        "Z" : "1610.000000"
      }
    ]


  --------
  
  To Load:

    void CAL_LEVEL_3::load_history_and_settings(HMC5883L_PROPERTIES &Props)
    {
      bool ret_success = false;

      JSON_INTERFACE configuration_json;

      string json_state_file = file_to_string(Props.OFFSET_HISTORY_FILE_NAME, ret_success);

      if (ret_success == true)
      {
        ret_success = configuration_json.load_json_from_string(json_state_file);

        if (ret_success == true)
        {
          for(int root = 0; root < (int)configuration_json.ROOT.DATA.size(); root++)
          {
            // load settings
            if (configuration_json.ROOT.DATA[root].label() == "settings")
            {
              float mount_offset =  0.0f;
              bool mount_offset_loaded = false;

              float declination =   0.0f;
              bool declination_loaded = false;

              for (int settings = 0; 
                  settings < (int)configuration_json.ROOT.DATA[root].DATA.size(); settings++)
              {
                mount_offset_loaded = configuration_json.ROOT.DATA[root].DATA[settings].get_if_is("mount offset", mount_offset);
                declination_loaded =  configuration_json.ROOT.DATA[root].DATA[settings].get_if_is("declination", declination);
              }

              // store as props but only if found
              if (mount_offset_loaded)
              {
                Props.CALIBRATION_MOUNT_OFFSET = mount_offset;
              }
              if (declination_loaded)
              {
                Props.CALIBRATION_LOCATION_DECLINATION = declination;
              }
            }

            // load configuration points
            if (configuration_json.ROOT.DATA[root].label() == "calibration points")
            {
              for (int points = 0; 
                  points < (int)configuration_json.ROOT.DATA[root].DATA.size(); points++)
              {
                float x = 0;
                float y = 0;
                float z = 0;

                for (int values = 0; 
                    values < (int)configuration_json.ROOT.DATA[root].DATA[points].DATA.size(); values++)
                {

                  configuration_json.ROOT.DATA[root].DATA[points].DATA[values].get_if_is("X", x);
                  configuration_json.ROOT.DATA[root].DATA[points].DATA[values].get_if_is("Y", y);
                  configuration_json.ROOT.DATA[root].DATA[points].DATA[values].get_if_is("Z", z);
                }

                ...

              }

              ...

            }
          }
        }
      }
    }

    Loads:

    {
      "settings" :
      [
        "mount offset" : "-180.0" ,
        "declination" : "4.0"
      ] ,
      "calibration points" :
      [
        {
          "X" : "560.000000" ,
          "Y" : "232.000000" ,
          "Z" : "1545.000000"
        } ,
        {
          "X" : "560.000000" ,
          "Y" : "224.000000" ,
          "Z" : "1554.000000"
        }
      ]
    }


  */

  private:
  
  void json_debug_to_string_deque(deque<string> &JSON_Print_Build, JSON_ENTRY Json_entry, int Level, int Count);

  void json_to_string_deque(deque<string> &JSON_Print_Build, JSON_ENTRY Json_entry, int Level, string Trailing_Seperator);

  public: 

  JSON_ENTRY ROOT;

  bool load_json_from_string(string JSON_Text);
  // boolean failure and success decisions not complete

  void json_debug_build_to_string_deque(deque<string> &JSON_Print_Build);
  // Creates an easy to read JSON representation into a string deque.

  void json_print_build_to_string_deque(deque<string> &JSON_Print_Build);
  // Creates a propery formated to read JSON into a string deque.
};


#endif