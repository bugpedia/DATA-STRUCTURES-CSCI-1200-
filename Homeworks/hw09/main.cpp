#include <cstdlib>
#include <cmath>
#include <vector>
#include <iostream>

#include "image.h"
#include "priority_queue.h"

// ===================================================================================================

// distance field method functions
double NaiveDistanceFieldMethod(Image<Color> &input, Image<DistancePixel> &distance_image);
double ImprovedDistanceFieldMethod(Image<Color> &input, Image<DistancePixel> &distance_image);
double FastMarchingMethod(Image<Color> &input, Image<DistancePixel> &distance_image);

// visualization style helper functions
Color Rainbow(double distance, double max_distance);
Color GreyBands(double distance, double max_distance, int num_bands);

// ===================================================================================================

int main(int argc, char* argv[]) {
  if (argc != 5) {
    std::cerr << "Usage: " << argv[0] << " input.ppm output.ppm distance_field_method visualization_style" << std::endl;
    exit(1);
  }

  // open the input image
  Image<Color> input;
  if (!input.Load(argv[1])) {
    std::cerr << "ERROR: Cannot open input file: " << argv[1] << std::endl;
    exit(1);
  }

  // a place to write the distance values
  Image<DistancePixel> distance_image;
  distance_image.Allocate(input.Width(),input.Height());

  // calculate the distance field (each function returns the maximum distance value)
  double max_distance = 0;
  if (std::string(argv[3]) == std::string("naive_method")) {
    max_distance = NaiveDistanceFieldMethod(input,distance_image);
  } else if (std::string(argv[3]) == std::string("improved_method")) {
    max_distance = ImprovedDistanceFieldMethod(input,distance_image);
  } else if (std::string(argv[3]) == std::string("pq_with_map")) {
    max_distance = FastMarchingMethod(input,distance_image);
  } else if (std::string(argv[3]) == std::string("pq_with_hash_table")) {
    // EXTRA CREDIT: implement FastMarchingMethod with a hash table
  } else {
    std::cerr << "ERROR: Unknown distance field method: " << argv[3] << std::endl;
    exit(1);
  }

  // convert distance values to a visualization
  Image<Color> output;
  output.Allocate(input.Width(),input.Height());
  for (int i = 0; i < input.Width(); i++) {
    for (int j = 0; j < input.Height(); j++) {
      double v = distance_image.GetPixel(i,j).getValue();
      if (std::string(argv[4]) == std::string("greyscale")) {
	     output.SetPixel(i,j,GreyBands(v,max_distance*1.01,1));
      } else if (std::string(argv[4]) == std::string("grey_bands")) {
	     output.SetPixel(i,j,GreyBands(v,max_distance,4));
      } else if (std::string(argv[4]) == std::string("rainbow")) {
	     output.SetPixel(i,j,Rainbow(v,max_distance));
      } else {
      	// EXTRA CREDIT: create other visualizations 
      	std::cerr << "ERROR: Unknown visualization style" << std::endl;
      	exit(0);
      }
    }
  }
  // save output
  if (!output.Save(argv[2])) {
    std::cerr << "ERROR: Cannot save to output file: " << argv[2] << std::endl;
    exit(1);
  }

  return 0;
}

// ===================================================================================================

double NaiveDistanceFieldMethod(Image<Color> &input, Image<DistancePixel> &distance_image) {
  int w = input.Width();
  int h = input.Height();
  // return the maximum distance value
  double answer = 0;
  // loop over the pixels in the input image
  for (int i = 0; i < w; i++)  {
    for (int j = 0; j < h; j++) {
      double closest = -1;      
      // loop over all other pixels in the input image
      for (int i2 = 0; i2 < w; i2++)  {
      	for (int j2 = 0; j2 < h; j2++) {
      	  const Color& c = input.GetPixel(i2,j2);      
      	  // skip all pixels that are not black
      	  if (!c.isBlack()) continue;
      	  // calculate the distance between the two pixels
      	  double distance = sqrt((i-i2)*(i-i2) + (j-j2)*(j-j2));
      	  // store the closest distance to a black pixel
      	  if (closest < 0 || distance < closest) {
      	    closest = distance;
      	  }
      	}
      }
      assert (closest >= 0);
      answer = std::max(answer,closest);
      // save the data to the distance image
      DistancePixel& p = distance_image.GetPixel(i,j);
      p.setValue(closest);
    }
  }
  return answer;
}


double ImprovedDistanceFieldMethod(Image<Color> &input, Image<DistancePixel> &distance_image) {
  int w = input.Width();
  int h = input.Height();

  // store all black pixels in a vector, so we don't have to search for
  // them at every single pixel
  vector<pair<int,int> > all_black_pixels;
  double answer = 0;

  // loop through all pixels, push back black ones to vector
  for (int i = 0; i < w; i++)
  {
    for (int j = 0; j < h; j++)
    {
      const Color& c = input.GetPixel(i,j);

      if (!c.isBlack()) continue;

      all_black_pixels.push_back(make_pair(i,j));
    }
  }

  for (int i = 0; i < w; i++)  {
    for (int j = 0; j < h; j++) {
      double closest = -1;      
      // loop over all black pixels in the input image
      for (int k = 0; k < all_black_pixels.size(); k++)
      {
        // set the x and y coordinates of the k-th black pixel to variables
        int first = all_black_pixels[k].first;
        int second = all_black_pixels[k].second;

        double distance = sqrt((i-first)*(i-first) + (j-second)*(j-second));

        if (closest < 0 || distance < closest)
        {
          closest = distance;
        }
      }

      assert (closest >= 0);
      answer = std::max(answer,closest);
      // save the data to the distance image
      DistancePixel& p = distance_image.GetPixel(i,j);
      p.setValue(closest);
    }
  }
  return answer;
}

double FastMarchingMethod(Image<Color> &input, Image<DistancePixel> &distance_image) {
  // initialize an empty priority queue
  DistancePixel_PriorityQueue pQueue;

  int w = input.Width();
  int h = input.Height();

  double answer = 0;

  // initially begin by looping through all pixels...
  for (int i = 0; i < w; i++)
  {
    for (int j = 0; j < h; j++)
    {
      Color& c = input.GetPixel(i,j);

      // if the pixel isn't black, set it to be blue and continue
      if (!c.isBlack())
      {
        DistancePixel& p = distance_image.GetPixel(i,j);
        c.r = 0;
        c.g = 0;
        c.b = 255;
        continue;
      } 
      // if it is black...
      else
      {
        DistancePixel& p = distance_image.GetPixel(i,j);
        // mark the distance value as 0
        p.setValue(0);
        // check the neighbors of each black pixel
        for (int l = -1; l <= 1; l++)
        {
          for (int r = -1; r <= 1; r++)
          {
            // corner cases to make sure we don't go off the image and to skip
            // the pixel itself
            if (i+l < 0 || i+l >= w || j+r < 0 || j+r >= h || (l == 0 && r == 0))
              continue;

            Color& c_n = input.GetPixel(i+l,j+r);
            DistancePixel& p_n = distance_image.GetPixel(i+l,j+r);

            // if the neighbor is not black and not 0, then set it to red,
            // and calculate the distance between it and the black pixel, 
            // otherwise continue
            if (!c_n.isBlack() && p_n.getValue() != 0)
            {
              c_n.r = 255;
              c_n.g = 0; 
              c_n.b = 0;

              double dist_b_n = sqrt((i-(i+l))*(i-(i+l)) + (j-(j+r))*(j-(j+r)));

              if (p_n.getValue() > p.getValue() + dist_b_n)
                p_n.setValue(p.getValue() + dist_b_n);

              p_n.setX(i+l);
              p_n.setY(j+r);
            }
            else continue;

            // if this neighbor pixel is not in the heap, push it, but if it
            // is, call the update_position
            if (!pQueue.in_heap(&p_n))
              pQueue.push(&p_n);
            else
              pQueue.update_position(&p_n);
          }
        }
      }
    }
  }
  
  // now that the initial queue has been created, loop through until the 
  // priority queue becomes empty, knowing we are done
  while (!pQueue.empty())
  {
    // access the top pixel from the priority queue
    const DistancePixel* top = pQueue.top();
    answer = top->getValue();

    int i = top->getX();
    int j = top->getY();
    Color& top_c = input.GetPixel(i,j);

    // mark its distance as known by making it black
    top_c.r = 0;
    top_c.g = 0;
    top_c.b = 0;

    // remove the value from the priority queue
    pQueue.pop();

    // again, check all the neighbors of the known pixel
    for (int l = -1; l <= 1; l++)
    {
      for (int r = -1; r <= 1; r++)
      {
        // check corner cases, like above
        if (i+l < 0 || i+l >= w || j+r < 0 || j+r >= h || (l == 0 && r == 0))
          continue;

        Color& c_n = input.GetPixel(i+l,j+r);
        DistancePixel& p_n = distance_image.GetPixel(i+l,j+r);

        // check if the pixel isn't black, calculate the distance between it
        // and the known pixel
        if (!c_n.isBlack() && p_n.getValue() != 0)
        {
          c_n.r = 255;
          c_n.g = 0; 
          c_n.b = 0;

          double dist_b_n = sqrt((i-(i+l))*(i-(i+l)) + (j-(j+r))*(j-(j+r)));

          if (p_n.getValue() > top->getValue() + dist_b_n)
            p_n.setValue(top->getValue() + dist_b_n);

          p_n.setX(i+l);
          p_n.setY(j+r);
        }
        else continue;

        // if it's in the priority queue, update its position, otherwise push 
        // it to the queue
        if (!pQueue.in_heap(&p_n))
          pQueue.push(&p_n);
        else
          pQueue.update_position(&p_n); 
      }
    }
  }

  return answer;
}

// ===================================================================================================

Color Rainbow(double distance, double max_distance) {
  Color answer;
  if (distance < 0.001) {
    // black
    answer.r = 0; answer.g = 0; answer.b = 0;
  } else if (distance < 0.2*max_distance) {
    // blue -> cyan
    double tmp = distance * 5.0 / max_distance;
    answer.r = 0;
    answer.g = tmp*255;
    answer.b = 255;
  } else if (distance < 0.4*max_distance) {
    // cyan -> green
    double tmp = (distance-0.2*max_distance) * 5.0 / max_distance;
    answer.r = 0;
    answer.g = 255;
    answer.b = (1-tmp*tmp)*255;
  } else if (distance < 0.6*max_distance) {
    // green -> yellow
    double tmp = (distance-0.4*max_distance) * 5.0 / max_distance;
    answer.r = sqrt(tmp)*255;
    answer.g = 255;
    answer.b = 0;
  } else if (distance < 0.8*max_distance) {
    // yellow -> red
    double tmp = (distance-0.6*max_distance) * 5.0 / max_distance;
    answer.r = 255;
    answer.g = (1-tmp*tmp)*255;
    answer.b = 0;
  } else if (distance < max_distance) {
    // red -> white
    double tmp = (distance-0.8*max_distance) * 5.0 / max_distance;
    answer.r = 255;
    answer.g = tmp*255;
    answer.b = tmp*255;
  } else {
    // white
    answer.r = answer.g = answer.b = 255;
  }  
  return answer;
}

Color GreyBands(double distance, double max_value, int num_bands) {
  Color answer;
  if (distance < 0.001) {
    // red
    answer.r = 255; answer.g = 0; answer.b = 0;
  } else {
    // shades of grey
    answer.r = answer.g = answer.b = int(num_bands*256*distance/double(max_value)) % 256;
  }  
  return answer;
}

// ===================================================================================================