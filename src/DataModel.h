
#ifndef DATAMODEL_H
#define DATAMODEL_H
#include <cstdint>
#include <memory>
#include <string>
#include <vector>

struct MRect{
  int16_t x1;
  int16_t y1;
  int16_t x2;
  int16_t y2;
};

struct MPoint{
  int16_t x;
  int16_t y;
};

using MColor = uint32_t;


struct MPointFeature{
  int16_t x =-1;
  int16_t y = -1;
  uint32_t color = 0;
  MPointFeature() = default;
  MPointFeature(int16_t x, int16_t y,uint32_t color): x(x), y(y), color(color){}
  bool operator ==(const MPointFeature &other) const{
    return x == other.x && y == other.y && color == other.color;
  }
  bool operator!=(const MPointFeature &other) const {
      return !(*this == other);
  }

  bool operator<(const MPointFeature &other) const {
    if (x != other.x) return x < other.x;
    if (y != other.y) return y < other.y;
    return color < other.color;
  }
};

class MFeature {
public:
  void add(const MPointFeature &feature);
  void remove(int index);
  void clear();
  int size();
  MPointFeature operator[](int index);
private:
  std::vector<MPointFeature> features_;
};




class DataModel {
public:
  DataModel();
  MRect selectedRect();
  void setSelectedRect(const MRect &rect);
  [[nodiscard]] MFeature &feature() const;
private:
  MRect selectedRect_;
  MFeature feature_;
  std::vector<MPointFeature> pointFeatures_;
};

using DataModelPtr = std::shared_ptr<DataModel>;

#endif // DATAMODEL_H