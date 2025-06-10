#ifndef GUI_H
#define GUI_H

#include <array>

/**
 * @brief GUI class to call for displaying data
 */
class Gui
{
public:
    Gui();
    ~Gui();
    void draw(const std::array<float, 10>& octave_bands);

    bool sine_on_{false};
    bool pink_on_{false};
    bool mic_on_{false};
};

#endif //GUI_H
