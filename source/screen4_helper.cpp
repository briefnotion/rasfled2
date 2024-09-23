// ***************************************************************************************
// *
// *    Core       | Everything within this document is proprietary to Core Dynamics.
// *    Dynamics   | Any unauthorized duplication will be subject to prosecution.
// *
// *    Department : (R+D)^2                        Name: screen4_helper.cpp
// *       Sub Dept: Programming
// *    Location ID: 856-45B
// *                                                      (c) 2856 - 2859 Core Dynamics
// ***************************************************************************************

#ifndef SCREEN4_HELPER_CPP
#define SCREEN4_HELPER_CPP

#include "screen4_helper.h"

// ---------------------------------------------------------------------------------------

ImVec2 operator+(ImVec2 V1, ImVec2 V2)
{
  return ImVec2(V1.x + V2.x, V1.y + V2.y);
}

ImVec2 operator-(ImVec2 V1, ImVec2 V2)
{
  return ImVec2(V1.x - V2.x, V1.y - V2.y);
}

ImVec2 operator*(ImVec2 V1, ImVec2 V2)
{
  return ImVec2(V1.x * V2.x, V1.y * V2.y);
}

ImVec2 operator*(ImVec2 V1, float Number)
{
  return ImVec2(V1.x * Number, V1.y * Number);
}

bool vector4_is_same(ImVec4 V1, ImVec4 V2)
{
  return (V1.x == V2.x) && (V1.y == V2.y) && (V1.z == V2.z) && (V1.w == V2.w);
}
// ---------------------------------------------------------------------------------------

CRGB convert_imvec4_to_crgb(ImVec4 Vec4)
{
  return CRGB(static_cast<unsigned char>(Vec4.x * 255.0f), static_cast<unsigned char>(Vec4.y * 255.0f), static_cast<unsigned char>(Vec4.z * 255.0f));
}

// ---------------------------------------------------------------------------------------
void NEO_COLOR::set_frame_time(unsigned long Time)
{
  CURRENT_TIME = (float)Time;
  
  if (!vector4_is_same(CURRENT_COLOR, NEW_COLOR))
  {
    if (CURRENT_TIME != CURRENT_TIME_PREV)
    {
      CURRENT_TIME_PREV = CURRENT_TIME;
      NEEDS_CALC = true;
    }
  }
}

ImColor NEO_COLOR::calc_transition()
{
  if (NEEDS_CALC == true)
  {
    float power = (CURRENT_TIME - START_TIME) / PROPS.DURATION;

    if (power >= 1.0f)
    {
      // Power, is at or above 1 at this pass. Meaning transition is complete.
      
      CURRENT_COLOR = NEW_COLOR;

      NEEDS_CALC = false; 

      return ImColor(CURRENT_COLOR);
    }
    else
    {
      float r = (power * NEW_COLOR.x) + ((1.0f - power) * PREV_COLOR.x);
      float g = (power * NEW_COLOR.y) + ((1.0f - power) * PREV_COLOR.y);
      float b = (power * NEW_COLOR.z) + ((1.0f - power) * PREV_COLOR.z);
      float w = (power * NEW_COLOR.w) + ((1.0f - power) * PREV_COLOR.w);

      CURRENT_COLOR = ImVec4(r, g, b, w);
      
      NEEDS_CALC = false;

      return ImColor(CURRENT_COLOR);
    }
  }
  else
  {
    return ImColor(CURRENT_COLOR);
  }
}

bool NEO_COLOR::reset_to_new_color(unsigned long Time, ImVec4 Color)
{
  if (!vector4_is_same(Color, NEW_COLOR))
  {
    set_frame_time(Time);
    
    calc_transition();

    START_TIME = CURRENT_TIME;

    PREV_COLOR = CURRENT_COLOR;
    NEW_COLOR = Color;

    return true;
  }
  else
  {
    if (!vector4_is_same(CURRENT_COLOR, NEW_COLOR))
    {
      set_frame_time(Time);

      return true;
    }
    else
    {
      return false;
    }
  }
}

bool NEO_COLOR::is_changing()
{
  return !vector4_is_same(CURRENT_COLOR, NEW_COLOR);
}

void NEO_COLOR::set_current_frame_time(unsigned long Time)
{
  set_frame_time(Time);
}

ImColor NEO_COLOR::color(unsigned long Time, ImColor Color)
{
  if (reset_to_new_color(Time, Color.Value))
  {
    return calc_transition();
  }
  else
  {
    return ImColor(CURRENT_COLOR);
  }
}

ImColor NEO_COLOR::color(unsigned long Time)
{
  // Do not check for color changes.  Just handle the trasition and return 
  //  either current color or previously requested color.

  set_frame_time(Time);
  return calc_transition();
}

ImColor NEO_COLOR::color()
{
  // Do not check for color changes.  Just handle the trasition and return 
  //  either current color or previously requested color.
  return calc_transition();
}

void NEO_COLOR::set_color(unsigned long Time, ImColor Color)
{
  // Set a color trasition change but do not return anything.
  set_current_frame_time(Time);
  reset_to_new_color(Time, Color.Value);
}

// ---------------------------------------------------------------------------------------
void NEO_COLOR_CRGB::set_frame_time(unsigned long Time)
{
  CURRENT_TIME = (float)Time;
  
  if (!vector4_is_same(CURRENT_COLOR, NEW_COLOR))
  {
    if (CURRENT_TIME != CURRENT_TIME_PREV)
    {
      CURRENT_TIME_PREV = CURRENT_TIME;
      NEEDS_CALC = true;
    }
  }
}

CRGB NEO_COLOR_CRGB::calc_transition()
{
  if (NEEDS_CALC == true)
  {
    float power = (CURRENT_TIME - START_TIME) / PROPS.DURATION;

    if (power >= 1.0f)
    {
      // Power, is at or above 1 at this pass. Meaning transition is complete.
      
      CURRENT_COLOR = NEW_COLOR;

      NEEDS_CALC = false; 

      return CRGB(CURRENT_COLOR.x, CURRENT_COLOR.y, CURRENT_COLOR.z);
    }
    else
    {
      float r = (power * NEW_COLOR.x) + ((1.0f - power) * PREV_COLOR.x);
      float g = (power * NEW_COLOR.y) + ((1.0f - power) * PREV_COLOR.y);
      float b = (power * NEW_COLOR.z) + ((1.0f - power) * PREV_COLOR.z);
      float w = (power * NEW_COLOR.w) + ((1.0f - power) * PREV_COLOR.w);

      CURRENT_COLOR = ImVec4(r, g, b, w);
      
      NEEDS_CALC = false;

      return CRGB(CURRENT_COLOR.x, CURRENT_COLOR.y, CURRENT_COLOR.z);
    }
  }
  else
  {
    return CRGB(CURRENT_COLOR.x, CURRENT_COLOR.y, CURRENT_COLOR.z);
  }
}

bool NEO_COLOR_CRGB::reset_to_new_color(unsigned long Time, ImVec4 Color)
{
  if (!vector4_is_same(Color, NEW_COLOR))
  {
    set_frame_time(Time);
    
    calc_transition();

    START_TIME = CURRENT_TIME;

    PREV_COLOR = CURRENT_COLOR;
    NEW_COLOR = Color;

    return true;
  }
  else
  {
    if (!vector4_is_same(CURRENT_COLOR, NEW_COLOR))
    {
      set_frame_time(Time);

      return true;
    }
    else
    {
      return false;
    }
  }
}

bool NEO_COLOR_CRGB::is_changing()
{
  return !vector4_is_same(CURRENT_COLOR, NEW_COLOR);
}

void NEO_COLOR_CRGB::set_current_frame_time(unsigned long Time)
{
  set_frame_time(Time);
}

CRGB NEO_COLOR_CRGB::color(unsigned long Time, CRGB Color)
{
  if (reset_to_new_color(Time, ImVec4(Color.r, Color.g, Color.b, 255.0f)))
  {
    return calc_transition();
  }
  else
  {
    return CRGB(CURRENT_COLOR.x, CURRENT_COLOR.y, CURRENT_COLOR.z);
  }
}

CRGB NEO_COLOR_CRGB::color(unsigned long Time)
{
  // Do not check for color changes.  Just handle the trasition and return 
  //  either current color or previously requested color.

  set_frame_time(Time);
  return calc_transition();
}

CRGB NEO_COLOR_CRGB::color()
{
  // Do not check for color changes.  Just handle the trasition and return 
  //  either current color or previously requested color.
  return calc_transition();
}

void NEO_COLOR_CRGB::set_color(unsigned long Time, CRGB Color)
{
  // Set a color trasition change but do not return anything.
  set_current_frame_time(Time);
  reset_to_new_color(Time, ImVec4(Color.r, Color.g, Color.b, 255.0f));
}

// ---------------------------------------------------------------------------------------

void COLOR_COMBO::set_rgb(float R, float G, float B, float A, float Intensity)
{
  TEXT        = ImColor((R *0.8f + 0.5f) * Intensity, (G *0.8f + 0.5f) * Intensity, (B *0.8f + 0.5f) * Intensity, A);
  BACKGROUND  = ImColor(R * Intensity, G * Intensity, B * Intensity, A * Intensity *0.2f);
  DIM         = ImColor(R * Intensity, G * Intensity, B * Intensity, A * Intensity *0.5f);
  STANDARD    = ImColor(R * Intensity, G * Intensity, B * Intensity, A *0.7f);
  STANDARD_V  = ImColor(R * Intensity, G * Intensity, B * Intensity, A *0.7f);
  HOVERED     = ImColor(R * Intensity, G * Intensity, B * Intensity, A *0.8f);
  ACTIVE      = ImColor(R * Intensity, G * Intensity, B * Intensity, A *0.9f);
  SIMPLE_RGB  = convert_imvec4_to_crgb(ImVec4(R, G, B, A));
}

void COLOR_COMBO::set_rgb_v(float R, float G, float B, float A, float Intensity)
{
  TEXT        = ImColor((1.0f *0.8f + 0.5f) * Intensity, (1.0f *0.8f + 0.5f) * Intensity, (1.0f *0.8f + 0.5f) * Intensity, 1.0f);
  BACKGROUND  = ImColor(R * Intensity, G * Intensity, B * Intensity, A * Intensity *0.2f);
  DIM         = ImColor(R * Intensity, G * Intensity, B * Intensity, A * Intensity *0.5f);
  STANDARD    = ImColor(R * Intensity, G * Intensity, B * Intensity, A *0.7f);
  STANDARD_V  = ImColor((1.0f *0.8f + 0.5f) * Intensity, (1.0f *0.8f + 0.5f) * Intensity, (1.0f *0.8f + 0.5f) * Intensity, 1.0f);
  HOVERED     = ImColor(R * Intensity, G * Intensity, B * Intensity, A *0.8f);
  ACTIVE      = ImColor(R * Intensity, G * Intensity, B * Intensity, A *0.9f);
  SIMPLE_RGB  = convert_imvec4_to_crgb(ImVec4(R, G, B, A));
}

void COLOR_COMBO::set_rgb_black()
{
  TEXT        = ImColor(0.0f, 0.0f, 0.0f, 1.0f);
  BACKGROUND  = ImColor(0.0f, 0.0f, 0.0f, 1.0f);
  DIM         = ImColor(0.0f, 0.0f, 0.0f, 1.0f);
  STANDARD    = ImColor(0.0f, 0.0f, 0.0f, 1.0f);
  STANDARD_V  = ImColor(0.0f, 0.0f, 0.0f, 1.0f);
  HOVERED     = ImColor(0.0f, 0.0f, 0.0f, 1.0f);
  ACTIVE      = ImColor(0.0f, 0.0f, 0.0f, 1.0f);
  SIMPLE_RGB  = CRGB(0, 0, 0);
}

// ---------------------------------------------------------------------------------------

void NEO_COLOR_COMBO::set_neo_rgb(unsigned long Time, COLOR_COMBO &Color_Combo)
{
  TEXT.set_color(Time, Color_Combo.TEXT);
  BACKGROUND.set_color(Time, Color_Combo.BACKGROUND);
  DIM.set_color(Time, Color_Combo.DIM);
  STANDARD.set_color(Time, Color_Combo.STANDARD);
  STANDARD_V.set_color(Time, Color_Combo.STANDARD_V);
  HOVERED.set_color(Time, Color_Combo.HOVERED);
  ACTIVE.set_color(Time, Color_Combo.ACTIVE);
  SIMPLE_RGB.set_color(Time, Color_Combo.SIMPLE_RGB);
}

// ---------------------------------------------------------------------------------------

void COLOR_COMBOS::set_neo_colors_with_color_change(unsigned long Time)
{
  if (void_color)
  {
    if (void_color_value == -1)
    {
      COLOR_COMBINATIONS_NEO[0].set_neo_rgb(Time, COLOR_COMBINATIONS_V[0]);
      COLOR_COMBINATIONS_NEO[1].set_neo_rgb(Time, CUSTOM_COLOR_COMBO);
      COLOR_COMBINATIONS_NEO[2].set_neo_rgb(Time, CUSTOM_COLOR_COMBO);
      COLOR_COMBINATIONS_NEO[3].set_neo_rgb(Time, CUSTOM_COLOR_COMBO);
      COLOR_COMBINATIONS_NEO[4].set_neo_rgb(Time, CUSTOM_COLOR_COMBO);
      COLOR_COMBINATIONS_NEO[5].set_neo_rgb(Time, CUSTOM_COLOR_COMBO);
      COLOR_COMBINATIONS_NEO[6].set_neo_rgb(Time, CUSTOM_COLOR_COMBO);
      COLOR_COMBINATIONS_NEO[7].set_neo_rgb(Time, CUSTOM_COLOR_COMBO);
      COLOR_COMBINATIONS_NEO[8].set_neo_rgb(Time, CUSTOM_COLOR_COMBO);
      COLOR_COMBINATIONS_NEO[9].set_neo_rgb(Time, CUSTOM_COLOR_COMBO);
      COLOR_COMBINATIONS_NEO[10].set_neo_rgb(Time, CUSTOM_COLOR_COMBO);
    }
    else
    {
      COLOR_COMBINATIONS_NEO[0].set_neo_rgb(Time, COLOR_COMBINATIONS_V[0]);
      COLOR_COMBINATIONS_NEO[1].set_neo_rgb(Time, COLOR_COMBINATIONS_V[void_color_value]);
      COLOR_COMBINATIONS_NEO[2].set_neo_rgb(Time, COLOR_COMBINATIONS_V[void_color_value]);
      COLOR_COMBINATIONS_NEO[3].set_neo_rgb(Time, COLOR_COMBINATIONS_V[void_color_value]);
      COLOR_COMBINATIONS_NEO[4].set_neo_rgb(Time, COLOR_COMBINATIONS_V[void_color_value]);
      COLOR_COMBINATIONS_NEO[5].set_neo_rgb(Time, COLOR_COMBINATIONS_V[void_color_value]);
      COLOR_COMBINATIONS_NEO[6].set_neo_rgb(Time, COLOR_COMBINATIONS_V[void_color_value]);
      COLOR_COMBINATIONS_NEO[7].set_neo_rgb(Time, COLOR_COMBINATIONS_V[void_color_value]);
      COLOR_COMBINATIONS_NEO[8].set_neo_rgb(Time, COLOR_COMBINATIONS_V[void_color_value]);
      COLOR_COMBINATIONS_NEO[9].set_neo_rgb(Time, COLOR_COMBINATIONS_V[void_color_value]);
      COLOR_COMBINATIONS_NEO[10].set_neo_rgb(Time, COLOR_COMBINATIONS_V[void_color_value]);
    }
  }
  else
  {
    COLOR_COMBINATIONS_NEO[0].set_neo_rgb(Time, COLOR_COMBINATIONS[0]);
    COLOR_COMBINATIONS_NEO[1].set_neo_rgb(Time, COLOR_COMBINATIONS[1]);
    COLOR_COMBINATIONS_NEO[2].set_neo_rgb(Time, COLOR_COMBINATIONS[2]);
    COLOR_COMBINATIONS_NEO[3].set_neo_rgb(Time, COLOR_COMBINATIONS[3]);
    COLOR_COMBINATIONS_NEO[4].set_neo_rgb(Time, COLOR_COMBINATIONS[4]);
    COLOR_COMBINATIONS_NEO[5].set_neo_rgb(Time, COLOR_COMBINATIONS[5]);
    COLOR_COMBINATIONS_NEO[6].set_neo_rgb(Time, COLOR_COMBINATIONS[6]);
    COLOR_COMBINATIONS_NEO[7].set_neo_rgb(Time, COLOR_COMBINATIONS[7]);
    COLOR_COMBINATIONS_NEO[8].set_neo_rgb(Time, COLOR_COMBINATIONS[8]);
    COLOR_COMBINATIONS_NEO[9].set_neo_rgb(Time, COLOR_COMBINATIONS[9]);
    COLOR_COMBINATIONS_NEO[10].set_neo_rgb(Time, COLOR_COMBINATIONS[10]);
  }
}

void COLOR_COMBOS::set_neo_duration(float Duration)
{
  for (int pos = 0; pos < (int)COLOR_COMBINATIONS_NEO.size(); pos++)
  {
    COLOR_COMBINATIONS_NEO[pos].TEXT.PROPS.DURATION = Duration;
    COLOR_COMBINATIONS_NEO[pos].BACKGROUND.PROPS.DURATION = Duration;
    COLOR_COMBINATIONS_NEO[pos].DIM.PROPS.DURATION = Duration;
    COLOR_COMBINATIONS_NEO[pos].STANDARD.PROPS.DURATION = Duration;
    COLOR_COMBINATIONS_NEO[pos].STANDARD_V.PROPS.DURATION = Duration;
    COLOR_COMBINATIONS_NEO[pos].HOVERED.PROPS.DURATION = Duration;
    COLOR_COMBINATIONS_NEO[pos].ACTIVE.PROPS.DURATION = Duration;
    COLOR_COMBINATIONS_NEO[pos].SIMPLE_RGB.PROPS.DURATION = Duration;
  }
}

void COLOR_COMBOS::set_frame_time(unsigned long Time)
{
  for (int pos = 0; pos < (int)COLOR_COMBINATIONS_NEO.size(); pos++)
  {
    COLOR_COMBINATIONS_NEO[pos].TEXT.set_current_frame_time(Time);
    COLOR_COMBINATIONS_NEO[pos].BACKGROUND.set_current_frame_time(Time);
    COLOR_COMBINATIONS_NEO[pos].DIM.set_current_frame_time(Time);
    COLOR_COMBINATIONS_NEO[pos].STANDARD.set_current_frame_time(Time);
    COLOR_COMBINATIONS_NEO[pos].STANDARD_V.set_current_frame_time(Time);
    COLOR_COMBINATIONS_NEO[pos].HOVERED.set_current_frame_time(Time);
    COLOR_COMBINATIONS_NEO[pos].ACTIVE.set_current_frame_time(Time);
    COLOR_COMBINATIONS_NEO[pos].SIMPLE_RGB.set_current_frame_time(Time);
  }
}

void COLOR_COMBOS::init(unsigned long Time, float Intensity)
{
  if (ALREADY_INITIALIZED == false)
  {
    ALREADY_INITIALIZED = true; //prevent another routine from running again

    COLOR_COMBO tmp_color_combo; 
    NEO_COLOR_COMBO tmp_neo_color_combo;

    // Base Colors
    COLOR_COMBINATIONS.push_back(tmp_color_combo);  // Black
    COLOR_COMBINATIONS.push_back(tmp_color_combo);  // White
    COLOR_COMBINATIONS.push_back(tmp_color_combo);  // Grey
    COLOR_COMBINATIONS.push_back(tmp_color_combo);  // Red
    COLOR_COMBINATIONS.push_back(tmp_color_combo);  // Orange
    COLOR_COMBINATIONS.push_back(tmp_color_combo);  // Yellow
    COLOR_COMBINATIONS.push_back(tmp_color_combo);  // Green
    COLOR_COMBINATIONS.push_back(tmp_color_combo);  // Cyan
    COLOR_COMBINATIONS.push_back(tmp_color_combo);  // Blue
    COLOR_COMBINATIONS.push_back(tmp_color_combo);  // Purple
    COLOR_COMBINATIONS.push_back(tmp_color_combo);  // Pink
    COLOR_COMBINATIONS.push_back(tmp_color_combo);  // Monochrome

    // Base Colors V
    COLOR_COMBINATIONS_V.push_back(tmp_color_combo);  // Black
    COLOR_COMBINATIONS_V.push_back(tmp_color_combo);  // White
    COLOR_COMBINATIONS_V.push_back(tmp_color_combo);  // Grey
    COLOR_COMBINATIONS_V.push_back(tmp_color_combo);  // Red
    COLOR_COMBINATIONS_V.push_back(tmp_color_combo);  // Orange
    COLOR_COMBINATIONS_V.push_back(tmp_color_combo);  // Yellow
    COLOR_COMBINATIONS_V.push_back(tmp_color_combo);  // Green
    COLOR_COMBINATIONS_V.push_back(tmp_color_combo);  // Cyan
    COLOR_COMBINATIONS_V.push_back(tmp_color_combo);  // Blue
    COLOR_COMBINATIONS_V.push_back(tmp_color_combo);  // Purple
    COLOR_COMBINATIONS_V.push_back(tmp_color_combo);  // Pink
    COLOR_COMBINATIONS_V.push_back(tmp_color_combo);  // Monochrome

    // Neo Colors
    COLOR_COMBINATIONS_NEO.push_back(tmp_neo_color_combo);  // Black
    COLOR_COMBINATIONS_NEO.push_back(tmp_neo_color_combo);  // White
    COLOR_COMBINATIONS_NEO.push_back(tmp_neo_color_combo);  // Grey
    COLOR_COMBINATIONS_NEO.push_back(tmp_neo_color_combo);  // Red
    COLOR_COMBINATIONS_NEO.push_back(tmp_neo_color_combo);  // Orange
    COLOR_COMBINATIONS_NEO.push_back(tmp_neo_color_combo);  // Yellow
    COLOR_COMBINATIONS_NEO.push_back(tmp_neo_color_combo);  // Green
    COLOR_COMBINATIONS_NEO.push_back(tmp_neo_color_combo);  // Cyan
    COLOR_COMBINATIONS_NEO.push_back(tmp_neo_color_combo);  // Blue
    COLOR_COMBINATIONS_NEO.push_back(tmp_neo_color_combo);  // Purple
    COLOR_COMBINATIONS_NEO.push_back(tmp_neo_color_combo);  // Pink
    COLOR_COMBINATIONS_NEO.push_back(tmp_neo_color_combo);  // Monochrome

    // Assign Colors
    COLOR_COMBINATIONS[0].set_rgb_black();  // Black
    COLOR_COMBINATIONS[1].set_rgb(1.0f, 1.0f, 1.0f, 1.0f, Intensity);  // White
    COLOR_COMBINATIONS[2].set_rgb(0.4f, 0.4f, 0.4f, 1.0f, Intensity);  // Grey
    COLOR_COMBINATIONS[3].set_rgb(1.0f, 0.0f, 0.0f, 1.0f, Intensity);  // Red
    COLOR_COMBINATIONS[4].set_rgb(1.0f, 0.75f, 0.0f, 1.0f, Intensity);  // Orange
    COLOR_COMBINATIONS[5].set_rgb(1.0f, 1.0f, 0.0f, 1.0f, Intensity);  // Yellow
    COLOR_COMBINATIONS[6].set_rgb(0.0f, 1.0f, 0.0f, 1.0f, Intensity);  // Green
    COLOR_COMBINATIONS[7].set_rgb(0.0f, 1.0f, 1.0f, 1.0f, Intensity);  // Cyan
    COLOR_COMBINATIONS[8].set_rgb(0.0f, 0.0f, 1.0f, 1.0f, Intensity);  // Blue
    COLOR_COMBINATIONS[9].set_rgb(1.0f, 0.0f, 1.0f, 1.0f, Intensity);  // Purple
    COLOR_COMBINATIONS[10].set_rgb(1.0f, 0.5f, 1.0f, 1.0f, Intensity);  // Pink
    COLOR_COMBINATIONS[11].set_rgb(0.0f, 0.0f, 0.0f, 1.0f, Intensity);  // Monochrome

    COLOR_COMBINATIONS_V[0].set_rgb_black();                                // Black
    COLOR_COMBINATIONS_V[1].set_rgb_v(1.0f, 1.0f, 1.0f, 1.0f, Intensity);  // White
    COLOR_COMBINATIONS_V[2].set_rgb_v(0.4f, 0.4f, 0.4f, 1.0f, Intensity);  // Grey
    COLOR_COMBINATIONS_V[3].set_rgb_v(1.0f, 0.0f, 0.0f, 1.0f, Intensity);  // Red
    COLOR_COMBINATIONS_V[4].set_rgb_v(1.0f, 0.75f, 0.0f, 1.0f, Intensity);  // Orange
    COLOR_COMBINATIONS_V[5].set_rgb_v(1.0f, 1.0f, 0.0f, 1.0f, Intensity);  // Yellow
    COLOR_COMBINATIONS_V[6].set_rgb_v(0.0f, 1.0f, 0.0f, 1.0f, Intensity);  // Green
    COLOR_COMBINATIONS_V[7].set_rgb_v(0.0f, 1.0f, 1.0f, 1.0f, Intensity);  // Cyan
    COLOR_COMBINATIONS_V[8].set_rgb_v(0.0f, 0.0f, 1.0f, 1.0f, Intensity);  // Blue
    COLOR_COMBINATIONS_V[9].set_rgb_v(1.0f, 0.0f, 1.0f, 1.0f, Intensity);  // Purple
    COLOR_COMBINATIONS_V[10].set_rgb_v(1.0f, 0.5f, 1.0f, 1.0f, Intensity);  // Pink
    COLOR_COMBINATIONS_V[11].set_rgb_v(0.0f, 0.0f, 0.0f, 1.0f, Intensity);  // Monochrome

    set_neo_colors_with_color_change(Time);
  }
}

COLOR_COMBO COLOR_COMBOS::pure_color(int Color)
{
  return COLOR_COMBINATIONS[Color];
}

COLOR_COMBO COLOR_COMBOS::color(int Color)
{

  return COLOR_COMBINATIONS[Color];
}

ImColor COLOR_COMBOS::neo_color_TEXT(int Color)
{
  if (Color == -1)
  {
    return COLOR_COMBINATIONS_NEO[void_color_prev_value].TEXT.color();
  }
  else
  {
    return COLOR_COMBINATIONS_NEO[Color].TEXT.color();
  }
}

ImColor COLOR_COMBOS::neo_color_BACKGROUND(int Color)
{
  if (Color == -1)
  {
    return COLOR_COMBINATIONS_NEO[void_color_prev_value].BACKGROUND.color();
  }
  else
  {
    return COLOR_COMBINATIONS_NEO[Color].BACKGROUND.color();
  }
}

ImColor COLOR_COMBOS::neo_color_DIM(int Color)
{
  if (Color == -1)
  {
    return COLOR_COMBINATIONS_NEO[void_color_prev_value].DIM.color();
  }
  else
  {
    return COLOR_COMBINATIONS_NEO[Color].DIM.color();
  }
}

ImColor COLOR_COMBOS::neo_color_STANDARD(int Color)
{
  if (Color == -1)
  {
    return COLOR_COMBINATIONS_NEO[void_color_prev_value].STANDARD.color();
  }
  else
  {
    return COLOR_COMBINATIONS_NEO[Color].STANDARD.color();
  }
}

ImColor COLOR_COMBOS::neo_color_STANDARD_V(int Color)
{
  if (Color == -1)
  {
    return COLOR_COMBINATIONS_NEO[void_color_prev_value].STANDARD_V.color();
  }
  else
  {
    return COLOR_COMBINATIONS_NEO[Color].STANDARD_V.color();
  }
}

ImColor COLOR_COMBOS::neo_color_HOVERED(int Color)
{
  if (Color == -1)
  {
    return COLOR_COMBINATIONS_NEO[void_color_prev_value].HOVERED.color();
  }
  else
  {
    return COLOR_COMBINATIONS_NEO[Color].HOVERED.color();
  }
}

ImColor COLOR_COMBOS::neo_color_ACTIVE(int Color)
{
  if (Color == -1)
  {
    return COLOR_COMBINATIONS_NEO[void_color_prev_value].ACTIVE.color();
  }
  else
  {
    return COLOR_COMBINATIONS_NEO[Color].ACTIVE.color();
  }
}

CRGB COLOR_COMBOS::neo_color_SIMPLE_RGB(int Color)
{
  if (Color == -1)
  {
    return COLOR_COMBINATIONS_NEO[void_color_prev_value].SIMPLE_RGB.color();
  }
  else
  {
    return COLOR_COMBINATIONS_NEO[Color].SIMPLE_RGB.color();
  }
}

bool COLOR_COMBOS::neo_color_is_changing()
{
  return COLOR_COMBINATIONS_NEO[RAS_WHITE].STANDARD.is_changing();
}

void COLOR_COMBOS::toggle_void_color(unsigned long Time)
{
  void_color = !void_color;
  set_neo_colors_with_color_change(Time);
}

void COLOR_COMBOS::void_color_set(unsigned long Time, int Color)
{
  void_color_value = Color;
  set_neo_colors_with_color_change(Time);
}

void COLOR_COMBOS::void_color_set(unsigned long Time, float R, float G, float B, float Intensity)
{
  CUSTOM_COLOR_COMBO.set_rgb_v(R, G, B, 1.0f, Intensity);

  // for now, avoid invalid value, monocrome and black
  if (void_color_value != -1 && void_color_value != 0 && void_color_value != 11)
  {
    void_color_prev_value = void_color_value;
  }

  void_color_value = -1;

  set_neo_colors_with_color_change(Time);
}

int COLOR_COMBOS::void_colr()
{
  return void_color_value;
}

//int COLOR_COMBOS::custom_colr()
//{
//  return -1;
//}

int COLOR_COMBOS::void_colr_prev()
{
  return void_color_prev_value;
}

// ---------------------------------------------------------------------------------------

void CONSOLE_COMMUNICATION::carrage_return_set()
{
  CARRAGE_RETURN = true;
  CHANGED = true;
}

bool CONSOLE_COMMUNICATION::carrage_return_get()
{
  if (CARRAGE_RETURN == true)
  {
    CARRAGE_RETURN = false;
    return true;
  }
  else
  {
    return false;
  }
}

void CONSOLE_COMMUNICATION::command_text_set(string Text)
{
  if (COMMAND_TEXT != Text)
  {
    COMMAND_TEXT = Text;
    COMMAND_TEXT_CLEAR = false;
    CHANGED = true;
  }
}

bool CONSOLE_COMMUNICATION::command_text_changed()
{
  if (CHANGED == true)
  {
    CHANGED = false;
    return true;
  }
  else
  {
    return false;
  }
}

void CONSOLE_COMMUNICATION::command_pending_set(string Text)
{
  PENDING_CONFIRM_COMMAND = Text;
  PENDING_CONFIRM_WITH_CR = false;
}

void CONSOLE_COMMUNICATION::command_pending_set_cr(string Text)
{
  PENDING_CONFIRM_COMMAND = Text;
  PENDING_CONFIRM_WITH_CR = true;
}

void CONSOLE_COMMUNICATION::command_pending_send()
{
  command_text_set(PENDING_CONFIRM_COMMAND);
  if (PENDING_CONFIRM_WITH_CR)
  {
    carrage_return_set();
  }
}

string CONSOLE_COMMUNICATION::command_text_get()
{
  return COMMAND_TEXT;
}

void CONSOLE_COMMUNICATION::command_text_clear()
{
  COMMAND_TEXT = "";
  CARRAGE_RETURN = false;
  CHANGED = true;
  COMMAND_TEXT_CLEAR = true;
}

bool CONSOLE_COMMUNICATION::command_text_clear_get()
{
  if (COMMAND_TEXT_CLEAR == true)
  {
    COMMAND_TEXT_CLEAR = false;
    return true;
  }
  else
  {
    return false;
  }
}

bool CONSOLE_COMMUNICATION::printw_q_avail()
{
  return PRINT_Q_AVAIL;
}

string CONSOLE_COMMUNICATION::printw_q_get()
{
  string ret_q = PRINT_Q;
  PRINT_Q = "";
  PRINT_Q_AVAIL = false;
  return ret_q;
}

void CONSOLE_COMMUNICATION::printw(string Text)
{
  PRINT_Q_AVAIL = true;
  PRINT_Q = PRINT_Q + Text + "\n";
}

// ---------------------------------------------------------------------------------------

#endif
