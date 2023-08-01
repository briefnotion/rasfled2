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

// utility structure for realtime plot

PLOT_POINT_BUFFER::PLOT_POINT_BUFFER() 
{
    OFFSET  = 0;
    DATA.reserve(MAX_SIZE);
}

void PLOT_POINT_BUFFER::transform() 
{
  // transform 1

  int t1_watch_pos = 30;
  int t1_average_size = 4;

  //if (DATA.size() >= t1_watch_pos)
  //{
    /*
    if (t1_pos > 4)
    {
      //float average_point = (DATA[t1_watch_pos +0 ].x + DATA[t1_watch_pos +1 ].x + DATA[t1_watch_pos +2 ].x + DATA[t1_watch_pos +3 ].x) / 4;

      //DATA[t1_watch_pos].x = average_point;

      DATA.erase(DATA.begin() + t1_watch_pos + 1);
      DATA.erase(DATA.begin() +  t1_watch_pos + 1);
      DATA.erase(DATA.begin() +  t1_watch_pos + 1);

      //DATA.erase(DATA.begin() + 61, DATA.begin() + 63);

      t1_pos = 0;
    }
    else
    {
      t1_pos = t1_pos + 1;
    }
    */
  //}

  if (DATA.size() > 100 && DATA.size() % 4 == 0)
  {
    DATA.erase(DATA.begin() + 100 - 1);
  }
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

  //transform();
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

// -------------------------------------------------------------------------------------
void W_GUAGE_PLOT::create()
{
 
}

void W_GUAGE_PLOT::update_value(system_data &sdSysData, float Value1, float Value2, float Value3)
{

  VALUE1 = Value1;
  VALUE2 = Value2;
  VALUE3 = Value3;

  if (UPDATE_DATA.ping_down(sdSysData.tmeCURRENT_FRAME_TIME) == false)
  {
    //DATA_INPUT_POS = DATA1.size();

    DATA1.add_point(DATA_INPUT_POS, Value1);
    DATA2.add_point(DATA_INPUT_POS, Value2);
    DATA3.add_point(DATA_INPUT_POS, Value3);
    
    DATA_INPUT_POS = DATA_INPUT_POS + 1;

    UPDATE_DATA.ping_up(sdSysData.tmeCURRENT_FRAME_TIME, 500);
  }
}

void W_GUAGE_PLOT::draw(system_data &sdSysData)
{
  // Got this working as a concept, but barely.  Needs to be reintegrated.

  ImPlotFlags plot_flags = ImPlotFlags_CanvasOnly;
  ImPlotLineFlags axis_flags = ImPlotAxisFlags_NoDecorations ;

  ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels;

  if (ImPlot::BeginPlot("Speed Over Time", ImVec2((ImGui::GetContentRegionAvail().x) - 57, ImGui::GetContentRegionAvail().y), plot_flags)) 
  {
    ImPlot::SetupAxes(nullptr, nullptr, flags, flags);

    ImPlot::SetupAxisLimits(ImAxis_X1, DATA_INPUT_POS - PROPS.BUFFER_SIZE, DATA_INPUT_POS, ImGuiCond_Always);
    ImPlot::SetupAxisLimits(ImAxis_Y1, 0, PROPS.VALUE_MAX);

    ImPlot::PlotLine("SPEED", &DATA1.DATA[0].x, &DATA1.DATA[0].y, DATA1.DATA.size(), 0, DATA1.OFFSET, 2*sizeof(float));
    ImPlot::PlotLine("S-TEMP", &DATA2.DATA[0].x, &DATA2.DATA[0].y, DATA2.DATA.size(), 0, DATA2.OFFSET, 2*sizeof(float));
    ImPlot::PlotLine("TACH", &DATA3.DATA[0].x, &DATA3.DATA[0].y, DATA3.DATA.size(), 0, DATA3.OFFSET, 2*sizeof(float));
    ImPlot::EndPlot();
  }

  {
    ImGui::SameLine();
    
    const ImVec2 small_slider_size(10, ImGui::GetContentRegionAvail().y);

    float slider_pos_1 = VALUE1/ (float)PROPS.VALUE_MAX;
    float slider_pos_2 = VALUE2/ (float)PROPS.VALUE_MAX;
    float slider_pos_3 = VALUE3/ (float)PROPS.VALUE_MAX;

    ImGui::PushStyleColor(ImGuiCol_FrameBg, sdSysData.COLOR_SELECT.COLOR_COMB_BLUE.BACKGROUND);
    //ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, sdSysData.COLOR_SELECT.COLOR_COMB_BLUE);
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, sdSysData.COLOR_SELECT.COLOR_COMB_BLUE.STANDARD);
    ImGui::PushStyleColor(ImGuiCol_SliderGrab, sdSysData.COLOR_SELECT.COLOR_COMB_BLUE.ACTIVE);
    ImGui::VSliderFloat("##v", small_slider_size, &slider_pos_1, 0.0f, 1.0f, "", sdSysData.SCREEN_DEFAULTS.flags_vs);
    ImGui::PopStyleColor(3);

    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_FrameBg, sdSysData.COLOR_SELECT.COLOR_COMB_ORANGE.BACKGROUND);
    //ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, sdSysData.COLOR_SELECT.COLOR_COMB_ORANGE);
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, sdSysData.COLOR_SELECT.COLOR_COMB_ORANGE.STANDARD);
    ImGui::PushStyleColor(ImGuiCol_SliderGrab, sdSysData.COLOR_SELECT.COLOR_COMB_ORANGE.ACTIVE);
    ImGui::VSliderFloat("##v", small_slider_size, &slider_pos_2, 0.0f, 1.0f, "", sdSysData.SCREEN_DEFAULTS.flags_vs);
    ImGui::PopStyleColor(3);

    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_FrameBg, sdSysData.COLOR_SELECT.COLOR_COMB_GREEN.BACKGROUND);
    //ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, sdSysData.COLOR_SELECT.COLOR_COMB_GREEN);
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, sdSysData.COLOR_SELECT.COLOR_COMB_GREEN.STANDARD);
    ImGui::PushStyleColor(ImGuiCol_SliderGrab, sdSysData.COLOR_SELECT.COLOR_COMB_GREEN.ACTIVE);
    ImGui::VSliderFloat("##v", small_slider_size, &slider_pos_3, 0.0f, 1.0f, "", sdSysData.SCREEN_DEFAULTS.flags_vs);
    ImGui::PopStyleColor(3);
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
        ImGui::PushStyleColor(ImGuiCol_Text, sdSysData.COLOR_SELECT.COLOR_COMB_GREEN.ACTIVE);
      }
      else
      {
        ImGui::PushStyleColor(ImGuiCol_Text, sdSysData.COLOR_SELECT.COLOR_COMB_YELLOW.ACTIVE);
      }
      ImGui::Text("%2d", (int)VALUE);
      ImGui::PopStyleColor();
    }
    else if (VALUE <= -1)
    {
      ImGui::PushStyleColor(ImGuiCol_Text, sdSysData.COLOR_SELECT.COLOR_COMB_YELLOW.ACTIVE);
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
    ImGui::PushStyleColor(ImGuiCol_Text, sdSysData.COLOR_SELECT.COLOR_COMB_RED.ACTIVE);
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

    ImGui::PushStyleColor(ImGuiCol_Text, sdSysData.COLOR_SELECT.COLOR_COMB_GREY.ACTIVE);
    
    ImGui::Text("%s  %s", min.c_str(), max.c_str());
    
    ImGui::PopStyleColor();
  }
  else
  {
    ImGui::Text("      ");
  }

  pos = ImGui::GetCursorScreenPos();
  pos.y = pos.y - 6; 
  //ImGui::SetCursorScreenPos(pos);
}

// -------------------------------------------------------------------------------------

void W_GUAGE::create()
{
  MIN_MAX.PROP.TIME_SPAN = PROPS.MIN_MAX_TIME_SPAN;
  MIN_MAX.PROP.SLICES = PROPS.MIN_MAX_TIME_SLICES;
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
    DSP_MIN.PROPS.COLOR = sdSysData.COLOR_SELECT.COLOR_COMB_DEFAULT;
  }
  
  if ((int)(VALUE) < 0)
  {
    DSP_VALUE.PROPS.COLOR = sdSysData.COLOR_SELECT.COLOR_COMB_YELLOW;
  }
  else
  {
    DSP_VALUE.PROPS.COLOR = sdSysData.COLOR_SELECT.COLOR_COMB_DEFAULT;
  }

  if ((int)(MIN_MAX.max()) < 0)
  {
    DSP_MAX.PROPS.COLOR = sdSysData.COLOR_SELECT.COLOR_COMB_YELLOW;
  }
  else
  {
    DSP_MAX.PROPS.COLOR = sdSysData.COLOR_SELECT.COLOR_COMB_DEFAULT;
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

  //ImGui::PushStyleColor(ImGuiCol_TextSelectedBg, sdSysData.COLOR_SELECT.COLOR_COMB_BLUE.BACKGROUND);
  ImGui::ProgressBar((abs)(VALUE / PROPS.VALUE_MAX), ImVec2(-1.0f,18.0f), (to_string_round_to_nth(VALUE, 1)).c_str());
  //ImGui::PopStyleColor();
}

// ---------------------------------------------------------------------------------------

void T_DATA_DISPLAY::create(system_data &sdSysData)
{
  LABEL.update_text(sdSysData, PROPS.LABEL);
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

  pos.x = pos.x + PROPS.DATA_OFFSET;
  ImGui::SetCursorScreenPos(pos);

  if (PROPS.COLOR_SCALE.active())
  {
    ImGui::PushStyleColor(ImGuiCol_Text, PROPS.COLOR_SCALE.get_color(VALUE_FLOAT).ACTIVE);
    DATA.draw(sdSysData);
    ImGui::PopStyleColor();
  }
  else
  {
    if (PROPS.COLOR.DEFAULT == true)
    {
      DATA.draw(sdSysData);
    }
    else
    {
      DATA.PROPS.COLOR = PROPS.COLOR;
      DATA.draw(sdSysData);
    }
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

  SDATA.D_FUEL_RAIL_PRESSURE.PROPS.LABEL = right_justify(9, "F Rail P:");
  SDATA.D_FUEL_RAIL_PRESSURE.create(sdSysData);

  SDATA.D_EVAP_SYSTEM_VAP_PRESSURE.PROPS.LABEL = right_justify(9, "Sys Vp P:");
  SDATA.D_EVAP_SYSTEM_VAP_PRESSURE.create(sdSysData);

  SDATA.D_VOLTAGE.PROPS.LABEL = right_justify(9, "Voltage:");
  SDATA.D_VOLTAGE.PROPS.COLOR_SCALE.add_color_value_pair(8, sdSysData.COLOR_SELECT.COLOR_COMB_RED);
  SDATA.D_VOLTAGE.PROPS.COLOR_SCALE.add_color_value_pair(11, sdSysData.COLOR_SELECT.COLOR_COMB_YELLOW);
  SDATA.D_VOLTAGE.PROPS.COLOR_SCALE.add_color_value_pair(13, sdSysData.COLOR_SELECT.COLOR_COMB_GREEN);
  SDATA.D_VOLTAGE.PROPS.COLOR_SCALE.add_color_value_pair(16, sdSysData.COLOR_SELECT.COLOR_COMB_YELLOW);
  SDATA.D_VOLTAGE.PROPS.COLOR_SCALE.add_color_value_pair(100, sdSysData.COLOR_SELECT.COLOR_COMB_RED);
  SDATA.D_VOLTAGE.create(sdSysData);

  SDATA.D_BAROMETER.PROPS.LABEL = right_justify(9, "Baro:");
  SDATA.D_BAROMETER.create(sdSysData);

  SDATA.D_TEMP_AMBIANT.PROPS.LABEL = right_justify(9, "Ambiant:");
  SDATA.D_TEMP_AMBIANT.PROPS.COLOR_SCALE.add_color_value_pair(0, sdSysData.COLOR_SELECT.COLOR_COMB_BLUE);
  SDATA.D_TEMP_AMBIANT.PROPS.COLOR_SCALE.add_color_value_pair(30, sdSysData.COLOR_SELECT.COLOR_COMB_GREEN);
  SDATA.D_TEMP_AMBIANT.PROPS.COLOR_SCALE.add_color_value_pair(38, sdSysData.COLOR_SELECT.COLOR_COMB_YELLOW);
  SDATA.D_TEMP_AMBIANT.PROPS.COLOR_SCALE.add_color_value_pair(43, sdSysData.COLOR_SELECT.COLOR_COMB_RED);
  SDATA.D_TEMP_AMBIANT.PROPS.COLOR_SCALE.add_color_value_pair(100, sdSysData.COLOR_SELECT.COLOR_COMB_PURPLE);
  SDATA.D_TEMP_AMBIANT.create(sdSysData);

  SDATA.D_TEMP_INTAKE.PROPS.LABEL = right_justify(9, "Intake:");
  SDATA.D_TEMP_INTAKE.PROPS.COLOR_SCALE.add_color_value_pair(10, sdSysData.COLOR_SELECT.COLOR_COMB_BLUE);
  SDATA.D_TEMP_INTAKE.PROPS.COLOR_SCALE.add_color_value_pair(40, sdSysData.COLOR_SELECT.COLOR_COMB_GREEN);
  SDATA.D_TEMP_INTAKE.PROPS.COLOR_SCALE.add_color_value_pair(48, sdSysData.COLOR_SELECT.COLOR_COMB_YELLOW);
  SDATA.D_TEMP_INTAKE.PROPS.COLOR_SCALE.add_color_value_pair(53, sdSysData.COLOR_SELECT.COLOR_COMB_RED);
  SDATA.D_TEMP_INTAKE.PROPS.COLOR_SCALE.add_color_value_pair(100, sdSysData.COLOR_SELECT.COLOR_COMB_PURPLE);
  SDATA.D_TEMP_INTAKE.create(sdSysData);

  SDATA.D_TEMP_COOLANT.PROPS.LABEL = right_justify(9, "Coolant:");
  SDATA.D_TEMP_COOLANT.PROPS.COLOR_SCALE.add_color_value_pair(60, sdSysData.COLOR_SELECT.COLOR_COMB_BLUE);
  SDATA.D_TEMP_COOLANT.PROPS.COLOR_SCALE.add_color_value_pair(90, sdSysData.COLOR_SELECT.COLOR_COMB_GREEN);
  SDATA.D_TEMP_COOLANT.PROPS.COLOR_SCALE.add_color_value_pair(100, sdSysData.COLOR_SELECT.COLOR_COMB_YELLOW);
  SDATA.D_TEMP_COOLANT.PROPS.COLOR_SCALE.add_color_value_pair(110, sdSysData.COLOR_SELECT.COLOR_COMB_RED);
  SDATA.D_TEMP_COOLANT.PROPS.COLOR_SCALE.add_color_value_pair(200, sdSysData.COLOR_SELECT.COLOR_COMB_PURPLE);
  SDATA.D_TEMP_COOLANT.create(sdSysData);

  SDATA.D_TEMP_CATALYST.PROPS.LABEL = right_justify(9, "Catalyst:");
  SDATA.D_TEMP_CATALYST.PROPS.COLOR_SCALE.add_color_value_pair(150, sdSysData.COLOR_SELECT.COLOR_COMB_BLUE);
  SDATA.D_TEMP_CATALYST.PROPS.COLOR_SCALE.add_color_value_pair(750, sdSysData.COLOR_SELECT.COLOR_COMB_GREEN);
  SDATA.D_TEMP_CATALYST.PROPS.COLOR_SCALE.add_color_value_pair(850, sdSysData.COLOR_SELECT.COLOR_COMB_YELLOW);
  SDATA.D_TEMP_CATALYST.PROPS.COLOR_SCALE.add_color_value_pair(950, sdSysData.COLOR_SELECT.COLOR_COMB_RED);
  SDATA.D_TEMP_CATALYST.PROPS.COLOR_SCALE.add_color_value_pair(2000, sdSysData.COLOR_SELECT.COLOR_COMB_PURPLE);
  SDATA.D_TEMP_CATALYST.create(sdSysData);

  SDATA.D_TEMP_SUPER_TEMP.PROPS.LABEL = right_justify(9, "S-Temp:");
  SDATA.D_TEMP_SUPER_TEMP.PROPS.COLOR_SCALE.add_color_value_pair(40, sdSysData.COLOR_SELECT.COLOR_COMB_BLUE);
  SDATA.D_TEMP_SUPER_TEMP.PROPS.COLOR_SCALE.add_color_value_pair(50, sdSysData.COLOR_SELECT.COLOR_COMB_GREEN);
  SDATA.D_TEMP_SUPER_TEMP.PROPS.COLOR_SCALE.add_color_value_pair(60, sdSysData.COLOR_SELECT.COLOR_COMB_YELLOW);
  SDATA.D_TEMP_SUPER_TEMP.PROPS.COLOR_SCALE.add_color_value_pair(70, sdSysData.COLOR_SELECT.COLOR_COMB_RED);
  SDATA.D_TEMP_SUPER_TEMP.PROPS.COLOR_SCALE.add_color_value_pair(100, sdSysData.COLOR_SELECT.COLOR_COMB_PURPLE);
  SDATA.D_TEMP_SUPER_TEMP.create(sdSysData);

  SDATA.D_LF_TTL.PROPS.LABEL = right_justify(9, "TTL LF:");
  SDATA.D_LF_TTL.PROPS.COLOR_SCALE.add_color_value_pair(25, sdSysData.COLOR_SELECT.COLOR_COMB_RED);
  SDATA.D_LF_TTL.PROPS.COLOR_SCALE.add_color_value_pair(50, sdSysData.COLOR_SELECT.COLOR_COMB_YELLOW);
  SDATA.D_LF_TTL.PROPS.COLOR_SCALE.add_color_value_pair(200, sdSysData.COLOR_SELECT.COLOR_COMB_GREEN);
  SDATA.D_LF_TTL.create(sdSysData);

  SDATA.D_RF_TTL.PROPS.LABEL = right_justify(9, "TTL RF:");
  SDATA.D_RF_TTL.PROPS.COLOR_SCALE.add_color_value_pair(25, sdSysData.COLOR_SELECT.COLOR_COMB_RED);
  SDATA.D_RF_TTL.PROPS.COLOR_SCALE.add_color_value_pair(50, sdSysData.COLOR_SELECT.COLOR_COMB_YELLOW);
  SDATA.D_RF_TTL.PROPS.COLOR_SCALE.add_color_value_pair(200, sdSysData.COLOR_SELECT.COLOR_COMB_GREEN);
  SDATA.D_RF_TTL.create(sdSysData);

  SDATA.D_LB_TTL.PROPS.LABEL = right_justify(9, "TTL LB:");
  SDATA.D_LB_TTL.PROPS.COLOR_SCALE.add_color_value_pair(25, sdSysData.COLOR_SELECT.COLOR_COMB_RED);
  SDATA.D_LB_TTL.PROPS.COLOR_SCALE.add_color_value_pair(50, sdSysData.COLOR_SELECT.COLOR_COMB_YELLOW);
  SDATA.D_LB_TTL.PROPS.COLOR_SCALE.add_color_value_pair(200, sdSysData.COLOR_SELECT.COLOR_COMB_GREEN);
  SDATA.D_LB_TTL.create(sdSysData);

  SDATA.D_RB_TTL.PROPS.LABEL = right_justify(9, "TTL RB:");
  SDATA.D_RB_TTL.PROPS.COLOR_SCALE.add_color_value_pair(25, sdSysData.COLOR_SELECT.COLOR_COMB_RED);
  SDATA.D_RB_TTL.PROPS.COLOR_SCALE.add_color_value_pair(50, sdSysData.COLOR_SELECT.COLOR_COMB_YELLOW);
  SDATA.D_RB_TTL.PROPS.COLOR_SCALE.add_color_value_pair(200, sdSysData.COLOR_SELECT.COLOR_COMB_GREEN);
  SDATA.D_RB_TTL.create(sdSysData);

  SDATA.D_LIGHTS.PROPS.LABEL = right_justify(9, "LIGHTS:");
  SDATA.D_LIGHTS.create(sdSysData);

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

  SDATA.P_SPEED.create();
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

  SDATA.LF_TTL = sdSysData.CAR_INFO.CALCULATED.LF_TTL.life_percentage_mean();
  SDATA.LF_TTL_VAL = sdSysData.CAR_INFO.CALCULATED.LF_TTL.val_life_percentage_mean();

  SDATA.RF_TTL = sdSysData.CAR_INFO.CALCULATED.RF_TTL.life_percentage_mean();
  SDATA.RF_TTL_VAL = sdSysData.CAR_INFO.CALCULATED.RF_TTL.val_life_percentage_mean();

  SDATA.LB_TTL = sdSysData.CAR_INFO.CALCULATED.LB_TTL.life_percentage_mean();
  SDATA.LB_TTL_VAL = sdSysData.CAR_INFO.CALCULATED.LB_TTL.val_life_percentage_mean();

  SDATA.RB_TTL = sdSysData.CAR_INFO.CALCULATED.RB_TTL.life_percentage_mean();
  SDATA.RB_TTL_VAL = sdSysData.CAR_INFO.CALCULATED.RB_TTL.val_life_percentage_mean();

  SDATA.LIGHTS_ON = sdSysData.CAR_INFO.STATUS.INDICATORS.val_lights_headlights_on();
  SDATA.LIGHTS_BEAM_ON = sdSysData.CAR_INFO.STATUS.INDICATORS.val_lights_high_beam_on();
  SDATA.LIGHTS_SWITCH = sdSysData.CAR_INFO.STATUS.INDICATORS.lights_switch();
  
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
    if (SDATA.LIGHTS_ON == true)
    {
      SDATA.D_LIGHTS.PROPS.COLOR = sdSysData.COLOR_SELECT.COLOR_COMB_DEFAULT;
    }
    else
    {
      SDATA.D_LIGHTS.PROPS.COLOR = sdSysData.COLOR_SELECT.COLOR_COMB_BLUE;
    }
  }

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

  SDATA.P_SPEED.update_value(sdSysData, SDATA.SPEED, SDATA.TEMP_S_TEMP, SDATA.RPM/50);

  /*
  //--

  MESSAGES.set_text(to_string(sdSysData.CAR_INFO.message_count));

  AD_UNKNOWN.set_text(sdSysData.CAR_INFO.DATA.AD_UNKNOWN.ORIG);

  //-----------
  // Large Velocity
  if (sdSysData.CAR_INFO.STATUS.SPEED.SPEED_TRANS.val_mph() >= 10)
  {
    LARGE_SPEED_10.set_text(CHARACTERS_6X5.number(get_2_pos_of_int((int)sdSysData.CAR_INFO.STATUS.SPEED.SPEED_TRANS.val_mph())), tmeFrame_Time);
  }
  else
  {
    LARGE_SPEED_10.set_text(CHARACTERS_6X5.space(), tmeFrame_Time);
  }

  LARGE_SPEED_1.set_text(CHARACTERS_6X5.number(get_1_pos_of_int((int)sdSysData.CAR_INFO.STATUS.SPEED.SPEED_TRANS.val_mph())), tmeFrame_Time);

  if (sdSysData.CAR_INFO.STATUS.INDICATORS.cruise_control() == true)
  {
    if (is_within(sdSysData.CAR_INFO.STATUS.SPEED.SPEED_TRANS.val_mph(), 
                  sdSysData.CAR_INFO.STATUS.INDICATORS.cruise_control_speed() - .5,
                  sdSysData.CAR_INFO.STATUS.INDICATORS.cruise_control_speed() + .5) == true)
    {
      LARGE_SPEED_1.set_color(COLOR_GREEN, COLOR_BLACK);
      LARGE_SPEED_10.set_color(COLOR_GREEN, COLOR_BLACK);
    }
    else
    {
      LARGE_SPEED_1.set_color(COLOR_YELLOW, COLOR_BLACK);
      LARGE_SPEED_10.set_color(COLOR_YELLOW, COLOR_BLACK);
    }
  }
  else
  {
    LARGE_SPEED_1.set_color(COLOR_WHITE, COLOR_BLACK);
    LARGE_SPEED_10.set_color(COLOR_WHITE, COLOR_BLACK);
  }

  //-----------
  // Large ACCELERATION
  LARGE_ACCELERATION_10.set_text(CHARACTERS_6X5.number(get_2_pos_of_int(10 * abs(sdSysData.CAR_INFO.CALCULATED.acceleration())), true), tmeFrame_Time);
  LARGE_ACCELERATION_1.set_text(CHARACTERS_6X5.number(get_1_pos_of_int(10 * abs(sdSysData.CAR_INFO.CALCULATED.acceleration()))), tmeFrame_Time);

  if (sdSysData.CAR_INFO.CALCULATED.acceleration() >= -.1)
  {
    LARGE_ACCELERATION_DESC.set_color(COLOR_WHITE, COLOR_BLUE);
  }
  else
  {
    LARGE_ACCELERATION_DESC.set_color(COLOR_YELLOW, COLOR_BLACK);
  }

  if (sdSysData.CAR_INFO.CALCULATED.acceleration() >= -5 && sdSysData.CAR_INFO.CALCULATED.acceleration() <= 3)
  {
    LARGE_ACCELERATION_10.set_color(COLOR_WHITE, COLOR_BLACK);
    LARGE_ACCELERATION_1.set_color(COLOR_WHITE, COLOR_BLACK);
    LARGE_ACCELERATION_MIN.set_color(COLOR_WHITE, COLOR_BLACK);
    LARGE_ACCELERATION_MAX.set_color(COLOR_WHITE, COLOR_BLACK);
  }
  else
  {
    LARGE_ACCELERATION_10.set_color(COLOR_YELLOW, COLOR_BLACK);
    LARGE_ACCELERATION_1.set_color(COLOR_YELLOW, COLOR_BLACK);
    LARGE_ACCELERATION_MIN.set_color(COLOR_YELLOW, COLOR_BLACK);
    LARGE_ACCELERATION_MAX.set_color(COLOR_YELLOW, COLOR_BLACK);
  }

  LARGE_ACCELERATION_MIN.set_text(to_string_round_to_nth(abs(sdSysData.CAR_INFO.CALCULATED.ACCELERATION_MIN_MAX_HISTORY.min_float()), 1));
  LARGE_ACCELERATION_MAX.set_text(to_string_round_to_nth(sdSysData.CAR_INFO.CALCULATED.ACCELERATION_MIN_MAX_HISTORY.max_float(), 1));
  
  //-----------
  // Large GEAR
  
  LARGE_GEAR_1.set_text(CHARACTERS_6X5.number(sdSysData.CAR_INFO.STATUS.GEAR.reported()), tmeFrame_Time);
  LARGE_GEAR_S.set_text(CHARACTERS_6X5.character(sdSysData.CAR_INFO.STATUS.GEAR.gear_selection_short_desc()[0]), tmeFrame_Time);

  // Large TACH
  LARGE_TACH_1.set_text(CHARACTERS_6X5.number(get_2_pos_of_int(sdSysData.CAR_INFO.STATUS.RPM.val_rpm() / 100)), tmeFrame_Time);
  LARGE_TACH_10.set_text(CHARACTERS_6X5.number(get_1_pos_of_int(sdSysData.CAR_INFO.STATUS.RPM.val_rpm() / 100)), tmeFrame_Time);

  TACHOMETER_MIN_MAX.put_value(sdSysData.CAR_INFO.STATUS.RPM.val_rpm(), tmeFrame_Time);
  TACHOMETER_VALUE.set_text(to_string(sdSysData.CAR_INFO.STATUS.RPM.val_rpm()), tmeFrame_Time);
  TACHOMETER_MAX.set_text(to_string(TACHOMETER_MIN_MAX.max()), tmeFrame_Time);

  //-----------

  ACCELERATION.set_text(to_string_round_to_nth(abs(sdSysData.CAR_INFO.CALCULATED.acceleration()), 2));
  
  //-----------
  // Steering Wheel

  if (sdSysData.CAR_INFO.STATUS.STEERING.val_left_of_center() == true)
  {
    STEERING_WHEEL.update( -1 * (sdSysData.CAR_INFO.STATUS.STEERING.val_steering_wheel_angle()) + 180, tmeFrame_Time);
  }
  else 
  {
    STEERING_WHEEL.update(sdSysData.CAR_INFO.STATUS.STEERING.val_steering_wheel_angle() + 180, tmeFrame_Time);
  }
  
  STEERING_WHEEL_ANGLE.set_text(sdSysData.CAR_INFO.STATUS.STEERING.left_of_center() + " " + 
                                  sdSysData.CAR_INFO.STATUS.STEERING.turning_direction() + " " + 
                                  sdSysData.CAR_INFO.STATUS.STEERING.steering_wheel_angle());

  //-----------
  // Tachometer
  TACHOMETER.update((float)sdSysData.CAR_INFO.STATUS.RPM.val_rpm() / 100, tmeFrame_Time);
  TACHOMETER2.update((float)sdSysData.CAR_INFO.STATUS.RPM.val_rpm() / 100, tmeFrame_Time);

  //-----------
  // Torque
  TORQUE.update(((float)sdSysData.CAR_INFO.STATUS.POWER.val_load()) / 3, tmeFrame_Time);
  
  TORQUE_VAL.set_text(to_string(sdSysData.CAR_INFO.STATUS.POWER.val_load() / 3), tmeFrame_Time);
  TORQUE_MIN.set_text(to_string(TORQUE.MIN_MAX_HISTORY.min()), tmeFrame_Time);
  TORQUE_MAX.set_text(to_string(TORQUE.MIN_MAX_HISTORY.max()), tmeFrame_Time);

  //-----------
  // Speed
  SPEEDO.update(sdSysData.CAR_INFO.STATUS.SPEED.SPEED_TRANS.val_mph(), tmeFrame_Time);

  if (SPEEDO.jecto_ready() == true)
  {
    SPEEDO_WATERFALL.update_via_slice(SPEEDO.jecto(), tmeFrame_Time);
  }

  //-----------
  // Super Temp
  SUPER_TEMP.update(sdSysData.CAR_INFO.CALCULATED.s_temp(), tmeFrame_Time);

  // Handle slices in the waterfall.
  if (SUPER_TEMP.jecto_ready() == true)
  {
    SUPER_TEMP_WATERFALL.update_via_slice(SUPER_TEMP.jecto(), tmeFrame_Time);
  }

  //-----------
  // Col 2

  // Temperature   0, 32, 38, 43, 
  ST_1_0.set_text("  " + sdSysData.CAR_INFO.STATUS.TEMPS.AMBIANT_AIR_46.c(), tmeFrame_Time);
  ST_1_0.set_color(color_range_reverse(sdSysData.CAR_INFO.STATUS.TEMPS.AMBIANT_AIR_46.val_c(), 0, 32, 38, 43, 48), COLOR_BLACK);

  ST_1_1.set_text("  " + sdSysData.CAR_INFO.STATUS.TEMPS.AIR_INTAKE_0f.c(), tmeFrame_Time);
  int air_intake_ambiant_diff = sdSysData.CAR_INFO.STATUS.TEMPS.AIR_INTAKE_0f.val_c() - sdSysData.CAR_INFO.STATUS.TEMPS.AMBIANT_AIR_46.val_c();
  ST_1_1.set_color(color_range_reverse(air_intake_ambiant_diff, 0, 10, 15, 20, 25), COLOR_BLACK);

  // Coolant - 60, 90, 110, 130, 140 
  ST_1_2.set_text("  " + sdSysData.CAR_INFO.STATUS.TEMPS.COOLANT_05.c(), tmeFrame_Time);
  ST_1_2.set_color(color_range_reverse(sdSysData.CAR_INFO.STATUS.TEMPS.COOLANT_05.val_c(), 60, 90, 110, 130, 140), COLOR_BLACK);

  // Coolant - 60, 90, 110, 130, 140 
  ST_1_3.set_text("  " + sdSysData.CAR_INFO.STATUS.TEMPS.CATALYST_3C.c(), tmeFrame_Time);
  ST_1_3.set_color(color_range_reverse(sdSysData.CAR_INFO.STATUS.TEMPS.CATALYST_3C.val_c(), 150, 600, 800, 1200, 1400), COLOR_BLACK);

  //ST_1_4.set_text(" " + sdSysData.CAR_INFO.STATUS.ELECTRICAL.CONTROL_UNIT_42.v(), tmeFrame_Time);

  //ST_1_5.set_text(sdSysData.CAR_INFO.STATUS.FUEL.FUEL_RAIL_PRESSURE.kPa());

  //ST_1_6.set_text(sdSysData.CAR_INFO.STATUS.FUEL.EVAP_SYSTEM_VAP_PRESSURE.Pa(), tmeFrame_Time);

  //ST_1_7.set_text(sdSysData.CAR_INFO.STATUS.
  
  // Voltage
  //ST_1_8.set_text(" " + sdSysData.CAR_INFO.STATUS.ELECTRICAL.CONTROL_UNIT_42.v(), tmeFrame_Time);

  //-----------
  // Col 4

  ST_4_0.set_text(sdSysData.CAR_INFO.STATUS.FUEL.FUEL_RAIL_PRESSURE.kPa());
  
  ST_4_1.set_text(sdSysData.CAR_INFO.STATUS.FUEL.EVAP_SYSTEM_VAP_PRESSURE.Pa(), tmeFrame_Time);
 
  //ST_4_2.set_text("  " + sdSysData.CAR_INFO.STATUS.TEMPS.COOLANT_05.c(), tmeFrame_Time);

  // Voltage
  ST_4_3.set_text(" " + sdSysData.CAR_INFO.STATUS.ELECTRICAL.CONTROL_UNIT_42.v(), tmeFrame_Time);
  ST_4_3.set_color(color_range(sdSysData.CAR_INFO.STATUS.ELECTRICAL.CONTROL_UNIT_42.val_v(), 0, 8, 10, 14, 16 ), COLOR_BLACK);

  //ST_4_4.set_text(" " + sdSysData.CAR_INFO.STATUS.ELECTRICAL.CONTROL_UNIT_42.v(), tmeFrame_Time);

  ST_4_5.set_text(sdSysData.CAR_INFO.STATUS.TEMPS.BARO_33.inHg(), tmeFrame_Time);

  //ST_4_6.set_text(sdSysData.CAR_INFO.STATUS.FUEL.EVAP_SYSTEM_VAP_PRESSURE.Pa(), tmeFrame_Time);

  //ST_4_7.set_text(sdSysData.CAR_INFO.STATUS.
  
  //ST_4_8.set_text(" " + sdSysData.CAR_INFO.STATUS.ELECTRICAL.CONTROL_UNIT_42.v(), tmeFrame_Time);
  
  //-----------

  /*
  DATA_SET_01.update(sdSysData.CAR_INFO.DATA.AD_130, sdSysData.CAR_INFO.DATA.AD_130.DATA[6], sdSysData.CAR_INFO.DATA.AD_130.DATA[7], "", tmeFrame_Time);

  DATA_SET_02.update(sdSysData.CAR_INFO.DATA.AD_D0, sdSysData.CAR_INFO.DATA.AD_D0.DATA[6], sdSysData.CAR_INFO.DATA.AD_D0.DATA[7], 
                      "[1] " + to_string_binary(sdSysData.CAR_INFO.DATA.AD_D0.DATA[1]), tmeFrame_Time);
  */

  //DATA_SET_03.PROP.DESCRIPTION = "07 E8";
  //DATA_SET_03.update(sdSysData.CAR_INFO.DATA.AD_7E8, sdSysData.CAR_INFO.DATA.AD_7E8.DATA[3], sdSysData.CAR_INFO.DATA.AD_7E8.DATA[4], sdSysData.CAR_INFO.requested_pid(), tmeFrame_Time);

  //DATA_SET_04.PROP.DESCRIPTION = "07 E9";
  //DATA_SET_04.update(sdSysData.CAR_INFO.DATA.AD_7E9, sdSysData.CAR_INFO.DATA.AD_7E9.DATA[3], sdSysData.CAR_INFO.DATA.AD_7E9.DATA[4], sdSysData.CAR_INFO.requested_pid(), tmeFrame_Time);

  /*
  DATA_SET_05.update(sdSysData.CAR_INFO.DATA.AD_7E8, sdSysData.CAR_INFO.DATA.AD_7E8.DATA[5], sdSysData.CAR_INFO.DATA.AD_7E8.DATA[6], "", tmeFrame_Time);

  DATA_SET_06.update(sdSysData.CAR_INFO.DATA.AD_7E9, sdSysData.CAR_INFO.DATA.AD_7E9.DATA[5], sdSysData.CAR_INFO.DATA.AD_7E9.DATA[6], "", tmeFrame_Time);
  */

  //DATA_SET_07.PROP.DESCRIPTION = "07 EA";
  //DATA_SET_07.update(sdSysData.CAR_INFO.DATA.AD_7EA, sdSysData.CAR_INFO.DATA.AD_7EA.DATA[3], sdSysData.CAR_INFO.DATA.AD_7EA.DATA[4], sdSysData.CAR_INFO.requested_pid(), tmeFrame_Time);

  //DATA_SET_08.PROP.DESCRIPTION = "07 EB";
  //DATA_SET_08.update(sdSysData.CAR_INFO.DATA.AD_7EB, sdSysData.CAR_INFO.DATA.AD_7EB.DATA[3], sdSysData.CAR_INFO.DATA.AD_7EB.DATA[4], sdSysData.CAR_INFO.requested_pid(), tmeFrame_Time);

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
      SDATA.D_LF_TTL.draw(sdSysData);
      SDATA.D_RF_TTL.draw(sdSysData);
      SDATA.D_LB_TTL.draw(sdSysData);
      SDATA.D_RB_TTL.draw(sdSysData);
      
      //SDATA.THING1.draw(sdSysData);
    }
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("Auto Data 3", ImVec2(175, disp_y), true, sdSysData.SCREEN_DEFAULTS.flags_c);
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
    }
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("Gear Display", ImVec2(disp_x, disp_y), true, sdSysData.SCREEN_DEFAULTS.flags_c);
    {
      SDATA.L_GEAR.draw(sdSysData);
      SDATA.L_TACH.draw(sdSysData);
    }
    ImGui::EndChild();

    ImGui::BeginChild("Data 1", ImVec2(ImGui::GetContentRegionAvail().x * 2 / 3, ImGui::GetContentRegionAvail().y), true, sdSysData.SCREEN_DEFAULTS.flags_c);
    {  
      SDATA.P_SPEED.draw(sdSysData);
    }
    ImGui::EndChild();

    ImGui::SameLine();

    ImGui::BeginChild("Data 3", ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y), true, sdSysData.SCREEN_DEFAULTS.flags_c);
    {
      SDATA.G_SPEED.draw(sdSysData);

      SDATA.G_ACCELERATION.draw(sdSysData);
      SDATA.G_RPM.draw(sdSysData);
      SDATA.G_TORQUE.draw(sdSysData);
    }
    ImGui::EndChild();
  //}
  //ImGui::End();
}

// ---------------------------------------------------------------------------------------
#endif