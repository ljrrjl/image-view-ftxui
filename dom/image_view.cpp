// Copyright 2023 ljrrjl. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <opencv2/opencv.hpp>

#include <algorithm>  // for min
#include <memory>     // for make_shared
#include <string>     // for string, wstring
#include <utility>    // for move
#include <vector>     // for vector

#include "ftxui/dom/deprecated.hpp"   // for text, vtext
#include "ftxui/dom/elements.hpp"     // for Element, text, vtext
#include "ftxui/dom/node.hpp"         // for Node
#include "ftxui/dom/requirement.hpp"  // for Requirement
#include "ftxui/screen/box.hpp"       // for Box
#include "ftxui/screen/screen.hpp"    // for Pixel, Screen
#include "ftxui/screen/string.hpp"  // for string_width, Utf8ToGlyphs, to_string

namespace ftxui {

namespace {
using ftxui::Screen;

class ImageView: public Node {
 public:
  explicit ImageView(std::string_view url) : url_(url) {}

  void ComputeRequirement() override {
    img_ = cv::imread(url_, cv::IMREAD_GRAYSCALE);
    width_ = img_.cols * 2 - 1;
    height_ = img_.rows;
    requirement_.min_x = width_;
    requirement_.min_y = height_;
  }

  void Render(Screen& screen) override {
    int img_width = (box_.x_max - box_.x_min + 1) / 2;
    int img_height = box_.y_max - box_.y_min + 1;
    int x = box_.x_min;
    int y = box_.y_min;
    
    cv::resize(img_, img_, cv::Size(img_width, img_height));
    
    for (int i = 0; i < img_height; i++){
      for (int j = 0; j < img_width; j++){
        int idx = img_.at<uchar>(i, j) / 32;
        screen.PixelAt(x++, y).character = charset[idx];
        if(j != img_width - 1)
            screen.PixelAt(x++, y).character = charset[0];
      }
      x = box_.x_min;
      ++y;
    }
  }

 private:
  std::string url_;
  cv::Mat     img_;
  int         width_;
  int         height_;
  inline static const std::string charset[] = {" ", ".", "-", "+", "=", "*", "#", "@"};
};

}  // namespace

Element image_view(std::string_view url) {
  return std::make_shared<ImageView>(url);
}

}  // namespace ftxui
