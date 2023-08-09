// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: widgets_drawing.cpp
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2859 Core Dynamics
// ***************************************************************************************

#ifndef WIDGETS_DRAWING_CPP
#define WIDGETS_DRAWING_CPP

#include "widgets_drawing.h"

// ---------------------------------------------------------------------------------------

/*
void THING::draw(system_data &sdSysData)
{
  
  ImDrawList* draw_list = ImGui::GetWindowDrawList();

  static ImVec4 colf = ImVec4(1.0f, 1.0f, 0.4f, 1.0f);
  const ImU32 col = ImColor(colf);

  const ImVec2 p = ImGui::GetCursorScreenPos();
  
  float x = p.x + 4.0f;
  float y = p.y + 4.0f;
  const float spacing = 10.0f;
  static float sz = 10.0f;

  //draw_list->AddNgon(ImVec2(x + sz*0.5f, y + sz*0.5f), sz*0.5f, col, 4, 2);                 
  //x += sz + spacing;
  
  draw_list->AddNgonFilled(ImVec2(x + sz * 0.5f, y + sz * 0.5f), sz*0.5f, col, 4);               
  x += sz + spacing;  // N-gon

  ImGui::Dummy(ImVec2((sz + spacing) * 10.2f, (sz + spacing) * 3.0f));

}
*/

// ---------------------------------------------------------------------------------------

void simple_wrap_text_box()
{
  ImDrawList* draw_list = ImGui::GetWindowDrawList();
  draw_list->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(255, 255, 0, 128));
}

// ---------------------------------------------------------------------------------------


void ONE_CHAR_LINE_GRAPH::update_value(system_data &sdSysData, float Value, float Max_Value)
{
  if (Max_Value > 0)
  {
    VALUE = Value / Max_Value;
  }
  else
  {
    VALUE = -1;
  }
}

void ONE_CHAR_LINE_GRAPH::draw(system_data &sdSysData)
{
  ImDrawList* draw_list = ImGui::GetWindowDrawList();

  //ImVec4 colf = PROPS.COLOR.TEXT;
  ImVec4 colf = ImVec4(0.8f, 0.8f, 0.8f, 1.0f);

  ImU32 col = ImColor(colf);
  //ImU32 col = ImColor(PROPS.COLOR.TEXT);

  ImVec2 position = ImGui::GetCursorScreenPos();
  
  float x = position.x + 4.0f;
  float y = position.y + 4.0f;
  const float spacing = 10.0f;
  static float size = 12.0f;


  if (VALUE > 0.0f && VALUE < 0.6f)
  {
    draw_list->AddNgonFilled(ImVec2(x + size * 0.25f, y + size * 0.25f), size*0.25f, col, 4.0f);
  }

  if (VALUE >= 0.1f && VALUE < 0.7f)
  {
    draw_list->AddNgonFilled(ImVec2(x + size * 0.75f, y + size * 0.75f), size*0.25f, col, 4.0f);
  }

  if (VALUE >= 0.2f && VALUE < 0.8f)
  {
    draw_list->AddNgonFilled(ImVec2(x + size * 0.75f, y + size * 0.25f), size*0.25f, col, 4.0f);
  }

  if (VALUE >= 0.3f && VALUE < 0.9f)
  {
    draw_list->AddNgonFilled(ImVec2(x + size * 0.25f, y + size * 0.75f), size*0.25f, col, 4.0f);
  }

  if (VALUE == 0 || (VALUE >= 0.5f && VALUE < 1.0f))
  {
    draw_list->AddNgon(ImVec2(x + size * 0.5f, y + size * 0.5f), size * 0.5f, col, 8, 1.5f);
  }

  if (VALUE >= 1.0f)
  {
    draw_list->AddNgonFilled(ImVec2(x + size * 0.5f, y + size * 0.5f), size * 0.5f, col, 8);
  }
  
  ImGui::Dummy(ImVec2((spacing), (spacing)));

  position.x = position.x + spacing;
  ImGui::SetCursorScreenPos(position);
}

// ---------------------------------------------------------------------------------------

#endif