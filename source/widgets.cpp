// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: widgets.cpp
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2859 Core Dynamics
// ***************************************************************************************

#ifndef WIDGETS_CPP
#define WIDGETS_CPP

#include "widgets.h"

// ---------------------------------------------------------------------------------------

ImColor gradiant_color(system_data &sdSysData, unsigned long Start_time, unsigned long Duration, 
                      ImColor Start_Color, ImColor End_Color)
{
  ImColor ret_color;

  float power = (float)(sdSysData.PROGRAM_TIME.current_frame_time() - (float)Start_time) / (float)Duration;
  
  float r = (power * End_Color.Value.x) + ((1.0f - power) * Start_Color.Value.x);
  float g = (power * End_Color.Value.y) + ((1.0f - power) * Start_Color.Value.y);
  float b = (power * End_Color.Value.z) + ((1.0f - power) * Start_Color.Value.z);
  float w = End_Color.Value.w;

  return ImColor(r, g, b, w);
}

// ---------------------------------------------------------------------------------------

ImColor BOOLEAN_GRADIANT::boolean_color(system_data &sdSysData, bool Value, ImColor True_Color, ImColor False_Color)
{
  if (VALUE != Value)
  {
    VALUE = Value;

    if (VALUE)
    {
      COLOR_START = False_Color;
      COLOR_DESTINATION = True_Color;
    }
    else
    {
      COLOR_START = True_Color;
      COLOR_DESTINATION = False_Color;
    }

    if (ACTIVE.ping_down(sdSysData.PROGRAM_TIME.current_frame_time()))
    { 
      unsigned long time_elapsed = sdSysData.PROGRAM_TIME.current_frame_time() - ACTIVE.start_time();
      unsigned long time_remaining = DURATION - (time_elapsed);

      ACTIVE.ping_up(sdSysData.PROGRAM_TIME.current_frame_time() - time_remaining, DURATION);
    }
    else
    {
      ACTIVE.ping_up(sdSysData.PROGRAM_TIME.current_frame_time(), DURATION);
    }

  }

  if (ACTIVE.ping_down(sdSysData.PROGRAM_TIME.current_frame_time()))
  {
    return gradiant_color(sdSysData, ACTIVE.start_time(), DURATION, COLOR_START, COLOR_DESTINATION);
  }
  else
  {
    if (VALUE)
    {
      return True_Color;
    }
    else
    {
      return False_Color;
    }
  }
}

// ---------------------------------------------------------------------------------------

void IMPACT_RESISTANCE_FLOAT_FRAME_COUNT::first_run()
{
  VALUE_COLLECTION_LOWER.reserve(SIZE);
  VALUE_COLLECTION_UPPER.reserve(SIZE);

  for (int count = VALUE_COLLECTION_LOWER.size(); count < SIZE; count++)
  {
    VALUE_COLLECTION_LOWER.push_back( 0.0f );
  }

  for (int count = VALUE_COLLECTION_UPPER.size(); count < SIZE; count++)
  {
    VALUE_COLLECTION_UPPER.push_back( 0.0f );
  }

  READ_WRITE_POS = 0;

  FIRST_RUN = false;
}

void IMPACT_RESISTANCE_FLOAT_FRAME_COUNT::set_size(int Size)
{
  SIZE = (int)(Size * DEF_FRAME_RATE_DELAY_MULTIPLIER);
  FIRST_RUN = true;
}

void IMPACT_RESISTANCE_FLOAT_FRAME_COUNT::set_value(float Value)
{
  if (FIRST_RUN)
  {
    first_run();
  }
  
  LATEST_VALUE = Value;
}

float IMPACT_RESISTANCE_FLOAT_FRAME_COUNT::value()
{
  if (FIRST_RUN)
  {
    return 0;
  }
  else
  {
    READ_WRITE_POS++;
    if (READ_WRITE_POS >= SIZE)
    {
      READ_WRITE_POS = 0;
    }

    SUMMATION_LOWER = SUMMATION_LOWER + LATEST_VALUE - VALUE_COLLECTION_LOWER[READ_WRITE_POS];
    
    VALUE_COLLECTION_LOWER[READ_WRITE_POS] = LATEST_VALUE;

    if (VALUE_COLLECTION_LOWER.size() > 0)
    {
      return (SUMMATION_LOWER / (float)SIZE);
    }
    else
    {
      return LATEST_VALUE;
    }
  }

}

float IMPACT_RESISTANCE_FLOAT_FRAME_COUNT::value_no_roll(float Roll_Value)
{  
  if (FIRST_RUN)
  {
    return 0;
  }
  else
  {
    float ret_value = 0;

    READ_WRITE_POS++;
    if (READ_WRITE_POS >= SIZE)
    {
      READ_WRITE_POS = 0;
    }

    // Mid value calculations
    SUMMATION_LOWER = SUMMATION_LOWER + LATEST_VALUE - VALUE_COLLECTION_LOWER[READ_WRITE_POS];
    VALUE_COLLECTION_LOWER[READ_WRITE_POS] = LATEST_VALUE;

    // Outer value calculations
    if (LATEST_VALUE > (Roll_Value / 2.0f))
      {
        // store negative values in outer
        SUMMATION_UPPER = SUMMATION_UPPER + LATEST_VALUE - Roll_Value - VALUE_COLLECTION_UPPER[READ_WRITE_POS];
        VALUE_COLLECTION_UPPER[READ_WRITE_POS] = LATEST_VALUE - Roll_Value;
      }
      else
      {
        // store positive values in outer
        SUMMATION_UPPER = SUMMATION_UPPER + LATEST_VALUE - VALUE_COLLECTION_UPPER[READ_WRITE_POS];
        VALUE_COLLECTION_UPPER[READ_WRITE_POS] = LATEST_VALUE;
      }

    // Choose to return mid or outer values
    if (LATEST_VALUE > (Roll_Value / 4.0f) && LATEST_VALUE < (Roll_Value - (Roll_Value / 4.0f)))
    {
      // return mid values
      if (VALUE_COLLECTION_LOWER.size() > 0)
      {
        ret_value = (SUMMATION_LOWER / (float)SIZE);
      }
      else
      {
        ret_value = LATEST_VALUE;
      }
    }
    else
    {
      // return outer values
      if (VALUE_COLLECTION_UPPER.size() > 0)
      {
        ret_value = (SUMMATION_UPPER / (float)SIZE);
      }
      else
      {
        ret_value = LATEST_VALUE;
      }

      if (ret_value < 0.0f)
      {
        ret_value += Roll_Value;
      }
    }

    return ret_value;
  }
}

// ---------------------------------------------------------------------------------------

bool NEW_COLOR_SCALE::active()
{
  if (COLOR_SCALE.size() > 0)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void NEW_COLOR_SCALE::add_color_value_pair(float Value_Is_LT_or_EQ, int Return_Color)
{
  COLOR_VALUE_PAIR temp_color_pair;

  temp_color_pair.LT_or_EQ = Value_Is_LT_or_EQ;
  temp_color_pair.COLOR = Return_Color;

  COLOR_SCALE.push_back(temp_color_pair);
}

int NEW_COLOR_SCALE::get_color(float Value)
{
  int ret_color_combo = 0;
  bool found = false;
  
  if (COLOR_SCALE.size() > 0)
  {
    for (int pos = 0; pos < (int)COLOR_SCALE.size() && found == false; pos++)
    {
      if (Value <= COLOR_SCALE[pos].LT_or_EQ)
      {
        ret_color_combo = COLOR_SCALE[pos].COLOR;
        found = true;
      }
    }
  }
  
  return ret_color_combo;
}

// ---------------------------------------------------------------------------------------

void W_TEXT::update_text(system_data &sdSysData, string Text)
{
  if (TEXT != Text)
  {
    TEXT = Text;

    UPDATE_TIMED.ping_up(sdSysData.PROGRAM_TIME.current_frame_time(), 500);
  }
}

void W_TEXT::draw(system_data &sdSysData)
{
  if (PROPS.CHANGE_NOTIFICATION == true && UPDATE_TIMED.ping_down(sdSysData.PROGRAM_TIME.current_frame_time()))
  {
    if (PROPS.STANDARD_COLOR)
    {
      ImGui::PushStyleColor(ImGuiCol_Text, ImU32(gradiant_color(sdSysData, UPDATE_TIMED.start_time(), 500, sdSysData.COLOR_SELECT.neo_color_ACTIVE(RAS_ORANGE), sdSysData.COLOR_SELECT.neo_color_STANDARD(PROPS.COLOR))));
    }
    else
    {
      ImGui::PushStyleColor(ImGuiCol_Text, ImU32(gradiant_color(sdSysData, UPDATE_TIMED.start_time(), 500, sdSysData.COLOR_SELECT.neo_color_ACTIVE(RAS_ORANGE), sdSysData.COLOR_SELECT.neo_color_TEXT(PROPS.COLOR))));
    }
    
    ImGui::Text(TEXT.c_str());
    ImGui::PopStyleColor();
  }
  else
  {
    if (PROPS.STANDARD_COLOR)
    {
      ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.neo_color_STANDARD(PROPS.COLOR)));
    }
    else
    {
      ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.neo_color_TEXT(PROPS.COLOR)));
    }
    ImGui::Text(TEXT.c_str());
    ImGui::PopStyleColor();
  }
}

// ---------------------------------------------------------------------------------------

void W_TEXT_TF::update_text(string True_Text, string False_Text)
{
  TEXT_TRUE = True_Text;
  TEXT_FALSE = False_Text;
}

bool W_TEXT_TF::update_tf(bool True_False)
{
  if (TRUE_FALSE == True_False)
  {
    return false;
  }
  else
  {
    TRUE_FALSE = True_False;
    return true;
  }
}

void W_TEXT_TF::draw(system_data &sdSysData)
{
  if (TRUE_FALSE == true)
  {
    TEXT_TRUE_FALSE.PROPS.STANDARD_COLOR = false;
    TEXT_TRUE_FALSE.PROPS.COLOR = PROPS.COLOR_TRUE;
    TEXT_TRUE_FALSE.update_text(sdSysData, TEXT_TRUE);
  }
  else
  {
    TEXT_TRUE_FALSE.PROPS.STANDARD_COLOR = true;
    TEXT_TRUE_FALSE.PROPS.COLOR = PROPS.COLOR_FALSE;
    TEXT_TRUE_FALSE.update_text(sdSysData, TEXT_FALSE);
  }

  TEXT_TRUE_FALSE.draw(sdSysData);
}

// ---------------------------------------------------------------------------------------

void TEXT_CONSOLE::add_line(string Text)
{
  CONSOLE_TEXT = CONSOLE_TEXT + filter_non_printable(Text);
  CONSOLE_SCROLL_TO_BOTTOM = true;

  if (CONSOLE_TEXT.size() > 1024 * 10)
  {
    size_t pos = CONSOLE_TEXT.find_first_of("\n", 512);
    if (pos != std::string::npos)
    {
      CONSOLE_TEXT.erase(0, pos + 1);
    }
    else
    {
      CONSOLE_TEXT.erase(0, 512);
    }
  } 
}

void TEXT_CONSOLE::add_line_with_indent(string Text)
{
  add_line("    " + Text);
}

/*
void TEXT_CONSOLE::display(system_data &sdSysData, const char *name, bool *p_open, ImGuiWindowFlags flags)
{ 
  ImGui::Begin(name, p_open, flags);
  {
    ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.neo_color_TEXT(RAS_WHITE)));

    ImGui::TextUnformatted(CONSOLE_TEXT.c_str());
    if (CONSOLE_SCROLL_TO_BOTTOM == true && ImGui::GetScrollMaxY() > 0)
    {
      ImGui::SetScrollHereY(1.0f);
      CONSOLE_SCROLL_TO_BOTTOM = false;
    }
    
    ImGui::PopStyleColor();
  }
  ImGui::End();
}
*/

void TEXT_CONSOLE::display(system_data &sdSysData, const char *name, bool *p_open, ImGuiWindowFlags flags)
{ 
  ImGui::Begin(name, p_open, flags);
  {
    ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.neo_color_TEXT(RAS_WHITE)));

    // Enable text wrapping
    ImGui::PushTextWrapPos(ImGui::GetWindowContentRegionMax().x - 10); 
    ImGui::TextUnformatted(CONSOLE_TEXT.c_str());
    ImGui::PopTextWrapPos(); // Restore wrap settings

    // Auto-scroll logic
    if (CONSOLE_SCROLL_TO_BOTTOM && ImGui::GetScrollMaxY() > 0)
    {
        ImGui::SetScrollHereY(1.0f);
        CONSOLE_SCROLL_TO_BOTTOM = false;
    }

    ImGui::PopStyleColor();
  }
  ImGui::End();
}

// ---------------------------------------------------------------------------------------

void text_simple_bool(system_data &sdSysData, string Text, bool Indication, int COLOR)
{
  if (Indication == false)
  {
    ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.neo_color_TEXT(COLOR)));
    ImGui::Text(Text.c_str());
    ImGui::PopStyleColor();
  }
  else
  {
    ImGui::Text(Text.c_str());
  }
}

bool button_simple_enabled(system_data &sdSysData, string Text, bool Enabled, ImVec2 ImVec2_Size)
{
  bool ret_value = false;

  ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.neo_color_TEXT(RAS_WHITE)));

  if (Enabled == true)
  {
    if (ImGui::Button(Text.c_str(), ImVec2_Size))
    {
      ret_value = true;
    }
  }
  else
  {
    ImGui::PushStyleColor(ImGuiCol_Button, ImU32(sdSysData.COLOR_SELECT.neo_color_BACKGROUND(RAS_BLUE))); 
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImU32(sdSysData.COLOR_SELECT.neo_color_BACKGROUND(RAS_BLUE)));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImU32(sdSysData.COLOR_SELECT.neo_color_BACKGROUND(RAS_BLUE)));
    ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.neo_color_STANDARD(RAS_BLUE)));
    ImGui::Button(Text.c_str(), ImVec2_Size);
    ImGui::PopStyleColor(4);
  }

  ImGui::PopStyleColor();

  return ret_value;
}

// ---------------------------------------------------------------------------------------

bool BUTTON_INVISIBLE::button_flash_color(ImDrawList *Draw_List, system_data &sdSysData, string Text, int Color, ImVec2 Start_Position, ImVec2 Size)
{
  bool ret_value = false;

  ImGui::SetCursorScreenPos(Start_Position);
  
  if (ImGui::InvisibleButton(Text.c_str(), Size))
  {
    FLASH_COLOR.set_color_jump(sdSysData.PROGRAM_TIME.current_frame_time(), sdSysData.COLOR_SELECT.neo_color_STANDARD(Color), IM_COL32(0, 0, 0, 0));
    
    ret_value = true;
  }
  else
  {
    FLASH_COLOR.set_current_frame_time(sdSysData.PROGRAM_TIME.current_frame_time());
  }

  if (FLASH_COLOR.is_changing())
  {
    Draw_List->AddRectFilled(Start_Position, Start_Position + Size, FLASH_COLOR.color());
  }

  return ret_value;
}

bool BUTTON_COLOR::button_color(system_data &sdSysData, string Text, int Color, ImVec2 ImVec2_Size)
{
  bool ret_value = false;

  ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.neo_color_TEXT(RAS_WHITE)));

  ImGui::PushStyleColor(ImGuiCol_Button, ImU32(BUTTON_COLOR.color(sdSysData.PROGRAM_TIME.current_frame_time(), sdSysData.COLOR_SELECT.neo_color_STANDARD(Color))));
  ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImU32(BUTTON_HOVERED_COLOR.color(sdSysData.PROGRAM_TIME.current_frame_time(), sdSysData.COLOR_SELECT.neo_color_HOVERED(Color))));
  ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImU32(BUTTON_ACTIVE_COLOR.color(sdSysData.PROGRAM_TIME.current_frame_time(), sdSysData.COLOR_SELECT.neo_color_ACTIVE(Color))));

  //if (ImGui::Button(TEXT.value(Text).c_str(), ImVec2_Size))
  if (ImGui::Button(Text.c_str(), ImVec2_Size))
  {
    ret_value = true;
  }
  ImGui::PopStyleColor(4);

  return ret_value;
}

bool BUTTON_TOGGLE_COLOR::button_toggle_color(system_data &sdSysData, string True_Value_Text, string False_Value_Text, bool Toggle, 
                                int True_Color, int False_Color, ImVec2 ImVec2_Size)
{
  // Does not control toggle, just shows value.
  bool ret_value = false;

  ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.neo_color_TEXT(RAS_WHITE)));

  if (Toggle == false)
  {
    ImGui::PushStyleColor(ImGuiCol_Button, ImU32(BUTTON_COLOR.color(sdSysData.PROGRAM_TIME.current_frame_time(), sdSysData.COLOR_SELECT.neo_color_STANDARD(False_Color))));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImU32(BUTTON_HOVERED_COLOR.color(sdSysData.PROGRAM_TIME.current_frame_time(), sdSysData.COLOR_SELECT.neo_color_HOVERED(False_Color))));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImU32(BUTTON_ACTIVE_COLOR.color(sdSysData.PROGRAM_TIME.current_frame_time(), sdSysData.COLOR_SELECT.neo_color_ACTIVE(False_Color))));

    //if (ImGui::Button(TEXT.value(False_Value_Text).c_str(), ImVec2_Size))
    if (ImGui::Button(False_Value_Text.c_str(), ImVec2_Size))
    {
      ret_value = true;
    }
    ImGui::PopStyleColor(4);
  }
  else
  {
    ImGui::PushStyleColor(ImGuiCol_Button, ImU32(BUTTON_COLOR.color(sdSysData.PROGRAM_TIME.current_frame_time(), sdSysData.COLOR_SELECT.neo_color_STANDARD(True_Color))));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImU32(BUTTON_HOVERED_COLOR.color(sdSysData.PROGRAM_TIME.current_frame_time(), sdSysData.COLOR_SELECT.neo_color_HOVERED(True_Color))));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImU32(BUTTON_ACTIVE_COLOR.color(sdSysData.PROGRAM_TIME.current_frame_time(), sdSysData.COLOR_SELECT.neo_color_ACTIVE(True_Color))));

    //if (ImGui::Button(TEXT.value(True_Value_Text).c_str(), ImVec2_Size))
    if (ImGui::Button(True_Value_Text.c_str(), ImVec2_Size))
    {
      ret_value = true;
    }
    ImGui::PopStyleColor(4);
  }

  return ret_value;
}

// ---------------------------------------------------------------------------------------

bool CONFIRM_DIALOG::confirm_dialog(system_data &sdSysData, bool &Choice)
{
  bool ret_choice_clicked = false;

  ImGui::SetNextWindowSize(ImVec2(90.0f, 195.0f));
  
  if (ImGui::Begin("Continue", nullptr, sdSysData.SCREEN_DEFAULTS.flags_w_pop)) 
  {
    if (CONFIRM.button_color(sdSysData, "CONFIRM", RAS_YELLOW, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
    {
      Choice = true;
      ret_choice_clicked = true;
    }
    
    if (DENY.button_color(sdSysData, "DENY", RAS_BLUE, sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
    {
      Choice = false;
      ret_choice_clicked = true;
    }
  }
  ImGui::End();

  return ret_choice_clicked;
}

// ---------------------------------------------------------------------------------------


void draw_compass(ImDrawList *Draw_List, system_data &sdSysData, int Version, ImVec2 Screen_Position, float Size, bool Main, bool Valid_Position, 
                  bool Valid_Heading_1, float Heading_1, bool Valid_Heading_2, float Heading_2, bool Draw_North_Pointer, 
                  bool Jitter_Active, float Jitter_Heading_Min, float Jitter_Heading_Max, float Map_Bearing, int Color)
{
  // Heading 1 - Track or Aircraft Nav Heading
  // Heading 2 - Compass or Aircraft Track Heading.

  // standards small
  // size 15
  float needle_size = 7.5f; // Size / 2
  if (Version == 2)
  {
    needle_size = Size / 4.0f; // Size / 2
  }

  float size_outline = 3.0f;  // Size / 5

  float font_height = 18.0f;

  // Set Color
  if (Valid_Position == false)
  {
    Color = RAS_YELLOW;
  }

  if (Valid_Position)
  {
    if (Version == 1)
    {
      // Draw Location (Circle)
      Draw_List->AddNgonFilled(Screen_Position, (needle_size / 2.0f) + 2.0f, (ImU32)sdSysData.COLOR_SELECT.neo_color_STANDARD(RAS_BLACK), 12.0f);
      Draw_List->AddNgonFilled(Screen_Position, needle_size / 2.0f, (ImU32)sdSysData.COLOR_SELECT.neo_color_STANDARD_V(Color), 12.0f);
      Draw_List->AddNgon(Screen_Position, Size + 2.0f, (ImU32)sdSysData.COLOR_SELECT.neo_color_STANDARD(RAS_BLACK), 12.0f, size_outline);
      Draw_List->AddNgon(Screen_Position, Size, (ImU32)sdSysData.COLOR_SELECT.neo_color_STANDARD_V(Color), 12.0f, size_outline);

      // Draw Double Location (Second Circle)
      if (Main)
      {
        Draw_List->AddNgon(Screen_Position, Size + 4.0f, (ImU32)sdSysData.COLOR_SELECT.neo_color_STANDARD_V(Color), 12.0f, 2.0f);
      }
    }
    else
    {
      // Draw Center Location (Solid Circle)
      Draw_List->AddNgonFilled(Screen_Position, 15.0f / 2.0f, (ImU32)sdSysData.COLOR_SELECT.neo_color_STANDARD(RAS_BLACK), 12.0f + 4.0f);
      Draw_List->AddNgonFilled(Screen_Position, 15.0f / 2.0f, (ImU32)sdSysData.COLOR_SELECT.neo_color_STANDARD_V(Color), 12.0f);
      Draw_List->AddNgon(Screen_Position, 15.0f + 2.0f, (ImU32)sdSysData.COLOR_SELECT.neo_color_STANDARD(RAS_BLACK), 12.0f, size_outline);
      Draw_List->AddNgon(Screen_Position, 15.0f, (ImU32)sdSysData.COLOR_SELECT.neo_color_STANDARD_V(Color), 12.0f, size_outline);
    }
  }
  else
  {
    if (Version == 1)
    {
      // Draw Location (Circle)
      Draw_List->AddNgonFilled(Screen_Position, (needle_size / 2.0f) + 2.0f, (ImU32)sdSysData.COLOR_SELECT.neo_color_STANDARD(RAS_BLACK), 12.0f);
      Draw_List->AddNgonFilled(Screen_Position, needle_size / 2.0f, (ImU32)sdSysData.COLOR_SELECT.neo_color_STANDARD(Color), 12.0f);
      Draw_List->AddNgon(Screen_Position, Size + 2.0f, (ImU32)sdSysData.COLOR_SELECT.neo_color_STANDARD(RAS_BLACK), 12.0f, size_outline);
      Draw_List->AddNgon(Screen_Position, Size, (ImU32)sdSysData.COLOR_SELECT.neo_color_STANDARD(Color), 12.0f, size_outline);

      // Draw Double Location (Second Circle)
      if (Main)
      {
        Draw_List->AddNgon(Screen_Position, Size + 4.0f, (ImU32)sdSysData.COLOR_SELECT.neo_color_STANDARD(Color), 12.0f, 2.0f);
      }
    }
    else
    {
      // Draw Center Location (Solid Circle)
      Draw_List->AddNgonFilled(Screen_Position, 15.0f / 2.0f, (ImU32)sdSysData.COLOR_SELECT.neo_color_STANDARD(RAS_BLACK), 12.0f + 4.0f);
      Draw_List->AddNgonFilled(Screen_Position, 15.0f / 2.0f, (ImU32)sdSysData.COLOR_SELECT.neo_color_STANDARD(Color), 12.0f);
      Draw_List->AddNgon(Screen_Position, 15.0f + 2.0f, (ImU32)sdSysData.COLOR_SELECT.neo_color_STANDARD(RAS_BLACK), 12.0f, size_outline);
      Draw_List->AddNgon(Screen_Position, 15.0f, (ImU32)sdSysData.COLOR_SELECT.neo_color_STANDARD(Color), 12.0f, size_outline);
    }
  }

  // Draw Heading 2
  if (Valid_Heading_2)
  {
    float rad = 0.0f;
    float rad_1 = 0.0f;
    float rad_2 = 0.0f;
    ImVec2 p1;
    ImVec2 p2;

    // North pointer drawn only if heading provided.
    if (Draw_North_Pointer)
    {


      // This may be a problem

      if (Map_Bearing != 0.0f)
      {
        // Map is rotating. Point north up relative to map rotation.
        rad = (90.0f - Map_Bearing) * float_PI / 180.0f;
      }
      else
      {
        // Map is not rotating. Point north opposite of heading.
        rad = (90.0f - Heading_2) * float_PI / 180.0f;
      }



      
      // Calculate p1 (base) and p2 (tip) as before.
      p1 = Screen_Position;
      p2 = ImVec2(Screen_Position.x + (Size) * cos(rad + float_PI),
                  Screen_Position.y + (Size) * sin(rad + float_PI));

      // Settings for the arrowhead.
      const float headLength = (Size / 6.0f) + 10.0f;         // Length of the arrowhead lines
      const float headAngle = 30.0f * float_PI / 180.0f;      // Arrowhead angle in radians

      // The direction in which the arrow is pointing (same as the shaft).
      float lineAngle = rad + float_PI;

      // Calculate the positions of the two arrowhead endpoints.
      ImVec2 arrowLeft = ImVec2(p2.x + headLength * cos(lineAngle - headAngle),
                                p2.y + headLength * sin(lineAngle - headAngle));

      ImVec2 arrowRight = ImVec2(p2.x + headLength * cos(lineAngle + headAngle),
                                  p2.y + headLength * sin(lineAngle + headAngle));

      // Draw the arrowhead lines.
      if (Valid_Position)
      {
        Draw_List->AddLine(p2, arrowLeft, sdSysData.COLOR_SELECT.neo_color_STANDARD_V(Color), 3.0f);
        Draw_List->AddLine(p2, arrowRight, sdSysData.COLOR_SELECT.neo_color_STANDARD_V(Color), 3.0f);
      }
      else
      {
        Draw_List->AddLine(p2, arrowLeft, sdSysData.COLOR_SELECT.neo_color_STANDARD(Color), 3.0f);
        Draw_List->AddLine(p2, arrowRight, sdSysData.COLOR_SELECT.neo_color_STANDARD(Color), 3.0f);
      }
    }

    if (Jitter_Active)
    {
      if (Version == 2)
      {
        if (Jitter_Heading_Min < Jitter_Heading_Max)
        {
          rad_1 = ((Jitter_Heading_Min) - 90.0f - Map_Bearing) * float_PI / 180.0f;
          rad_2 = ((Jitter_Heading_Max) - 90.0f - Map_Bearing) * float_PI / 180.0f;
        }
        else
        {
          rad_2 = ((Jitter_Heading_Min) - 90.0f - Map_Bearing) * float_PI / 180.0f;
          rad_1 = ((Jitter_Heading_Max + 360.0f) - 90.0f - Map_Bearing) * float_PI / 180.0f;
        }

        if (Valid_Position)
        {
          Draw_List->PathArcTo(Screen_Position, Size + (font_height * 1.25f), rad_1, rad_2, 32);
          Draw_List->PathStroke(sdSysData.COLOR_SELECT.neo_color_STANDARD_V(Color), false, font_height / 2.0f);
        }
        else
        {
          Draw_List->PathArcTo(Screen_Position, Size + (font_height * 1.25f), rad_1, rad_2, 32);
          Draw_List->PathStroke(sdSysData.COLOR_SELECT.neo_color_STANDARD(Color), false, font_height / 2.0f);
        }

      }
    }

    // Outer Ring
    // Adjust the degrees into heading. Convert direction from degrees to radians
    rad   = (Heading_2 + 90.0f - Map_Bearing) * float_PI / 180.0f;

    if (Version == 1)
    {
      // Version 1

      // Calculate
      p1 = ImVec2(Screen_Position.x + Size * cos(rad + float_PI), 
                  Screen_Position.y + Size * sin(rad + float_PI));

      p2 = ImVec2(Screen_Position.x + ( 2.0f * Size) * cos(rad + float_PI), 
                  Screen_Position.y + ( 2.0f * Size) * sin(rad + float_PI));
 
      // Draw the line
      if (Valid_Position)
      {
        Draw_List->AddLine(p1, p2, sdSysData.COLOR_SELECT.neo_color_STANDARD(RAS_BLACK), needle_size + 4.0f);
        Draw_List->AddLine(p1, p2, sdSysData.COLOR_SELECT.neo_color_STANDARD_V(Color), needle_size);
      }
      else
      {
        Draw_List->AddLine(p1, p2, sdSysData.COLOR_SELECT.neo_color_STANDARD(RAS_BLACK), needle_size + 4.0f);
        Draw_List->AddLine(p1, p2, sdSysData.COLOR_SELECT.neo_color_STANDARD(Color), needle_size);
      }
    }
    else
    {
      rad_1 = (Heading_2 - 90.0f - (2000.0f / Size) - Map_Bearing) * float_PI / 180.0f;
      rad_2 = (Heading_2 - 90.0f + (2000.0f / Size) - Map_Bearing) * float_PI / 180.0f;

      if (Valid_Position)
      {
        Draw_List->PathArcTo(Screen_Position, Size + (font_height / 2.0f), rad_1, rad_2, 32);
        Draw_List->PathStroke(sdSysData.COLOR_SELECT.neo_color_STANDARD_V(Color), false, font_height);
      }
      else
      {
        Draw_List->PathArcTo(Screen_Position, Size + (font_height / 2.0f), rad_1, rad_2, 32);
        Draw_List->PathStroke(sdSysData.COLOR_SELECT.neo_color_STANDARD(Color), false, font_height);
      }
    }

    // Text Descriptor for version 2
    if (Version == 2)
    { 
      float rad2 = 0.0f;
      ImVec2 p3;

      ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.neo_color_TEXT(RAS_BLACK)));

      if (Heading_2 - Map_Bearing < 90.0f || Heading_2 - Map_Bearing > 270.0f)
      {
        rad2 = (Heading_2 + 90.0f - Map_Bearing) * float_PI / 180.0f;

        p3 = ImVec2(Screen_Position.x + (Size + font_height) * cos(rad2 + float_PI), 
                    Screen_Position.y + (Size + font_height) * sin(rad2 + float_PI));

        ImGui::SetCursorScreenPos(p3);

        Text_Rotate(to_string((int)Heading_2), 180.0f - Heading_2 + Map_Bearing, BB_TL);
      }
      else
      {
        rad2 = (Heading_2 + 90.0f - Map_Bearing) * float_PI / 180.0f;

        p3 = ImVec2(Screen_Position.x + (Size) * cos(rad2 + float_PI), 
                    Screen_Position.y + (Size) * sin(rad2 + float_PI));

        ImGui::SetCursorScreenPos(p3);

        Text_Rotate(to_string((int)Heading_2), -Heading_2 + Map_Bearing, BB_TL);
      }
      
      ImGui::PopStyleColor();
    }
  }

  // Draw Heading 1
  if (Valid_Heading_1)
  {
    // Adjust the degrees into heading. Convert direction from degrees to radians
    float rad = (Heading_1 + 90.0f - Map_Bearing) * float_PI / 180.0f;
    
    // Calculate
    ImVec2 p1 = Screen_Position;
    
    if (Version == 2)
    {
      p1 = ImVec2(Screen_Position.x + (Size - font_height) * cos(rad + float_PI), 
                  Screen_Position.y + (Size - font_height) * sin(rad + float_PI));
    }

    ImVec2 p2 = ImVec2(Screen_Position.x + Size * cos(rad + float_PI), 
                        Screen_Position.y + Size * sin(rad + float_PI));
 

    // Text Descriptor for version 2
    if (Version == 1)
    {
      // Draw the line
      if (Valid_Position)
      {
        Draw_List->AddLine(p1, p2, sdSysData.COLOR_SELECT.neo_color_STANDARD(RAS_BLACK), needle_size + 4.0f);
        Draw_List->AddLine(p1, p2, sdSysData.COLOR_SELECT.neo_color_STANDARD_V(Color), needle_size);
      }
      else
      {
        Draw_List->AddLine(p1, p2, sdSysData.COLOR_SELECT.neo_color_STANDARD(RAS_BLACK), needle_size + 4.0f);
        Draw_List->AddLine(p1, p2, sdSysData.COLOR_SELECT.neo_color_STANDARD(Color), needle_size);
      }
    }
    else // if (Version == 2)  // Inner Ring
    {
      float rad_1 = (Heading_1 - 90.0f - (2000.0f / Size) - Map_Bearing) * float_PI / 180.0f;
      float rad_2 = (Heading_1 - 90.0f + (2000.0f / Size) - Map_Bearing) * float_PI / 180.0f;
      
      if (Valid_Position)
      {
        Draw_List->PathArcTo(Screen_Position, Size - (font_height / 2.0f), rad_1, rad_2, 32);
        Draw_List->PathStroke(sdSysData.COLOR_SELECT.neo_color_STANDARD_V(Color), false, font_height);
      }
      else
      {
        Draw_List->PathArcTo(Screen_Position, Size - (font_height / 2.0f), rad_1, rad_2, 32);
        Draw_List->PathStroke(sdSysData.COLOR_SELECT.neo_color_STANDARD(Color), false, font_height);
      }

      // Nothing written yet to handle other sized version 2 compass rotated heading text.
      float rad2 = 0.0f;
      ImVec2 p3;

      ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.neo_color_TEXT(RAS_BLACK)));

      if (Heading_1 - Map_Bearing < 90.0f || Heading_1 - Map_Bearing > 270.0f)
      {
        rad2 = (Heading_1 + 90.0f - Map_Bearing) * float_PI / 180.0f;

        p3 = ImVec2(Screen_Position.x + Size * cos(rad2 + float_PI), 
                          Screen_Position.y + Size * sin(rad2 + float_PI));
        ImGui::SetCursorScreenPos(p3);

        Text_Rotate(to_string((int)Heading_1), 180.0f - Heading_1 + Map_Bearing, BB_TL);
      }
      else
      {
        rad2 = (Heading_1 + 90.0f - Map_Bearing) * float_PI / 180.0f;

        p3 = ImVec2(Screen_Position.x + (Size - font_height) * cos(rad2 + float_PI), 
                    Screen_Position.y + (Size - font_height) * sin(rad2 + float_PI));
        ImGui::SetCursorScreenPos(p3);

        Text_Rotate(to_string((int)Heading_1), -Heading_1 + Map_Bearing, BB_TL);
      }

      ImGui::PopStyleColor();
    }
  }
}

void draw_compass(ImDrawList *Draw_List, system_data &sdSysData, int Version, ImVec2 Screen_Position, float Size, bool Main, bool Valid_Position, 
                        bool Valid_Heading_1, float Heading_1, bool Valid_Heading_2, float Heading_2, bool Draw_North_Pointer, float Map_Bearing)
{
  draw_compass(Draw_List, sdSysData, Version, Screen_Position, Size, Main, 
                Valid_Position, Valid_Heading_1, Heading_1, 
                Valid_Heading_2, Heading_2, Draw_North_Pointer, 
                false, 0.0f, 0.0f, Map_Bearing, RAS_WHITE);
}

void draw_compass(ImDrawList *Draw_List, system_data &sdSysData, int Version, ImVec2 Screen_Position, float Size, bool Main, bool Valid_Position, 
                        bool Valid_Heading_1, float Heading_1, bool Valid_Heading_2, float Heading_2, bool Draw_North_Pointer, float Map_Bearing, 
                        int Color)
{
  draw_compass(Draw_List, sdSysData, Version, Screen_Position, Size, Main, 
                Valid_Position, Valid_Heading_1, Heading_1, 
                Valid_Heading_2, Heading_2, Draw_North_Pointer, 
                false, 0.0f, 0.0f, Map_Bearing, Color);
}

void COMPASS_WIDGET::set_size(int Heading_1_Size, int Heading_2_Size)
{
  HEADING_1.set_size(Heading_1_Size);
  HEADING_2.set_size(Heading_2_Size);
}

void COMPASS_WIDGET::draw(ImDrawList *Draw_List, system_data &sdSysData, int Version, ImVec2 Screen_Position, float Size, bool Main, bool Valid_Position, 
                        bool Valid_Heading_1, float Heading_1, bool Valid_Heading_2, float Heading_2, bool Draw_North_Pointer, 
                        bool Jitter_Active, float Jitter_Heading_Min, float Jitter_Heading_Max, float Map_Bearing)
{
  if (Valid_Heading_1)
  {
    HEADING_1.set_value(Heading_1);
  }

  if (Valid_Heading_2)
  {
    HEADING_2.set_value(Heading_2);
  }

  draw_compass(Draw_List, sdSysData, Version, Screen_Position, Size, Main, Valid_Position, 
                        Valid_Heading_1, HEADING_1.value_no_roll(360.0f), Valid_Heading_2, HEADING_2.value_no_roll(360.0f), Draw_North_Pointer, 
                        Jitter_Active, Jitter_Heading_Min, Jitter_Heading_Max, Map_Bearing, PROPS.COLOR);
}

void COMPASS_WIDGET::draw(ImDrawList *Draw_List, system_data &sdSysData, int Version, ImVec2 Screen_Position, float Size, bool Main, bool Valid_Position, 
                        bool Valid_Heading_1, float Heading_1, bool Valid_Heading_2, float Heading_2, bool Draw_North_Pointer, float Map_Bearing)
{
  if (Valid_Heading_1)
  {
    HEADING_1.set_value(Heading_1);
  }

  if (Valid_Heading_2)
  {
    HEADING_2.set_value(Heading_2);
  }

  draw_compass(Draw_List, sdSysData, Version, Screen_Position, Size, Main, Valid_Position, 
                        Valid_Heading_1, HEADING_1.value_no_roll(360.0f), Valid_Heading_2, HEADING_2.value_no_roll(360.0f), Draw_North_Pointer, Map_Bearing);
}

// ---------------------------------------------------------------------------------------

void BAR_TECH::draw_min_max_val(system_data &sdSysData)
{
  if ((int)MIN_MAX.min() < 0)
  {
    DSP_MIN.PROPS.COLOR = RAS_WHITE;
  }
  else
  {
    DSP_MIN.PROPS.COLOR = RAS_WHITE;
  }
  
  if ((int)(VALUE) < 0)
  {
    DSP_VALUE.PROPS.COLOR = RAS_YELLOW;
  }
  else
  {
    DSP_VALUE.PROPS.COLOR = RAS_WHITE;
  }

  if ((int)(MIN_MAX.max()) < 0)
  {
    DSP_MAX.PROPS.COLOR = RAS_YELLOW;
  }
  else
  {
    DSP_MAX.PROPS.COLOR = RAS_WHITE;
  }

  // Display Values
  if (PROPS.DISPLAY_SINGLE_POINT_FLOAT)
  {
    DSP_MIN.update_text(sdSysData, to_string_round_to_nth(MIN_MAX.min_float(), 1));
    DSP_VALUE.update_text(sdSysData, to_string_round_to_nth(VALUE, 1));
    DSP_MAX.update_text(sdSysData, to_string_round_to_nth(MIN_MAX.max_float(), 1));
  }
  else
  {
    DSP_MIN.update_text(sdSysData, to_string((int)(MIN_MAX.min())));
    DSP_VALUE.update_text(sdSysData, to_string((int)(VALUE)));
    DSP_MAX.update_text(sdSysData, to_string((int)(MIN_MAX.max())));  
  }

  if (ImGui::BeginTable("Automobile Guage Val Min Max", 4, 0))
  {
    {
      ImGui::TableNextRow();
      ImGui::TableNextColumn();
      
      ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.neo_color_TEXT(RAS_WHITE)));
      ImGui::Text(PROPS.LABEL.c_str());
      ImGui::PopStyleColor();

      ImGui::TableNextColumn();
      DSP_MIN.draw(sdSysData);
      ImGui::TableNextColumn();
      DSP_VALUE.draw(sdSysData);
      ImGui::TableNextColumn();
      DSP_MAX.draw(sdSysData);
    }
    ImGui::EndTable();
  }
}

void BAR_TECH::create()
{
  MIN_MAX.PROP.TIME_SPAN = PROPS.MIN_MAX_TIME_SPAN;
  MIN_MAX.PROP.SLICES = PROPS.MIN_MAX_TIME_SLICES;

  RULER.PROPS.COLOR = PROPS.COLOR_RULER;
  RULER.PROPS.MAX_VALUE = PROPS.MAX;
  RULER.PROPS.MAX_TICK_LEVEL = PROPS.MAX_TICK_LEVEL;
  RULER.PROPS.HORIZONTAL = PROPS.HORIZONTAL;

  DSP_MIN.PROPS.CHANGE_NOTIFICATION = true;
  DSP_MAX.PROPS.CHANGE_NOTIFICATION = true;
  DSP_VALUE.PROPS.CHANGE_NOTIFICATION = false;

  VALUE_MARKER.set_size(PROPS.DRAW_FRAME_COUNT);
}

void BAR_TECH::update_min_max_value(float Min_Value, float Max_Value)
{
  if (Min_Value == 0.0f && Max_Value == 0.0f)
  {
    ALT_MIN_MAX = false;
  }
  else
  {
    ALT_MIN_MAX = true;
  }

  ALT_MIN = Min_Value;
  ALT_MAX = Max_Value;
}

void BAR_TECH::update_value(system_data &sdSysData, float Value)
{
  VALUE = Value;
  VALUE_MARKER.set_value(Value);

  if (PROPS.DRAW_MIN_MAX)
  {
    MIN_MAX.put_value(Value, sdSysData.PROGRAM_TIME.current_frame_time());
  }
}

void BAR_TECH::draw(ImDrawList *Draw_List, system_data &sdSysData)
{
  // Min Max
  if (PROPS.HORIZONTAL && PROPS.DRAW_MIN_MAX && PROPS.DRAW_MIN_MAX_ON_TOP)
  {
    draw_min_max_val(sdSysData);
  }

  // Draw
  ImVec2 pos = ImGui::GetCursorScreenPos();
  
  ImVec2 size = ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);

  // Draw Background - Red if out of range, Normal color if positive.
  if (PROPS.HORIZONTAL)
  {
    if (is_within(VALUE, PROPS.MIN, PROPS.MAX))
    {
      Draw_List->AddRectFilled(pos, ImVec2(pos.x + size.x , pos.y + PROPS.BAR_HEIGHT), NEO_BACKGROUND_DIM.color(sdSysData.PROGRAM_TIME.current_frame_time(), sdSysData.COLOR_SELECT.neo_color_DIM(PROPS.COLOR_BACKGROUND)), 5.0f, ImDrawFlags_None);
      Draw_List->AddRect(pos, ImVec2(pos.x + size.x , pos.y + PROPS.BAR_HEIGHT), NEO_BACKGROUND.color(sdSysData.PROGRAM_TIME.current_frame_time(), sdSysData.COLOR_SELECT.neo_color_BACKGROUND(PROPS.COLOR_BACKGROUND)), 5.0f, ImDrawFlags_None, 2.0f);
    }
    else
    {
      Draw_List->AddRectFilled(pos, ImVec2(pos.x + size.x , pos.y + PROPS.BAR_HEIGHT), NEO_BACKGROUND_DIM.color(sdSysData.PROGRAM_TIME.current_frame_time(), sdSysData.COLOR_SELECT.neo_color_STANDARD_V(RAS_RED)), 5.0f, ImDrawFlags_None);
      Draw_List->AddRect(pos, ImVec2(pos.x + size.x , pos.y + PROPS.BAR_HEIGHT), NEO_BACKGROUND.color(sdSysData.PROGRAM_TIME.current_frame_time(), sdSysData.COLOR_SELECT.neo_color_BACKGROUND(RAS_RED)), 5.0f, ImDrawFlags_None, 2.0f);
    }
  }
  else
  {
    if (is_within(VALUE, PROPS.MIN, PROPS.MAX))
    {
      Draw_List->AddRectFilled(pos, ImVec2(pos.x + PROPS.BAR_HEIGHT, pos.y + size.y), NEO_BACKGROUND_DIM.color(sdSysData.PROGRAM_TIME.current_frame_time(), sdSysData.COLOR_SELECT.neo_color_DIM(PROPS.COLOR_BACKGROUND)), 5.0f, ImDrawFlags_None);
      Draw_List->AddRect(pos, ImVec2(pos.x + PROPS.BAR_HEIGHT, pos.y + size.y), NEO_BACKGROUND.color(sdSysData.PROGRAM_TIME.current_frame_time(), sdSysData.COLOR_SELECT.neo_color_BACKGROUND(PROPS.COLOR_BACKGROUND)), 5.0f, ImDrawFlags_None, 2.0f);
    }
    else
    {
      Draw_List->AddRectFilled(pos, ImVec2(pos.x + PROPS.BAR_HEIGHT, pos.y + size.y), NEO_BACKGROUND_DIM.color(sdSysData.PROGRAM_TIME.current_frame_time(), sdSysData.COLOR_SELECT.neo_color_STANDARD_V(RAS_RED)), 5.0f, ImDrawFlags_None);
      Draw_List->AddRect(pos, ImVec2(pos.x + PROPS.BAR_HEIGHT, pos.y + size.y), NEO_BACKGROUND.color(sdSysData.PROGRAM_TIME.current_frame_time(), sdSysData.COLOR_SELECT.neo_color_BACKGROUND(RAS_RED)), 5.0f, ImDrawFlags_None, 2.0f);
    }
  }

  // Draw Min Max Bar
  if (PROPS.HORIZONTAL)
  {
    if (PROPS.DRAW_MIN_MAX)
    {
      float min_location = 0.0f;
      float max_location = 0.0f;

      if (ALT_MIN_MAX)
      {
        min_location = ((ALT_MIN - PROPS.MIN) / (PROPS.MAX - PROPS.MIN)) * size.x;
        max_location = ((ALT_MAX - PROPS.MIN) / (PROPS.MAX - PROPS.MIN)) * size.x;
      }
      else
      {
        min_location = ((MIN_MAX.min_float() - PROPS.MIN) / (PROPS.MAX - PROPS.MIN)) * size.x;
        max_location = ((MIN_MAX.max_float() - PROPS.MIN) / (PROPS.MAX - PROPS.MIN)) * size.x;
      }

      if (min_location < 0.0f)
      {
        min_location = 0.0f;
      }
      else if (min_location > size.x)
      {
        min_location = size.x;
      }

      if (max_location < 0.0f)
      {
        max_location = 0.0f;
      }
      else if (max_location > size.x)
      {
        max_location = size.x;
      }

      Draw_List->AddRectFilled(ImVec2(pos.x + min_location, pos.y), 
                                ImVec2(pos.x + max_location, pos.y + PROPS.BAR_HEIGHT), 
                                NEO_MIN_MAX_DIM.color(sdSysData.PROGRAM_TIME.current_frame_time(), sdSysData.COLOR_SELECT.neo_color_DIM(PROPS.COLOR_MARKER)), 
                                5.0f, ImDrawFlags_None);

      Draw_List->AddRect(ImVec2(pos.x + min_location, pos.y + 2.0f), 
                                ImVec2(pos.x + max_location, pos.y + PROPS.BAR_HEIGHT - 2.0f), 
                                NEO_MIN_MAX.color(sdSysData.PROGRAM_TIME.current_frame_time(), sdSysData.COLOR_SELECT.neo_color_STANDARD_V(PROPS.COLOR_MARKER)), 
                                5.0f, 
                                ImDrawFlags_None, 2.0f);

      if (PROPS.DRAW_RULER)
      {
        RULER.draw(Draw_List, sdSysData, pos, ImVec2(pos.x + size.x , pos.y + PROPS.BAR_HEIGHT));
      }
    }
  }
  else
  {
    if (PROPS.DRAW_MIN_MAX)
    {
      float min_location = 0.0f;
      float max_location = 0.0f;

      if (ALT_MIN_MAX)
      {
        min_location = ((ALT_MIN - PROPS.MIN) / (PROPS.MAX - PROPS.MIN)) * size.y;
        max_location = ((ALT_MAX - PROPS.MIN) / (PROPS.MAX - PROPS.MIN)) * size.y;
      }
      else
      {
        min_location = ((MIN_MAX.min_float() - PROPS.MIN) / (PROPS.MAX - PROPS.MIN)) * size.y;
        max_location = ((MIN_MAX.max_float() - PROPS.MIN) / (PROPS.MAX - PROPS.MIN)) * size.y;
      }

      if (min_location < 0)
      {
        min_location = 0.0f;
      }
      else if (min_location > size.y)
      {
        min_location = size.y;
      }

      if (max_location < 0)
      {
        max_location = 0.0f;
      }
      else if (max_location > size.y)
      {
        max_location = size.y;
      }

      Draw_List->AddRectFilled(ImVec2(pos.x, pos.y + size.y - max_location), 
                                ImVec2(pos.x + PROPS.BAR_HEIGHT, pos.y + size.y - min_location), 
                                NEO_MIN_MAX_DIM.color(sdSysData.PROGRAM_TIME.current_frame_time(), sdSysData.COLOR_SELECT.neo_color_DIM(PROPS.COLOR_MARKER)), 
                                5.0f, ImDrawFlags_None);

      Draw_List->AddRect(ImVec2(pos.x, pos.y + size.y - max_location - 2.0f), 
                                ImVec2(pos.x + PROPS.BAR_HEIGHT, pos.y + size.y - min_location + 2.0f), 
                                NEO_MIN_MAX.color(sdSysData.PROGRAM_TIME.current_frame_time(), sdSysData.COLOR_SELECT.neo_color_STANDARD_V(PROPS.COLOR_MARKER)), 
                                5.0f, ImDrawFlags_None, 2.0f);

      if (PROPS.DRAW_RULER)
      {
        RULER.draw(Draw_List, sdSysData, pos, ImVec2(pos.x + PROPS.BAR_HEIGHT, pos.y + size.y));
      }
    }
  }

  // Draw Value Marker
  if (PROPS.HORIZONTAL)
  {
    if ((PROPS.MARKER_LIMIT_AT_RANGE == false) || 
        (is_within(VALUE_MARKER.value(), PROPS.MIN, PROPS.MAX)))
    {
      float value = VALUE_MARKER.value();

      if (value <  PROPS.MIN)
      {
        value = PROPS.MIN + (PROPS.MIN - value);
      }
      else if (value > PROPS.MAX)
      {
        value = PROPS.MAX - (value - PROPS.MAX);
      }

      float marker_location = ((value - PROPS.MIN) / (PROPS.MAX - PROPS.MIN)) * size.x + 1.0f;

      if (marker_location < 0.0f)
      {
        marker_location = 0.0f;
      }
      if (marker_location > size.x)
      {
        marker_location = size.x;
      }

      Draw_List->AddRectFilled(ImVec2(pos.x + marker_location - PROPS.MARKER_SIZE / 2.0f, pos.y), 
                                ImVec2(pos.x + marker_location + PROPS.MARKER_SIZE / 2.0f , pos.y + PROPS.BAR_HEIGHT), 
                                NEO_VALUE.color(sdSysData.PROGRAM_TIME.current_frame_time(), sdSysData.COLOR_SELECT.neo_color_STANDARD_V(PROPS.COLOR_MARKER)), 
                                5.0f, ImDrawFlags_None);    
      
      Draw_List->AddRect(ImVec2(pos.x + marker_location - PROPS.MARKER_SIZE / 2.0f, pos.y), 
                                ImVec2(pos.x + marker_location + PROPS.MARKER_SIZE / 2.0f , pos.y + PROPS.BAR_HEIGHT), 
                                sdSysData.COLOR_SELECT.neo_color_STANDARD(RAS_BLACK), 
                                5.0f, ImDrawFlags_None, 2.0f);
    }
    else
    {
      // Draw out of range markers (HORIZONTAL)
      float marker_location = 0.0f;

      if (VALUE_MARKER.value() < PROPS.MIN)
      {
        marker_location = 0.0f;
      }
      else
      {
        marker_location = size.x;
      }

      Draw_List->AddCircleFilled(ImVec2(pos.x + marker_location, pos.y + PROPS.BAR_HEIGHT / 2.0f), 
                                PROPS.BAR_HEIGHT / 4.0f, 
                                NEO_VALUE.color(sdSysData.PROGRAM_TIME.current_frame_time(), sdSysData.COLOR_SELECT.neo_color_STANDARD_V(PROPS.COLOR_MARKER)), 
                                12.0f);    
      
      Draw_List->AddCircle(ImVec2(pos.x + marker_location, pos.y + PROPS.BAR_HEIGHT / 2.0f), 
                                PROPS.BAR_HEIGHT / 4.0f, 
                                sdSysData.COLOR_SELECT.neo_color_STANDARD(RAS_BLACK), 
                                12.0f, 2.0f);
    }

    // Move Cursor Pos to new position
    ImGui::SetCursorScreenPos(ImVec2(pos.x, pos.y + PROPS.BAR_HEIGHT));
  }
  else
  { // (PROPS.HORIZONTAL == false (vertical)
    if ((PROPS.MARKER_LIMIT_AT_RANGE == false) || 
        (is_within(VALUE_MARKER.value(), PROPS.MIN, PROPS.MAX)))
    {
      float value = VALUE_MARKER.value();

      if (value <  PROPS.MIN)
      {
        value = PROPS.MIN + (PROPS.MIN - value);
      }
      else if (value > PROPS.MAX)
      {
        value = PROPS.MAX - (value - PROPS.MAX);
      }

      float marker_location = ((value - PROPS.MIN) / (PROPS.MAX - PROPS.MIN)) * size.y + 1.0f;

      if (marker_location < 0.0f)
      {
        marker_location = 0.0f;
      }
      if (marker_location > size.y)
      {
        marker_location = size.y;
      }

      Draw_List->AddRectFilled(ImVec2(pos.x, pos.y + size.y - (marker_location + PROPS.MARKER_SIZE / 2.0f)), 
                                ImVec2(pos.x + PROPS.BAR_HEIGHT, pos.y + size.y - (marker_location - PROPS.MARKER_SIZE / 2.0f)), 
                                NEO_VALUE.color(sdSysData.PROGRAM_TIME.current_frame_time(), sdSysData.COLOR_SELECT.neo_color_STANDARD_V(PROPS.COLOR_MARKER)), 
                                5.0f, ImDrawFlags_None);

      Draw_List->AddRect(ImVec2(pos.x, pos.y + size.y - (marker_location + PROPS.MARKER_SIZE / 2.0f)), 
                                ImVec2(pos.x + PROPS.BAR_HEIGHT, pos.y + size.y - (marker_location - PROPS.MARKER_SIZE / 2.0f)), 
                                sdSysData.COLOR_SELECT.neo_color_STANDARD(RAS_BLACK), 
                                5.0f, ImDrawFlags_None, 2.0f);
    }
    else
    {
      // Draw out of range markers  (VERTICAL)
      float marker_location = 0.0f;

      if (VALUE_MARKER.value() < PROPS.MIN)
      {
        marker_location = 0.0f;
      }
      else
      {
        marker_location = size.y;
      }

      Draw_List->AddCircleFilled(ImVec2(pos.x + PROPS.BAR_HEIGHT / 2.0f, pos.y + size.y + marker_location), 
                                PROPS.BAR_HEIGHT / 4.0f, 
                                NEO_VALUE.color(sdSysData.PROGRAM_TIME.current_frame_time(), sdSysData.COLOR_SELECT.neo_color_STANDARD_V(PROPS.COLOR_MARKER)), 
                                12.0f);

      Draw_List->AddCircle(ImVec2(pos.x + PROPS.BAR_HEIGHT / 2.0f, pos.y + size.y + marker_location), 
                                PROPS.BAR_HEIGHT / 4.0f, 
                                sdSysData.COLOR_SELECT.neo_color_STANDARD(RAS_BLACK), 
                                12.0f, 2.0f);
    }

    // Move Cursor Pos to new position
    ImGui::SetCursorScreenPos(ImVec2(pos.x + PROPS.BAR_HEIGHT, pos.y));
  }

  // Min Max
  if (PROPS.HORIZONTAL && PROPS.DRAW_MIN_MAX && PROPS.DRAW_MIN_MAX_ON_BOTTOM)
  {
    draw_min_max_val(sdSysData);
  }
}

// ---------------------------------------------------------------------------------------
#endif