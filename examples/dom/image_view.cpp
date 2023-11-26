// Copyright 2023 ljrrjl. All rights reserved.
// Use of this source code is governed by the MIT license that can be found in
// the LICENSE file.
#include <ftxui/dom/elements.hpp>  // for operator|, text, Element, hcenter, Fit, hbox, size, window, Elements, bold, dim, EQUAL, WIDTH
#include <ftxui/screen/screen.hpp>  // for Screen

#include<opencv2/opencv.hpp>

#include <memory>                   // for allocator, shared_ptr
#include <string>                   // for string, to_string
#include <sstream>
#include <utility>                  // for move
#include <vector>
#include <cassert>
#include <chrono>
#include <thread>

#include "ftxui/dom/node.hpp"      // for Render
#include "ftxui/screen/color.hpp"  // for ftxui

using namespace cv;
using namespace std;
using namespace ftxui;

int main()
{
    VideoCapture capture;
    Mat frame;
    frame= capture.open("./apple.gif"); 

    assert(capture.isOpened() && "Fail to open gif file!");
    capture.set(CAP_PROP_POS_FRAMES, 0);

    std::vector<cv::Mat> frames;
    while (capture.read(frame) && !frame.empty())
    {
        frames.push_back(frame.clone());
    }
    capture.release();

    std::string reset_position;
    for(int i = 0; i < frames.size(); i++)
    {
      std::stringstream ss;
      cv::imwrite("tmp.png", frames[i]);
      auto video_screen = window(text("apple") | hcenter | bold,
                      image_view("tmp.png"));                  // load picture
      video_screen = video_screen | size(WIDTH, EQUAL, 100) |size(HEIGHT, EQUAL, 30);
      auto document = vbox({
          video_screen,
          gauge(static_cast<float>(i) / frames.size()) | flex
      });
      document |= border;
      auto screen = Screen::Create(Dimension::Fit(document), Dimension::Fit(document));
      Render(screen, document);
      std::cout << reset_position;
      screen.Print();
      reset_position = screen.ResetPosition();
      std::this_thread::sleep_for(std::chrono::milliseconds(40));
    }
    return 0;
}
