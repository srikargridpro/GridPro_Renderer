#ifndef _HELIUM_FRAME_BUFFER_CLASS_H_
#define _HELIUM_FRAME_BUFFER_CLASS_H_
#include <vector>
#include <cstdint>
#include "gp_gui_renderer_api.h"

namespace gridpro_gui 
{

class framebuffer 
{
 public :

  framebuffer();
 ~framebuffer();
  enum class ScanMode {PER_PIXEL, LEFT_RIGHT, SPIRAL};
  ScanMode scan_mode;

  void update_current_frame_buffer();
  uint32_t color_id_at(const float current_mouse_x, const float current_mouse_y);
  std::vector<uint32_t> pick_matrix(const float current_mouse_x, const float current_mouse_y, const float pick_matrix_length, const float pick_matrix_width);
  uint32_t pixel_at(const float current_mouse_x, const float current_mouse_y);
  const std::vector<unsigned char>* data();
  float depth_at(const float current_mouse_x, const float current_mouse_y);
  float last_hit_depth();

 private :
 std::vector<unsigned char> framebufferData;
 std::vector<float> DepthBufferData;
 uint32_t framebufferWidth ;
 uint32_t framebufferHeight;
 uint32_t color_id;
 float last_hit_x, last_hit_y;

};

}
 
#endif