#ifndef DRAWING_UTILS_
#define DRAWING_UTILS_

#include <cstdint>
#include <iostream>
#include "CImg.h"

class Point2D {
protected:
   long double x;
   long double y;

public:
   static const int dim = 2;

   Point2D() {}

   Point2D(long double x, long double y) : x(x), y(y) {}

   Point2D(const Point2D& other) : x(other.x), y(other.y) {}

   long double getX() const {return x;}
   long double getY() const {return y;}
   static int getDim() {return dim;}

   void setX(long double x) {this->x = x;}
   void setY(long double y) {this->y = y;}

   Point2D operator+(const Point2D& other) const {
      return Point2D(x + other.x, y + other.y);
   }

   Point2D operator-(const Point2D& other) const {
      return Point2D(x - other.x, y - other.y);
   }

   Point2D operator*(long double t) const {
      return Point2D(x*t, y*t);
   }

   Point2D operator/(long double t) const {
      return Point2D(x/t, y/t);
   }

   Point2D& operator=(const Point2D& other) {
      x = other.x;
      y = other.y;
      return *this;        
   }

   bool operator==(const Point2D& other) const {
      return x == other.x && y == other.y;
   }

   bool operator<(const Point2D& other) const {
      return (x < other.x) || (x == other.x && y < other.y);
   }

   bool operator>(const Point2D& other) const {
      return other < *this;
   }

   bool operator<=(const Point2D& other) const {
      return (*this < other) || (*this == other);
   }

   bool operator>=(const Point2D& other) const {
      return (*this > other) || (*this == other);
   }

   long double dot(const Point2D& other) const{
      return x*other.x + y*other.y;
   }

   long double cross(const Point2D& other) const{
      return x*other.y - y*other.x; 
   }

   long double computeEuclidDistance(const Point2D& other) const {
      return sqrtl(powl(x - other.x, 2) + powl(y - other.y, 2));
   }

   friend std::ostream& operator<<(std::ostream& os, const Point2D& p) {
      os << "Point2D(" << p.x << ' ' << p.y << ')';
      return os;
   }
};


Point2D rotate(const Point2D& A, long double angle) {
   long double cos_angle = cosl(angle);
   long double sin_angle = sinl(angle);
   long double x = A.getX();
   long double y = A.getY();
   return Point2D(x*cos_angle - y*sin_angle, x*sin_angle + y*cos_angle);
}

Point2D rotate(const Point2D& A, const Point2D& O, long double angle) {
   Point2D A1 = A - O;
   Point2D R1 = rotate(A1, angle);
   return R1 + O;
}

namespace draw_utils {
   unsigned char RED[]    = { 255, 0, 0 };
   unsigned char GREEN[]    = { 0, 255, 0 };
   unsigned char BLUE[] = { 0, 0, 255 };
   unsigned char CYAN[]    = { 0, 255, 255 };
   unsigned char MAGENTA[] = { 255, 0, 255 };
   unsigned char YELLOW[]  = { 255, 255, 0 };
   unsigned char BLACK[]  = { 0, 0, 0 };
   unsigned char WHITE[]  = { 255, 255, 255 };

   void draw_line(cimg_library::CImg<uint8_t>& image,
      const int x1, const int y1,
      const int x2, const int y2,
      const uint8_t* const color,
      const uint8_t line_width,
      const double opacity=1.0)
   {
      if (x1 == x2 && y1 == y2) {
         return;
      }
      // Convert line (p1, p2) to polygon (pa, pb, pc, pd)
      const double x_diff = (x1 - x2);
      const double y_diff = (y1 - y2);
      const double w_diff = line_width / 2.0;

      // Triangle between pa and p1: x_adj^2 + y_adj^2 = w_diff^2
      // Triangle between p1 and p2: x_diff^2 + y_diff^2 = length^2 
      // Similar triangles: y_adj / x_diff = x_adj / y_diff = w_diff / length
      // -> y_adj / x_diff = w_diff / sqrt(x_diff^2 + y_diff^2) 
      const int x_adj = y_diff * w_diff / std::sqrt(std::pow(x_diff, 2) + std::pow(y_diff, 2));
      const int y_adj = x_diff * w_diff / std::sqrt(std::pow(x_diff, 2) + std::pow(y_diff, 2));

      // Points are listed in clockwise order, starting from top-left
      cimg_library::CImg<int> points(4, 2);
      points(0, 0) = x1 - x_adj;
      points(0, 1) = y1 + y_adj;
      points(1, 0) = x1 + x_adj;
      points(1, 1) = y1 - y_adj;
      points(2, 0) = x2 + x_adj;
      points(2, 1) = y2 - y_adj;
      points(3, 0) = x2 - x_adj;
      points(3, 1) = y2 + y_adj;

      image.draw_polygon(points, color, opacity);
   }

   void draw_sector_1(CImg<unsigned char>& img, int c_x, int c_y, long double radius,
                  long double angle, long double orientation_angle, const unsigned char* color, double opacity=1.0
   ) {
      Point2D O(c_x, c_y);
      Point2D O1(c_x + radius, c_y);
      Point2D O2 = rotate(O1, O, orientation_angle);
      double half_angle = angle/2;
      Point2D A = rotate(O2, O, -half_angle);
      Point2D B = rotate(O2, O, half_angle);

      Point2D C = (O + A)/2;
      Point2D D = (O + B)/2;
      long double w = radius/2*(sqrtl(4 - powl(sinl(half_angle), 2)) - cosl(half_angle));

      Point2D V(w, 0);
      Point2D E = D + V;
      Point2D F = C + V;

      cimg_library::CImg<int> points(5, 2);
      points(0, 0) = C.getX();
      points(0, 1) = C.getY();
      points(1, 0) = F.getX();
      points(1, 1) = F.getY();
      points(2, 0) = E.getX();
      points(2, 1) = E.getY();
      points(3, 0) = D.getX();
      points(3, 1) = D.getY();
      points(4, 0) = O.getX();
      points(4, 1) = O.getY();

      img.draw_polygon(points, color, opacity);
   }

}

#endif //DRAWING_UTILS_