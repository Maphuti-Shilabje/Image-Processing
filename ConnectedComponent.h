#ifndef CONNECTED_COMPONENT_H
#define CONNECTED_COMPONENT_H

#include <vector>
#include <utility>

/*
 *The class is a connected component in a binary image
 * It stores the pixel Coordinates, size and id
 *
 * */

class ConnectedComponent
{
   private:
      int id     // id for the component
      int pixels  // number of pixels in the component
      std::vector<std::pair<int x, int y>> pixels;    // coordinates of the pixels in the component
						  //
   public:
      explicit ConnectedComponent(int id);
      ~ConnectedComponent() = default;
	
  
    ConnectedComponent(const ConnectedComponent& other);             // Copy constructor
    ConnectedComponent& operator=(const ConnectedComponent& other);  // Copy assignment operator
    ConnectedComponent(ConnectedComponent&& other) noexcept;	     // Move constructor
    ConnectedComponent& operator=(ConnectedComponent&& other) noexcept;  // Move assignment operator

    // methods
    void addPixel(int x, int y); // add a pixel to the component
    int getId() const;           // get component id
    int getNumPixels() const;    // get total pixels in component
    const std::vector<std::pair<int, int>>& getPixels() const; // get pixel coordinates
		

};

#endif
