#include "road.h"

using namespace std;

void Road::update_road(vector<Vehicle> left_lane, vector<Vehicle> center_lane, vector<Vehicle> right_lane){
  this->left_lane = left_lane;
  this->center_lane = center_lane;
  this->right_lane = right_lane;
}

vector<Vehicle> Road::get_lane_status(LANE lane){
  vector<Vehicle> rlane;
  if (lane == LANE::LEFT) {
    rlane = this->left_lane;
  } else if (lane == LANE::CENTER) {
    rlane = this->center_lane;
  } else {
    rlane = this->right_lane;
  }
  return rlane;
}

bool Road::safe_lane(Vehicle& car, LANE lane){
  vector<Vehicle> r_car_lane = this->get_lane_status(lane);
  bool safe = true;
  // If one of the cars in the current lane is too close (< Safety distance), I have to change
  for (int i = 0; i < r_car_lane.size(); i++) {
    double distance = r_car_lane[i].get_s() - car.get_s();
    if(distance > 0 && distance < SAFETY_DISTANCE){
      safe = false;
    }
  }
  return safe;
}

bool Road::free_lane(Vehicle& car, LANE lane){
  vector<Vehicle> target_lane = this->get_lane_status(lane);
  bool is_free = true;
  // If one of the cars in the target lane is too close (< Guard distance), I can't change
  for (int i = 0; i < target_lane.size(); i++) {
    double distance = abs(target_lane[i].get_s() - car.get_s());
    if(distance < GUARD_DISTANCE){
      is_free = false;
    }
  }
  return is_free;
}

LANE Road::lane_change_available(Vehicle& car){
  LANE car_lane = car.lane();
  // by default stay where you are
  LANE target_lane = car_lane;

  // if car is left or right, check if center lane is free
  if (car_lane == LANE::LEFT || car_lane == LANE::RIGHT) {
    if (this->free_lane(car, LANE::CENTER)) {
      target_lane = LANE::CENTER;
    }
  } 
  else {
	// if car is in center lane, check if left lane is free
    if (this->free_lane(car, LANE::LEFT)) {
      target_lane = LANE::LEFT;
    } 
	// if car is in center lane, and left lane is blocked, check if right lane is free
	else if (this->free_lane(car, LANE::RIGHT)) {
      target_lane = LANE::RIGHT;
    }
  }
  return target_lane;
}