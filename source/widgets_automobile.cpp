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

// ---------------------------------------------------------------------------------------

/*
void BLANK::display(const char *name, bool *p_open, ImGuiWindowFlags flags)
{ 
  ImGui::Begin(name, p_open, flags);
  {

  }
  ImGui::End();
}
*/

// ---------------------------------------------------------------------------------------

void PLOT_POINT_BUFFER::create(int Max_Size)
{
    OFFSET  = 0;
    MAX_SIZE = Max_Size;
    DATA.reserve(MAX_SIZE);
}

void PLOT_POINT_BUFFER::add_point(float x, float y) 
{
  if (DATA.size() < MAX_SIZE)
  {
      DATA.push_back(ImVec2(x,y));
  }
  else 
  {
      DATA[OFFSET] = ImVec2(x,y);
      OFFSET =  (OFFSET + 1) % MAX_SIZE;
  }
}

void PLOT_POINT_BUFFER::erase() 
{
  if (DATA.size() > 0) 
  {
      DATA.shrink(0);
      OFFSET  = 0;
  }
}

int PLOT_POINT_BUFFER::size()
{
  return DATA.size();
}

// ---------------------------------------------------------------------------------------

void MIN_MAX_PLOT_POINT_BUFFER::create(int Max_Size)
{
    OFFSET  = 0;
    MAX_SIZE = Max_Size;
    MIN.reserve(MAX_SIZE);
    MAX.reserve(MAX_SIZE);
}

void MIN_MAX_PLOT_POINT_BUFFER::add_point(float x, float Min_y, float Max_y) 
{
  if (MIN.size() < MAX_SIZE)
  {
      MIN.push_back(ImVec2(x, Min_y));
      MAX.push_back(ImVec2(x, Max_y));
  }
  else 
  {
      MIN[OFFSET] = ImVec2(x, Min_y);
      MAX[OFFSET] = ImVec2(x, Max_y);
      OFFSET =  (OFFSET + 1) % MAX_SIZE;
  }
}

void MIN_MAX_PLOT_POINT_BUFFER::erase() 
{
  if (MIN.size() > 0) 
  {
      MIN.shrink(0);
      OFFSET  = 0;
  }
  if (MAX.size() > 0) 
  {
      MAX.shrink(0);
      OFFSET  = 0;
  }
}

int MIN_MAX_PLOT_POINT_BUFFER::size()
{
  return MIN.size();
}

// -------------------------------------------------------------------------------------
void W_GUAGE_PLOT::create()
{
  for (int count = 0; count < PROPS.PLOT_LINE_COUNT; count++)
  {
    MIN_MAX_TIME_SLICE temp_min_max_data;
    string temp_prop_label = to_string(count);
    
    MIN_MAX_DATA.push_back(temp_min_max_data);
    PROPS.PLOT_LABEL.push_back(temp_prop_label);
  }

  if (PROPS.MIN_MAX)
  {
    for (int count = 0; count < PROPS.PLOT_LINE_COUNT; count++)
    {
      MIN_MAX_PLOT_POINT_BUFFER temp_min_max_plot_data;

      temp_min_max_plot_data.create(PROPS.BUFFER_SIZE);
      MIN_MAX_PLOT_DATA.push_back(temp_min_max_plot_data);
    }
  }
  else
  {
    for (int count = 0; count < PROPS.PLOT_LINE_COUNT; count++)
    {
      PLOT_POINT_BUFFER temp_plot_data;

      temp_plot_data.create(PROPS.BUFFER_SIZE);
      PLOT_DATA.push_back(temp_plot_data);
    }
  }
}

void W_GUAGE_PLOT::update_value(system_data &sdSysData, int Position, float Value)
{
  if (PROPS.MIN_MAX)
  {
    MIN_MAX_DATA[Position].store_value(Value);

    if (UPDATE_DATA.ping_down(sdSysData.tmeCURRENT_FRAME_TIME) == false)
    {
      MIN_MAX_PLOT_DATA[Position].add_point(DATA_INPUT_POS, MIN_MAX_DATA[Position].min(), MIN_MAX_DATA[Position].max());

      MIN_MAX_DATA[Position].clear();

      boop_ready = true;
    }
  }
  else
  {
    PLOT_DATA[Position].add_point(DATA_INPUT_POS, Value);

    boop_ready = true;
  }
}

void W_GUAGE_PLOT::update_boop(system_data &sdSysData)
{
  if (boop_ready)
  {
    if (PROPS.LEFT_TO_RIGHT)
    {
        DATA_INPUT_POS = DATA_INPUT_POS - 1;
    }
    else
    {
        DATA_INPUT_POS = DATA_INPUT_POS + 1;
    }

    if (PROPS.MIN_MAX)
    {
      UPDATE_DATA.ping_up(sdSysData.tmeCURRENT_FRAME_TIME, PROPS.MIN_MAX_WAIT);
    }

    boop_ready = false;

  }
}

void W_GUAGE_PLOT::draw(system_data &sdSysData, ImVec2 Size)
{
  // Got this working as a concept, but barely.  Needs to be reintegrated.

  ImPlotFlags plot_flags = ImPlotFlags_CanvasOnly;
  ImPlotLineFlags axis_flags = ImPlotAxisFlags_NoDecorations ;

  ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels;

  if (ImPlot::BeginPlot(PROPS.LABEL.c_str(), Size, plot_flags)) 
  {
    ImPlot::SetupAxes(nullptr, nullptr, flags, flags);

    if (PROPS.LEFT_TO_RIGHT)
    {
      ImPlot::SetupAxisLimits(ImAxis_X1, PROPS.BUFFER_SIZE + DATA_INPUT_POS, DATA_INPUT_POS, ImGuiCond_Always);
    }
    else
    {
      ImPlot::SetupAxisLimits(ImAxis_X1, DATA_INPUT_POS - PROPS.BUFFER_SIZE, DATA_INPUT_POS, ImGuiCond_Always);
    }

    ImPlot::SetupAxisLimits(ImAxis_Y1, 0, PROPS.VALUE_MAX);

    if (PROPS.MIN_MAX)
    {
      for (int count = 0; count < MIN_MAX_PLOT_DATA.size(); count++)
      {
        ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL,0.8f);
        ImPlot::PlotShaded(PROPS.PLOT_LABEL[count].c_str(), &MIN_MAX_PLOT_DATA[count].MIN[0].x, &MIN_MAX_PLOT_DATA[count].MIN[0].y, &MIN_MAX_PLOT_DATA[count].MAX[0].y, MIN_MAX_PLOT_DATA[count].MIN.size(), 0, MIN_MAX_PLOT_DATA[count].OFFSET, 2*sizeof(float));
      }
    }
    else
    {
      for (int count = 0; count < PLOT_DATA.size(); count++)
      {
        ImPlot::PlotLine(PROPS.PLOT_LABEL[count].c_str(), &PLOT_DATA[count].DATA[0].x, &PLOT_DATA[count].DATA[0].y, PLOT_DATA[count].DATA.size(), 0, PLOT_DATA[count].OFFSET, 2*sizeof(float));
      }
    }

    ImPlot::EndPlot();
  }
}

// -------------------------------------------------------------------------------------
  
void T_LARGE_NUMBER_DISPLAY::create()
{
  MIN_MAX.PROP.TIME_SPAN = PROPS.MIN_MAX_TIME_SPAN;
  MIN_MAX.PROP.SLICES = PROPS.MIN_MAX_TIME_SLICES;
}

void T_LARGE_NUMBER_DISPLAY::update_value(system_data &sdSysData, float Value, float Compare_Value, bool Is_Within)
{
  MIN_MAX.put_value(Value, sdSysData.tmeCURRENT_FRAME_TIME);
  VALUE = Value;
  VALUE_COMPARE = Compare_Value;
  IS_TEXT = false;
  ACTIVE_WITHIN = Is_Within;
}

void T_LARGE_NUMBER_DISPLAY::update_value(system_data &sdSysData, float Value)
{
  update_value(sdSysData, Value, -1, false);
}

void T_LARGE_NUMBER_DISPLAY::update_value(system_data &sdSysData, string Text)
{
  VALUE_TEXT = Text;
  IS_TEXT = true;
}

void T_LARGE_NUMBER_DISPLAY::draw(system_data &sdSysData)
{
  ImGuiIO &io = ImGui::GetIO();

  if (PROPS.LABEL_ON_LEFT == true)
  {
    ImGui::TextUnformatted(PROPS.LABEL.c_str());
    ImGui::SameLine();
  }

  //---

  ImVec2 pos = ImGui::GetCursorScreenPos();

  //---
  ImGui::PushFont(io.Fonts->Fonts.Data[1]);

  // Draw Compare Number
  if (VALUE_COMPARE >= 0 && !IS_TEXT)
  {
    if (ACTIVE_WITHIN == true)
    {
      ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.COLOR_COMB_ORANGE.STANDARD));
    }
    else
    {
      ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.COLOR_COMB_BLUE.STANDARD));
    }

    ImGui::Text("%2d", (int)VALUE_COMPARE);
    ImGui::PopStyleColor();
    
    ImGui::SetCursorScreenPos(pos);
  }

  // Draw Value
  if (IS_TEXT)
  {
    ImGui::Text("%s", VALUE_TEXT.c_str());
  }
  else if (VALUE < 100)
  {
    if (ACTIVE_WITHIN == true)
    {
      // If cruise is on change the color
      if (is_within(VALUE, VALUE_COMPARE - PROPS.WITHIN_VALUE, VALUE_COMPARE + PROPS.WITHIN_VALUE) == true)
      {
        ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.COLOR_COMB_GREEN.TEXT));
      }
      else
      {
        ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.COLOR_COMB_YELLOW.TEXT));
      }
      ImGui::Text("%2d", (int)VALUE);
      ImGui::PopStyleColor();
    }
    else if (VALUE <= -1)
    {
      ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.COLOR_COMB_YELLOW.TEXT));
      ImGui::Text("%2d", abs((int)VALUE));
      ImGui::PopStyleColor();
    }
    else  // Cruis is off, print the text.
    {
      ImGui::Text("%2d", (int)VALUE);
    }
  }
  else
  {
    ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.COLOR_COMB_RED.TEXT));
    ImGui::Text(">>");
    ImGui::PopStyleColor();
  }

  ImGui::PopFont();

  //---

  if (PROPS.LABEL_ON_LEFT == false)
  {
    ImGui::SameLine();
    ImGui::TextUnformatted(PROPS.LABEL.c_str());
  }

  pos.y = pos.y + 47;
  ImGui::SetCursorScreenPos(pos);

  if (PROPS.DISPLAY_MIN_MAX)
  {
    string min = "";
    string max = "";
    min = right_justify(2, to_string(abs(MIN_MAX.min())));
    max = right_justify(2, to_string(abs(MIN_MAX.max())));

    ImGui::PushStyleColor(ImGuiCol_Text, ImU32(sdSysData.COLOR_SELECT.COLOR_COMB_GREY.TEXT));
    
    ImGui::Text("%s  %s", min.c_str(), max.c_str());
    
    ImGui::PopStyleColor();
  }
  else
  {
    ImGui::Text("      ");
  }

  pos = ImGui::GetCursorScreenPos();
  pos.y = pos.y - 6; 
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
  MIN_MAX.put_value(Value, sdSysData.tmeCURRENT_FRAME_TIME);
}

void W_GUAGE::draw(system_data &sdSysData)
{
  if ((int)MIN_MAX.min() < 0)
  {
    DSP_MIN.PROPS.COLOR = sdSysData.COLOR_SELECT.COLOR_COMB_YELLOW;
  }
  else
  {
    DSP_MIN.PROPS.COLOR = sdSysData.COLOR_SELECT.COLOR_COMB_WHITE;
  }
  
  if ((int)(VALUE) < 0)
  {
    DSP_VALUE.PROPS.COLOR = sdSysData.COLOR_SELECT.COLOR_COMB_YELLOW;
  }
  else
  {
    DSP_VALUE.PROPS.COLOR = sdSysData.COLOR_SELECT.COLOR_COMB_WHITE;
  }

  if ((int)(MIN_MAX.max()) < 0)
  {
    DSP_MAX.PROPS.COLOR = sdSysData.COLOR_SELECT.COLOR_COMB_YELLOW;
  }
  else
  {
    DSP_MAX.PROPS.COLOR = sdSysData.COLOR_SELECT.COLOR_COMB_WHITE;
  }

  DSP_MIN.update_text(sdSysData, to_string((int)(MIN_MAX.min())));
  DSP_VALUE.update_text(sdSysData, to_string((int)(VALUE)));
  DSP_MAX.update_text(sdSysData, to_string((int)(MIN_MAX.max())));

  if (ImGui::BeginTable("Automobile Guage Val Min Max", 4, 0))
  {
    {
      ImGui::TableNextRow();
      ImGui::TableNextColumn();
      ImGui::Text(PROPS.LABEL.c_str());
      ImGui::TableNextColumn();
      DSP_MIN.draw(sdSysData);
      ImGui::TableNextColumn();
      DSP_VALUE.draw(sdSysData);
      ImGui::TableNextColumn();
      DSP_MAX.draw(sdSysData);
    }
    ImGui::EndTable();
  }

  ImGui::ProgressBar((abs)(VALUE / PROPS.VALUE_MAX), ImVec2(-1.0f,18.0f), (to_string_round_to_nth(VALUE, 1)).c_str());
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
  update_value(sdSysData, String_Value, 0);
}

void T_DATA_DISPLAY::draw(system_data &sdSysData)
{
  ImVec2 pos = ImGui::GetCursorScreenPos();

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

// ---------------------------------------------------------------------------------------
void AUTOMOBILE_SCREEN::create(system_data &sdSysData)
{
  SDATA.L_SPEED.PROPS.LABEL = "V\nE\nL";
  SDATA.L_SPEED.PROPS.LABEL_ON_LEFT = true;
  SDATA.L_SPEED.PROPS.DISPLAY_MIN_MAX = true;
  SDATA.L_SPEED.create();

  SDATA.L_ACCELERATION.PROPS.LABEL = "A\nC\nC";
  SDATA.L_ACCELERATION.PROPS.LABEL_ON_LEFT = true;
  SDATA.L_ACCELERATION.PROPS.DISPLAY_MIN_MAX = true;
  SDATA.L_ACCELERATION.create();
  
  SDATA.L_GEAR.PROPS.LABEL = "G\nE\nR";
  SDATA.L_GEAR.PROPS.LABEL_ON_LEFT = false;
  SDATA.L_GEAR.create();
  
  SDATA.L_TACH.PROPS.LABEL = "T\nA\nK";
  SDATA.L_TACH.PROPS.LABEL_ON_LEFT = false;
  SDATA.L_TACH.PROPS.DISPLAY_MIN_MAX = true;
  SDATA.L_TACH.create();

  SDATA.D_FUEL_RAIL_PRESSURE.PROPS.LABEL = right_justify(10, "F Rail P: ");
  SDATA.D_FUEL_RAIL_PRESSURE.PROPS.COLOR = sdSysData.COLOR_SELECT.COLOR_COMB_WHITE;
  SDATA.D_FUEL_RAIL_PRESSURE.create(sdSysData);

  SDATA.D_EVAP_SYSTEM_VAP_PRESSURE.PROPS.LABEL = right_justify(10, "Sys Vp P: ");
  SDATA.D_EVAP_SYSTEM_VAP_PRESSURE.PROPS.COLOR = sdSysData.COLOR_SELECT.COLOR_COMB_WHITE;
  SDATA.D_EVAP_SYSTEM_VAP_PRESSURE.create(sdSysData);

  SDATA.D_VOLTAGE.PROPS.LABEL = right_justify(10, "Voltage: ");
  SDATA.D_VOLTAGE.PROPS.COLOR = sdSysData.COLOR_SELECT.COLOR_COMB_WHITE;
  SDATA.D_VOLTAGE.PROPS.COLOR_SCALE.add_color_value_pair(8, sdSysData.COLOR_SELECT.COLOR_COMB_RED);
  SDATA.D_VOLTAGE.PROPS.COLOR_SCALE.add_color_value_pair(11, sdSysData.COLOR_SELECT.COLOR_COMB_YELLOW);
  SDATA.D_VOLTAGE.PROPS.COLOR_SCALE.add_color_value_pair(13, sdSysData.COLOR_SELECT.COLOR_COMB_GREEN);
  SDATA.D_VOLTAGE.PROPS.COLOR_SCALE.add_color_value_pair(16, sdSysData.COLOR_SELECT.COLOR_COMB_YELLOW);
  SDATA.D_VOLTAGE.PROPS.COLOR_SCALE.add_color_value_pair(100, sdSysData.COLOR_SELECT.COLOR_COMB_RED);
  SDATA.D_VOLTAGE.create(sdSysData);

  SDATA.D_BAROMETER.PROPS.LABEL = right_justify(10, "Baro: ");
  SDATA.D_BAROMETER.PROPS.COLOR = sdSysData.COLOR_SELECT.COLOR_COMB_WHITE;
  SDATA.D_BAROMETER.create(sdSysData);

  SDATA.D_TEMP_AMBIANT.PROPS.LABEL = right_justify(10, "Ambiant: ");
  SDATA.D_TEMP_AMBIANT.PROPS.COLOR = sdSysData.COLOR_SELECT.COLOR_COMB_WHITE;
  SDATA.D_TEMP_AMBIANT.PROPS.COLOR_SCALE.add_color_value_pair(0, sdSysData.COLOR_SELECT.COLOR_COMB_BLUE);
  SDATA.D_TEMP_AMBIANT.PROPS.COLOR_SCALE.add_color_value_pair(30, sdSysData.COLOR_SELECT.COLOR_COMB_GREEN);
  SDATA.D_TEMP_AMBIANT.PROPS.COLOR_SCALE.add_color_value_pair(38, sdSysData.COLOR_SELECT.COLOR_COMB_YELLOW);
  SDATA.D_TEMP_AMBIANT.PROPS.COLOR_SCALE.add_color_value_pair(43, sdSysData.COLOR_SELECT.COLOR_COMB_RED);
  SDATA.D_TEMP_AMBIANT.PROPS.COLOR_SCALE.add_color_value_pair(100, sdSysData.COLOR_SELECT.COLOR_COMB_PURPLE);
  SDATA.D_TEMP_AMBIANT.create(sdSysData);

  SDATA.D_TEMP_INTAKE.PROPS.LABEL = right_justify(10, "Intake: ");
  SDATA.D_TEMP_INTAKE.PROPS.COLOR = sdSysData.COLOR_SELECT.COLOR_COMB_WHITE;
  SDATA.D_TEMP_INTAKE.PROPS.COLOR_SCALE.add_color_value_pair(10, sdSysData.COLOR_SELECT.COLOR_COMB_BLUE);
  SDATA.D_TEMP_INTAKE.PROPS.COLOR_SCALE.add_color_value_pair(40, sdSysData.COLOR_SELECT.COLOR_COMB_GREEN);
  SDATA.D_TEMP_INTAKE.PROPS.COLOR_SCALE.add_color_value_pair(48, sdSysData.COLOR_SELECT.COLOR_COMB_YELLOW);
  SDATA.D_TEMP_INTAKE.PROPS.COLOR_SCALE.add_color_value_pair(53, sdSysData.COLOR_SELECT.COLOR_COMB_RED);
  SDATA.D_TEMP_INTAKE.PROPS.COLOR_SCALE.add_color_value_pair(100, sdSysData.COLOR_SELECT.COLOR_COMB_PURPLE);
  SDATA.D_TEMP_INTAKE.create(sdSysData);

  SDATA.D_TEMP_COOLANT.PROPS.LABEL = right_justify(10, "Coolant: ");
  SDATA.D_TEMP_COOLANT.PROPS.COLOR = sdSysData.COLOR_SELECT.COLOR_COMB_WHITE;
  SDATA.D_TEMP_COOLANT.PROPS.COLOR_SCALE.add_color_value_pair(60, sdSysData.COLOR_SELECT.COLOR_COMB_BLUE);
  SDATA.D_TEMP_COOLANT.PROPS.COLOR_SCALE.add_color_value_pair(90, sdSysData.COLOR_SELECT.COLOR_COMB_GREEN);
  SDATA.D_TEMP_COOLANT.PROPS.COLOR_SCALE.add_color_value_pair(100, sdSysData.COLOR_SELECT.COLOR_COMB_YELLOW);
  SDATA.D_TEMP_COOLANT.PROPS.COLOR_SCALE.add_color_value_pair(110, sdSysData.COLOR_SELECT.COLOR_COMB_RED);
  SDATA.D_TEMP_COOLANT.PROPS.COLOR_SCALE.add_color_value_pair(200, sdSysData.COLOR_SELECT.COLOR_COMB_PURPLE);
  SDATA.D_TEMP_COOLANT.create(sdSysData);

  SDATA.D_TEMP_CATALYST.PROPS.LABEL = right_justify(10, "Catalyst: ");
  SDATA.D_TEMP_CATALYST.PROPS.COLOR = sdSysData.COLOR_SELECT.COLOR_COMB_WHITE;
  SDATA.D_TEMP_CATALYST.PROPS.COLOR_SCALE.add_color_value_pair(150, sdSysData.COLOR_SELECT.COLOR_COMB_BLUE);
  SDATA.D_TEMP_CATALYST.PROPS.COLOR_SCALE.add_color_value_pair(750, sdSysData.COLOR_SELECT.COLOR_COMB_GREEN);
  SDATA.D_TEMP_CATALYST.PROPS.COLOR_SCALE.add_color_value_pair(850, sdSysData.COLOR_SELECT.COLOR_COMB_YELLOW);
  SDATA.D_TEMP_CATALYST.PROPS.COLOR_SCALE.add_color_value_pair(950, sdSysData.COLOR_SELECT.COLOR_COMB_RED);
  SDATA.D_TEMP_CATALYST.PROPS.COLOR_SCALE.add_color_value_pair(2000, sdSysData.COLOR_SELECT.COLOR_COMB_PURPLE);
  SDATA.D_TEMP_CATALYST.create(sdSysData);

  SDATA.D_TEMP_SUPER_TEMP.PROPS.LABEL = right_justify(10, "S-Temp: ");
  SDATA.D_TEMP_SUPER_TEMP.PROPS.COLOR = sdSysData.COLOR_SELECT.COLOR_COMB_WHITE;
  SDATA.D_TEMP_SUPER_TEMP.PROPS.COLOR_SCALE.add_color_value_pair(40, sdSysData.COLOR_SELECT.COLOR_COMB_BLUE);
  SDATA.D_TEMP_SUPER_TEMP.PROPS.COLOR_SCALE.add_color_value_pair(50, sdSysData.COLOR_SELECT.COLOR_COMB_GREEN);
  SDATA.D_TEMP_SUPER_TEMP.PROPS.COLOR_SCALE.add_color_value_pair(60, sdSysData.COLOR_SELECT.COLOR_COMB_YELLOW);
  SDATA.D_TEMP_SUPER_TEMP.PROPS.COLOR_SCALE.add_color_value_pair(70, sdSysData.COLOR_SELECT.COLOR_COMB_RED);
  SDATA.D_TEMP_SUPER_TEMP.PROPS.COLOR_SCALE.add_color_value_pair(100, sdSysData.COLOR_SELECT.COLOR_COMB_PURPLE);
  SDATA.D_TEMP_SUPER_TEMP.create(sdSysData);

  SDATA.OC_LF_TTL.PROPS.COLOR = sdSysData.COLOR_SELECT.COLOR_COMB_ORANGE;
  SDATA.OC_RF_TTL.PROPS.COLOR = sdSysData.COLOR_SELECT.COLOR_COMB_ORANGE;
  SDATA.OC_LB_TTL.PROPS.COLOR = sdSysData.COLOR_SELECT.COLOR_COMB_ORANGE;
  SDATA.OC_RB_TTL.PROPS.COLOR = sdSysData.COLOR_SELECT.COLOR_COMB_ORANGE;

  SDATA.D_LF_TTL.PROPS.LABEL = "TTL LF: ";
  SDATA.D_LF_TTL.PROPS.COLOR = sdSysData.COLOR_SELECT.COLOR_COMB_WHITE;
  SDATA.D_LF_TTL.PROPS.COLOR_SCALE.add_color_value_pair(25, sdSysData.COLOR_SELECT.COLOR_COMB_RED);
  SDATA.D_LF_TTL.PROPS.COLOR_SCALE.add_color_value_pair(50, sdSysData.COLOR_SELECT.COLOR_COMB_YELLOW);
  SDATA.D_LF_TTL.PROPS.COLOR_SCALE.add_color_value_pair(200, sdSysData.COLOR_SELECT.COLOR_COMB_GREEN);
  SDATA.D_LF_TTL.create(sdSysData);

  SDATA.D_RF_TTL.PROPS.LABEL = "TTL RF: ";
  SDATA.D_RF_TTL.PROPS.COLOR = sdSysData.COLOR_SELECT.COLOR_COMB_WHITE;
  SDATA.D_RF_TTL.PROPS.COLOR_SCALE.add_color_value_pair(25, sdSysData.COLOR_SELECT.COLOR_COMB_RED);
  SDATA.D_RF_TTL.PROPS.COLOR_SCALE.add_color_value_pair(50, sdSysData.COLOR_SELECT.COLOR_COMB_YELLOW);
  SDATA.D_RF_TTL.PROPS.COLOR_SCALE.add_color_value_pair(200, sdSysData.COLOR_SELECT.COLOR_COMB_GREEN);
  SDATA.D_RF_TTL.create(sdSysData);

  SDATA.D_LB_TTL.PROPS.LABEL = "TTL LB: ";
  SDATA.D_LB_TTL.PROPS.COLOR = sdSysData.COLOR_SELECT.COLOR_COMB_WHITE;
  SDATA.D_LB_TTL.PROPS.COLOR_SCALE.add_color_value_pair(25, sdSysData.COLOR_SELECT.COLOR_COMB_RED);
  SDATA.D_LB_TTL.PROPS.COLOR_SCALE.add_color_value_pair(50, sdSysData.COLOR_SELECT.COLOR_COMB_YELLOW);
  SDATA.D_LB_TTL.PROPS.COLOR_SCALE.add_color_value_pair(200, sdSysData.COLOR_SELECT.COLOR_COMB_GREEN);
  SDATA.D_LB_TTL.create(sdSysData);

  SDATA.D_RB_TTL.PROPS.LABEL = "TTL RB: ";
  SDATA.D_RB_TTL.PROPS.COLOR = sdSysData.COLOR_SELECT.COLOR_COMB_WHITE;
  SDATA.D_RB_TTL.PROPS.COLOR_SCALE.add_color_value_pair(25, sdSysData.COLOR_SELECT.COLOR_COMB_RED);
  SDATA.D_RB_TTL.PROPS.COLOR_SCALE.add_color_value_pair(50, sdSysData.COLOR_SELECT.COLOR_COMB_YELLOW);
  SDATA.D_RB_TTL.PROPS.COLOR_SCALE.add_color_value_pair(200, sdSysData.COLOR_SELECT.COLOR_COMB_GREEN);
  SDATA.D_RB_TTL.create(sdSysData);

  SDATA.D_LIGHTS.PROPS.LABEL = right_justify(10, "LIGHTS: ");
  SDATA.D_LIGHTS.PROPS.COLOR = sdSysData.COLOR_SELECT.COLOR_COMB_WHITE;
  SDATA.D_LIGHTS.create(sdSysData);

  SDATA.D_FUEL_LEVEL.PROPS.COLOR_SCALE.add_color_value_pair(1, sdSysData.COLOR_SELECT.COLOR_COMB_RED);
  SDATA.D_FUEL_LEVEL.PROPS.COLOR_SCALE.add_color_value_pair(4, sdSysData.COLOR_SELECT.COLOR_COMB_YELLOW);
  SDATA.D_FUEL_LEVEL.PROPS.COLOR_SCALE.add_color_value_pair(14, sdSysData.COLOR_SELECT.COLOR_COMB_GREEN);
  SDATA.D_FUEL_LEVEL.PROPS.LABEL = right_justify(10, "FUEL LVL: ");
  SDATA.D_FUEL_LEVEL.PROPS.COLOR = sdSysData.COLOR_SELECT.COLOR_COMB_WHITE;
  SDATA.D_FUEL_LEVEL.create(sdSysData);

  SDATA.D_PARKING_BRAKE.PROPS.COLOR_SCALE.add_color_value_pair(0, sdSysData.COLOR_SELECT.COLOR_COMB_GREEN);
  SDATA.D_PARKING_BRAKE.PROPS.COLOR_SCALE.add_color_value_pair(1, sdSysData.COLOR_SELECT.COLOR_COMB_RED);
  SDATA.D_PARKING_BRAKE.PROPS.LABEL = right_justify(10, "PARK BRK: ");
  SDATA.D_PARKING_BRAKE.PROPS.COLOR = sdSysData.COLOR_SELECT.COLOR_COMB_WHITE;
  SDATA.D_PARKING_BRAKE.create(sdSysData);

  SDATA.D_CRUISE_ON.PROPS.LABEL = right_justify(10, "C ON: ");
  SDATA.D_CRUISE_ON.PROPS.COLOR = sdSysData.COLOR_SELECT.COLOR_COMB_WHITE;
  SDATA.D_CRUISE_ON.create(sdSysData);

  SDATA.D_CRUISE_SPEED.PROPS.LABEL = right_justify(10, "C SPD: ");
  SDATA.D_CRUISE_SPEED.PROPS.COLOR = sdSysData.COLOR_SELECT.COLOR_COMB_WHITE;
  SDATA.D_CRUISE_SPEED.create(sdSysData);

  SDATA.G_SPEED.PROPS.LABEL = "Speed";
  SDATA.G_SPEED.PROPS.VALUE_MAX = 75;
  SDATA.G_SPEED.create();

  SDATA.G_ACCELERATION.PROPS.LABEL = "Accel";
  SDATA.G_ACCELERATION.PROPS.VALUE_MAX = 10;
  SDATA.G_ACCELERATION.create();

  SDATA.G_RPM.PROPS.LABEL = "Tach";
  SDATA.G_RPM.PROPS.VALUE_MAX = 6000;
  SDATA.G_RPM.create();

  SDATA.G_TORQUE.PROPS.LABEL = "Req Toq";
  SDATA.G_TORQUE.PROPS.VALUE_MAX = 255;
  SDATA.G_TORQUE.create();

  /*
  SDATA.G_TEMP_AMBIANT.PROPS.LABEL = "Ambi";
  SDATA.G_TEMP_AMBIANT.PROPS.VALUE_MAX = 40;
  SDATA.G_TEMP_AMBIANT.create();

  SDATA.G_TEMP_INTAKE.PROPS.LABEL = "Intake";
  SDATA.G_TEMP_INTAKE.PROPS.VALUE_MAX = 60;
  SDATA.G_TEMP_INTAKE.create();

  SDATA.G_TEMP_COOLANT.PROPS.LABEL = "Coolant";
  SDATA.G_TEMP_COOLANT.PROPS.VALUE_MIN = 40;
  SDATA.G_TEMP_COOLANT.PROPS.VALUE_MAX = 120;
  SDATA.G_TEMP_COOLANT.create();

  SDATA.G_TEMP_CATALYST.PROPS.LABEL = "Catlys";
  SDATA.G_TEMP_CATALYST.PROPS.VALUE_MAX = 1000;
  SDATA.G_TEMP_CATALYST.create();

  SDATA.G_TEMP_SUPER_TEMP.PROPS.LABEL = "S-Temp";
  SDATA.G_TEMP_SUPER_TEMP.PROPS.VALUE_MAX = 70;
  SDATA.G_TEMP_SUPER_TEMP.create();
  */

  SDATA.P_SPEED_SLOW.PROPS.LABEL = "Long Time Plot";
  SDATA.P_SPEED_SLOW.PROPS.PLOT_LINE_COUNT = 2;
  SDATA.P_SPEED_SLOW.PROPS.VALUE_MAX = 80;
  SDATA.P_SPEED_SLOW.PROPS.LEFT_TO_RIGHT = false;
  
  //SDATA.P_SPEED_SLOW.PROPS.BUFFER_SIZE = 1080;
  //SDATA.P_SPEED_SLOW.PROPS.MIN_MAX_WAIT = 20000;
  
  SDATA.P_SPEED_SLOW.PROPS.BUFFER_SIZE = 360;
  SDATA.P_SPEED_SLOW.PROPS.MIN_MAX_WAIT = 60000;
  
  //SDATA.P_SPEED_SLOW.PROPS.BUFFER_SIZE = 540;
  //SDATA.P_SPEED_SLOW.PROPS.MIN_MAX_WAIT = 20000;

  SDATA.P_SPEED_SLOW.PROPS.MIN_MAX = true;
  SDATA.P_SPEED_SLOW.create();
  SDATA.P_SPEED_SLOW.PROPS.PLOT_LABEL[0] = "SPEED";
  SDATA.P_SPEED_SLOW.PROPS.PLOT_LABEL[1] = "S-TEMP";

  SDATA.P_SPEED.PROPS.LABEL = "Short Time Plot";
  SDATA.P_SPEED.PROPS.PLOT_LINE_COUNT = 3;
  SDATA.P_SPEED.PROPS.VALUE_MAX = 80;
  SDATA.P_SPEED.PROPS.LEFT_TO_RIGHT = true;
  SDATA.P_SPEED.PROPS.BUFFER_SIZE = 100;
  SDATA.P_SPEED.create();
  SDATA.P_SPEED.PROPS.PLOT_LABEL[0] = "SPEED";
  SDATA.P_SPEED.PROPS.PLOT_LABEL[1] = "S-TEMP";
  SDATA.P_SPEED.PROPS.PLOT_LABEL[2] = "TACH";

  SDATA.VB_SPEED.PROPS.COLOR = sdSysData.COLOR_SELECT.COLOR_COMB_BLUE;
  SDATA.VB_SPEED.PROPS.VALUE_MAX = 80;

  SDATA.VB_TACH.PROPS.COLOR = sdSysData.COLOR_SELECT.COLOR_COMB_GREEN;
  SDATA.VB_TACH.PROPS.VALUE_MAX = 80;

  SDATA.VB_S_TEMP.PROPS.COLOR = sdSysData.COLOR_SELECT.COLOR_COMB_ORANGE;
  SDATA.VB_S_TEMP.PROPS.VALUE_MAX = 80;
}

void AUTOMOBILE_SCREEN::update(system_data &sdSysData)
{
  // Gather Data

  SDATA.MESSAGES = sdSysData.CAR_INFO.message_count;
  SDATA.LATEST_UNKNOWN_MESSAGE = sdSysData.CAR_INFO.DATA.AD_UNKNOWN.ORIG;

  SDATA.SPEED = sdSysData.CAR_INFO.STATUS.SPEED.SPEED_TRANS.val_mph();
  SDATA.ACCELERATION = sdSysData.CAR_INFO.CALCULATED.acceleration();

  SDATA.CRUISE_CONTROL_SET = sdSysData.CAR_INFO.STATUS.INDICATORS.cruise_control();
  SDATA.CRUISE_CONTROL_SPEED = sdSysData.CAR_INFO.STATUS.INDICATORS.cruise_control_speed();

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

  // Pressure

  SDATA.FUEL_RAIL_PRESSURE = sdSysData.CAR_INFO.STATUS.FUEL.FUEL_RAIL_PRESSURE.kPa();
  SDATA.FUEL_RAIL_PRESSURE_VAL = sdSysData.CAR_INFO.STATUS.FUEL.FUEL_RAIL_PRESSURE.val_kPa();

  SDATA.EVAP_SYSTEM_VAP_PRESSURE = sdSysData.CAR_INFO.STATUS.FUEL.EVAP_SYSTEM_VAP_PRESSURE.Pa();
  SDATA.EVAP_SYSTEM_VAP_PRESSURE_VAL = sdSysData.CAR_INFO.STATUS.FUEL.EVAP_SYSTEM_VAP_PRESSURE.val_Pa();

  SDATA.VOLTAGE = sdSysData.CAR_INFO.STATUS.ELECTRICAL.CONTROL_UNIT_42.v();
  SDATA.VOLTAGE_VAL = sdSysData.CAR_INFO.STATUS.ELECTRICAL.CONTROL_UNIT_42.val_v();

  SDATA.BAROMETER = sdSysData.CAR_INFO.STATUS.TEMPS.BARO_33.inHg();
  SDATA.BAROMETER_VAL = sdSysData.CAR_INFO.STATUS.TEMPS.BARO_33.val_inHg();

  // Steering

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

  // Large Indicators

  SDATA.L_SPEED.update_value(sdSysData, SDATA.SPEED, SDATA.CRUISE_CONTROL_SPEED, SDATA.CRUISE_CONTROL_SET);
  SDATA.L_ACCELERATION.update_value(sdSysData, 10 * SDATA.ACCELERATION);

  SDATA.L_GEAR.update_value(sdSysData, (SDATA.GEAR_SELECTION.c_str() + SDATA.GEAR));
  SDATA.L_TACH.update_value(sdSysData, SDATA.RPM / 100);

  // Display
  
  SDATA.D_FUEL_RAIL_PRESSURE.update_value(sdSysData, SDATA.FUEL_RAIL_PRESSURE, SDATA.FUEL_RAIL_PRESSURE_VAL);
  SDATA.D_EVAP_SYSTEM_VAP_PRESSURE.update_value(sdSysData, SDATA.EVAP_SYSTEM_VAP_PRESSURE, SDATA.EVAP_SYSTEM_VAP_PRESSURE_VAL);
  SDATA.D_VOLTAGE.update_value(sdSysData, SDATA.VOLTAGE, SDATA.VOLTAGE_VAL);
  SDATA.D_BAROMETER.update_value(sdSysData, SDATA.BAROMETER, SDATA.BAROMETER_VAL);

  SDATA.D_TEMP_AMBIANT.update_value(sdSysData, SDATA.TEMP_AMBIANT_STRING, SDATA.TEMP_AMBIANT);
  SDATA.D_TEMP_INTAKE.update_value(sdSysData, SDATA.TEMP_AIR_INTAKE_STRING, SDATA.TEMP_AIR_INTAKE);
  SDATA.D_TEMP_COOLANT.update_value(sdSysData, SDATA.TEMP_COOLANT_STRING, SDATA.TEMP_COOLANT);
  SDATA.D_TEMP_CATALYST.update_value(sdSysData, SDATA.TEMP_CATALYST_STRING, SDATA.TEMP_CATALYST);
  SDATA.D_TEMP_SUPER_TEMP.update_value(sdSysData, SDATA.TEMP_S_TEMP_STRING, SDATA.TEMP_S_TEMP);

  SDATA.OC_LF_TTL.update_value(sdSysData, SDATA.LF_TTL_BUFFER_FILL, SDATA.LF_TTL_BUFFER_FILL_MAX);
  SDATA.OC_RF_TTL.update_value(sdSysData, SDATA.RF_TTL_BUFFER_FILL, SDATA.RF_TTL_BUFFER_FILL_MAX);
  SDATA.OC_LB_TTL.update_value(sdSysData, SDATA.LB_TTL_BUFFER_FILL, SDATA.LB_TTL_BUFFER_FILL_MAX);
  SDATA.OC_RB_TTL.update_value(sdSysData, SDATA.RB_TTL_BUFFER_FILL, SDATA.RB_TTL_BUFFER_FILL_MAX);

  SDATA.D_LF_TTL.update_value(sdSysData, SDATA.LF_TTL, (int)SDATA.LF_TTL_VAL);
  SDATA.D_RF_TTL.update_value(sdSysData, SDATA.RF_TTL, (int)SDATA.RF_TTL_VAL);
  SDATA.D_LB_TTL.update_value(sdSysData, SDATA.LB_TTL, (int)SDATA.LB_TTL_VAL);
  SDATA.D_RB_TTL.update_value(sdSysData, SDATA.RB_TTL, (int)SDATA.RB_TTL_VAL);

  // Lights

  SDATA.D_LIGHTS.update_value(sdSysData, SDATA.LIGHTS_SWITCH);

  if (SDATA.LIGHTS_BEAM_ON == true)
  {
    SDATA.D_LIGHTS.PROPS.COLOR = sdSysData.COLOR_SELECT.COLOR_COMB_CYAN;
  }
  else
  {
    if (SDATA.LIGHTS_HEDLIGHTS_ON == true)
    {
      SDATA.D_LIGHTS.PROPS.COLOR = sdSysData.COLOR_SELECT.COLOR_COMB_WHITE;
    }
    else
    {
      SDATA.D_LIGHTS.PROPS.COLOR = sdSysData.COLOR_SELECT.COLOR_COMB_BLUE;
    }
  }

  SDATA.D_FUEL_LEVEL.update_value(sdSysData, SDATA.FUEL_LEVEL, SDATA.FUEL_LEVEL_VAL);

  if (SDATA.PARKING_BRAKE == true)
  {
    SDATA.D_PARKING_BRAKE.update_value(sdSysData, "ON", 1);
  }
  else
  {
    SDATA.D_PARKING_BRAKE.update_value(sdSysData, "OFF", 0);
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

  // Guages
  
  SDATA.G_SPEED.update_value(sdSysData, SDATA.SPEED);

  SDATA.G_ACCELERATION.update_value(sdSysData, SDATA.ACCELERATION);
  SDATA.G_RPM.update_value(sdSysData, SDATA.RPM);
  SDATA.G_TORQUE.update_value(sdSysData, (float)SDATA.TORQUE_DEMANDED);

  /*
  SDATA.G_TEMP_AMBIANT.update_value(sdSysData,  SDATA.TEMP_AMBIANT);
  SDATA.G_TEMP_INTAKE.update_value(sdSysData, SDATA.TEMP_AIR_INTAKE);
  SDATA.G_TEMP_COOLANT.update_value(sdSysData, SDATA.TEMP_COOLANT);
  SDATA.G_TEMP_CATALYST.update_value(sdSysData, SDATA.TEMP_CATALYST);
  SDATA.G_TEMP_SUPER_TEMP.update_value(sdSysData, SDATA.TEMP_S_TEMP);
  */

  // Plot

  SDATA.VB_SPEED.update_value(sdSysData, SDATA.SPEED);
  SDATA.VB_S_TEMP.update_value(sdSysData, SDATA.TEMP_S_TEMP);
  SDATA.VB_TACH.update_value(sdSysData, SDATA.RPM/50);
  
  SDATA.P_SPEED_SLOW.update_value(sdSysData, 0, SDATA.SPEED);
  SDATA.P_SPEED_SLOW.update_value(sdSysData, 1, SDATA.TEMP_S_TEMP);
  SDATA.P_SPEED_SLOW.update_boop(sdSysData);

  SDATA.P_SPEED.update_value(sdSysData, 0, SDATA.SPEED);
  SDATA.P_SPEED.update_value(sdSysData, 1, SDATA.TEMP_S_TEMP);
  SDATA.P_SPEED.update_value(sdSysData, 2, SDATA.RPM/50);
  SDATA.P_SPEED.update_boop(sdSysData);
  
}

void AUTOMOBILE_SCREEN::display(system_data &sdSysData, CONSOLE_COMMUNICATION &Screen_Comms, 
                                const char *name, bool *p_open, ImGuiWindowFlags flags)
{ 
  //ImGui::Begin(name, p_open, flags);
  //{
    //ImGui::Text("Messages: %d", SDATA.MESSAGES); 
    //ImGui::SameLine();
    //ImGui::Text("Unknown: %s", SDATA.LATEST_UNKNOWN_MESSAGE.c_str());
    
    ImGuiIO &io = ImGui::GetIO();

    const int disp_x = 94;
    const int disp_y = 147;

    ImGui::BeginChild("Speed Display", ImVec2(disp_x, disp_y), true, sdSysData.SCREEN_DEFAULTS.flags_c);
    {
      SDATA.L_SPEED.draw(sdSysData);

      SDATA.L_ACCELERATION.draw(sdSysData); //ImGui::TextUnformatted("Δ\nv");
    }
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("Auto Data 1", ImVec2(200, disp_y), true, sdSysData.SCREEN_DEFAULTS.flags_c);
    {
      /*
      if (ImGui::BeginTable("Automobile Data", 2, 0))
      {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text("Messages");
        ImGui::TableNextColumn();
        ImGui::Text("%d", SDATA.MESSAGES); 

        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::Text("Steering Angle");
        ImGui::TableNextColumn();
        ImGui::Text("%s %s %s", SDATA.STEERING_WHEEL_ANGLE.c_str(), 
                                SDATA.STEERING_WHEEL_LEFT_OF_CENTER.c_str(), 
                                SDATA.STEERING_WHEEL_TURNING_DIRECTION.c_str());
        
        ImGui::EndTable();
      }
      */

      SDATA.D_FUEL_RAIL_PRESSURE.draw(sdSysData);
      SDATA.D_EVAP_SYSTEM_VAP_PRESSURE.draw(sdSysData);
      SDATA.D_VOLTAGE.draw(sdSysData);
      SDATA.D_BAROMETER.draw(sdSysData);
    }
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("Auto Data 2", ImVec2(165, disp_y), true, sdSysData.SCREEN_DEFAULTS.flags_c);
    {
      
      SDATA.OC_LF_TTL.draw(sdSysData);
      ImGui::SameLine();
      SDATA.D_LF_TTL.draw(sdSysData);

      SDATA.OC_RF_TTL.draw(sdSysData);
      ImGui::SameLine();
      SDATA.D_RF_TTL.draw(sdSysData);

      SDATA.OC_LB_TTL.draw(sdSysData);
      ImGui::SameLine();
      SDATA.D_LB_TTL.draw(sdSysData);

      SDATA.OC_RB_TTL.draw(sdSysData);
      ImGui::SameLine();
      SDATA.D_RB_TTL.draw(sdSysData);
      
      //SDATA.THING1.draw(sdSysData);
    }
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("Auto Data 3", ImVec2(160, disp_y), true, sdSysData.SCREEN_DEFAULTS.flags_c);
    {
      SDATA.D_TEMP_AMBIANT.draw(sdSysData);
      SDATA.D_TEMP_INTAKE.draw(sdSysData);
      SDATA.D_TEMP_COOLANT.draw(sdSysData);
      SDATA.D_TEMP_CATALYST.draw(sdSysData);
      SDATA.D_TEMP_SUPER_TEMP.draw(sdSysData);
    }
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("Auto Data 4", ImVec2((ImGui::GetContentRegionAvail().x - disp_x) - 10, disp_y), true, sdSysData.SCREEN_DEFAULTS.flags_c);
    {
      SDATA.D_LIGHTS.draw(sdSysData);
      SDATA.D_FUEL_LEVEL.draw(sdSysData);
      SDATA.D_PARKING_BRAKE.draw(sdSysData);
      SDATA.D_CRUISE_ON.draw(sdSysData);
      SDATA.D_CRUISE_SPEED.draw(sdSysData);
    }
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("Gear Display", ImVec2(disp_x, disp_y), true, sdSysData.SCREEN_DEFAULTS.flags_c);
    {
      SDATA.L_GEAR.draw(sdSysData);
      SDATA.L_TACH.draw(sdSysData);
    }
    ImGui::EndChild();

    //ImGui::BeginChild("Data 1", ImVec2(ImGui::GetContentRegionAvail().x * 2 / 3, ImGui::GetContentRegionAvail().y), true, sdSysData.SCREEN_DEFAULTS.flags_c);
    //{  
    //  SDATA.P_SPEED.draw(sdSysData);
    //}
    //ImGui::EndChild();

    /*
    ImGui::BeginChild("Data 1", ImVec2(ImGui::GetContentRegionAvail().x * 1 / 4, ImGui::GetContentRegionAvail().y), true, sdSysData.SCREEN_DEFAULTS.flags_c);
    {
      SDATA.P_SPEED_SLOW.draw(sdSysData);
    }
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("Data 2", ImVec2(ImGui::GetContentRegionAvail().x * 1 / 3, ImGui::GetContentRegionAvail().y), true, sdSysData.SCREEN_DEFAULTS.flags_c);
    {
      SDATA.VB_SPEED.draw(sdSysData, ImVec2(ImGui::GetContentRegionAvail().x * 1 / 3, ImGui::GetContentRegionAvail().y));
      ImGui::SameLine();
      SDATA.VB_TACH.draw(sdSysData, ImVec2(ImGui::GetContentRegionAvail().x * 1 / 2, ImGui::GetContentRegionAvail().y));
      ImGui::SameLine();
      SDATA.VB_S_TEMP.draw(sdSysData, ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y));
    }
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("Data 3", ImVec2(ImGui::GetContentRegionAvail().x * 1 / 2, ImGui::GetContentRegionAvail().y), true, sdSysData.SCREEN_DEFAULTS.flags_c);
    {
      SDATA.P_SPEED.draw(sdSysData);
    }
    ImGui::EndChild();
    */

    ImGui::BeginChild("Data 1", ImVec2(ImGui::GetContentRegionAvail().x * 2 / 3, ImGui::GetContentRegionAvail().y), true, sdSysData.SCREEN_DEFAULTS.flags_c);
    { 

      float size_1_3 = ImGui::GetContentRegionAvail().x * 2 / 3;

      SDATA.P_SPEED_SLOW.draw(sdSysData, ImVec2(size_1_3, ImGui::GetContentRegionAvail().y));

      ImGui::SameLine();

      SDATA.VB_S_TEMP.draw(sdSysData, ImVec2(10, ImGui::GetContentRegionAvail().y));
      ImGui::SameLine();
      SDATA.VB_SPEED.draw(sdSysData, ImVec2(10, ImGui::GetContentRegionAvail().y));
      ImGui::SameLine();
      SDATA.VB_TACH.draw(sdSysData, ImVec2(10, ImGui::GetContentRegionAvail().y));

      ImGui::SameLine();

      SDATA.P_SPEED.draw(sdSysData, ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y));
    }
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("Data 4", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true, sdSysData.SCREEN_DEFAULTS.flags_c);
    {
      SDATA.G_SPEED.draw(sdSysData);

      SDATA.G_ACCELERATION.draw(sdSysData);
      SDATA.G_RPM.draw(sdSysData);
      SDATA.G_TORQUE.draw(sdSysData);
    }
    ImGui::EndChild();
}

// ---------------------------------------------------------------------------------------
#endif