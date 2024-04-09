#include "gp_gui_framebuffer.h"

namespace gridpro_gui 
{

  /// @brief Constructor
 framebuffer::framebuffer() 
 {
     scan_mode = ScanMode::LEFT_RIGHT; 
 }
 
 /// @brief Destructor
 framebuffer::~framebuffer() {}

 /// @brief Update the current framebuffer data
 void framebuffer::update_current_frame_buffer()
 {
   GLint viewport[4];
   glGetIntegerv(GL_VIEWPORT , viewport);
   glReadBuffer(GL_BACK);
   framebufferWidth  = viewport[2];
   framebufferHeight = viewport[3];
   //framebufferData.clear();
   framebufferData.resize(framebufferWidth * framebufferHeight * 4);
   //DepthBufferData.clear();
   DepthBufferData.resize(framebufferData.size()/4);
   glReadPixels(0, 0, framebufferWidth, framebufferHeight, GL_RGBA, GL_UNSIGNED_BYTE, framebufferData.data());
   glReadPixels(0, 0, framebufferWidth, framebufferHeight, GL_DEPTH_COMPONENT, GL_FLOAT, DepthBufferData.data());
   
 }

 /// @brief Get the color id at the specified mouse coordinates
 uint32_t framebuffer::color_id_at(const float current_mouse_x, const float current_mouse_y)
 {
 //----------------------------------------------------------------------------------------------------------------
 // Here we are reading only one pixel , for using a pickmatrix iterate through every pixel in the pick matrix.
 // ----> current_mouse_x , viewport[3] - current_mouse_y is the origin [0,0].
 //-----------------------------------------------------------------------------------------------------------------
 //                Pixel Pick Matrix
 //                         x-----------+
 //                [-1, 1][0, 1][1, 1]  | y_max
 //                [-1, 0][0, 0][1, 0]  y
 //                [-1,-1][0,-1][1,-1]  | y_min
 //-----------------------------------------------------------------------------------------------------------------                   

  const float pick_matrix_length = 5;
  const float pick_matrix_width  = 5; 
  

if(scan_mode == ScanMode::LEFT_RIGHT)
{  
  for(float i = (current_mouse_x ) ; i < (current_mouse_x + pick_matrix_length) ; ++i)
    for(float j = (current_mouse_y) ; j < (current_mouse_y + pick_matrix_width) ; ++j)
         {  
            uint32_t hit  = pixel_at(i,j);
            if(hit != 0)    
            {  
              last_hit_x = i; last_hit_y = j; 
              DEBUG_PRINT("Hit at" , i , j , "Pixel ID = ", hit); 
              return hit;
            }
         }

  for(float i = (current_mouse_x - pick_matrix_length) ; i < (current_mouse_x) ; ++i)
    for(float j = (current_mouse_y - pick_matrix_width) ; j < (current_mouse_y) ; ++j)
         {  
            uint32_t hit  = pixel_at(i,j);
            if(hit != 0)    
            {  
              last_hit_x = i; last_hit_y = j; 
              DEBUG_PRINT("Hit at" , i , j , "Pixel ID = ", hit);
              return hit;
            }
         }
}

else if(scan_mode == ScanMode::SPIRAL)
{
int x = static_cast<int>(current_mouse_x);
int y = static_cast<int>(current_mouse_y);   
// Define the spiral parameters
int dx = 1;
int dy = 0;
int segment_length = 1;
int segment_passed = 0;

for (int i = 0; i < pick_matrix_length; ++i) {
    for (int j = 0; j < pick_matrix_width; ++j) {
        // Ensure the current coordinates are within the bounds of your matrix
        if (x >= 0 && x < framebufferWidth && y >= 0 && y < framebufferHeight) {
            uint32_t hit = pixel_at(x, y);
            if (hit != 0) {
                /*DEBUG_PRINT("Hit at", x, y, "Pixel ID =", hit);*/
                return hit;
            }
        }

        x += dx;
        y += dy;
        ++segment_passed;

        if (segment_passed == segment_length) {
            segment_passed = 0;

            // Change direction in a counter-clockwise spiral
            int temp = dx;
            dx = -dy;
            dy = temp;

            // Increase segment length every two turns
            if (dy == 0) {
                ++segment_length;
            }
        }
    }
}
}

else 
{
   uint32_t hit  = pixel_at(current_mouse_x,current_mouse_x);
   if(hit != 0) return hit; 
}
     return 0;    
 }
 

/// @brief Get the color id at the specified mouse coordinates with pick matrix
std::vector<uint32_t> framebuffer::pick_matrix(const float current_mouse_x, const float current_mouse_y, const float pick_matrix_length = 4, const float pick_matrix_width = 4)
 {
 //----------------------------------------------------------------------------------------------------------------
 // Here we are reading only one pixel , for using a pickmatrix iterate through every pixel in the pick matrix.
 // ----> current_mouse_x , viewport[3] - current_mouse_y is the origin [0,0].
 //-----------------------------------------------------------------------------------------------------------------
 //                Pixel Pick Matrix
 //                         x-----------+
 //                [-1, 1][0, 1][1, 1]  | y_max
 //                [-1, 0][0, 0][1, 0]  y
 //                [-1,-1][0,-1][1,-1]  | y_min
 //-----------------------------------------------------------------------------------------------------------------                   
 
 std::vector<uint32_t> hits;
  
  for(float i = (current_mouse_x ) ; i < (current_mouse_x + (pick_matrix_length/2)) ; ++i)
    for(float j = (current_mouse_y) ; j < (current_mouse_y + (pick_matrix_width/2)) ; ++j)
         {  
            uint32_t hit  = pixel_at(i,j);
            if(hit != 0)  { /*DEBUG_PRINT("Hit at" , i , j , "Pixel ID = ", hit);*/ hits.push_back(hit); }
         }

  for(float i = (current_mouse_x -  (pick_matrix_length/2)) ; i < (current_mouse_x) ; ++i)
    for(float j = (current_mouse_y - (pick_matrix_width/2)) ; j < (current_mouse_y) ; ++j)
         {  
            uint32_t hit  = pixel_at(i,j);
            if(hit != 0)  { /*DEBUG_PRINT("Hit at" , i , j , "Pixel ID = ", hit); */ hits.push_back(hit); }
         }

     return hits;    
 }
 
 /// @brief Get the depth at the specified mouse coordinates
 float framebuffer::depth_at(const float current_mouse_x, const float current_mouse_y)
 {
    // Calculate the pixel coordinates in the framebuffer
   int pixelX = static_cast<int>(current_mouse_x);
   int pixelY = framebufferHeight - static_cast<int>(current_mouse_y) - 1;
   if (pixelX >= 0 && pixelX < framebufferWidth && pixelY >= 0 && pixelY < framebufferHeight) {
            // Calculate the index in the DepthBuffer data array
            int index = (pixelY * framebufferWidth + pixelX); 
            return DepthBufferData[index];
        }
      return 0;
 }
 
 /// @brief Get the last hit depth
 float framebuffer::last_hit_depth()
 {
   return depth_at(last_hit_x, last_hit_y);
 }

 /// @brief Get the pixel at the specified mouse coordinates
 uint32_t framebuffer::pixel_at(const float current_mouse_x, const float current_mouse_y)
 {
    // Calculate the pixel coordinates in the framebuffer
   int pixelX = static_cast<int>(current_mouse_x);
   int pixelY = framebufferHeight - static_cast<int>(current_mouse_y) - 1;
   if (pixelX >= 0 && pixelX < framebufferWidth && pixelY >= 0 && pixelY < framebufferHeight) {
            // Calculate the index in the framebuffer data array
            int index = (pixelY * framebufferWidth + pixelX) * 4; // Assuming RGBA format

            // Get the pixel values at the specified coordinates
            GLubyte r = framebufferData[index];
            GLubyte g = framebufferData[index + 1];
            GLubyte b = framebufferData[index + 2];
            GLubyte a = framebufferData[index + 3];
            
            GLuint Selected_Primitive_ID = 0;
            Selected_Primitive_ID = ((Selected_Primitive_ID + b) << 8); 
            Selected_Primitive_ID = ((Selected_Primitive_ID + g) << 8);
            Selected_Primitive_ID =  (Selected_Primitive_ID + r) ;
 
            // Print or use the pixel values as needed
            // std::cout << "Pixel at (" << pixelX << ", " << pixelY << "): ";
            // std::cout << "Selected Corner ID using frame buffer = " << Selected_Primitive_ID  << "\n";
            // std::cout << "R: " << static_cast<int>(r) << " G: " << static_cast<int>(g) << " B: " << static_cast<int>(b) << " A: " << static_cast<int>(a) << std::endl;
            color_id = Selected_Primitive_ID;
            return Selected_Primitive_ID;
        }

      return 0;
 }

 /// @brief Get the framebuffer data
 const std::vector<unsigned char>* framebuffer::data()
 {
    return &framebufferData;
 }

}