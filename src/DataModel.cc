
#include "DataModel.h"


DataModel::DataModel():
  selectedRect_{-1,-1,-1,-1}{

}

MRect DataModel::selectedRect(){
  return selectedRect_;
}

void DataModel::setSelectedRect(const MRect &rect){
  selectedRect_ = rect;
}

void MFeature::add(const MPointFeature &feature){
  features_.push_back(feature);
}

void MFeature::remove(int index){
  features_.erase(features_.begin() + index);
}

void MFeature::clear(){
  features_.clear();
}

int MFeature::size(){
  return static_cast<int>(features_.size());
}

MPointFeature MFeature::operator[](int index){
  return features_[index];
}


MFeature &DataModel::feature() const{
  return const_cast<MFeature &>(feature_);
}
