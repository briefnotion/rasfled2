// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: widgets_automobile.cpp
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2859 Core Dynamics
// ***************************************************************************************

#ifndef WIDGETS_AUTOMOBILE_CPP
#define WIDGETS_AUTOMOBILE_CPP

#include "widgets_automobile.h"

// -------------------------------------------------------------------------------------
//  Nova Widget Class

void draw_bit(ImDrawList* Draw_List, system_data &sdSysData,  
              ImVec2 Start_Pos, bool Value, bool Ping, bool Details)
{
  if (Details)
  {
    if (Ping)
    {
      if (Value)
      {
        Draw_List->AddRectFilled(Start_Pos, ImVec2(Start_Pos.x + 4.0f, Start_Pos.y + 15.0f), sdSysData.COLOR_SELECT.neo_c_white().STANDARD_V.color());
      }
      else
      {
        Draw_List->AddRect(Start_Pos, ImVec2(Start_Pos.x + 4.0f, Start_Pos.y + 7.0f), sdSysData.COLOR_SELECT.neo_c_white().STANDARD_V.color());
      }
    }
    else
    {
      if (Value)
      {
        Draw_List->AddRectFilled(Start_Pos, ImVec2(Start_Pos.x + 4.0f, Start_Pos.y + 15.0f), sdSysData.COLOR_SELECT.neo_c_green().STANDARD.color());
      }
      else
      {
        Draw_List->AddRect(Start_Pos, ImVec2(Start_Pos.x + 4.0f, Start_Pos.y + 7.0f), sdSysData.COLOR_SELECT.neo_c_green().STANDARD.color());
      }
    }
  }
  else
  {
    if (Ping)
    {
      if (Value)
      {
        Draw_List->AddLine(Start_Pos, ImVec2(Start_Pos.x + 6.0f, Start_Pos.y), 
                            sdSysData.COLOR_SELECT.neo_c_white().STANDARD_V.color(), 5.0f);
      }
      else
      {
        Draw_List->AddLine(Start_Pos, ImVec2(Start_Pos.x + 6.0f, Start_Pos.y), 
                            sdSysData.COLOR_SELECT.neo_c_white().STANDARD_V.color(), 1.0f);
      }
    }
    else
    {
      if (Value)
      {
        Draw_List->AddLine(Start_Pos, ImVec2(Start_Pos.x + 6.0f, Start_Pos.y), 
                            sdSysData.COLOR_SELECT.neo_c_green().STANDARD.color(), 5.0f);
      }
      else
      {
        Draw_List->AddLine(Start_Pos, ImVec2(Start_Pos.x + 6.0f, Start_Pos.y), 
                            sdSysData.COLOR_SELECT.neo_c_green().STANDARD.color(), 1.0f);
      }
    }
  }
}

void nova_draw(ImDrawList *Draw_List, system_data &sdSysData, NOVA_BITS_VALUE &Value)
{
  ImVec2 current_position = ImGui::GetCursorScreenPos();
  unsigned long current_time_frame = sdSysData.PROGRAM_TIME.current_frame_time();

  if (Value.DETAILS)
  {
    ImGui::Text("%X", Value.ID);
  }

  for (int bit = 0; bit < 64; bit++)
  {
    draw_bit(Draw_List, sdSysData, ImVec2(current_position.x + 40.0f + (9.5f * (float)bit), current_position.y + 5.0f),
              Value.NOVA_BITS[bit], Value.HILIGHT[bit].ping_down(current_time_frame), Value.DETAILS);

    // draw lines
    if (bit % 8 == 0)
    {
      Draw_List->AddLine(ImVec2(current_position.x + 38.0f + (9.5f * (float)bit), current_position.y + 1.0f), 
                          ImVec2(current_position.x + 38.0f + (9.5f * (float)bit), current_position.y + 4.0f), 
                          sdSysData.COLOR_SELECT.neo_c_white().STANDARD_V.color(), 2.0f);
    }
  }

  if (Value.DETAILS)
  {
    ImGui::SetCursorScreenPos(ImVec2(current_position.x, current_position.y + 27.0f));
  }
  else
  {
    ImGui::SetCursorScreenPos(ImVec2(current_position.x, current_position.y + 7.0f));
  }
}

void nova_draw_byte(ImDrawList *Draw_List, system_data &sdSysData,  
                NOVA_BITS_VALUE &Value, int Byte_Position)
{
  ImVec2 current_position = ImGui::GetCursorScreenPos();
  unsigned long current_time_frame = sdSysData.PROGRAM_TIME.current_frame_time();

  for (int bit = 0; bit < 8; bit++)
  {
    draw_bit(Draw_List, sdSysData, ImVec2(current_position.x + 2.0f + (9.5f * (float)bit), current_position.y + 5.0f),
              Value.NOVA_BITS[(Byte_Position * 8) + bit], Value.HILIGHT[(Byte_Position * 8) + bit].ping_down(current_time_frame), true);
  }
}

void draw_nova_detail(system_data &sdSysData, NOVA_BITS_VALUE &Nova_Value, int &Item_Location)
{
  if (Item_Location != -1)
  {
    ImGui::SetNextWindowSize(ImVec2(150.0f, 210.0f));

    if (ImGui::Begin(("Message: " + to_string((int)Nova_Value.ID)).c_str(), 
                      nullptr, sdSysData.SCREEN_DEFAULTS.flags_w_pop)) 
    {
      ImDrawList* draw_list_nova_details = ImGui::GetWindowDrawList();

      for (int byte = 0; byte < 8; byte ++)
      {
        ImGui::Text("%d: %d", byte, Nova_Value.NOVA_BYTES[byte]);
        ImGui::SameLine();
        nova_draw_byte(draw_list_nova_details, sdSysData, Nova_Value, byte);
        ImGui::NewLine();
      }
      
      ImGui::End();
    }
  }
  else
  {
    ImGui::SetNextWindowSize(ImVec2(128.0f, 100.0f));

    if (ImGui::Begin("Message: NOT FOUND", 
                      nullptr, sdSysData.SCREEN_DEFAULTS.flags_w_pop)) 
    {
      ImGui::Text("Message PID\nnot found.");
      
      ImGui::End();
    }
  }
}
// -------------------------------------------------------------------------------------

void T_LARGE_NUMBER_DISPLAY::draw_scroll_num(ImDrawList *Draw_List, float Value, float Y_Height, ImVec2 Start_Pos, ImVec2 Zero_Font_Size)
{
  Value = abs(Value);

  // Number Positions
  ImVec2 pos1 = Start_Pos;

  pos1 = ImVec2(pos1.x, pos1.y + (Y_Height / 2.0f) - (Zero_Font_Size.y / 2.0f));
  ImVec2 pos2 = ImVec2(pos1.x + (0.9f * Zero_Font_Size.x), pos1.y);

  if (CHANGED)
  {
    POS_GRAD_TOP_1 = ImVec2(Start_Pos.x, Start_Pos.y);
    POS_GRAD_TOP_2 = ImVec2(Start_Pos.x + (1.8f * Zero_Font_Size.x), Start_Pos.y + (Y_Height / 2.0f));
    POS_GRAD_BOT_1 = ImVec2(Start_Pos.x, Start_Pos.y + (Y_Height / 2.0f));
    POS_GRAD_BOT_2 = ImVec2(Start_Pos.x + (1.8f * Zero_Font_Size.x), Start_Pos.y + Y_Height);
  }

  int value_tens = (int)(Value) / 10;
  int value_ones = (int)(Value) - value_tens * 10;

  int value_ones_p2 = 0;
  int value_ones_p1 = 0;
  int value_ones_m1 = 0;

  int value_tens_p2 = 0;
  int value_tens_p1 = 0;
  int value_tens_m1 = 0;

  {
    // Ones
    if (value_ones < 9)
    {
      value_ones_p1 = value_ones + 1;
    }
    else
    {
      value_ones_p1 = 0;
    }

    if (value_ones_p1 < 9)
    {
      value_ones_p2 = value_ones_p1 + 1;
    }
    else
    {
      value_ones_p2 = 0;
    }

    if (value_ones > 0)
    {
      value_ones_m1 = value_ones - 1;
    }
    else
    {
      value_ones_m1 = 9;
    }

    // Tens
    if (value_tens < 9)
    {
      value_tens_p1 = value_tens + 1;
    }
    else
    {
      value_tens_p1 = 0;
    }

    if (value_tens_p1 < 9)
    {
      value_tens_p2 = value_tens_p1 + 1;
    }
    else
    {
      value_tens_p2 = 0;
    }

    if (value_tens > 0)
    {
      value_tens_m1 = value_tens - 1;
    }
    else
    {
      value_tens_m1 = 9;
    }
  }

  // Offsets
  float offset_y = ((Value) - (int)(Value)) * (0.75f * Zero_Font_Size.y);
  float offset_y_tens = 0;
  if (value_ones == 9)
  {
    offset_y_tens = offset_y_tens + offset_y;
  }

  // Display
  
  // Tens
  ImGui::SetCursorScreenPos(ImVec2(pos1.x, pos1.y + offset_y_tens - (2.0f * 0.75f * Zero_Font_Size.y)));
  ImGui::Text("%d", value_tens_p2);

  ImGui::SetCursorScreenPos(ImVec2(pos1.x, pos1.y + offset_y_tens - (0.75f * Zero_Font_Size.y)));
  ImGui::Text("%d", value_tens_p1);

  ImGui::SetCursorScreenPos(ImVec2(pos1.x, pos1.y + offset_y_tens));
  ImGui::Text("%d", value_tens);

  ImGui::SetCursorScreenPos(ImVec2(pos1.x, pos1.y + offset_y_tens + (0.75f * Zero_Font_Size.y)));
  ImGui::Text("%d", value_tens_m1);
  
  // Ones
  ImGui::SetCursorScreenPos(ImVec2(pos2.x, pos2.y + offset_y - (2.0f * 0.75f * Zero_Font_Size.y)));
  ImGui::Text("%d", value_ones_p2);

  ImGui::SetCursorScreenPos(ImVec2(pos2.x, pos2.y + offset_y - (0.75f * Zero_Font_Size.y)));
  ImGui::Text("%d", value_ones_p1);

  ImGui::SetCursorScreenPos(ImVec2(pos2.x, pos2.y + offset_y));
  ImGui::Text("%d", value_ones);

  ImGui::SetCursorScreenPos(ImVec2(pos2.x, pos2.y + offset_y + (0.75f * Zero_Font_Size.y)));
  ImGui::Text("%d", value_ones_m1);

  // Mask Top Bot
  ImU32 col_a = ImGui::GetColorU32(IM_COL32(0, 0, 0, 255));
  ImU32 col_b = ImGui::GetColorU32(IM_COL32(0, 0, 0, 0));
  
  Draw_List->AddRectFilledMultiColor(POS_GRAD_TOP_1, POS_GRAD_TOP_2, col_a, col_a, col_b, col_b);
  Draw_List->AddRectFilledMultiColor(POS_GRAD_BOT_1, POS_GRAD_BOT_2, col_b, col_b, col_a, col_a);
}

void T_LARGE_NUMBER_DISPLAY::create()
{
  MIN_MAX.PROP.TIME_SPAN = PROPS.MIN_MAX_TIME_SPAN;
  MIN_MAX.PROP.SLICES = PROPS.MIN_MAX_TIME_SLICES;
  VALUE_WHEEL.set_size(PROPS.WHEEL_FRAME_SIZE);

  MARKER.PROPS.ENABLED = PROPS.DISPLAY_MARKER;
  MARKER.PROPS.COLOR = PROPS.DISPLAY_MARKER_COLOR;
  MARKER.PROPS.LEFT_SIDE = PROPS.LABEL_ON_LEFT;
  MARKER.PROPS.SIZE = 3.0f;
}

void T_LARGE_NUMBER_DISPLAY::update_value(system_data &sdSysData, float Value, float Compare_Value, bool Is_Within)
{
  MIN_MAX.put_value(Value, sdSysData.PROGRAM_TIME.current_frame_time());
  VALUE = Value;
  VALUE_WHEEL.set_value(Value);
  VALUE_COMPARE = Compare_Value;
  IS_TEXT = false;
  ACTIVE_WITHIN = Is_Within;
}

void T_LARGE_NUMBER_DISPLAY::update_value(system_data &sdSysData, float Value)
{
  update_value(sdSysData, Value, -1, false);
}

//void T_LARGE_NUMBER_DISPLAY::update_value(system_data &sdSysData, string Text)
void T_LARGE_NUMBER_DISPLAY::update_value(string Text)
{
  VALUE_TEXT = Text;
  IS_TEXT = true;
}

bool T_LARGE_NUMBER_DISPLAY::draw(system_data &sdSysData, bool Draw, float Y_Height)
{
  bool ret_clicked = false;
  ImVec2 start_position = ImGui::GetCursorScreenPos();

  if (Y_Height != PREV_Y_HEIGHT || 
        FRAME_START_POS.x != start_position.x || 
        FRAME_START_POS.y != start_position.y)
  {
    CHANGED = true;
  
    if (PROPS.VERY_LARGE)
    {
      DISPLAY_SIZE = ImVec2(120.0f, Y_Height);
    }
    else
    {
      DISPLAY_SIZE = ImVec2(83.0f, Y_Height);
    }

    FRAME_START_POS = start_position;
    PREV_Y_HEIGHT = Y_Height;
  }

  ImVec2 pos_offset = ImVec2(0.0f, 0.0f);

  ImGui::BeginChild(PROPS.LABEL.c_str(), DISPLAY_SIZE, false, sdSysData.SCREEN_DEFAULTS.flags_c);

  if (Draw)
  {
    // draw list
    ImDrawList* draw_list_large_number = ImGui::GetWindowDrawList();

    ImGuiIO &io = ImGui::GetIO();

    MARKER.draw(draw_list_large_number, sdSysData, start_position, DISPLAY_SIZE, true);

    ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.neo_c_white().TEXT.color()));

    if (PROPS.LABEL_ON_LEFT == true)
    {
      ImGui::SetCursorScreenPos(ImVec2(start_position.x, start_position.y + 10.0f));
      ImGui::TextUnformatted(PROPS.LABEL.c_str());
      pos_offset.x = pos_offset.x + 10.0f;
    }
    else
    { 
      ImGui::SetCursorScreenPos(ImVec2(start_position.x + DISPLAY_SIZE.x - 9.0f, start_position.y + 10.0f));
      ImGui::TextUnformatted(PROPS.LABEL.c_str());
    }

    ImGui::PopStyleColor();

    //---

    // Draw Large Display
    if (PROPS.VERY_LARGE)
    {
      ImGui::PushFont(io.Fonts->Fonts.Data[3]);
    }
    else
    {
      ImGui::PushFont(io.Fonts->Fonts.Data[1]);
    }

    if (CHANGED)
    {
      ZERO_SIZE = ImGui::CalcTextSize("0");
    }

    // Draw Value
    if (IS_TEXT)
    //  Value is text
    {
      ImGui::SetCursorScreenPos(ImVec2(start_position.x + pos_offset.x, start_position.y + 10.0f));

      ImGui::SetCursorScreenPos(ImVec2(start_position.x + pos_offset.x, start_position.y + (Y_Height / 2.0f) - (ZERO_SIZE.y / 2.0f)));

      ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.neo_c_white().TEXT.color()));
      ImGui::Text("%s", VALUE_TEXT.c_str());
      ImGui::PopStyleColor();
    }
    else if (abs(VALUE) < 100)  // Continue drawing rotating numbers if value is < 100
    {
      if (ACTIVE_WITHIN == true)
      {
        // If cruise is on change the color
        if (is_within(VALUE, VALUE_COMPARE - PROPS.WITHIN_VALUE, VALUE_COMPARE + PROPS.WITHIN_VALUE) == true)
        {
          ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.neo_c_green().TEXT.color()));
        }
        else
        {
          ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.neo_c_yellow().TEXT.color()));
        }

        draw_scroll_num(draw_list_large_number, VALUE_WHEEL.value(), Y_Height, ImVec2(start_position.x + pos_offset.x, start_position.y) ,ZERO_SIZE);

        ImGui::PopStyleColor();
      }
      else
      // Positive Values
      {
        if (PROPS.COLOR_SCALE.active())
        {
          ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.neo_color(PROPS.COLOR_SCALE.get_color(VALUE)).TEXT.color()));
        }
        else
        {
          ImGui::PushStyleColor(ImGuiCol_Text, ImU32(WHEEL_COLOR.boolean_color(sdSysData, (VALUE >= 0.0f), 
                                sdSysData.COLOR_SELECT.neo_c_white().TEXT.color(), 
                                sdSysData.COLOR_SELECT.neo_c_yellow().TEXT.color())));
        }

        draw_scroll_num(draw_list_large_number, VALUE_WHEEL.value(), Y_Height, ImVec2(start_position.x + pos_offset.x, start_position.y), ZERO_SIZE);
        ImGui::PopStyleColor();
      }
    }
    else
    // Value is number but exceeds pritable value
    {
      ImGui::SetCursorScreenPos(ImVec2(start_position.x + pos_offset.x, start_position.y + 10.0f));

      ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.neo_c_red().TEXT.color()));
      ImGui::Text(">>");
      ImGui::PopStyleColor();
    }

    ImGui::PopFont();

    // Draw Compare Number - Small
    if (VALUE_COMPARE >= 0.0f && !IS_TEXT)
    {
      if (PROPS.VERY_LARGE)
      {
        ImGui::PushFont(io.Fonts->Fonts.Data[1]);
      }

      if (ACTIVE_WITHIN == true)
      {
        ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.neo_c_green().TEXT.color()));
      }
      else
      {
        ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.neo_c_grey().TEXT.color()));
      }

      ImGui::SetCursorScreenPos(ImVec2(start_position.x + pos_offset.x, start_position.y));

      ImGui::Text("%2d", (int)VALUE_COMPARE);
      
      ImGui::PopStyleColor();

      if (PROPS.VERY_LARGE)
      {
        ImGui::PopFont();
      }
    }

    if (PROPS.DISPLAY_MIN_MAX)
    {
      string min = "";
      string max = "";
      min = right_justify(2, to_string(abs(MIN_MAX.min())));
      max = right_justify(2, to_string(abs(MIN_MAX.max())));

      ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.neo_c_grey().TEXT.color()));
      
      // Display Min Max
      {
        ImGui::SetCursorScreenPos(ImVec2(start_position.x + (1.75f * ZERO_SIZE.x) + pos_offset.x, start_position.y));
        ImGui::Text("%s", max.c_str());
        ImGui::SetCursorScreenPos(ImVec2(start_position.x + (1.75f * ZERO_SIZE.x) + pos_offset.x, start_position.y + 53.0f));
        ImGui::Text("%s", min.c_str());
      }

      ImGui::PopStyleColor();
    }

    CHANGED = false;
    
    ImGui::SetCursorScreenPos(start_position);
    if (ImGui::InvisibleButton("InvisibleButton", DISPLAY_SIZE))
    {
      ret_clicked = true;
    }
  }
  ImGui::EndChild();

  return ret_clicked;
}

bool T_LARGE_NUMBER_DISPLAY::draw(system_data &sdSysData, bool Draw)
{
  return draw(sdSysData, Draw, 75.0f);
}

bool T_LARGE_NUMBER_DISPLAY::draw(system_data &sdSysData)
{
  return draw(sdSysData, true, 75.0f);
}

// -------------------------------------------------------------------------------------

void W_GUAGE::create()
{
  MIN_MAX.PROP.TIME_SPAN = PROPS.MIN_MAX_TIME_SPAN;
  MIN_MAX.PROP.SLICES = PROPS.MIN_MAX_TIME_SLICES;

  DSP_MIN.PROPS.CHANGE_NOTIFICATION = false;
  DSP_MAX.PROPS.CHANGE_NOTIFICATION = false;
  DSP_VALUE.PROPS.CHANGE_NOTIFICATION = false;
}

void W_GUAGE::update_value(system_data &sdSysData, float Value)
{
  VALUE = Value;
  MIN_MAX.put_value(Value, sdSysData.PROGRAM_TIME.current_frame_time());
}

void W_GUAGE::draw(system_data &sdSysData)
{
  if ((int)MIN_MAX.min() < 0)
  {
    DSP_MIN.PROPS.COLOR = sdSysData.COLOR_SELECT.yellow();
  }
  else
  {
    DSP_MIN.PROPS.COLOR = sdSysData.COLOR_SELECT.white();
  }
  
  if ((int)(VALUE) < 0)
  {
    DSP_VALUE.PROPS.COLOR = sdSysData.COLOR_SELECT.yellow();
  }
  else
  {
    DSP_VALUE.PROPS.COLOR = sdSysData.COLOR_SELECT.white();
  }

  if ((int)(MIN_MAX.max()) < 0)
  {
    DSP_MAX.PROPS.COLOR = sdSysData.COLOR_SELECT.yellow();
  }
  else
  {
    DSP_MAX.PROPS.COLOR = sdSysData.COLOR_SELECT.white();
  }

  DSP_MIN.update_text(sdSysData, to_string((int)(MIN_MAX.min())));
  DSP_VALUE.update_text(sdSysData, to_string((int)(VALUE)));
  DSP_MAX.update_text(sdSysData, to_string((int)(MIN_MAX.max())));

  if (ImGui::BeginTable("Automobile Guage Val Min Max", 4, 0))
  {
    {
      ImGui::TableNextRow();
      ImGui::TableNextColumn();
      
      ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.neo_c_white().TEXT.color()));
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

  ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.neo_c_white().TEXT.color()));
  ImGui::ProgressBar((abs)(VALUE / PROPS.VALUE_MAX), ImVec2(-1.0f,18.0f), (to_string_round_to_nth(VALUE, 1)).c_str());
  ImGui::PopStyleColor();
}

// ---------------------------------------------------------------------------------------

void T_DATA_DISPLAY::create(system_data &sdSysData)
{
  LABEL.update_text(sdSysData, PROPS.LABEL);
  LABEL.PROPS.COLOR = PROPS.COLOR;
  DATA.PROPS.COLOR = PROPS.COLOR;

  if (PROPS.LABEL_TEXT_SIZE == -1)
  {
    LABEL_PIXEL_SIZE = ImGui::CalcTextSize(PROPS.LABEL.c_str());
  }
  else
  {
    LABEL_PIXEL_SIZE = ImGui::CalcTextSize(linefill(PROPS.LABEL_TEXT_SIZE, "X").c_str());
  }

  MARKER.PROPS.ENABLED = PROPS.DISPLAY_MARKER;
  MARKER.PROPS.COLOR = PROPS.DISPLAY_MARKER_COLOR;
  MARKER.PROPS.LEFT_SIDE = true;
  MARKER.PROPS.SIZE = 5.0f;
  MARKER.PROPS.HORIZONTAL = true;
}

void T_DATA_DISPLAY::update_value(system_data &sdSysData, string String_Value, float Float_Value)
{
  VALUE_STRING = String_Value;
  VALUE_FLOAT = Float_Value;

  //---

  DATA.update_text(sdSysData, VALUE_STRING);
}

void T_DATA_DISPLAY::update_value(system_data &sdSysData, string String_Value)
{
  update_value(sdSysData, String_Value, 0.0f);
}

void T_DATA_DISPLAY::draw(ImDrawList *Draw_List, system_data &sdSysData, bool Draw_Marker)
{
  ImVec2 pos = ImGui::GetCursorScreenPos();

  MARKER.draw(Draw_List, sdSysData, pos, ImVec2(50.0f, 20.0f), Draw_Marker);

  LABEL.draw(sdSysData);
  ImGui::SameLine();

  pos.x = pos.x + LABEL_PIXEL_SIZE.x;
  ImGui::SetCursorScreenPos(pos);

  if (PROPS.COLOR_SCALE.active())
  {
    DATA.PROPS.COLOR = PROPS.COLOR_SCALE.get_color(VALUE_FLOAT);
    DATA.draw(sdSysData);
  }
  else
  {
    DATA.draw(sdSysData);
  }
}

void T_DATA_DISPLAY::draw(ImDrawList *Draw_List, system_data &sdSysData)
{
  draw(Draw_List, sdSysData, false);
}

// ---------------------------------------------------------------------------------------

void AUTOMOBILE_SCREEN::nova(system_data &sdSysData)
{
  ImGui::BeginChild("Nova Display Buttons", ImVec2(90.0f, ImGui::GetContentRegionAvail().y), true, sdSysData.SCREEN_DEFAULTS.flags_c);
  {
    if (BTC_ENABLED.button_toggle_color(sdSysData, "ENABLE\n(On)", "ENABLE\n(Off)", sdSysData.CAR_INFO.NOVA.ENABLED, 
                    sdSysData.COLOR_SELECT.red(), sdSysData.COLOR_SELECT.blue(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
    {
      sdSysData.CAR_INFO.NOVA.ENABLED = !sdSysData.CAR_INFO.NOVA.ENABLED;
    }

    if (BTC_VAL.button_toggle_color(sdSysData, "VAL\n(On)", "VAL\n(Off)", NOVA_CLIP_DETAIL, 
                    sdSysData.COLOR_SELECT.red(), sdSysData.COLOR_SELECT.blue(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
    {
      NOVA_CLIP_DETAIL = !NOVA_CLIP_DETAIL;
    }

    ImGui::Text("Size: %d", sdSysData.CAR_INFO.NOVA.NOVA_ITEMS.size());

    ImGui::Text("CMQ: %d", sdSysData.CAR_INFO.STATISTICS.can_max_queue());

    ImGui::Text("CAN: %d", sdSysData.CAR_INFO.STATISTICS.can_sent());

    ImGui::Text("RAS: %d", sdSysData.CAR_INFO.STATISTICS.ras_recieved());

    ImGui::Text("CAN_T:");
    ImGui::Text(" %d", sdSysData.CAR_INFO.STATISTICS.can_sent_total_count());

    ImGui::Text("RAS_T:");
    ImGui::Text(" %d", sdSysData.CAR_INFO.STATISTICS.ras_recieved_total_count());

    //ImGui::Text("DIF_T:");
    //ImGui::Text(" %d", sdSysData.CAR_INFO.STATISTICS.can_sent_total_count() - 
    //                    sdSysData.CAR_INFO.STATISTICS.ras_recieved_total_count());

    ImGui::Text("ERR:");
    ImGui::Text(" %d", sdSysData.CAR_INFO.STATISTICS.errors());

    if (BC_NOVA_1.button_color(sdSysData, "NOVA", sdSysData.COLOR_SELECT.blue(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON))
    {
      DISPLAY_NOVA = false;
    }
  }
  ImGui::EndChild();

  ImGui::SameLine();

  ImGui::BeginChild("Nova Display Bit View", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true, sdSysData.SCREEN_DEFAULTS.flags_c);
  {
    ImDrawList* draw_list_nova = ImGui::GetWindowDrawList();

    ImVec2 button_adjust;

    for(int items = 0; items < (int)sdSysData.CAR_INFO.NOVA.NOVA_ITEMS.size(); items++)
    {
      if (sdSysData.CAR_INFO.NOVA.NOVA_ITEMS[items].NOVA_VALUE.NON_CONSISTANT == false)
      {
        button_adjust = ImGui::GetCursorScreenPos();

        // Rudimentry does it fit on screen comparison
        if (button_adjust.y < 1024.0f)
        {  
          if(sdSysData.CAR_INFO.NOVA.NOVA_ITEMS[items].NOVA_VALUE.DETAILS)
          {
            // Determini if expanded view is on or off.
            if (ImGui::InvisibleButton(("InvisibleButton" + to_string(items)).c_str(), ImVec2(650.0f ,27.0f)))
            {
              sdSysData.CAR_INFO.NOVA.NOVA_ITEMS[items].NOVA_VALUE.DETAILS = !sdSysData.CAR_INFO.NOVA.NOVA_ITEMS[items].NOVA_VALUE.DETAILS;
            }
          }
          else
          {
            if (ImGui::InvisibleButton(("InvisibleButton" + to_string(items)).c_str(), ImVec2(650.0f ,7.0f)))
            {
              sdSysData.CAR_INFO.NOVA.NOVA_ITEMS[items].NOVA_VALUE.DETAILS = !sdSysData.CAR_INFO.NOVA.NOVA_ITEMS[items].NOVA_VALUE.DETAILS;
            }
          
            // If hovered, draw id.
            if (ImGui::IsItemHovered())
            {
              ImGui::Text("%X", sdSysData.CAR_INFO.NOVA.NOVA_ITEMS[items].NOVA_VALUE.ID);
            }
          }

          ImGui::SetCursorScreenPos(button_adjust);

          // Draw bit info.
          nova_draw(draw_list_nova, sdSysData, sdSysData.CAR_INFO.NOVA.NOVA_ITEMS[items].NOVA_VALUE);

          //if expanded, draw window with values.
          if (sdSysData.CAR_INFO.NOVA.NOVA_ITEMS[items].NOVA_VALUE.DETAILS)
          {
            draw_nova_detail(sdSysData, sdSysData.CAR_INFO.NOVA.NOVA_ITEMS[items].NOVA_VALUE, items);
          }
        }
      }
    }
  }
  
  ImGui::EndChild();

  // Nova Clip Detail Window
  if (NOVA_CLIP_DETAIL)
  {
    int item_location =  sdSysData.CAR_INFO.NOVA.find_location(sdSysData.CLIPBOARD.get_int_value());

    draw_nova_detail(sdSysData, sdSysData.CAR_INFO.NOVA.NOVA_ITEMS[item_location].NOVA_VALUE, item_location);
  }
}

void AUTOMOBILE_SCREEN::create(system_data &sdSysData)
{
  // Large Displays on Main Sidebar Screen
  SDATA.L_SPEED_SB.PROPS.LABEL = "S\nP\nD";
  SDATA.L_SPEED_SB.PROPS.LABEL_ON_LEFT = true;
  SDATA.L_SPEED_SB.PROPS.WHEEL_FRAME_SIZE = 8;
  SDATA.L_SPEED_SB.PROPS.DISPLAY_MIN_MAX = true;
  SDATA.L_SPEED_SB.PROPS.MIN_MAX_TIME_SPAN = 5* 60000;
  SDATA.L_SPEED_SB.PROPS.DISPLAY_MARKER = true;
  SDATA.L_SPEED_SB.PROPS.DISPLAY_MARKER_COLOR = sdSysData.COLOR_SELECT.white();
  SDATA.L_SPEED_SB.create();

  SDATA.L_ACCELERATION_SB.PROPS.LABEL = "A\nC\nC";
  SDATA.L_ACCELERATION_SB.PROPS.LABEL_ON_LEFT = true;
  SDATA.L_ACCELERATION_SB.PROPS.DISPLAY_MIN_MAX = true;
  SDATA.L_ACCELERATION_SB.PROPS.MIN_MAX_TIME_SPAN = 5* 60000;
  SDATA.L_ACCELERATION_SB.PROPS.WHEEL_FRAME_SIZE = 33;
  SDATA.L_ACCELERATION_SB.PROPS.DISPLAY_MARKER = true;
  SDATA.L_ACCELERATION_SB.PROPS.DISPLAY_MARKER_COLOR = sdSysData.COLOR_SELECT.cyan();
  SDATA.L_ACCELERATION_SB.create();
  
  SDATA.L_GEAR_SB.PROPS.LABEL = "G\nE\nR";
  SDATA.L_GEAR_SB.PROPS.LABEL_ON_LEFT = true;
  SDATA.L_GEAR_SB.PROPS.MIN_MAX_TIME_SPAN = 5* 60000;
  SDATA.L_GEAR_SB.create();
  
  SDATA.L_TACH_SB.PROPS.LABEL = "T\nA\nC";
  SDATA.L_TACH_SB.PROPS.LABEL_ON_LEFT = true;
  SDATA.L_TACH_SB.PROPS.DISPLAY_MIN_MAX = true;
  SDATA.L_TACH_SB.PROPS.MIN_MAX_TIME_SPAN = 5* 60000;
  SDATA.L_TACH_SB.PROPS.WHEEL_FRAME_SIZE = 4;
  SDATA.L_TACH_SB.PROPS.DISPLAY_MARKER = true;
  SDATA.L_TACH_SB.PROPS.DISPLAY_MARKER_COLOR = sdSysData.COLOR_SELECT.green();
  SDATA.L_TACH_SB.create();

  SDATA.L_VOLTAGE_SB.PROPS.LABEL = "V\nL\nT";
  SDATA.L_VOLTAGE_SB.PROPS.LABEL_ON_LEFT = true;
  SDATA.L_VOLTAGE_SB.PROPS.DISPLAY_MIN_MAX = true;
  SDATA.L_VOLTAGE_SB.PROPS.MIN_MAX_TIME_SPAN = 5* 60000;
  SDATA.L_VOLTAGE_SB.PROPS.COLOR_SCALE.add_color_value_pair(8.0f, sdSysData.COLOR_SELECT.red());
  SDATA.L_VOLTAGE_SB.PROPS.COLOR_SCALE.add_color_value_pair(12.0f, sdSysData.COLOR_SELECT.yellow());
  SDATA.L_VOLTAGE_SB.PROPS.COLOR_SCALE.add_color_value_pair(14.3f, sdSysData.COLOR_SELECT.green());
  SDATA.L_VOLTAGE_SB.PROPS.COLOR_SCALE.add_color_value_pair(16.0f, sdSysData.COLOR_SELECT.yellow());
  SDATA.L_VOLTAGE_SB.PROPS.COLOR_SCALE.add_color_value_pair(100.0f, sdSysData.COLOR_SELECT.red());
  SDATA.L_VOLTAGE_SB.PROPS.DISPLAY_MARKER = true;
  SDATA.L_VOLTAGE_SB.PROPS.DISPLAY_MARKER_COLOR = sdSysData.COLOR_SELECT.red();
  SDATA.L_VOLTAGE_SB.PROPS.WHEEL_FRAME_SIZE = 18;
  SDATA.L_VOLTAGE_SB.create();
  
  SDATA.L_S_TEMP_SB.PROPS.LABEL = "T\nM\nP";
  SDATA.L_S_TEMP_SB.PROPS.LABEL_ON_LEFT = true;
  SDATA.L_S_TEMP_SB.PROPS.DISPLAY_MIN_MAX = true;
  SDATA.L_S_TEMP_SB.PROPS.MIN_MAX_TIME_SPAN = 5* 60000;
  SDATA.L_S_TEMP_SB.PROPS.COLOR_SCALE.add_color_value_pair(40.0f, sdSysData.COLOR_SELECT.blue());
  SDATA.L_S_TEMP_SB.PROPS.COLOR_SCALE.add_color_value_pair(50.0f, sdSysData.COLOR_SELECT.green());
  SDATA.L_S_TEMP_SB.PROPS.COLOR_SCALE.add_color_value_pair(60.0f, sdSysData.COLOR_SELECT.yellow());
  SDATA.L_S_TEMP_SB.PROPS.COLOR_SCALE.add_color_value_pair(70.0f, sdSysData.COLOR_SELECT.red());
  SDATA.L_S_TEMP_SB.PROPS.COLOR_SCALE.add_color_value_pair(100.0f, sdSysData.COLOR_SELECT.purple());
  SDATA.L_S_TEMP_SB.PROPS.DISPLAY_MARKER = true;
  SDATA.L_S_TEMP_SB.PROPS.DISPLAY_MARKER_COLOR = sdSysData.COLOR_SELECT.orange();
  SDATA.L_S_TEMP_SB.PROPS.WHEEL_FRAME_SIZE = 33;
  SDATA.L_S_TEMP_SB.create();

  // Continuing Configuring Main Screen Widgets

  SDATA.D_FUEL_RAIL_PRESSURE.PROPS.LABEL = right_justify(10, "F Rail P: ");
  SDATA.D_FUEL_RAIL_PRESSURE.PROPS.COLOR = sdSysData.COLOR_SELECT.white();
  SDATA.D_FUEL_RAIL_PRESSURE.PROPS.DISPLAY_MARKER = true;
  SDATA.D_FUEL_RAIL_PRESSURE.PROPS.DISPLAY_MARKER_COLOR = sdSysData.COLOR_SELECT.purple();
  SDATA.D_FUEL_RAIL_PRESSURE.create(sdSysData);

  SDATA.D_EVAP_SYSTEM_VAP_PRESSURE.PROPS.LABEL = right_justify(10, "Sys Vp P: ");
  SDATA.D_EVAP_SYSTEM_VAP_PRESSURE.PROPS.COLOR = sdSysData.COLOR_SELECT.white();
  SDATA.D_EVAP_SYSTEM_VAP_PRESSURE.PROPS.DISPLAY_MARKER = true;
  SDATA.D_EVAP_SYSTEM_VAP_PRESSURE.PROPS.DISPLAY_MARKER_COLOR = sdSysData.COLOR_SELECT.pink();
  SDATA.D_EVAP_SYSTEM_VAP_PRESSURE.create(sdSysData);

  SDATA.D_VOLTAGE.PROPS.LABEL = right_justify(10, "Voltage: ");
  SDATA.D_VOLTAGE.PROPS.COLOR = sdSysData.COLOR_SELECT.white();
  SDATA.D_VOLTAGE.PROPS.COLOR_SCALE.add_color_value_pair(8.0f, sdSysData.COLOR_SELECT.red());
  SDATA.D_VOLTAGE.PROPS.COLOR_SCALE.add_color_value_pair(12.0f, sdSysData.COLOR_SELECT.yellow());
  SDATA.D_VOLTAGE.PROPS.COLOR_SCALE.add_color_value_pair(14.0f, sdSysData.COLOR_SELECT.green());
  SDATA.D_VOLTAGE.PROPS.COLOR_SCALE.add_color_value_pair(16.0f, sdSysData.COLOR_SELECT.yellow());
  SDATA.D_VOLTAGE.PROPS.COLOR_SCALE.add_color_value_pair(100.0f, sdSysData.COLOR_SELECT.red());
  SDATA.D_VOLTAGE.PROPS.DISPLAY_MARKER = true;
  SDATA.D_VOLTAGE.PROPS.DISPLAY_MARKER_COLOR = sdSysData.COLOR_SELECT.red();
  SDATA.D_VOLTAGE.create(sdSysData);

  SDATA.D_BAROMETER.PROPS.LABEL = right_justify(10, "Baro: ");
  SDATA.D_BAROMETER.PROPS.COLOR = sdSysData.COLOR_SELECT.white();
  SDATA.D_BAROMETER.create(sdSysData);

  SDATA.D_CAM_COMM_ERROR.PROPS.LABEL = right_justify(10, "Errors: ");
  SDATA.D_CAM_COMM_ERROR.PROPS.COLOR = sdSysData.COLOR_SELECT.white();
  SDATA.D_CAM_COMM_ERROR.create(sdSysData);

  SDATA.D_TEMP_AMBIANT.PROPS.LABEL = right_justify(10, "Ambiant: ");
  SDATA.D_TEMP_AMBIANT.PROPS.COLOR = sdSysData.COLOR_SELECT.white();
  SDATA.D_TEMP_AMBIANT.PROPS.COLOR_SCALE.add_color_value_pair(0.0f, sdSysData.COLOR_SELECT.blue());
  SDATA.D_TEMP_AMBIANT.PROPS.COLOR_SCALE.add_color_value_pair(30.0f, sdSysData.COLOR_SELECT.green());
  SDATA.D_TEMP_AMBIANT.PROPS.COLOR_SCALE.add_color_value_pair(38.0f, sdSysData.COLOR_SELECT.yellow());
  SDATA.D_TEMP_AMBIANT.PROPS.COLOR_SCALE.add_color_value_pair(43.0f, sdSysData.COLOR_SELECT.red());
  SDATA.D_TEMP_AMBIANT.PROPS.COLOR_SCALE.add_color_value_pair(100.0f, sdSysData.COLOR_SELECT.purple());
  SDATA.D_TEMP_AMBIANT.PROPS.DISPLAY_MARKER = true;
  SDATA.D_TEMP_AMBIANT.PROPS.DISPLAY_MARKER_COLOR = sdSysData.COLOR_SELECT.cyan();
  SDATA.D_TEMP_AMBIANT.create(sdSysData);

  SDATA.D_TEMP_INTAKE.PROPS.LABEL = right_justify(10, "Intake: ");
  SDATA.D_TEMP_INTAKE.PROPS.COLOR = sdSysData.COLOR_SELECT.white();
  SDATA.D_TEMP_INTAKE.PROPS.COLOR_SCALE.add_color_value_pair(10.0f, sdSysData.COLOR_SELECT.blue());
  SDATA.D_TEMP_INTAKE.PROPS.COLOR_SCALE.add_color_value_pair(40.0f, sdSysData.COLOR_SELECT.green());
  SDATA.D_TEMP_INTAKE.PROPS.COLOR_SCALE.add_color_value_pair(48.0f, sdSysData.COLOR_SELECT.yellow());
  SDATA.D_TEMP_INTAKE.PROPS.COLOR_SCALE.add_color_value_pair(53.0f, sdSysData.COLOR_SELECT.red());
  SDATA.D_TEMP_INTAKE.PROPS.COLOR_SCALE.add_color_value_pair(100.0f, sdSysData.COLOR_SELECT.purple());
  SDATA.D_TEMP_INTAKE.PROPS.DISPLAY_MARKER = true;
  SDATA.D_TEMP_INTAKE.PROPS.DISPLAY_MARKER_COLOR = sdSysData.COLOR_SELECT.yellow();
  SDATA.D_TEMP_INTAKE.create(sdSysData);

  SDATA.D_TEMP_COOLANT.PROPS.LABEL = right_justify(10, "Coolant: ");
  SDATA.D_TEMP_COOLANT.PROPS.COLOR = sdSysData.COLOR_SELECT.white();
  SDATA.D_TEMP_COOLANT.PROPS.COLOR_SCALE.add_color_value_pair(50.0f, sdSysData.COLOR_SELECT.blue());
  SDATA.D_TEMP_COOLANT.PROPS.COLOR_SCALE.add_color_value_pair(90.0f, sdSysData.COLOR_SELECT.green());
  SDATA.D_TEMP_COOLANT.PROPS.COLOR_SCALE.add_color_value_pair(100.0f, sdSysData.COLOR_SELECT.yellow());
  SDATA.D_TEMP_COOLANT.PROPS.COLOR_SCALE.add_color_value_pair(110.0f, sdSysData.COLOR_SELECT.red());
  SDATA.D_TEMP_COOLANT.PROPS.COLOR_SCALE.add_color_value_pair(200.0f, sdSysData.COLOR_SELECT.purple());
  SDATA.D_TEMP_COOLANT.PROPS.DISPLAY_MARKER = true;
  SDATA.D_TEMP_COOLANT.PROPS.DISPLAY_MARKER_COLOR = sdSysData.COLOR_SELECT.white();
  SDATA.D_TEMP_COOLANT.create(sdSysData);

  SDATA.D_TEMP_CATALYST.PROPS.LABEL = right_justify(10, "Catalyst: ");
  SDATA.D_TEMP_CATALYST.PROPS.COLOR = sdSysData.COLOR_SELECT.white();
  SDATA.D_TEMP_CATALYST.PROPS.COLOR_SCALE.add_color_value_pair(150.0f, sdSysData.COLOR_SELECT.blue());
  SDATA.D_TEMP_CATALYST.PROPS.COLOR_SCALE.add_color_value_pair(750.0f, sdSysData.COLOR_SELECT.green());
  SDATA.D_TEMP_CATALYST.PROPS.COLOR_SCALE.add_color_value_pair(850.0f, sdSysData.COLOR_SELECT.yellow());
  SDATA.D_TEMP_CATALYST.PROPS.COLOR_SCALE.add_color_value_pair(950.0f, sdSysData.COLOR_SELECT.red());
  SDATA.D_TEMP_CATALYST.PROPS.COLOR_SCALE.add_color_value_pair(2000.0f, sdSysData.COLOR_SELECT.purple());
  SDATA.D_TEMP_CATALYST.PROPS.DISPLAY_MARKER = true;
  SDATA.D_TEMP_CATALYST.PROPS.DISPLAY_MARKER_COLOR = sdSysData.COLOR_SELECT.purple();
  SDATA.D_TEMP_CATALYST.create(sdSysData);

  SDATA.D_TEMP_SUPER_TEMP.PROPS.LABEL = right_justify(10, "S-Temp: ");
  SDATA.D_TEMP_SUPER_TEMP.PROPS.COLOR = sdSysData.COLOR_SELECT.white();
  SDATA.D_TEMP_SUPER_TEMP.PROPS.COLOR_SCALE.add_color_value_pair(40.0f, sdSysData.COLOR_SELECT.blue());
  SDATA.D_TEMP_SUPER_TEMP.PROPS.COLOR_SCALE.add_color_value_pair(50.0f, sdSysData.COLOR_SELECT.green());
  SDATA.D_TEMP_SUPER_TEMP.PROPS.COLOR_SCALE.add_color_value_pair(60.0f, sdSysData.COLOR_SELECT.yellow());
  SDATA.D_TEMP_SUPER_TEMP.PROPS.COLOR_SCALE.add_color_value_pair(70.0f, sdSysData.COLOR_SELECT.red());
  SDATA.D_TEMP_SUPER_TEMP.PROPS.COLOR_SCALE.add_color_value_pair(100.0f, sdSysData.COLOR_SELECT.purple());
  SDATA.D_TEMP_SUPER_TEMP.PROPS.DISPLAY_MARKER = true;
  SDATA.D_TEMP_SUPER_TEMP.PROPS.DISPLAY_MARKER_COLOR = sdSysData.COLOR_SELECT.orange();
  SDATA.D_TEMP_SUPER_TEMP.create(sdSysData);

  SDATA.OC_LF_TTL.PROPS.COLOR = sdSysData.COLOR_SELECT.orange();
  SDATA.OC_RF_TTL.PROPS.COLOR = sdSysData.COLOR_SELECT.orange();
  SDATA.OC_LB_TTL.PROPS.COLOR = sdSysData.COLOR_SELECT.orange();
  SDATA.OC_RB_TTL.PROPS.COLOR = sdSysData.COLOR_SELECT.orange();

  SDATA.D_LF_TTL.PROPS.LABEL = "TTL LF: ";
  SDATA.D_LF_TTL.PROPS.COLOR = sdSysData.COLOR_SELECT.white();
  SDATA.D_LF_TTL.PROPS.COLOR_SCALE.add_color_value_pair(25.0f, sdSysData.COLOR_SELECT.red());
  SDATA.D_LF_TTL.PROPS.COLOR_SCALE.add_color_value_pair(50.0f, sdSysData.COLOR_SELECT.yellow());
  SDATA.D_LF_TTL.PROPS.COLOR_SCALE.add_color_value_pair(200.0f, sdSysData.COLOR_SELECT.green());
  SDATA.D_LF_TTL.create(sdSysData);

  SDATA.D_RF_TTL.PROPS.LABEL = "TTL RF: ";
  SDATA.D_RF_TTL.PROPS.COLOR = sdSysData.COLOR_SELECT.white();
  SDATA.D_RF_TTL.PROPS.COLOR_SCALE.add_color_value_pair(25.0f, sdSysData.COLOR_SELECT.red());
  SDATA.D_RF_TTL.PROPS.COLOR_SCALE.add_color_value_pair(50.0f, sdSysData.COLOR_SELECT.yellow());
  SDATA.D_RF_TTL.PROPS.COLOR_SCALE.add_color_value_pair(200.0f, sdSysData.COLOR_SELECT.green());
  SDATA.D_RF_TTL.create(sdSysData);

  SDATA.D_LB_TTL.PROPS.LABEL = "TTL LB: ";
  SDATA.D_LB_TTL.PROPS.COLOR = sdSysData.COLOR_SELECT.white();
  SDATA.D_LB_TTL.PROPS.COLOR_SCALE.add_color_value_pair(25.0f, sdSysData.COLOR_SELECT.red());
  SDATA.D_LB_TTL.PROPS.COLOR_SCALE.add_color_value_pair(50.0f, sdSysData.COLOR_SELECT.yellow());
  SDATA.D_LB_TTL.PROPS.COLOR_SCALE.add_color_value_pair(200.0f, sdSysData.COLOR_SELECT.green());
  SDATA.D_LB_TTL.create(sdSysData);

  SDATA.D_RB_TTL.PROPS.LABEL = "TTL RB: ";
  SDATA.D_RB_TTL.PROPS.COLOR = sdSysData.COLOR_SELECT.white();
  SDATA.D_RB_TTL.PROPS.COLOR_SCALE.add_color_value_pair(25.0f, sdSysData.COLOR_SELECT.red());
  SDATA.D_RB_TTL.PROPS.COLOR_SCALE.add_color_value_pair(50.0f, sdSysData.COLOR_SELECT.yellow());
  SDATA.D_RB_TTL.PROPS.COLOR_SCALE.add_color_value_pair(200.0f, sdSysData.COLOR_SELECT.green());
  SDATA.D_RB_TTL.create(sdSysData);

  SDATA.D_LIGHTS.PROPS.LABEL = right_justify(10, "LIGHTS: ");
  SDATA.D_LIGHTS.PROPS.COLOR = sdSysData.COLOR_SELECT.white();
  SDATA.D_LIGHTS.create(sdSysData);

  SDATA.D_FUEL_LEVEL.PROPS.LABEL = right_justify(10, "FUEL LVL: ");
  SDATA.D_FUEL_LEVEL.PROPS.COLOR = sdSysData.COLOR_SELECT.white();
  SDATA.D_FUEL_LEVEL.PROPS.COLOR_SCALE.add_color_value_pair(1.0f, sdSysData.COLOR_SELECT.red());
  SDATA.D_FUEL_LEVEL.PROPS.COLOR_SCALE.add_color_value_pair(4.0f, sdSysData.COLOR_SELECT.yellow());
  SDATA.D_FUEL_LEVEL.PROPS.COLOR_SCALE.add_color_value_pair(14.0f, sdSysData.COLOR_SELECT.green());
  SDATA.D_FUEL_LEVEL.PROPS.DISPLAY_MARKER = true;
  SDATA.D_FUEL_LEVEL.PROPS.DISPLAY_MARKER_COLOR = sdSysData.COLOR_SELECT.cyan();
  SDATA.D_FUEL_LEVEL.create(sdSysData);

  SDATA.D_PARKING_BRAKE.PROPS.COLOR_SCALE.add_color_value_pair(0.0f, sdSysData.COLOR_SELECT.green());
  SDATA.D_PARKING_BRAKE.PROPS.COLOR_SCALE.add_color_value_pair(1.0f, sdSysData.COLOR_SELECT.red());
  SDATA.D_PARKING_BRAKE.PROPS.LABEL = right_justify(10, "PARK BRK: ");
  SDATA.D_PARKING_BRAKE.PROPS.COLOR = sdSysData.COLOR_SELECT.white();
  SDATA.D_PARKING_BRAKE.create(sdSysData);

  SDATA.D_CRUISE_ON.PROPS.LABEL = right_justify(10, "C ON: ");
  SDATA.D_CRUISE_ON.PROPS.COLOR = sdSysData.COLOR_SELECT.white();
  SDATA.D_CRUISE_ON.create(sdSysData);

  SDATA.D_CRUISE_SPEED.PROPS.LABEL = right_justify(10, "C SPD: ");
  SDATA.D_CRUISE_SPEED.PROPS.COLOR = sdSysData.COLOR_SELECT.white();
  SDATA.D_CRUISE_SPEED.create(sdSysData);

  // ------------------------------------------
  // Outside
  {
    SDATA.L_SPEED_OUTSIDE.PROPS.LABEL = "S\nP\nE\nE\nD";
    SDATA.L_SPEED_OUTSIDE.PROPS.LABEL_ON_LEFT = true;
    SDATA.L_SPEED_OUTSIDE.PROPS.WHEEL_FRAME_SIZE = 8;
    SDATA.L_SPEED_OUTSIDE.PROPS.DISPLAY_MIN_MAX = true;
    SDATA.L_SPEED_OUTSIDE.PROPS.MIN_MAX_TIME_SPAN = 5* 60000;
    SDATA.L_SPEED_OUTSIDE.PROPS.DISPLAY_MARKER = true;
    SDATA.L_SPEED_OUTSIDE.PROPS.DISPLAY_MARKER_COLOR = sdSysData.COLOR_SELECT.white();
    SDATA.L_SPEED_OUTSIDE.PROPS.VERY_LARGE = true;
    SDATA.L_SPEED_OUTSIDE.create();
  }

  // Slow Plot line
  {
    SDATA.PLOT_SLOW.PROPS.LABEL = "Speed Plot";
    SDATA.PLOT_SLOW.PROPS.COLOR_GRID = sdSysData.COLOR_SELECT.white();
    SDATA.PLOT_SLOW.PROPS.GRID_SEPERATOR_COUNT_HORIZONTAL = 4;
    SDATA.PLOT_SLOW.PROPS.DATA_POINTS_VALUE_MAX = 80;        // 80mph
    SDATA.PLOT_SLOW.PROPS.LEFT_TO_RIGHT = false;
    SDATA.PLOT_SLOW.PROPS.BOTTOM_TO_TOP = true;

    SDATA.PLOT_SLOW.PROPS.GRID_SEPERATOR_COUNT_VERTICAL = 5;
    SDATA.PLOT_SLOW.create_subgraph(300,         10 * 1000, "10s");
    SDATA.PLOT_SLOW.create_subgraph(58,         290 * 1000, "5m (4:50m)");
    SDATA.PLOT_SLOW.create_subgraph(60,     40 * 60 * 1000, "45m (40m)");
    SDATA.PLOT_SLOW.create_subgraph(60,    140 * 60 * 1000, "3h (2:20m)");
    SDATA.PLOT_SLOW.create_subgraph(60, 5 * 60 * 60 * 1000, "8h (5h)"); 

    SDATA.PLOT_SLOW.create_line(sdSysData.COLOR_SELECT.green(), false, false, 2.0f, 1.0f, true);
    SDATA.PLOT_SLOW.create_line(sdSysData.COLOR_SELECT.orange(), false, false, 2.0f, 1.0f, true);
    SDATA.PLOT_SLOW.create_line(sdSysData.COLOR_SELECT.red(), true, true, 2.0f, 1.0f);
    SDATA.PLOT_SLOW.create_line(sdSysData.COLOR_SELECT.white(), true, true, 2.0f, 1.0f);
    SDATA.PLOT_SLOW.create_line(sdSysData.COLOR_SELECT.orange(), true, true, 2.0f, 1.0f);
    SDATA.PLOT_SLOW.create_line(sdSysData.COLOR_SELECT.cyan(), true, true, 2.0f, 1.0f);
    
    SDATA.PLOT_SLOW.create(sdSysData.PROGRAM_TIME.current_frame_time());
  }

  // Vertical Bars
  {
    {
      SDATA.VB_SPEED.PROPS.LABEL = "Speed Vertical";
      SDATA.VB_SPEED.PROPS.BAR_HEIGHT = 30 / 4;
      SDATA.VB_SPEED.PROPS.MARKER_SIZE = 15;
      SDATA.VB_SPEED.PROPS.COLOR_BACKGROUND = sdSysData.COLOR_SELECT.blue();
      SDATA.VB_SPEED.PROPS.COLOR_MARKER = sdSysData.COLOR_SELECT.white();
      SDATA.VB_SPEED.PROPS.DISPLAY_SINGLE_POINT_FLOAT = true;
      SDATA.VB_SPEED.PROPS.DRAW_MIN_MAX = true;
      SDATA.VB_SPEED.PROPS.MAX = 80.0f;
      SDATA.VB_SPEED.PROPS.HORIZONTAL = false;
      SDATA.VB_SPEED.PROPS.DRAW_RULER = true;
      SDATA.VB_SPEED.PROPS.COLOR_RULER = sdSysData.COLOR_SELECT.white();
      SDATA.VB_SPEED.PROPS.MAX_TICK_LEVEL = 2;
      SDATA.VB_SPEED.create();
    }
    {
      SDATA.VB_S_TEMP.PROPS.LABEL = "Temp Vertical";
      SDATA.VB_S_TEMP.PROPS.BAR_HEIGHT = 30 / 4;
      SDATA.VB_S_TEMP.PROPS.MARKER_SIZE = 15;
      SDATA.VB_S_TEMP.PROPS.COLOR_BACKGROUND = sdSysData.COLOR_SELECT.blue();
      SDATA.VB_S_TEMP.PROPS.COLOR_MARKER = sdSysData.COLOR_SELECT.orange();
      SDATA.VB_S_TEMP.PROPS.DISPLAY_SINGLE_POINT_FLOAT = true;
      SDATA.VB_S_TEMP.PROPS.DRAW_MIN_MAX = true;
      SDATA.VB_S_TEMP.PROPS.MAX = 80.0f;
      SDATA.VB_S_TEMP.PROPS.HORIZONTAL = false;
      SDATA.VB_S_TEMP.PROPS.DRAW_RULER = true;
      SDATA.VB_S_TEMP.PROPS.COLOR_RULER = sdSysData.COLOR_SELECT.white();
      SDATA.VB_S_TEMP.PROPS.MAX_TICK_LEVEL = 2;
      SDATA.VB_S_TEMP.create();
    }
    {
      SDATA.VB_S_FUEL.PROPS.LABEL = "Fuel Vertical";
      SDATA.VB_S_FUEL.PROPS.BAR_HEIGHT = 30 / 4;
      SDATA.VB_S_FUEL.PROPS.MARKER_SIZE = 15;
      SDATA.VB_S_FUEL.PROPS.COLOR_BACKGROUND = sdSysData.COLOR_SELECT.blue();
      SDATA.VB_S_FUEL.PROPS.COLOR_MARKER = sdSysData.COLOR_SELECT.cyan();
      SDATA.VB_S_FUEL.PROPS.DISPLAY_SINGLE_POINT_FLOAT = true;
      SDATA.VB_S_FUEL.PROPS.DRAW_MIN_MAX = true;
      SDATA.VB_S_FUEL.PROPS.MAX = 80.0f;
      SDATA.VB_S_FUEL.PROPS.HORIZONTAL = false;
      SDATA.VB_S_FUEL.PROPS.DRAW_RULER = true;
      SDATA.VB_S_FUEL.PROPS.COLOR_RULER = sdSysData.COLOR_SELECT.white();
      SDATA.VB_S_FUEL.PROPS.MAX_TICK_LEVEL = 2;
      SDATA.VB_S_FUEL.create();
    }
    {
      SDATA.VB_S_VOLTAGE.PROPS.LABEL = "Voltage Vertical";
      SDATA.VB_S_VOLTAGE.PROPS.BAR_HEIGHT = 30 / 4;
      SDATA.VB_S_VOLTAGE.PROPS.MARKER_SIZE = 15;
      SDATA.VB_S_VOLTAGE.PROPS.COLOR_BACKGROUND = sdSysData.COLOR_SELECT.blue();
      SDATA.VB_S_VOLTAGE.PROPS.COLOR_MARKER = sdSysData.COLOR_SELECT.red();
      SDATA.VB_S_VOLTAGE.PROPS.DISPLAY_SINGLE_POINT_FLOAT = true;
      SDATA.VB_S_VOLTAGE.PROPS.DRAW_MIN_MAX = true;
      SDATA.VB_S_VOLTAGE.PROPS.MAX = 80.0f;
      SDATA.VB_S_VOLTAGE.PROPS.HORIZONTAL = false;
      SDATA.VB_S_VOLTAGE.PROPS.DRAW_RULER = true;
      SDATA.VB_S_VOLTAGE.PROPS.COLOR_RULER = sdSysData.COLOR_SELECT.white();
      SDATA.VB_S_VOLTAGE.PROPS.MAX_TICK_LEVEL = 2;
      SDATA.VB_S_VOLTAGE.create();
    }
  }

  // ------------------------------------------
  // Mid Bottom

  // DISPLAY_MID_BOTTOM = 0
  {
    {
      SDATA.TB_STEERING.PROPS.LABEL = "Steering Wheel";
      SDATA.TB_STEERING.PROPS.BAR_HEIGHT = 5;
      SDATA.TB_STEERING.PROPS.MARKER_SIZE = 20;
      SDATA.TB_STEERING.PROPS.COLOR_BACKGROUND = sdSysData.COLOR_SELECT.blue();
      SDATA.TB_STEERING.PROPS.COLOR_MARKER = sdSysData.COLOR_SELECT.yellow();
      SDATA.TB_STEERING.PROPS.MAX = 360.0f;
      SDATA.TB_STEERING.PROPS.MARKER_LIMIT_AT_RANGE = false;
      SDATA.TB_STEERING.create();
    }
    {
      SDATA.TB_SPEED.PROPS.LABEL = "Speed";
      SDATA.TB_SPEED.PROPS.BAR_HEIGHT = 20;
      SDATA.TB_SPEED.PROPS.MARKER_SIZE = 20;
      SDATA.TB_SPEED.PROPS.COLOR_BACKGROUND = sdSysData.COLOR_SELECT.blue();
      SDATA.TB_SPEED.PROPS.COLOR_MARKER = sdSysData.COLOR_SELECT.yellow();
      SDATA.TB_SPEED.PROPS.DRAW_MIN_MAX_ON_TOP = true;
      SDATA.TB_SPEED.PROPS.DISPLAY_SINGLE_POINT_FLOAT = true;
      SDATA.TB_SPEED.PROPS.DRAW_MIN_MAX = true;
      SDATA.TB_SPEED.PROPS.MAX = 80.0f;
      SDATA.TB_SPEED.PROPS.DRAW_RULER = true;
      SDATA.TB_SPEED.PROPS.COLOR_RULER = sdSysData.COLOR_SELECT.white();
      SDATA.TB_SPEED.PROPS.MAX_TICK_LEVEL = 3;
      SDATA.TB_SPEED.create();
    }
    {
      SDATA.TB_ACCELERATION.PROPS.LABEL = "Accel";
      SDATA.TB_ACCELERATION.PROPS.BAR_HEIGHT = 20;
      SDATA.TB_ACCELERATION.PROPS.MARKER_SIZE = 20;
      SDATA.TB_ACCELERATION.PROPS.COLOR_BACKGROUND = sdSysData.COLOR_SELECT.blue();
      SDATA.TB_ACCELERATION.PROPS.COLOR_MARKER = sdSysData.COLOR_SELECT.yellow();
      SDATA.TB_ACCELERATION.PROPS.DRAW_MIN_MAX = true;
      SDATA.TB_ACCELERATION.PROPS.DRAW_MIN_MAX_ON_BOTTOM = true;
      SDATA.TB_ACCELERATION.PROPS.DISPLAY_SINGLE_POINT_FLOAT = true;
      SDATA.TB_ACCELERATION.PROPS.MAX = 10.0f;
      SDATA.TB_ACCELERATION.PROPS.DRAW_RULER = true;
      SDATA.TB_ACCELERATION.PROPS.COLOR_RULER = sdSysData.COLOR_SELECT.white();
      SDATA.TB_ACCELERATION.PROPS.MAX_TICK_LEVEL = 2;
      SDATA.TB_ACCELERATION.PROPS.MARKER_LIMIT_AT_RANGE = false;
      SDATA.TB_ACCELERATION.create();
    }
    {
      SDATA.TB_RPM.PROPS.LABEL = "Tach";
      SDATA.TB_RPM.PROPS.BAR_HEIGHT = 20;
      SDATA.TB_RPM.PROPS.MARKER_SIZE = 20;
      SDATA.TB_RPM.PROPS.COLOR_BACKGROUND = sdSysData.COLOR_SELECT.blue();
      SDATA.TB_RPM.PROPS.COLOR_MARKER = sdSysData.COLOR_SELECT.yellow();
      SDATA.TB_RPM.PROPS.DRAW_MIN_MAX = true;
      SDATA.TB_RPM.PROPS.DRAW_MIN_MAX_ON_BOTTOM = true;
      SDATA.TB_RPM.PROPS.MAX = 6000.0f;
      SDATA.TB_RPM.PROPS.DRAW_RULER = true;
      SDATA.TB_RPM.PROPS.COLOR_RULER = sdSysData.COLOR_SELECT.white();
      SDATA.TB_RPM.PROPS.MAX_TICK_LEVEL = 4;
      SDATA.TB_RPM.create();
    }
    {
      SDATA.TB_TORQUE.PROPS.LABEL = "Req Toq";
      SDATA.TB_TORQUE.PROPS.BAR_HEIGHT = 20;
      SDATA.TB_TORQUE.PROPS.MARKER_SIZE = 20;
      SDATA.TB_TORQUE.PROPS.COLOR_BACKGROUND = sdSysData.COLOR_SELECT.blue();
      SDATA.TB_TORQUE.PROPS.COLOR_MARKER = sdSysData.COLOR_SELECT.yellow();
      SDATA.TB_TORQUE.PROPS.DRAW_MIN_MAX = true;
      SDATA.TB_TORQUE.PROPS.MAX = 255.0f;
      SDATA.TB_TORQUE.PROPS.DRAW_RULER = true;
      SDATA.TB_TORQUE.PROPS.COLOR_RULER = sdSysData.COLOR_SELECT.white();
      SDATA.TB_TORQUE.PROPS.MAX_TICK_LEVEL = 3;
      SDATA.TB_TORQUE.PROPS.MARKER_LIMIT_AT_RANGE = false;
      SDATA.TB_TORQUE.create();
    }
    {
      SDATA.VB_DECELERATOR.PROPS.LABEL = "Decelerator";
      SDATA.VB_DECELERATOR.PROPS.BAR_HEIGHT = 20;
      SDATA.VB_DECELERATOR.PROPS.MARKER_SIZE = 5;
      SDATA.VB_DECELERATOR.PROPS.COLOR_BACKGROUND = sdSysData.COLOR_SELECT.blue();
      SDATA.VB_DECELERATOR.PROPS.COLOR_MARKER = sdSysData.COLOR_SELECT.yellow();
      SDATA.VB_DECELERATOR.PROPS.DRAW_MIN_MAX = true;
      SDATA.VB_DECELERATOR.PROPS.MAX = 5.0f;
      SDATA.VB_DECELERATOR.PROPS.HORIZONTAL = false;
      SDATA.VB_DECELERATOR.PROPS.DRAW_RULER = true;
      SDATA.VB_DECELERATOR.PROPS.COLOR_RULER = sdSysData.COLOR_SELECT.white();
      SDATA.VB_DECELERATOR.PROPS.MAX_TICK_LEVEL = 2;
      SDATA.VB_DECELERATOR.create();
    }
    {
      SDATA.VB_BRAKE.PROPS.LABEL = "Brake";
      SDATA.VB_BRAKE.PROPS.BAR_HEIGHT = 20;
      SDATA.VB_BRAKE.PROPS.MARKER_SIZE = 5;
      SDATA.VB_BRAKE.PROPS.COLOR_BACKGROUND = sdSysData.COLOR_SELECT.blue();
      SDATA.VB_BRAKE.PROPS.COLOR_MARKER = sdSysData.COLOR_SELECT.yellow();
      SDATA.VB_BRAKE.PROPS.DRAW_MIN_MAX = true;
      SDATA.VB_BRAKE.PROPS.MAX = 3.0f * 256.0f;
      SDATA.VB_BRAKE.PROPS.HORIZONTAL = false;
      SDATA.VB_BRAKE.PROPS.DRAW_RULER = true;
      SDATA.VB_BRAKE.PROPS.COLOR_RULER = sdSysData.COLOR_SELECT.white();
      SDATA.VB_BRAKE.PROPS.MAX_TICK_LEVEL = 2;
      SDATA.VB_BRAKE.create();
    }
    {
      SDATA.VB_ACCELERATOR.PROPS.LABEL = "Accelerator";
      SDATA.VB_ACCELERATOR.PROPS.BAR_HEIGHT = 20;
      SDATA.VB_ACCELERATOR.PROPS.MARKER_SIZE = 5;
      SDATA.VB_ACCELERATOR.PROPS.COLOR_BACKGROUND = sdSysData.COLOR_SELECT.blue();
      SDATA.VB_ACCELERATOR.PROPS.COLOR_MARKER = sdSysData.COLOR_SELECT.yellow();
      SDATA.VB_ACCELERATOR.PROPS.DRAW_MIN_MAX = true;
      SDATA.VB_ACCELERATOR.PROPS.MAX = 1.0f * 256.0f;
      SDATA.VB_ACCELERATOR.PROPS.HORIZONTAL = false;
      SDATA.VB_ACCELERATOR.PROPS.DRAW_RULER = true;
      SDATA.VB_ACCELERATOR.PROPS.COLOR_RULER = sdSysData.COLOR_SELECT.white();
      SDATA.VB_ACCELERATOR.PROPS.MAX_TICK_LEVEL = 2;
      SDATA.VB_ACCELERATOR.create();
    }
  }

  /*
  // DISPLAY_MID_BOTTOM = 1
  {
    {
      SDATA.PLOT_VOLTAGE.PROPS.LABEL = "Voltage Plot";
      SDATA.PLOT_VOLTAGE.PROPS.COLOR_GRID = sdSysData.COLOR_SELECT.COLOR_COMB_WHITE;
      SDATA.PLOT_VOLTAGE.PROPS.GRID_SEPERATOR_COUNT_HORIZONTAL = 4;
      SDATA.PLOT_VOLTAGE.PROPS.DATA_POINTS_VALUE_MAX = 15;        // 80mph
      SDATA.PLOT_VOLTAGE.PROPS.LEFT_TO_RIGHT = false;
      SDATA.PLOT_VOLTAGE.PROPS.BOTTOM_TO_TOP = true;
      
      SDATA.PLOT_VOLTAGE.PROPS.GRID_SEPERATOR_COUNT_VERTICAL = 5;
      SDATA.PLOT_VOLTAGE.create_subgraph(300,         10 * 1000, "10s");
      SDATA.PLOT_VOLTAGE.create_subgraph(58,         290 * 1000, "5m (4:50m)");
      SDATA.PLOT_VOLTAGE.create_subgraph(60,     40 * 60 * 1000, "45m (40m)");
      SDATA.PLOT_VOLTAGE.create_subgraph(60,    140 * 60 * 1000, "3h (2:20m)");
      SDATA.PLOT_VOLTAGE.create_subgraph(60, 5 * 60 * 60 * 1000, "8h (5h)"); 

      // Plot Voltage Line
      SDATA.PLOT_VOLTAGE.create_line(sdSysData.COLOR_SELECT.COLOR_COMB_YELLOW, true, true, 2.0f, 1.0f);

      SDATA.PLOT_VOLTAGE.create(sdSysData.PROGRAM_TIME.current_frame_time());
    }

    // Vertical Bars
    {
      {
        SDATA.VB_VOLTAGE.PROPS.LABEL = "Voltage Vertical";
        SDATA.VB_VOLTAGE.PROPS.BAR_HEIGHT = 30;
        SDATA.VB_VOLTAGE.PROPS.MARKER_SIZE = 5;
        SDATA.VB_VOLTAGE.PROPS.COLOR_BACKGROUND = sdSysData.COLOR_SELECT.COLOR_COMB_BLUE;
        SDATA.VB_VOLTAGE.PROPS.COLOR_MARKER = sdSysData.COLOR_SELECT.COLOR_COMB_YELLOW;
        SDATA.VB_VOLTAGE.PROPS.DISPLAY_SINGLE_POINT_FLOAT = true;
        SDATA.VB_VOLTAGE.PROPS.DRAW_MIN_MAX = true;
        SDATA.VB_VOLTAGE.PROPS.MAX = 15;
        SDATA.VB_VOLTAGE.PROPS.HORIZONTAL = false;
        SDATA.VB_VOLTAGE.PROPS.DRAW_RULER = true;
        SDATA.VB_VOLTAGE.PROPS.COLOR_RULER = sdSysData.COLOR_SELECT.COLOR_COMB_WHITE;
        SDATA.VB_VOLTAGE.PROPS.MAX_TICK_LEVEL = 4;
        SDATA.VB_VOLTAGE.create();
      }
    }
  }
  */

    // DISPLAY_MID_BOTTOM = 2 -- Temperature
  {
    {
      SDATA.PLOT_TEMPERATURE.PROPS.LABEL = "Temperature Plot";
      SDATA.PLOT_TEMPERATURE.PROPS.COLOR_GRID = sdSysData.COLOR_SELECT.white();
      SDATA.PLOT_TEMPERATURE.PROPS.GRID_SEPERATOR_COUNT_HORIZONTAL = 5;
      SDATA.PLOT_TEMPERATURE.PROPS.DATA_POINTS_VALUE_MAX = 100.0f;        // 80mph
      SDATA.PLOT_TEMPERATURE.PROPS.LEFT_TO_RIGHT = false;
      SDATA.PLOT_TEMPERATURE.PROPS.BOTTOM_TO_TOP = true;
      
      SDATA.PLOT_TEMPERATURE.PROPS.GRID_SEPERATOR_COUNT_VERTICAL = 5;
      SDATA.PLOT_TEMPERATURE.create_subgraph(300,         10 * 1000, "10s");
      SDATA.PLOT_TEMPERATURE.create_subgraph(58,         290 * 1000, "5m (4:50m)");
      SDATA.PLOT_TEMPERATURE.create_subgraph(60,     40 * 60 * 1000, "45m (40m)");
      SDATA.PLOT_TEMPERATURE.create_subgraph(60,    140 * 60 * 1000, "3h (2:20m)");
      SDATA.PLOT_TEMPERATURE.create_subgraph(60, 5 * 60 * 60 * 1000, "8h (5h)"); 

      // Plot Voltage Line
      SDATA.PLOT_TEMPERATURE.create_line(sdSysData.COLOR_SELECT.purple(), true, true, 2.0f, 1.0f);
      SDATA.PLOT_TEMPERATURE.create_line(sdSysData.COLOR_SELECT.orange(), true, true, 2.0f, 1.0f);
      SDATA.PLOT_TEMPERATURE.create_line(sdSysData.COLOR_SELECT.yellow(), true, true, 2.0f, 1.0f);
      SDATA.PLOT_TEMPERATURE.create_line(sdSysData.COLOR_SELECT.cyan(), true, true, 2.0f, 1.0f);
      SDATA.PLOT_TEMPERATURE.create_line(sdSysData.COLOR_SELECT.white(), true, true, 2.0f, 1.0f);

      SDATA.PLOT_TEMPERATURE.create(sdSysData.PROGRAM_TIME.current_frame_time());
    }

    // Vertical Bars
    {
      {
        SDATA.VB_TEMPERATURE_COOLANT.PROPS.LABEL = "Coolant Vertical";
        SDATA.VB_TEMPERATURE_COOLANT.PROPS.BAR_HEIGHT = 30.0f / 5.0f;
        SDATA.VB_TEMPERATURE_COOLANT.PROPS.MARKER_SIZE = 15.0f;
        SDATA.VB_TEMPERATURE_COOLANT.PROPS.COLOR_BACKGROUND = sdSysData.COLOR_SELECT.blue();
        SDATA.VB_TEMPERATURE_COOLANT.PROPS.COLOR_MARKER = sdSysData.COLOR_SELECT.white();
        SDATA.VB_TEMPERATURE_COOLANT.PROPS.DISPLAY_SINGLE_POINT_FLOAT = true;
        SDATA.VB_TEMPERATURE_COOLANT.PROPS.DRAW_MIN_MAX = true;
        SDATA.VB_TEMPERATURE_COOLANT.PROPS.MAX = 100.0f;
        SDATA.VB_TEMPERATURE_COOLANT.PROPS.HORIZONTAL = false;
        SDATA.VB_TEMPERATURE_COOLANT.PROPS.DRAW_RULER = true;
        SDATA.VB_TEMPERATURE_COOLANT.PROPS.COLOR_RULER = sdSysData.COLOR_SELECT.white();
        SDATA.VB_TEMPERATURE_COOLANT.PROPS.MAX_TICK_LEVEL = 2;
        SDATA.VB_TEMPERATURE_COOLANT.create();
      }
      {
        SDATA.VB_TEMPERATURE_INTAKE.PROPS.LABEL = "Intake Vertical";
        SDATA.VB_TEMPERATURE_INTAKE.PROPS.BAR_HEIGHT = 30.0f / 5.0f;
        SDATA.VB_TEMPERATURE_INTAKE.PROPS.MARKER_SIZE = 15.0f;
        SDATA.VB_TEMPERATURE_INTAKE.PROPS.COLOR_BACKGROUND = sdSysData.COLOR_SELECT.blue();
        SDATA.VB_TEMPERATURE_INTAKE.PROPS.COLOR_MARKER = sdSysData.COLOR_SELECT.yellow();
        SDATA.VB_TEMPERATURE_INTAKE.PROPS.DISPLAY_SINGLE_POINT_FLOAT = true;
        SDATA.VB_TEMPERATURE_INTAKE.PROPS.DRAW_MIN_MAX = true;
        SDATA.VB_TEMPERATURE_INTAKE.PROPS.MAX = 100.0f;
        SDATA.VB_TEMPERATURE_INTAKE.PROPS.HORIZONTAL = false;
        SDATA.VB_TEMPERATURE_INTAKE.PROPS.DRAW_RULER = true;
        SDATA.VB_TEMPERATURE_INTAKE.PROPS.COLOR_RULER = sdSysData.COLOR_SELECT.white();
        SDATA.VB_TEMPERATURE_INTAKE.PROPS.MAX_TICK_LEVEL = 2;
        SDATA.VB_TEMPERATURE_INTAKE.create();
      }
      {
        SDATA.VB_TEMPERATURE_AMBIANT.PROPS.LABEL = "Ambiant Vertical";
        SDATA.VB_TEMPERATURE_AMBIANT.PROPS.BAR_HEIGHT = 30.0f / 5.0f;
        SDATA.VB_TEMPERATURE_AMBIANT.PROPS.MARKER_SIZE = 15.0f;
        SDATA.VB_TEMPERATURE_AMBIANT.PROPS.COLOR_BACKGROUND = sdSysData.COLOR_SELECT.blue();
        SDATA.VB_TEMPERATURE_AMBIANT.PROPS.COLOR_MARKER = sdSysData.COLOR_SELECT.cyan();
        SDATA.VB_TEMPERATURE_AMBIANT.PROPS.DISPLAY_SINGLE_POINT_FLOAT = true;
        SDATA.VB_TEMPERATURE_AMBIANT.PROPS.DRAW_MIN_MAX = true;
        SDATA.VB_TEMPERATURE_AMBIANT.PROPS.MAX = 100.0f;
        SDATA.VB_TEMPERATURE_AMBIANT.PROPS.HORIZONTAL = false;
        SDATA.VB_TEMPERATURE_AMBIANT.PROPS.DRAW_RULER = true;
        SDATA.VB_TEMPERATURE_AMBIANT.PROPS.COLOR_RULER = sdSysData.COLOR_SELECT.white();
        SDATA.VB_TEMPERATURE_AMBIANT.PROPS.MAX_TICK_LEVEL = 2;
        SDATA.VB_TEMPERATURE_AMBIANT.create();
      }
      {
        SDATA.VB_TEMPERATURE_CATALYST.PROPS.LABEL = "Catalyst Vertical";
        SDATA.VB_TEMPERATURE_CATALYST.PROPS.BAR_HEIGHT = 30.0f / 5.0f;
        SDATA.VB_TEMPERATURE_CATALYST.PROPS.MARKER_SIZE = 15.0f;
        SDATA.VB_TEMPERATURE_CATALYST.PROPS.COLOR_BACKGROUND = sdSysData.COLOR_SELECT.blue();
        SDATA.VB_TEMPERATURE_CATALYST.PROPS.COLOR_MARKER = sdSysData.COLOR_SELECT.purple();
        SDATA.VB_TEMPERATURE_CATALYST.PROPS.DISPLAY_SINGLE_POINT_FLOAT = true;
        SDATA.VB_TEMPERATURE_CATALYST.PROPS.DRAW_MIN_MAX = true;
        SDATA.VB_TEMPERATURE_CATALYST.PROPS.MAX = 100.0f;
        SDATA.VB_TEMPERATURE_CATALYST.PROPS.HORIZONTAL = false;
        SDATA.VB_TEMPERATURE_CATALYST.PROPS.DRAW_RULER = true;
        SDATA.VB_TEMPERATURE_CATALYST.PROPS.COLOR_RULER = sdSysData.COLOR_SELECT.white();
        SDATA.VB_TEMPERATURE_CATALYST.PROPS.MAX_TICK_LEVEL = 2;
        SDATA.VB_TEMPERATURE_CATALYST.create();
      }
      {
        SDATA.VB_TEMPERATURE_S_TEMP.PROPS.LABEL = "S Temp Vertical";
        SDATA.VB_TEMPERATURE_S_TEMP.PROPS.BAR_HEIGHT = 30.0f / 5.0f;
        SDATA.VB_TEMPERATURE_S_TEMP.PROPS.MARKER_SIZE = 15.0f;
        SDATA.VB_TEMPERATURE_S_TEMP.PROPS.COLOR_BACKGROUND = sdSysData.COLOR_SELECT.blue();
        SDATA.VB_TEMPERATURE_S_TEMP.PROPS.COLOR_MARKER = sdSysData.COLOR_SELECT.orange();
        SDATA.VB_TEMPERATURE_S_TEMP.PROPS.DISPLAY_SINGLE_POINT_FLOAT = true;
        SDATA.VB_TEMPERATURE_S_TEMP.PROPS.DRAW_MIN_MAX = true;
        SDATA.VB_TEMPERATURE_S_TEMP.PROPS.MAX = 100.0f;
        SDATA.VB_TEMPERATURE_S_TEMP.PROPS.HORIZONTAL = false;
        SDATA.VB_TEMPERATURE_S_TEMP.PROPS.DRAW_RULER = true;
        SDATA.VB_TEMPERATURE_S_TEMP.PROPS.COLOR_RULER = sdSysData.COLOR_SELECT.white();
        SDATA.VB_TEMPERATURE_S_TEMP.PROPS.MAX_TICK_LEVEL = 2;
        SDATA.VB_TEMPERATURE_S_TEMP.create();
      }
    }
  }

  // DISPLAY_MID_BOTTOM = 3 -- Power
  {
    {
      SDATA.PLOT_POWER.PROPS.LABEL = "Power Plot";
      SDATA.PLOT_POWER.PROPS.COLOR_GRID = sdSysData.COLOR_SELECT.white();
      SDATA.PLOT_POWER.PROPS.GRID_SEPERATOR_COUNT_HORIZONTAL = 5;
      SDATA.PLOT_POWER.PROPS.DATA_POINTS_VALUE_MAX = 100.0f;        // 80mph
      SDATA.PLOT_POWER.PROPS.LEFT_TO_RIGHT = false;
      SDATA.PLOT_POWER.PROPS.BOTTOM_TO_TOP = true;
      
      SDATA.PLOT_POWER.PROPS.GRID_SEPERATOR_COUNT_VERTICAL = 5;
      SDATA.PLOT_POWER.create_subgraph(300,         10 * 1000, "10s");
      SDATA.PLOT_POWER.create_subgraph(58,         290 * 1000, "5m (4:50m)");
      SDATA.PLOT_POWER.create_subgraph(60,     40 * 60 * 1000, "45m (40m)");
      SDATA.PLOT_POWER.create_subgraph(60,    140 * 60 * 1000, "3h (2:20m)");
      SDATA.PLOT_POWER.create_subgraph(60, 5 * 60 * 60 * 1000, "8h (5h)"); 

      // Plot Voltage Line
      SDATA.PLOT_POWER.create_line(sdSysData.COLOR_SELECT.purple(), true, true, 2.0f, 1.0f);
      SDATA.PLOT_POWER.create_line(sdSysData.COLOR_SELECT.pink(), true, true, 2.0f, 1.0f);
      SDATA.PLOT_POWER.create_line(sdSysData.COLOR_SELECT.green(), true, true, 2.0f, 1.0f);
      SDATA.PLOT_POWER.create_line(sdSysData.COLOR_SELECT.red(), true, true, 2.0f, 1.0f);
      SDATA.PLOT_POWER.create_line(sdSysData.COLOR_SELECT.cyan(), true, true, 2.0f, 1.0f);

      SDATA.PLOT_POWER.create(sdSysData.PROGRAM_TIME.current_frame_time());
    }

    // Vertical Bars
    {
      {
        SDATA.VB_POWER_TACH.PROPS.LABEL = "Tach Vertical";
        SDATA.VB_POWER_TACH.PROPS.BAR_HEIGHT = 30.0f / 5.0f;
        SDATA.VB_POWER_TACH.PROPS.MARKER_SIZE = 15.0f;
        SDATA.VB_POWER_TACH.PROPS.COLOR_BACKGROUND = sdSysData.COLOR_SELECT.blue();
        SDATA.VB_POWER_TACH.PROPS.COLOR_MARKER = sdSysData.COLOR_SELECT.green();
        SDATA.VB_POWER_TACH.PROPS.DISPLAY_SINGLE_POINT_FLOAT = true;
        SDATA.VB_POWER_TACH.PROPS.DRAW_MIN_MAX = true;
        SDATA.VB_POWER_TACH.PROPS.MAX = 100.0f;
        SDATA.VB_POWER_TACH.PROPS.HORIZONTAL = false;
        SDATA.VB_POWER_TACH.PROPS.DRAW_RULER = true;
        SDATA.VB_POWER_TACH.PROPS.COLOR_RULER = sdSysData.COLOR_SELECT.white();
        SDATA.VB_POWER_TACH.PROPS.MAX_TICK_LEVEL = 2;
        SDATA.VB_POWER_TACH.create();
      }
      {
        SDATA.VB_POWER_TORQE.PROPS.LABEL = "Torque Vertical";
        SDATA.VB_POWER_TORQE.PROPS.BAR_HEIGHT = 30.0f / 5.0f;
        SDATA.VB_POWER_TORQE.PROPS.MARKER_SIZE = 15.0f;
        SDATA.VB_POWER_TORQE.PROPS.COLOR_BACKGROUND = sdSysData.COLOR_SELECT.blue();
        SDATA.VB_POWER_TORQE.PROPS.COLOR_MARKER = sdSysData.COLOR_SELECT.red();
        SDATA.VB_POWER_TORQE.PROPS.DISPLAY_SINGLE_POINT_FLOAT = true;
        SDATA.VB_POWER_TORQE.PROPS.DRAW_MIN_MAX = true;
        SDATA.VB_POWER_TORQE.PROPS.MAX = 100.0f;
        SDATA.VB_POWER_TORQE.PROPS.HORIZONTAL = false;
        SDATA.VB_POWER_TORQE.PROPS.DRAW_RULER = true;
        SDATA.VB_POWER_TORQE.PROPS.COLOR_RULER = sdSysData.COLOR_SELECT.white();
        SDATA.VB_POWER_TORQE.PROPS.MAX_TICK_LEVEL = 2;
        SDATA.VB_POWER_TORQE.create();
      }
      {
        SDATA.VB_POWER_ACCELERATION.PROPS.LABEL = "Acceleration Vertical";
        SDATA.VB_POWER_ACCELERATION.PROPS.BAR_HEIGHT = 30.0f / 5.0f;
        SDATA.VB_POWER_ACCELERATION.PROPS.MARKER_SIZE = 15.0f;
        SDATA.VB_POWER_ACCELERATION.PROPS.COLOR_BACKGROUND = sdSysData.COLOR_SELECT.blue();
        SDATA.VB_POWER_ACCELERATION.PROPS.COLOR_MARKER = sdSysData.COLOR_SELECT.cyan();
        SDATA.VB_POWER_ACCELERATION.PROPS.DISPLAY_SINGLE_POINT_FLOAT = true;
        SDATA.VB_POWER_ACCELERATION.PROPS.DRAW_MIN_MAX = true;
        SDATA.VB_POWER_ACCELERATION.PROPS.MAX = 100.0f;
        SDATA.VB_POWER_ACCELERATION.PROPS.HORIZONTAL = false;
        SDATA.VB_POWER_ACCELERATION.PROPS.DRAW_RULER = true;
        SDATA.VB_POWER_ACCELERATION.PROPS.COLOR_RULER = sdSysData.COLOR_SELECT.white();
        SDATA.VB_POWER_ACCELERATION.PROPS.MAX_TICK_LEVEL = 2;
        SDATA.VB_POWER_ACCELERATION.create();
      }
      {
        SDATA.VB_POWER_FUEL_RAIL_P.PROPS.LABEL = "Fuel Rail Pressure Vertical";
        SDATA.VB_POWER_FUEL_RAIL_P.PROPS.BAR_HEIGHT = 30.0f / 5.0f;
        SDATA.VB_POWER_FUEL_RAIL_P.PROPS.MARKER_SIZE = 15.0f;
        SDATA.VB_POWER_FUEL_RAIL_P.PROPS.COLOR_BACKGROUND = sdSysData.COLOR_SELECT.blue();
        SDATA.VB_POWER_FUEL_RAIL_P.PROPS.COLOR_MARKER = sdSysData.COLOR_SELECT.purple();
        SDATA.VB_POWER_FUEL_RAIL_P.PROPS.DISPLAY_SINGLE_POINT_FLOAT = true;
        SDATA.VB_POWER_FUEL_RAIL_P.PROPS.DRAW_MIN_MAX = true;
        SDATA.VB_POWER_FUEL_RAIL_P.PROPS.MAX = 100.0f;
        SDATA.VB_POWER_FUEL_RAIL_P.PROPS.HORIZONTAL = false;
        SDATA.VB_POWER_FUEL_RAIL_P.PROPS.DRAW_RULER = true;
        SDATA.VB_POWER_FUEL_RAIL_P.PROPS.COLOR_RULER = sdSysData.COLOR_SELECT.white();
        SDATA.VB_POWER_FUEL_RAIL_P.PROPS.MAX_TICK_LEVEL = 2;
        SDATA.VB_POWER_FUEL_RAIL_P.create();
      }
      {
        SDATA.VB_POWER_SYSTEM_VAPER_P.PROPS.LABEL = "System Vaper Pressure Vertical";
        SDATA.VB_POWER_SYSTEM_VAPER_P.PROPS.BAR_HEIGHT = 30.0f / 5.0f;
        SDATA.VB_POWER_SYSTEM_VAPER_P.PROPS.MARKER_SIZE = 15.0f;
        SDATA.VB_POWER_SYSTEM_VAPER_P.PROPS.COLOR_BACKGROUND = sdSysData.COLOR_SELECT.blue();
        SDATA.VB_POWER_SYSTEM_VAPER_P.PROPS.COLOR_MARKER = sdSysData.COLOR_SELECT.pink();
        SDATA.VB_POWER_SYSTEM_VAPER_P.PROPS.DISPLAY_SINGLE_POINT_FLOAT = true;
        SDATA.VB_POWER_SYSTEM_VAPER_P.PROPS.DRAW_MIN_MAX = true;
        SDATA.VB_POWER_SYSTEM_VAPER_P.PROPS.MAX = 100.0f;
        SDATA.VB_POWER_SYSTEM_VAPER_P.PROPS.HORIZONTAL = false;
        SDATA.VB_POWER_SYSTEM_VAPER_P.PROPS.DRAW_RULER = true;
        SDATA.VB_POWER_SYSTEM_VAPER_P.PROPS.COLOR_RULER = sdSysData.COLOR_SELECT.white();
        SDATA.VB_POWER_SYSTEM_VAPER_P.PROPS.MAX_TICK_LEVEL = 2;
        SDATA.VB_POWER_SYSTEM_VAPER_P.create();
      }
    }
  }
}

void AUTOMOBILE_SCREEN::update(system_data &sdSysData)
{
  // Gather Data

  SDATA.MESSAGES = sdSysData.CAR_INFO.message_count;
  SDATA.LATEST_UNKNOWN_MESSAGE = sdSysData.CAR_INFO.DATA.AD_UNKNOWN.ORIG;

  SDATA.SPEED = sdSysData.CAR_INFO.STATUS.SPEED.SPEED_TRANS.val_mph();
  //SDATA.SPEED_IMPRES = sdSysData.CAR_INFO.STATUS.SPEED.SPEED_TRANS.val_mph_impres(sdSysData.tmeCURRENT_FRAME_TIME);
  SDATA.SPEED_IMPRES = sdSysData.CAR_INFO.CALCULATED.SPEED_ALL_TIRES_AVERAGE.val_mph();
  SDATA.ACCELERATION = sdSysData.CAR_INFO.CALCULATED.ACCELERATION_QUICK_MEAN_HISTORY.impact(sdSysData.PROGRAM_TIME.current_frame_time());

  SDATA.CRUISE_CONTROL_SET = sdSysData.CAR_INFO.STATUS.INDICATORS.cruise_control();
  SDATA.CRUISE_CONTROL_SPEED = sdSysData.CAR_INFO.STATUS.INDICATORS.cruise_control_speed();

  if (SDATA.GEAR_VAL != sdSysData.CAR_INFO.STATUS.GEAR.reported())
  {
    SDATA.GEAR_VAL = sdSysData.CAR_INFO.STATUS.GEAR.reported();
    SDATA.GEAR_SWITCH_DELAY.ping_up(sdSysData.PROGRAM_TIME.current_frame_time(), 500);
  }

  if (sdSysData.CAR_INFO.STATUS.GEAR.reported() > 0 && sdSysData.CAR_INFO.STATUS.GEAR.reported() < 10)
  {
    SDATA.GEAR = to_string(sdSysData.CAR_INFO.STATUS.GEAR.reported());
  }
  else
  {
    SDATA.GEAR = "X";
  }

  SDATA.GEAR_SELECTION = sdSysData.CAR_INFO.STATUS.GEAR.gear_selection_short_desc();

  SDATA.RPM = sdSysData.CAR_INFO.STATUS.RPM.val_rpm();
  SDATA.TORQUE_DEMANDED = sdSysData.CAR_INFO.STATUS.POWER.val_load();

  SDATA.ACCELERATOR_POWER = sdSysData.CAR_INFO.STATUS.ACCELERATOR.val_value();
  SDATA.BRAKE_POWER = sdSysData.CAR_INFO.STATUS.BRAKE.val_value();

  // Pressure

  SDATA.FUEL_RAIL_PRESSURE = sdSysData.CAR_INFO.STATUS.FUEL.FUEL_RAIL_PRESSURE.kPa();
  SDATA.FUEL_RAIL_PRESSURE_VAL = sdSysData.CAR_INFO.STATUS.FUEL.FUEL_RAIL_PRESSURE.val_kPa();

  SDATA.EVAP_SYSTEM_VAP_PRESSURE = sdSysData.CAR_INFO.STATUS.FUEL.EVAP_SYSTEM_VAP_PRESSURE.Pa();
  SDATA.EVAP_SYSTEM_VAP_PRESSURE_VAL = sdSysData.CAR_INFO.STATUS.FUEL.EVAP_SYSTEM_VAP_PRESSURE.val_Pa();

  SDATA.VOLTAGE = sdSysData.CAR_INFO.STATUS.ELECTRICAL.CONTROL_UNIT_42.v();
  SDATA.VOLTAGE_VAL = sdSysData.CAR_INFO.STATUS.ELECTRICAL.CONTROL_UNIT_42.val_v();

  SDATA.BAROMETER = sdSysData.CAR_INFO.STATUS.TEMPS.BARO_33.inHg();
  SDATA.BAROMETER_VAL = sdSysData.CAR_INFO.STATUS.TEMPS.BARO_33.val_inHg();

  SDATA.CAM_COMM_ERR = sdSysData.CAR_INFO.CALCULATED.CAM_COMM_ERRORS.error_count();
  while (SDATA.CAM_COMM_ERR > 99)
  {
    SDATA.CAM_COMM_ERR = SDATA.CAM_COMM_ERR / 10;
  }

  SDATA.CAM_STAT_ERR = sdSysData.CAR_INFO.STATISTICS.errors();

  // Steering

  SDATA.STEERING_WHEEL_ANGLE_VAL = sdSysData.CAR_INFO.STATUS.STEERING.val_steering_wheel_angle();
  SDATA.STEERING_WHEEL_LEFT_OF_CENTER_VAL = sdSysData.CAR_INFO.STATUS.STEERING.val_left_of_center();
  SDATA.STEERING_WHEEL_ANGLE = sdSysData.CAR_INFO.STATUS.STEERING.steering_wheel_angle();
  SDATA.STEERING_WHEEL_LEFT_OF_CENTER = sdSysData.CAR_INFO.STATUS.STEERING.left_of_center();
  SDATA.STEERING_WHEEL_TURNING_DIRECTION = sdSysData.CAR_INFO.STATUS.STEERING.turning_direction();

  // Temp

  SDATA.TEMP_AMBIANT_STRING = sdSysData.CAR_INFO.STATUS.TEMPS.AMBIANT_AIR_46.c();
  SDATA.TEMP_AMBIANT = sdSysData.CAR_INFO.STATUS.TEMPS.AMBIANT_AIR_46.val_c();

  SDATA.TEMP_AIR_INTAKE_STRING = sdSysData.CAR_INFO.STATUS.TEMPS.AIR_INTAKE_0f.c();
  SDATA.TEMP_AIR_INTAKE = sdSysData.CAR_INFO.STATUS.TEMPS.AIR_INTAKE_0f.val_c();

  SDATA.TEMP_COOLANT_STRING = sdSysData.CAR_INFO.STATUS.TEMPS.COOLANT_05.c();
  SDATA.TEMP_COOLANT = sdSysData.CAR_INFO.STATUS.TEMPS.COOLANT_05.val_c();

  SDATA.TEMP_CATALYST_STRING = sdSysData.CAR_INFO.STATUS.TEMPS.CATALYST_3C.c();
  SDATA.TEMP_CATALYST = sdSysData.CAR_INFO.STATUS.TEMPS.CATALYST_3C.val_c();

  SDATA.TEMP_S_TEMP_STRING = to_string((int)sdSysData.CAR_INFO.CALCULATED.s_temp());
  SDATA.TEMP_S_TEMP = sdSysData.CAR_INFO.CALCULATED.s_temp();

  // Tire TTL

  SDATA.LF_TTL_BUFFER_FILL = sdSysData.CAR_INFO.CALCULATED.LF_TTL.slice_size();
  SDATA.RF_TTL_BUFFER_FILL = sdSysData.CAR_INFO.CALCULATED.RF_TTL.slice_size();
  SDATA.LB_TTL_BUFFER_FILL = sdSysData.CAR_INFO.CALCULATED.LB_TTL.slice_size();
  SDATA.RB_TTL_BUFFER_FILL = sdSysData.CAR_INFO.CALCULATED.RB_TTL.slice_size();

  SDATA.LF_TTL_BUFFER_FILL_MAX = sdSysData.CAR_INFO.CALCULATED.LF_TTL.slice_size_max();
  SDATA.RF_TTL_BUFFER_FILL_MAX = sdSysData.CAR_INFO.CALCULATED.RF_TTL.slice_size_max();
  SDATA.LB_TTL_BUFFER_FILL_MAX = sdSysData.CAR_INFO.CALCULATED.LB_TTL.slice_size_max();
  SDATA.RB_TTL_BUFFER_FILL_MAX = sdSysData.CAR_INFO.CALCULATED.RB_TTL.slice_size_max();

  SDATA.LF_TTL = sdSysData.CAR_INFO.CALCULATED.LF_TTL.life_percentage_mean();
  SDATA.LF_TTL_VAL = sdSysData.CAR_INFO.CALCULATED.LF_TTL.val_life_percentage_mean();

  SDATA.RF_TTL = sdSysData.CAR_INFO.CALCULATED.RF_TTL.life_percentage_mean();
  SDATA.RF_TTL_VAL = sdSysData.CAR_INFO.CALCULATED.RF_TTL.val_life_percentage_mean();

  SDATA.LB_TTL = sdSysData.CAR_INFO.CALCULATED.LB_TTL.life_percentage_mean();
  SDATA.LB_TTL_VAL = sdSysData.CAR_INFO.CALCULATED.LB_TTL.val_life_percentage_mean();

  SDATA.RB_TTL = sdSysData.CAR_INFO.CALCULATED.RB_TTL.life_percentage_mean();
  SDATA.RB_TTL_VAL = sdSysData.CAR_INFO.CALCULATED.RB_TTL.val_life_percentage_mean();

  // Lights

  SDATA.LIGHTS_HEDLIGHTS_ON = sdSysData.CAR_INFO.STATUS.INDICATORS.val_lights_headlights_on();
  SDATA.LIGHTS_PARKING_ON = sdSysData.CAR_INFO.STATUS.INDICATORS.val_lights_parking_on();
  SDATA.LIGHTS_BEAM_ON = sdSysData.CAR_INFO.STATUS.INDICATORS.val_lights_high_beam_on();
  SDATA.LIGHTS_SWITCH = sdSysData.CAR_INFO.STATUS.INDICATORS.lights_switch();

  // Parking Brake

  SDATA.PARKING_BRAKE = sdSysData.CAR_INFO.STATUS.INDICATORS.val_parking_brake();

  // Fuel Level

  SDATA.FUEL_LEVEL = sdSysData.CAR_INFO.STATUS.FUEL.level();
  SDATA.FUEL_LEVEL_VAL = sdSysData.CAR_INFO.STATUS.FUEL.val_level();
  
  // Update Widgets ------

  // Large Displays on Main Sidebar Screen
  SDATA.L_SPEED_SB.update_value(sdSysData, SDATA.SPEED_IMPRES, SDATA.CRUISE_CONTROL_SPEED, SDATA.CRUISE_CONTROL_SET);
  SDATA.L_ACCELERATION_SB.update_value(sdSysData, 10.0f * SDATA.ACCELERATION);
  SDATA.L_GEAR_SB.update_value((SDATA.GEAR_SELECTION.c_str() + SDATA.GEAR));
  SDATA.L_TACH_SB.update_value(sdSysData, (float)SDATA.RPM / 100.0f);
  SDATA.L_VOLTAGE_SB.update_value(sdSysData, SDATA.VOLTAGE_VAL);
  SDATA.L_S_TEMP_SB.update_value(sdSysData, SDATA.TEMP_S_TEMP);

  // Display
  
  SDATA.D_FUEL_RAIL_PRESSURE.update_value(sdSysData, SDATA.FUEL_RAIL_PRESSURE, SDATA.FUEL_RAIL_PRESSURE_VAL);
  SDATA.D_EVAP_SYSTEM_VAP_PRESSURE.update_value(sdSysData, SDATA.EVAP_SYSTEM_VAP_PRESSURE, SDATA.EVAP_SYSTEM_VAP_PRESSURE_VAL);
  SDATA.D_VOLTAGE.update_value(sdSysData, SDATA.VOLTAGE, SDATA.VOLTAGE_VAL);
  SDATA.D_BAROMETER.update_value(sdSysData, SDATA.BAROMETER, SDATA.BAROMETER_VAL);
  SDATA.D_CAM_COMM_ERROR.update_value(sdSysData, to_string(SDATA.CAM_COMM_ERR));

  SDATA.D_TEMP_COOLANT.update_value(sdSysData, SDATA.TEMP_COOLANT_STRING, SDATA.TEMP_COOLANT);
  SDATA.D_TEMP_INTAKE.update_value(sdSysData, SDATA.TEMP_AIR_INTAKE_STRING, SDATA.TEMP_AIR_INTAKE);
  SDATA.D_TEMP_AMBIANT.update_value(sdSysData, SDATA.TEMP_AMBIANT_STRING, SDATA.TEMP_AMBIANT);
  SDATA.D_TEMP_CATALYST.update_value(sdSysData, SDATA.TEMP_CATALYST_STRING, SDATA.TEMP_CATALYST);
  SDATA.D_TEMP_SUPER_TEMP.update_value(sdSysData, SDATA.TEMP_S_TEMP_STRING, SDATA.TEMP_S_TEMP);

  SDATA.OC_LF_TTL.update_value(SDATA.LF_TTL_BUFFER_FILL, SDATA.LF_TTL_BUFFER_FILL_MAX);
  SDATA.OC_RF_TTL.update_value(SDATA.RF_TTL_BUFFER_FILL, SDATA.RF_TTL_BUFFER_FILL_MAX);
  SDATA.OC_LB_TTL.update_value(SDATA.LB_TTL_BUFFER_FILL, SDATA.LB_TTL_BUFFER_FILL_MAX);
  SDATA.OC_RB_TTL.update_value(SDATA.RB_TTL_BUFFER_FILL, SDATA.RB_TTL_BUFFER_FILL_MAX);

  SDATA.D_LF_TTL.update_value(sdSysData, SDATA.LF_TTL, (int)SDATA.LF_TTL_VAL);
  SDATA.D_RF_TTL.update_value(sdSysData, SDATA.RF_TTL, (int)SDATA.RF_TTL_VAL);
  SDATA.D_LB_TTL.update_value(sdSysData, SDATA.LB_TTL, (int)SDATA.LB_TTL_VAL);
  SDATA.D_RB_TTL.update_value(sdSysData, SDATA.RB_TTL, (int)SDATA.RB_TTL_VAL);

  // Lights

  SDATA.D_LIGHTS.update_value(sdSysData, SDATA.LIGHTS_SWITCH);

  if (SDATA.LIGHTS_BEAM_ON == true)
  {
    SDATA.D_LIGHTS.PROPS.COLOR = sdSysData.COLOR_SELECT.cyan();
  }
  else
  {
    if (SDATA.LIGHTS_HEDLIGHTS_ON == true)
    {
      SDATA.D_LIGHTS.PROPS.COLOR = sdSysData.COLOR_SELECT.white();
    }
    else
    {
      SDATA.D_LIGHTS.PROPS.COLOR = sdSysData.COLOR_SELECT.blue();
    }
  }

  SDATA.D_FUEL_LEVEL.update_value(sdSysData, SDATA.FUEL_LEVEL, SDATA.FUEL_LEVEL_VAL);

  if (SDATA.PARKING_BRAKE == true)
  {
    SDATA.D_PARKING_BRAKE.update_value(sdSysData, "ON", 1.0f);
  }
  else
  {
    SDATA.D_PARKING_BRAKE.update_value(sdSysData, "OFF", 0.0f);
  }

  if (SDATA.CRUISE_CONTROL_SET == true)
  {
    SDATA.D_CRUISE_ON.update_value(sdSysData, "ON");
  }
  else
  {
    SDATA.D_CRUISE_ON.update_value(sdSysData, "OFF");
  }

  SDATA.D_CRUISE_SPEED.update_value(sdSysData, to_string((int)SDATA.CRUISE_CONTROL_SPEED));

  // ------------------------------------------
  // Outside
  {
    SDATA.L_SPEED_OUTSIDE.update_value(sdSysData, SDATA.SPEED_IMPRES, SDATA.CRUISE_CONTROL_SPEED, SDATA.CRUISE_CONTROL_SET);
  }

  // ------------------------------------------
  // Mid Top
  {
    if (SDATA.CAM_COMM_ERR > SDATA.PREV_D_CAM_COMM_ERROR)
    {
      SDATA.PLOT_SLOW.update(sdSysData.PROGRAM_TIME.current_frame_time(), 0, 1.0f);
      SDATA.PREV_D_CAM_COMM_ERROR = SDATA.CAM_COMM_ERR;
      sdSysData.ALERTS_AUTO.sound_alert(2);
    }

    if (SDATA.CAM_STAT_ERR != SDATA.PREV_D_CAM_STAT_ERROR)
    {
      SDATA.PLOT_SLOW.update(sdSysData.PROGRAM_TIME.current_frame_time(), 1, 1.0f);
      SDATA.PREV_D_CAM_STAT_ERROR = SDATA.CAM_STAT_ERR;
      sdSysData.ALERTS_AUTO.sound_alert(1);
    }

    SDATA.PLOT_SLOW.update(sdSysData.PROGRAM_TIME.current_frame_time(), 2, SDATA.VOLTAGE_VAL * 10.0f / 2.0f);
    SDATA.PLOT_SLOW.update(sdSysData.PROGRAM_TIME.current_frame_time(), 3, SDATA.SPEED_IMPRES);
    SDATA.PLOT_SLOW.update(sdSysData.PROGRAM_TIME.current_frame_time(), 4, SDATA.TEMP_S_TEMP);
    SDATA.PLOT_SLOW.update(sdSysData.PROGRAM_TIME.current_frame_time(), 5, SDATA.FUEL_LEVEL_VAL * 10.0f / 2.0f);

    SDATA.VB_SPEED.update_value(sdSysData, SDATA.SPEED);
    SDATA.VB_S_TEMP.update_value(sdSysData, SDATA.TEMP_S_TEMP);
    SDATA.VB_S_FUEL.update_value(sdSysData, SDATA.FUEL_LEVEL_VAL * 10.0f / 2.0f);
    SDATA.VB_S_VOLTAGE.update_value(sdSysData, SDATA.VOLTAGE_VAL * 10.0f / 2.0f);
  }

  // ------------------------------------------
  // Mid Bottom
  {
    // DISPLAY_MID_BOTTOM = 0
    {
      if (SDATA.STEERING_WHEEL_LEFT_OF_CENTER_VAL)
      {
        SDATA.TB_STEERING.update_value(sdSysData, -(SDATA.STEERING_WHEEL_ANGLE_VAL) + 180.0f);
      }
      else
      {
        SDATA.TB_STEERING.update_value(sdSysData, (SDATA.STEERING_WHEEL_ANGLE_VAL) + 180.0f);
      }

      SDATA.TB_SPEED.update_value(sdSysData, SDATA.SPEED);
      SDATA.TB_ACCELERATION.update_value(sdSysData, SDATA.ACCELERATION);
      SDATA.TB_RPM.update_value(sdSysData, SDATA.RPM);
      SDATA.TB_TORQUE.update_value(sdSysData, (float)SDATA.TORQUE_DEMANDED);

      if (SDATA.ACCELERATION < 0)
      {
        SDATA.VB_DECELERATOR.update_value(sdSysData, -(float)SDATA.ACCELERATION);
      }
      else
      {
        SDATA.VB_DECELERATOR.update_value(sdSysData, 0.0f);
      }
      
      SDATA.VB_BRAKE.update_value(sdSysData, (float)SDATA.BRAKE_POWER);
      SDATA.VB_ACCELERATOR.update_value(sdSysData, (float)SDATA.ACCELERATOR_POWER);
    }

    /*
    // DISPLAY_MID_BOTTOM = 1 - Voltage
    {
      SDATA.PLOT_VOLTAGE.update(sdSysData.PROGRAM_TIME.current_frame_time(), 0, SDATA.VOLTAGE_VAL);
      SDATA.VB_VOLTAGE.update_value(sdSysData, SDATA.VOLTAGE_VAL);
    }
    */

    // DISPLAY_MID_BOTTOM = 2 - Temperature
    {
      SDATA.PLOT_TEMPERATURE.update(sdSysData.PROGRAM_TIME.current_frame_time(), 0, SDATA.TEMP_CATALYST / 10.0f);
      SDATA.PLOT_TEMPERATURE.update(sdSysData.PROGRAM_TIME.current_frame_time(), 1, SDATA.TEMP_S_TEMP);
      SDATA.PLOT_TEMPERATURE.update(sdSysData.PROGRAM_TIME.current_frame_time(), 2, SDATA.TEMP_AIR_INTAKE);
      SDATA.PLOT_TEMPERATURE.update(sdSysData.PROGRAM_TIME.current_frame_time(), 3, SDATA.TEMP_AMBIANT);
      SDATA.PLOT_TEMPERATURE.update(sdSysData.PROGRAM_TIME.current_frame_time(), 4, SDATA.TEMP_COOLANT);

      SDATA.VB_TEMPERATURE_COOLANT.update_value(sdSysData, SDATA.TEMP_COOLANT);
      SDATA.VB_TEMPERATURE_INTAKE.update_value(sdSysData, SDATA.TEMP_AIR_INTAKE);
      SDATA.VB_TEMPERATURE_AMBIANT.update_value(sdSysData, SDATA.TEMP_AMBIANT);
      SDATA.VB_TEMPERATURE_CATALYST.update_value(sdSysData, SDATA.TEMP_CATALYST / 10.0f);
      SDATA.VB_TEMPERATURE_S_TEMP.update_value(sdSysData, SDATA.TEMP_S_TEMP);
    }

    // DISPLAY_MID_BOTTOM = 3 - Power
    {
      SDATA.PLOT_POWER.update(sdSysData.PROGRAM_TIME.current_frame_time(), 0, (SDATA.FUEL_RAIL_PRESSURE_VAL / 40.0f) + 50.0f);
      SDATA.PLOT_POWER.update(sdSysData.PROGRAM_TIME.current_frame_time(), 1, (SDATA.EVAP_SYSTEM_VAP_PRESSURE_VAL / 50.0f) + 50.0f);
      SDATA.PLOT_POWER.update(sdSysData.PROGRAM_TIME.current_frame_time(), 2, (float)SDATA.RPM/50);
      SDATA.PLOT_POWER.update(sdSysData.PROGRAM_TIME.current_frame_time(), 3, ((float)SDATA.TORQUE_DEMANDED / 40.0f) + 50.0f);
      SDATA.PLOT_POWER.update(sdSysData.PROGRAM_TIME.current_frame_time(), 4, (SDATA.ACCELERATION * 10.0f) + 50.0f);

      SDATA.VB_POWER_TACH.update_value(sdSysData, (float)SDATA.RPM/50);
      SDATA.VB_POWER_TORQE.update_value(sdSysData, ((float)SDATA.TORQUE_DEMANDED / 40.0f) + 50.0f);
      SDATA.VB_POWER_ACCELERATION.update_value(sdSysData, (SDATA.ACCELERATION * 10.0f) + 50.0f);
      SDATA.VB_POWER_FUEL_RAIL_P.update_value(sdSysData, (SDATA.FUEL_RAIL_PRESSURE_VAL / 40.0f) + 50.0f);
      SDATA.VB_POWER_SYSTEM_VAPER_P.update_value(sdSysData, (SDATA.EVAP_SYSTEM_VAP_PRESSURE_VAL / 50.0f) + 50.0f);
    }
  }
}

void AUTOMOBILE_SCREEN::display(system_data &sdSysData, bool &Display_Confirm)
{ 
  //ImGuiIO &io = ImGui::GetIO();

  // ---
  // Left Side

  if (DISPLAY_DATA)
  {
    ImGui::BeginChild("Auto All Data", ImVec2(ImGui::GetContentRegionAvail().x * 7/32, ImGui::GetContentRegionAvail().y), true, sdSysData.SCREEN_DEFAULTS.flags_c);
    {
      // draw list for All_Data_Window
      ImDrawList* draw_list_all_data = ImGui::GetWindowDrawList();

      ImVec2 frame_start_pos = ImGui::GetCursorScreenPos();
      ImVec2 frame_size = ImGui::GetContentRegionAvail();

      {
        SDATA.D_LIGHTS.draw(draw_list_all_data, sdSysData);
        SDATA.D_FUEL_LEVEL.draw(draw_list_all_data, sdSysData, true);
        SDATA.D_VOLTAGE.draw(draw_list_all_data, sdSysData, true);
        SDATA.D_PARKING_BRAKE.draw(draw_list_all_data, sdSysData);
        SDATA.D_CRUISE_ON.draw(draw_list_all_data, sdSysData);
        SDATA.D_CRUISE_SPEED.draw(draw_list_all_data, sdSysData);
      }
      
      ImGui::Separator();

      {
        SDATA.D_TEMP_COOLANT.draw(draw_list_all_data, sdSysData, DISPLAY_MID_BOTTOM == 2);
        SDATA.D_TEMP_INTAKE.draw(draw_list_all_data, sdSysData, DISPLAY_MID_BOTTOM == 2);
        SDATA.D_TEMP_AMBIANT.draw(draw_list_all_data, sdSysData, DISPLAY_MID_BOTTOM == 2);
        SDATA.D_TEMP_CATALYST.draw(draw_list_all_data, sdSysData, DISPLAY_MID_BOTTOM == 2);
        SDATA.D_TEMP_SUPER_TEMP.draw(draw_list_all_data, sdSysData, true);
      }

      ImGui::Separator();

      {
        SDATA.D_FUEL_RAIL_PRESSURE.draw(draw_list_all_data, sdSysData, DISPLAY_MID_BOTTOM == 3);
        SDATA.D_EVAP_SYSTEM_VAP_PRESSURE.draw(draw_list_all_data, sdSysData, DISPLAY_MID_BOTTOM == 3);
        SDATA.D_BAROMETER.draw(draw_list_all_data, sdSysData);
        SDATA.D_CAM_COMM_ERROR.draw(draw_list_all_data, sdSysData);
      }
      
      ImGui::Separator();

      {
        SDATA.OC_LF_TTL.draw(draw_list_all_data, sdSysData);
        ImGui::SameLine();
        SDATA.D_LF_TTL.draw(draw_list_all_data, sdSysData);

        SDATA.OC_RF_TTL.draw(draw_list_all_data, sdSysData);
        ImGui::SameLine();
        SDATA.D_RF_TTL.draw(draw_list_all_data, sdSysData);

        SDATA.OC_LB_TTL.draw(draw_list_all_data, sdSysData);
        ImGui::SameLine();
        SDATA.D_LB_TTL.draw(draw_list_all_data, sdSysData);

        SDATA.OC_RB_TTL.draw(draw_list_all_data, sdSysData);
        ImGui::SameLine();
        SDATA.D_RB_TTL.draw(draw_list_all_data, sdSysData);
      }

      ImGui::SetCursorScreenPos(frame_start_pos);
      if (ImGui::InvisibleButton("E\nx\np\na\nn\nd", frame_size))
      {
        DISPLAY_DATA = false;
      }
    }
    ImGui::EndChild();
  }
  else
  {
    ImGui::BeginChild("Auto All Data", ImVec2(35.0f, ImGui::GetContentRegionAvail().y), true, sdSysData.SCREEN_DEFAULTS.flags_c);
    {
      if (ImGui::Button("###E\nx\np\na\nn\nd", ImGui::GetContentRegionAvail()))
      {
        DISPLAY_DATA = true;
      }
    }
    ImGui::EndChild();
  }

  ImGui::SameLine();

  // Show Center Region
  if (DISPLAY_NOVA)
  {
    ImGui::BeginChild("Nova", ImVec2(ImGui::GetContentRegionAvail().x - 106.0f, ImGui::GetContentRegionAvail().y), false, sdSysData.SCREEN_DEFAULTS.flags_c);
    {
      nova(sdSysData);
    }
    ImGui::EndChild();
  }
  else
  {
    ImGui::BeginChild("Auto Screen Right Side", ImVec2(ImGui::GetContentRegionAvail().x - 106.0f, ImGui::GetContentRegionAvail().y), false, sdSysData.SCREEN_DEFAULTS.flags_c);
    {
      // Show Upper Center Region
      ImGui::BeginChild("Data 1", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y * 8.0f / 16.0f), true, sdSysData.SCREEN_DEFAULTS.flags_c);
      {
        ImDrawList* draw_list_data_1 = ImGui::GetWindowDrawList();

        float size_1_3 = ImGui::GetContentRegionAvail().x - 35.0f;
        ImVec2 pos1 = ImGui::GetCursorScreenPos();
        ImVec2 pos2 = ImVec2(pos1.x + size_1_3, pos1.y + ImGui::GetContentRegionAvail().y);
        
        if (SDATA.PLOT_SLOW.draw(sdSysData, pos1, pos2))
        {
          //
        }

        ImGui::SameLine();

        SDATA.VB_SPEED.draw(draw_list_data_1, sdSysData);
        SDATA.VB_S_FUEL.draw(draw_list_data_1, sdSysData);
        SDATA.VB_S_TEMP.draw(draw_list_data_1, sdSysData);
        SDATA.VB_S_VOLTAGE.draw(draw_list_data_1, sdSysData);
      }
      ImGui::EndChild();

      // Show Alt Data
      ImGui::BeginChild("Auto Data Long Bars", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true, sdSysData.SCREEN_DEFAULTS.flags_c);
      {
        ImDrawList* draw_list_data_auto_data = ImGui::GetWindowDrawList();

        if (DISPLAY_MID_BOTTOM == 0)
        // Show bars
        {
          ImGui::BeginChild("Auto Data Long Bars Left", ImVec2(ImGui::GetContentRegionAvail().x - 70.0f, ImGui::GetContentRegionAvail().y - 50.0f), false, sdSysData.SCREEN_DEFAULTS.flags_c);
          {
            ImVec2 pos1 = ImGui::GetCursorScreenPos();
            
            ImDrawList* draw_list_data_auto_data_bars_left = ImGui::GetWindowDrawList();

            SDATA.TB_STEERING.draw(draw_list_data_auto_data_bars_left, sdSysData);
            SDATA.TB_TORQUE.draw(draw_list_data_auto_data_bars_left, sdSysData);
            SDATA.TB_RPM.draw(draw_list_data_auto_data_bars_left, sdSysData);

            ImGui::Separator();

            SDATA.TB_SPEED.draw(draw_list_data_auto_data_bars_left, sdSysData);
            SDATA.TB_ACCELERATION.draw(draw_list_data_auto_data_bars_left, sdSysData);
            
            // Change Screens
            ImGui::SetCursorScreenPos(pos1);
            if (ImGui::InvisibleButton("InvisibleButton", ImGui::GetContentRegionAvail()))
            {
              DISPLAY_MID_BOTTOM++;
            }
          }
          ImGui::EndChild();

          ImGui::SameLine();

          ImGui::BeginChild("Auto Data Long Bars Right", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y - 50.0f), false, sdSysData.SCREEN_DEFAULTS.flags_c);
          {
            ImDrawList* draw_list_data_auto_data_bars_right = ImGui::GetWindowDrawList();

            SDATA.VB_DECELERATOR.draw(draw_list_data_auto_data_bars_right, sdSysData);
            SDATA.VB_BRAKE.draw(draw_list_data_auto_data_bars_right, sdSysData);
            SDATA.VB_ACCELERATOR.draw(draw_list_data_auto_data_bars_right, sdSysData);
          }
          ImGui::EndChild();

          // DISPLAY_MID_BOTTOM - Buttons
          ImGui::BeginChild("Record Buttons Left", ImVec2(ImGui::GetContentRegionAvail().x - 322.0f, ImGui::GetContentRegionAvail().y), false, sdSysData.SCREEN_DEFAULTS.flags_c);
          {
            if (BC_NOVA_2.button_color(sdSysData, "NOVA", sdSysData.COLOR_SELECT.blue(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_TAB))
            {
              DISPLAY_NOVA = true;
            }
          }
          ImGui::EndChild();

          ImGui::SameLine();

          ImGui::BeginChild("Record Buttons Right", ImGui::GetContentRegionAvail(), false, sdSysData.SCREEN_DEFAULTS.flags_c);
          {
            if (BC_CLEAR_DIAG.button_color(sdSysData, "CLEAR DIAG\nCODES", sdSysData.COLOR_SELECT.red(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_TAB))
            {
              sdSysData.SCREEN_COMMS.command_pending_set_cr("qdiagc");
              Display_Confirm = !Display_Confirm;
            }

            ImGui::SameLine();

            if (BTC_RECORD.button_toggle_color(sdSysData, "RECORD\n(On)", "RECORD", sdSysData.COMMS_AUTO.record_in_progress(), 
                                  sdSysData.COLOR_SELECT.red(), sdSysData.COLOR_SELECT.blue(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_TAB))
            {
              if (sdSysData.COMMS_AUTO.record_in_progress())
              {
                sdSysData.SCREEN_COMMS.command_text_set(" autof");
              }
              else
              {
                sdSysData.SCREEN_COMMS.command_text_set(" autoo");
              }
            }

            ImGui::SameLine();

            if (BC_FLASH_REC.button_color(sdSysData, "FLASH REC", sdSysData.COLOR_SELECT.blue(), sdSysData.SCREEN_DEFAULTS.SIZE_BUTTON_TAB))
            {
              sdSysData.SCREEN_COMMS.command_text_set("]]");
            }
          }
          ImGui::EndChild();
        }

        else if (DISPLAY_MID_BOTTOM == 1)
        // Show Voltage
        {
          // advance
          DISPLAY_MID_BOTTOM++;
        }

        else if (DISPLAY_MID_BOTTOM == 2)
        // Show Temperature
        {
          float size_1_3 = ImGui::GetContentRegionAvail().x - 35.0f;
          ImVec2 pos1 = ImGui::GetCursorScreenPos();
          ImVec2 pos2 = ImVec2(pos1.x + size_1_3, pos1.y + ImGui::GetContentRegionAvail().y);

          // Change Screens
          if (SDATA.PLOT_TEMPERATURE.draw(sdSysData, pos1, pos2))
          {
            DISPLAY_MID_BOTTOM++;
          }

          ImGui::SameLine();

          SDATA.VB_TEMPERATURE_COOLANT.draw(draw_list_data_auto_data, sdSysData);
          SDATA.VB_TEMPERATURE_INTAKE.draw(draw_list_data_auto_data, sdSysData);
          SDATA.VB_TEMPERATURE_AMBIANT.draw(draw_list_data_auto_data, sdSysData);
          SDATA.VB_TEMPERATURE_CATALYST.draw(draw_list_data_auto_data, sdSysData);
          SDATA.VB_TEMPERATURE_S_TEMP.draw(draw_list_data_auto_data, sdSysData);

        }

        else if (DISPLAY_MID_BOTTOM == 3)
        // Show Power
        {
          float size_1_3 = ImGui::GetContentRegionAvail().x - 35.0f;
          ImVec2 pos1 = ImGui::GetCursorScreenPos();
          ImVec2 pos2 = ImVec2(pos1.x + size_1_3, pos1.y + ImGui::GetContentRegionAvail().y);

          // Change Screens
          if (SDATA.PLOT_POWER.draw(sdSysData, pos1, pos2))
          {
            DISPLAY_MID_BOTTOM++;
          }

          ImGui::SameLine();

          SDATA.VB_POWER_TACH.draw(draw_list_data_auto_data, sdSysData);
          SDATA.VB_POWER_ACCELERATION.draw(draw_list_data_auto_data, sdSysData);
          SDATA.VB_POWER_TORQE.draw(draw_list_data_auto_data, sdSysData);
          SDATA.VB_POWER_FUEL_RAIL_P.draw(draw_list_data_auto_data, sdSysData);
          SDATA.VB_POWER_SYSTEM_VAPER_P.draw(draw_list_data_auto_data, sdSysData);
        }

        else
        {
          // reset
          DISPLAY_MID_BOTTOM = 0;
        }
      }
      ImGui::EndChild();
    }
    ImGui::EndChild();
  }
}

void AUTOMOBILE_SCREEN::display_sidebar(system_data &sdSysData, bool Automobile_Screen_Selected, bool Restack_Windows)
{
  if (SDATA.L_SPEED_SB.draw(sdSysData, !Automobile_Screen_Selected))
  {
    SDATA.L_SPEED_OUTSIDE_DRAW = !SDATA.L_SPEED_OUTSIDE_DRAW;
  }

  if (SDATA.L_TACH_SB.draw(sdSysData))
  {
    //
  }

  if (SDATA.L_S_TEMP_SB.draw(sdSysData))
  {
    //
  }
  
  if (SDATA.L_VOLTAGE_SB.draw(sdSysData))
  {
    //
  }

  if (SDATA.L_GEAR_SB.draw(sdSysData))
  {
    //
  }

  if (SDATA.L_ACCELERATION_SB.draw(sdSysData)) //ImGui::TextUnformatted("Δ\nv");
  {
    //
  }

  if (SDATA.L_SPEED_OUTSIDE_DRAW && Restack_Windows == false)
  {
    ImGui::SetNextWindowSize(ImVec2(140, 210));

    if (ImGui::Begin("SPEED", nullptr, sdSysData.SCREEN_DEFAULTS.flags_w_pop)) 
    {
      if (SDATA.L_SPEED_OUTSIDE.draw(sdSysData, true, ImGui::GetContentRegionAvail().y))
      {
        SDATA.L_SPEED_OUTSIDE_DRAW = !SDATA.L_SPEED_OUTSIDE_DRAW;
      }
    }

    ImGui::End();

  }

}

// ---------------------------------------------------------------------------------------
#endif
